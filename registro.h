/*
 * Memoria de registro: tendencia y eventos.
 *
 * Dos productores distintos, dos particiones distintas, como plantea el
 * documento. El temporizador periodico escribe tendencia (cadencia fija de
 * 60 s); el evaluador de condiciones escribe eventos (asincrono, ante
 * transiciones). No hay un proceso clasificador en medio.
 *
 * Todo el acceso a flash esta serializado en un unico hilo propio: las
 * funciones de esta interfaz encolan y regresan de inmediato, de modo que ni
 * el ciclo de medicion ni un callback GATT se quedan bloqueados los ~85 ms que
 * puede costar reciclar una pagina.
 */
#ifndef REGISTRO_H_
#define REGISTRO_H_

#include <zephyr/kernel.h>
#include <stdbool.h>
#include <stdint.h>

/* --- Bits de estado comunes a tendencia y eventos --- */
#define REG_FLAG_TIEMPO_RELATIVO BIT(0) /* sin sincronizar: ts es desde arranque */
#define REG_FLAG_FLUJO_BAJO      BIT(1)
#define REG_FLAG_FLUJO_ALTO      BIT(2)
#define REG_FLAG_TEMP_FUERA      BIT(3)
#define REG_FLAG_BURBUJA         BIT(4)
#define REG_FLAG_SENSOR_FALLA    BIT(5)

#define REG_MASCARA_ALARMAS \
	(REG_FLAG_FLUJO_BAJO | REG_FLAG_FLUJO_ALTO | REG_FLAG_TEMP_FUERA | \
	 REG_FLAG_BURBUJA | REG_FLAG_SENSOR_FALLA)

/*
 * Registro de tendencia: 9 bytes.
 *
 * Los tres CRC-8 de la trama I2C se verifican al leer y se descartan: la
 * integridad del dato ya quedo garantizada antes de escribirlo, y el CRC-16 de
 * la pagina cubre el bloque completo. En su lugar entra la marca temporal de
 * 4 bytes, que vuelve cada entrada autodescriptiva.
 */
struct __packed reg_tendencia {
	uint32_t ts;    /* Unix, o segundos desde arranque si REG_FLAG_TIEMPO_RELATIVO */
	int16_t  flujo; /* centesimas de ml/min, promedio del minuto */
	int16_t  temp;  /* centesimas de grado C, promedio del minuto */
	uint8_t  flags;
};
BUILD_ASSERT(sizeof(struct reg_tendencia) == 9, "el registro de tendencia debe ser de 9 B");

enum evento_tipo {
	EVT_ARRANQUE = 1,
	EVT_TIEMPO_SINCRONIZADO,
	EVT_INFUSION_INICIO,
	EVT_INFUSION_FIN,
	EVT_ALARMA_ACTIVA,
	EVT_ALARMA_LIBERADA,
	EVT_MEMORIA_LLENA,
	EVT_REGISTROS_BORRADOS,
};

/*
 * Registro de evento: 16 bytes con el contexto congelado del sistema en el
 * instante de la transicion.
 */
struct __packed reg_evento {
	uint32_t ts;
	uint8_t  tipo;       /* enum evento_tipo */
	uint8_t  flags;      /* estado de alarmas tras la transicion */
	int16_t  flujo;
	int16_t  temp;
	uint32_t volumen_cl; /* volumen acumulado, centesimas de ml */
	uint16_t crc;        /* CRC-16 de los 14 bytes anteriores */
};
BUILD_ASSERT(sizeof(struct reg_evento) == 16, "el registro de evento debe ser de 16 B");

/*
 * Pagina de tendencia: 28 registros de 9 B + secuencia + CRC-16 = 256 B justos.
 * Ningun registro queda partido entre paginas y cada bloque es verificable de
 * forma independiente.
 */
#define REG_POR_PAGINA   28
#define REG_PAGINA_BYTES 256

struct __packed pagina_tendencia {
	struct reg_tendencia reg[REG_POR_PAGINA];
	uint16_t seq;
	uint16_t crc; /* CRC-16 sobre los 254 bytes anteriores */
};
BUILD_ASSERT(sizeof(struct pagina_tendencia) == REG_PAGINA_BYTES, "la pagina debe ser de 256 B");

/* Una pagina sin escribir queda en 0xFFFF; la secuencia nunca toma ese valor. */
#define REG_SEQ_VACIA 0xFFFFU
/* Ranura de relleno en una pagina cerrada a medias: ts a 0xFFFFFFFF. */
#define REG_TS_VACIO  0xFFFFFFFFU

struct registro_estado {
	uint16_t paginas_usadas;
	uint16_t paginas_totales;
	uint16_t eventos_usados;
	uint16_t eventos_totales;
	uint8_t  en_buffer; /* registros en RAM aun no escritos a flash */
	bool     llena;
};

int registro_init(void);

/* Deposita un registro de 9 B en el buffer en RAM. Al completar 28 registros
 * (~28 min) el buffer llena una pagina de 256 B y se escribe a flash.
 */
int registro_tendencia(const struct reg_tendencia *r);

/* Escribe un evento y fuerza el volcado del buffer parcial de tendencia, para
 * preservar los minutos previos al incidente.
 */
int registro_evento(const struct reg_evento *e);

/* Cierra la pagina en curso rellenando las ranuras libres. La pagina queda
 * consumida: lo que siga va a la siguiente.
 */
int registro_flush(void);

/* Vaciado de los registros, una vez confirmada la descarga. */
int registro_borrar(void);

void registro_leer_estado(struct registro_estado *e);

#endif /* REGISTRO_H_ */