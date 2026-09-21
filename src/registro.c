#include <zephyr/kernel.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/logging/log.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/sys/crc.h>
#include <string.h>

#include "registro.h"

LOG_MODULE_REGISTER(registro, LOG_LEVEL_INF);
enum tipo_cmd{
	CMD_TENDENCIA, 
	CMD_EVENTO,
	CMD_FLUSH,
	CMD_BORRAR,
};

// Unión de eventos:
struct cmd {
	enum tipo_cmd tipo;
	union {
		struct reg_tendencia tendencia; 
		struct reg_evento evento;
	};
};

// Creamos el queue de 8 comandos para el thread:
K_MSGQ_DEFINE(ordenes_q, sizeof(struct cmd), 8, 4);

#define STACK_REGISTRO 2048 // stack para el thread de registros
K_THREAD_STACK_DEFINE(stack_registro, STACK_REGISTRO);
static struct k_thread thread_registro; 

//----------------------------------------------------
// Variables que describen el estado del módulo (solo el thread escritor
// los puede modificar).
// La idea de "secuencia" es anclar el último punto donde se realizó escritura
// así es posible reencontrarlo en caso que el sistema entre en reposo. 
static const struct flash_area *area_tendencia; 
static const struct flash_area *area_eventos;
static size_t bloque_borrado;

static struct pagina_tendencia buffer_pagina; // página en RAM, aún sin escribir en flash
static uint8_t  num_registros; // registros ya metidos en el buffer
static uint16_t cursor_pagina; // próxima página libre de tendencia
static uint16_t paginas_totales;
static uint16_t secuencia_siguiente;

static uint16_t cursor_evento; // próxima página libre de eventos
static uint16_t eventos_totales; 

static bool memoria_llena;
//----------------------------------------------------
// Función para reiniciar el buffer actual (todo queda en 1 cuando
// se borra en una flash)
static void reiniciar_buffer(void){
	memset(&buffer_pagina, 0xFF, sizeof(buffer_pagina));
	num_registros = 0;
}
//----------------------------------------------------
// Función para encontrar el primer espacio sin escribir de cada partición de la flash
// Si su campo vale 0xFF (REG_SEQ_VACIA), es posible escribir ahí. Además, en los eventos
// simplemente se busca que el timestamp sea REG_TS_VACIO
static int localizar_cursores(void)
{
    // Si el barrido no encuentra hueco, el cursor queda en el total (llena)
    cursor_pagina = paginas_totales;
    secuencia_siguiente = 0;

    for (uint16_t i = 0; i < paginas_totales; i++){
        uint16_t seq;
		// Cosas clave: 
		//off_t es un tipo de dato de Zephyr Signed Integer que se usa para representar
		//posiciones de memoria
		// offsetof() entrega cuántos bytes hay desde el inicio de la estructura hasta el campo seq.
        off_t off = (off_t)i * REG_PAGINA_BYTES + offsetof(struct pagina_tendencia, seq);
        int err = flash_area_read(area_tendencia, off, &seq, sizeof(seq));
        if (err){
            return err;
        }

        if (seq == REG_SEQ_VACIA){ // encontró el Byte 0xFF
            cursor_pagina = i; // Posiciona el cursor en el espacio vacío
            break;
        }
        secuencia_siguiente = (uint16_t)(seq + 1U); // Última posición llena. 
    }

    // Para evitar el caso 0xFF y que se detecte una página que no está vacía en realidad
    if (secuencia_siguiente == REG_SEQ_VACIA){
        secuencia_siguiente = 0;
    }

	// Caso análogo, pero ahora con los eventos:
    cursor_evento = eventos_totales;

    for (uint16_t i = 0; i < eventos_totales; i++){
        uint32_t ts;
        off_t off = (off_t)i * sizeof(struct reg_evento);
        int err = flash_area_read(area_eventos, off, &ts, sizeof(ts));

        if (err){
            return err;
        }

        if (ts == REG_TS_VACIO){ 
            cursor_evento = i;
            break;
        }
    }

    // Activa el flag si se llenó cualquiera de las 2 particiones
    memoria_llena = (cursor_pagina >= paginas_totales) || (cursor_evento >= eventos_totales);

    return 0;
}
//----------------------------------------------------
// Prepara el bloque de borrado en cada inicio de bloque (cada 4096 bytes)
static int preparar_bloque(const struct flash_area *area, off_t off)
{
    if ((off % (off_t)bloque_borrado) != 0){ // no es el inicio: nada que borrar
        return 0;
    }

    return flash_area_erase(area, off, bloque_borrado);
}

