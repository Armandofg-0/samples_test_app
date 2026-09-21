#ifndef SENSOR_H_
#define SENSOR_H_

#include <zephyr/kernel.h>
#include <stdint.h>

// El sensor transmite 6 bytes de información 3 de redundancia
#define SENSOR_TRAMA_BYTES 9

// Definición de Macros para comandos importantes:
#define SENSOR_CMD_MEDIR_CONTINUO 0x3608U
#define SENSOR_CMD_DETENER 0x3FF9U

// Factor de escala para transformar las mediciones
#define SENSOR_ESCALA_FLUJO 500
#define SENSOR_ESCALA_TEMP 200

// Bits de la palabra de estado que devuelve el sensor: 
#define SENSOR_FLAG_BURBUJA BIT(0)
#define SENSOR_FLAG_FLUJO_ALTO BIT(1)

struct medicion {
    uint16_t flujo;
    uint16_t temp;
    uint16_t flags;
};

// Funciones que implementa la simulación del sensor (eventualmente
// el código que implemente el i2c)
int sensor_tx_init(void);
int sensor_tx_cmd(uint16_t cmd);
int sensor_tx_trama(uint8_t trama[SENSOR_TRAMA_BYTES]);

// Funciones para activar funcionalidades base del sensor:
int sensor_init(void);
int sensor_iniciar_continuo(void);
int sensor_detener(void);

// Lee la trama, detecta errores y devuelve la medición:
int sensor_leer(struct medicion *m);

uint8_t sensor_crc8(const uint8_t *datos, size_t len);
int sensor_decodificar(const uint8_t trama[SENSOR_TRAMA_BYTES],
struct medicion *m);

#endif