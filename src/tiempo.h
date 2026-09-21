#ifndef TIEMPO_H_
#define TIEMPO_H_

#include <stdbool.h>
#include <stdint.h>

int tiempo_init(void);
int sincronizar_tiempo(uint64_t unix_ms);
bool tiempo_valido(void);
uint32_t timestamp(void); 

#endif 