//----------------------------------------------------
// Función para escribir en la flash el buffer de tendencia y pasar a la pag sig.
static int cerrar_pagina(void)
{
    if (num_registros == 0){ // nada que escribir
        return 0;
    }

    if (cursor_pagina >= paginas_totales){ // memoria llena
        memoria_llena = true;
        LOG_ERR("particion de tendencia llena, se descartan %u registros", num_registros);
        buffer_reiniciar();
        return -ENOSPC;
    }
	// Definimos el offset en base al cursor ya encontrado
    off_t off = (off_t)cursor_pagina * REG_PAGINA_BYTES;
    int err = preparar_bloque(area_tendencia, off); // Borra con el offset obtenido

    if (err){
        LOG_ERR("borrado de bloque en 0x%lx fallo (err %d)", (long)off, err);
        return err;
    }

    buffer_pagina.seq = secuencia_siguiente;
    // Creamos el crc utilizando la página entera menos su propio campo: 
    buffer_pagina.crc = crc16_ansi((const uint8_t *)&buffer_pagina,
                                   REG_PAGINA_BYTES - sizeof(buffer_pagina.crc));
	
	// Escritura en la flash de la tendencia (registros)
    err = flash_area_write(area_tendencia, off, &buffer_pagina, REG_PAGINA_BYTES);
    if (err){
        LOG_ERR("escritura de pagina %u fallo (err %d)", cursor_pagina, err);
        return err;
    }

    LOG_INF("pagina %u escrita (seq=%u, %u registros)", cursor_pagina,
            buffer_pagina.seq, num_registros);

    // Avanza cursor y secuencia para próxima página:
    cursor_pagina++;
    secuencia_siguiente = (uint16_t)(secuencia_siguiente + 1U);
    if (secuencia_siguiente == REG_SEQ_VACIA){
        secuencia_siguiente = 0;
    }

    if (cursor_pagina >= paginas_totales){
        memoria_llena = true;
        LOG_WRN("particion de tendencia al limite");
    }

    buffer_reiniciar();

    return 0;
}
//----------------------------------------------------
// Función para meter un registro de tendencia en el buffer. Solo toca 
// la flash cuando la página se completa.
static int agregar_tendencia(const struct reg_tendencia *r)
{
    if (num_registros >= REG_POR_PAGINA){ // Por si acaso
        // No debería ocurrir: la página se cierra al completar el 28
        int err = cerrar_pagina();

        if (err){
            return err;
        }
    }
	// Incorporación al buffer:
    buffer_pagina.reg[num_registros++] = *r; 

    // 28 registros de 9 B completan los 252 B de datos de la página
    if (num_registros == REG_POR_PAGINA){
        return cerrar_pagina();
    }

    return 0;
}
//----------------------------------------------------
// Función para escribir un evento. A diferencia de la tendencia, los eventos
// van directo a flash: no se acumulan en RAM.
static int escribir_evento(const struct reg_evento *e)
{
    if (cursor_evento >= eventos_totales){
        memoria_llena = true;
        LOG_ERR("particion de eventos llena, evento tipo %u descartado", e->tipo);
        return -ENOSPC;
    }

    off_t off = (off_t)cursor_evento * sizeof(struct reg_evento);
    int err = preparar_bloque(area_eventos, off);

    if (err){
        return err;
    }
	// Escritura del evento en la flash:
    err = flash_area_write(area_eventos, off, e, sizeof(*e));
    if (err){
        LOG_ERR("escritura de evento %u fallo (err %d)", cursor_evento, err);
        return err;
    }

    cursor_evento++; // avanzamos a la ranura siguiente
    LOG_INF("evento tipo=%u ts=%u flags=0x%02x", e->tipo, e->ts, e->flags);

    return 0;
}
//----------------------------------------------------
// Borra las dos particiones enteras y deja los cursores en el "origen":
static int borrar_todo(void)
{	// fa_size indica el tamaño del área en bytes.
    int err = flash_area_erase(area_tendencia, 0, area_tendencia->fa_size);

    if (err){
        LOG_ERR("borrado de tendencia fallo (err %d)", err);
        return err;
    }

    err = flash_area_erase(area_eventos, 0, area_eventos->fa_size);
    if (err){
        LOG_ERR("borrado de eventos fallo (err %d)", err);
        return err;
    }

    buffer_reiniciar();
    cursor_pagina = 0;
    cursor_evento = 0;
    secuencia_siguiente = 0;
    memoria_llena = false;

    LOG_INF("registros borrados");

    return 0;
}

