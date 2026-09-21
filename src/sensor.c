#include <zephyr/logging/log.h>
#include "sensor.h"

LOG_MODULE_REGISTER(sensor, LOG_LEVEL_INF);
//----------------------------------------------------
// Función para hacer el cálculo del CRC: 
uint8_t sensor_crc8(const uint8_t *datos, size_t len)
{
    uint8_t crc = 0xFF; // Valor inicial del registro

    for (size_t i = 0; i < len; i++){
        crc ^= datos[i]; // Operación XOR elemento a elemento
        for (int bit = 0; bit < 8; bit++){ // Recorrido del byte
            // Si el MSB = 1: SR y XOR con 0x31 (del datasheet), si no, solo SR
            crc = (crc & 0x80) ? (uint8_t)((crc << 1) ^ 0x31) : (uint8_t)(crc << 1);
        }
    }
    return crc;
}
//----------------------------------------------------
// Función para decodificar información y detectar errores:
int sensor_decodificar(const uint8_t trama[SENSOR_TRAMA_BYTES], struct medicion *m)
{
    for (int campo = 0; campo < 3; campo ++){ // hay 3 campos en total, cada uno con 2 datos y un crc
        const uint8_t *puntero = &trama[campo * 3]; 
        if (sensor_crc8(puntero, 2) != puntero[2]) {// crc del campo
            LOG_WRN("CRC-8 invalido en el campo %d", campo);
            return -EIO;
        }
    }
    // Temperatura y flujo están codificadas en 16 bits:
    int16_t flujo_raw = (uint16_t)(((uint16_t)trama[0]<<8) | trama[1]);
    int16_t temp_raw = (uint16_t)(((uint16_t)trama[3]<<8) | trama[4]);

    // Transformación de los valores a unidades físicas (transformación dada por el datasheet):
	m->flujo = (int16_t)(((int32_t)flujo_raw * 100) / SENSOR_ESCALA_FLUJO);
	m->temp  = (int16_t)(((int32_t)temp_raw * 100) / SENSOR_ESCALA_TEMP);
	m->flags = ((uint16_t)trama[6] << 8) | trama[7];

    return 0; // mítico
}
//----------------------------------------------------
// Funciones para inicializar y enviar comandos:
int sensor_init(void)
{
	return sensor_tx_init();
}

int sensor_iniciar_continuo(void)
{
	return sensor_tx_cmd(SENSOR_CMD_MEDIR_CONTINUO);
}

int sensor_detener(void)
{
	return sensor_tx_cmd(SENSOR_CMD_DETENER);
}

int sensor_leer(struct medicion *m)
{
	uint8_t trama[SENSOR_TRAMA_BYTES];
	int err = sensor_tx_trama(trama);

	if (err) {
		return err;
	}

	return sensor_decodificar(trama, m);
}