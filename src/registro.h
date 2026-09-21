#ifndef REGISTRO_H_
#define REGISTRO_H_

#include <zephyr/kernel.h>
#include <stdbool.h>
#include <stdint.h>

#define REG_FLAG_TIEMPO_RELATIVO BIT(0) // Propio nuestro: tiempo no sincronizado
#define REG_FLAG_FLUJO_BAJO BIT(1)
#define REG_FLAG_FLUJO_ALTO BIT(2)
#define REG_FLAG_TEMP_FUERA BIT(3)
#define REG_FLAG_BURBUJA BIT(4)
#define REG_FLAG_SENSOR_FALLA BIT(5)

#define REG_MASCARA_ALARMAS \
	(REG_FLAG_FLUJO_BAJO | REG_FLAG_FLUJO_ALTO | REG_FLAG_TEMP_FUERA | \
	 REG_FLAG_BURBUJA | REG_FLAG_SENSOR_FALLA)


// Registro de tendencia: 9 bytes.
struct __packed reg_tendencia {
	uint32_t ts;    // Unix, o segundos desde arranque si REG_FLAG_TIEMPO_RELATIVO
	int16_t  flujo; // centesimas de ml/min, promedio del minuto 
	int16_t  temp;  // centesimas de grado C, promedio del minuto
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

//Registro de evento: 16 bytes con el contexto del sistema en el
// instante de la transicion.
struct __packed reg_evento {
	uint32_t ts;
	uint8_t  tipo; // enum evento_tipo */
	uint8_t  flags; // estado de alarmas tras la transicion */
	int16_t  flujo;
	int16_t  temp;
	uint32_t volumen_cl; //volumen acumulado, centesimas de ml
	uint16_t crc; // CRC-16 de los 14 bytes anteriores 
};
BUILD_ASSERT(sizeof(struct reg_evento) == 16, "el registro de evento debe ser de 16 B");

 // Pagina de tendencia: 28 registros de 9 B + secuencia + CRC-16 = 256 B.
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

// Esto se usa para entregar información a BLE de la memoria:
struct registro_estado {
	uint16_t paginas_usadas;
	uint16_t paginas_totales;
	uint16_t eventos_usados;
	uint16_t eventos_totales;
	uint8_t  en_buffer; /* registros en RAM aun no escritos a flash */
	bool     llena;
};

int registro_init(void);

// Mete un registro de 9B en un buffer en la RAM y cuando completa 28 registros, 
// el buffer escribe 256B en la flash. 
int registro_tendencia(const struct reg_tendencia *r);

//Escribe un evento y fuerza escritura del buffer parcial de tendencia, para
//preservar los minutos previos al incidente.
int registro_evento(const struct reg_evento *e);

int registro_flush(void);

// Se borran los registros cuando se descarguen los datos desde la app. 
int registro_borrar(void);

void registro_leer_estado(struct registro_estado *e);

#endif /* REGISTRO_H_ */