// Thread de escritura: el único que toca la flash. Se queda esperando en la queue
// hasta que llega un comando así no bloquea.
static void thread_escritor(void *a, void *b, void *c)
{
    ARG_UNUSED(a);
    ARG_UNUSED(b);
    ARG_UNUSED(c);

    struct cmd orden;

    while (1){
        k_msgq_get(&ordenes_q, &orden, K_FOREVER); // K_FOREVER = espera recepción

        switch (orden.tipo){
        case CMD_TENDENCIA:
            (void)agregar_tendencia(&orden.tendencia);
            break;
        case CMD_EVENTO:
            (void)escribir_evento(&orden.evento);
            // Un evento arrastra a flash el buffer parcial de tendencia, para
            // conservar los minutos previos al incidente.
            (void)cerrar_pagina();
            break;
        case CMD_FLUSH:
            (void)cerrar_pagina();
            break;
        case CMD_BORRAR:
            (void)borrar_todo();
            break;
        }
    }
}

//----------------------------------------------------
// Función para abrir las particiones, averiguar dónde quedamos y lanza el thread (0 = todo ok):
int registro_init(void)
{	// Extraemos las partitions de la flash de su respectivo mapa en 
	// tendencia y en eventos:
    int err = flash_area_open(PARTITION_ID(tendencia_partition), &area_tendencia);

    if (err){
        LOG_ERR("no se pudo abrir la particion de tendencia (err %d)", err);
        return err;
    }

    err = flash_area_open(PARTITION_ID(eventos_partition), &area_eventos);
    if (err){
        LOG_ERR("no se pudo abrir la particion de eventos (err %d)", err);
        return err;
    }

    // El tamaño del "erase block" lo sacamos del propio driver:
    struct flash_pages_info info; // struct donde vamos a sacar la información
    const struct device *dev = flash_area_get_device(area_tendencia); // nrf52840

    err = flash_get_page_info_by_offs(dev, area_tendencia->fa_off, &info); // rellena info
    if (err){
        LOG_ERR("no se pudo leer la geometria de la flash (err %d)", err);
        return err;
    }
    bloque_borrado = info.size; // bloque de borrado definido por el driver

    // Cuántos espacios (páginas) caben en cada partición:
    paginas_totales = (uint16_t)(area_tendencia->fa_size / REG_PAGINA_BYTES);
    eventos_totales = (uint16_t)(area_eventos->fa_size / sizeof(struct reg_evento));

    buffer_reiniciar();

    err = localizar_cursores();
    if (err){
        return err;
    }

    LOG_INF("tendencia: %u/%u paginas, eventos: %u/%u, bloque de borrado %u B",
            cursor_pagina, paginas_totales, cursor_evento, eventos_totales,
            (unsigned)bloque_borrado);

			// Creación y registro del thread: 
    k_thread_create(&thread_registro, stack_registro, STACK_REGISTRO,
                    thread_escritor, NULL, NULL, NULL,
                    K_PRIO_PREEMPT(10), 0, K_NO_WAIT);
    k_thread_name_set(&thread_registro, "registro");

    return 0;
}

// Mete una orden en la queue sin esperar (K_NO_WAIT): el productor nunca se
// queda bloqueado, y si la queue está saturada se reporta y se pierde.
static int encolar(const struct cmd *c)
{	// Copia los valores del struct de command (c) al buffer del queue y no espera, 
	// arroja error al tiro
    int err = k_msgq_put(&ordenes_q, c, K_NO_WAIT);

    if (err){
        LOG_ERR("cola de registro saturada, orden tipo %d perdida", c->tipo);
    }

    return err;
}
//----------------------------------------------------
// Funciones: cada una solo arma su orden y la "encola".
int registro_tendencia(const struct reg_tendencia *r)
{
    struct cmd c = { .tipo = CMD_TENDENCIA, .tendencia = *r };
    return encolar(&c);
}

int registro_evento(const struct reg_evento *e)
{
    struct cmd c = { .tipo = CMD_EVENTO, .evento = *e };
    return encolar(&c);
}

int registro_flush(void)
{
    struct cmd c = { .tipo = CMD_FLUSH };
    return encolar(&c);
}

int registro_borrar(void)
{
    struct cmd c = { .tipo = CMD_BORRAR };
    return encolar(&c);
}
//----------------------------------------------------
// estado de la memoria para quien la pida (BLE por ejemplo):
void registro_leer_estado(struct registro_estado *e)
{
    e->paginas_usadas = cursor_pagina;
    e->paginas_totales = paginas_totales;
    e->eventos_usados = cursor_evento;
    e->eventos_totales = eventos_totales;
    e->en_buffer = num_registros;
    e->llena = memoria_llena;
}