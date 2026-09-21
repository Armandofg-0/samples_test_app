#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/clock.h>
#include <time.h>
#include "tiempo.h"

// Para printear información (por eso log_inf)
LOG_MODULE_REGISTER(tiempo, LOG_LEVEL_INF);

static bool sincronizado; // flag de sincronización
//----------------------------------------------------
// El sistema inicia no sincronizado (0 = todo ok)
int tiempo_init(void)
{
    sincronizado = false;
    return 0;
}
//----------------------------------------------------
int sincronizar_tiempo(uint64_t unix_ms)
{
    // declaramos el tiempo en segundos y en nanosegundos
    // en el struct tipo timespec que time.h trae: 
    struct timespec ts = {
        .tv_sec = (time_t)(unix_ms / MSEC_PER_SEC),
        .tv_nsec = (long)((unix_ms % MSEC_PER_SEC) * NSEC_PER_MSEC),
    };

    // anclaje del contador a la hora dada (OJO FUNCIÓN)
    int err = sys_clock_setsystem(SYS_CLOCK_REALTIME, &ts);

    if (err){// err = 1 en caso de fallar.
        LOG_ERR("sys_clock_settime fallo (err %d)", err);
        return err; 
    }

    sincronizado = true; // subimos el flag y reportamos
    LOG_INF("hora sincronizada: %llu s", (unsigned long long)ts.tv_sec);
    return 0;
}
//----------------------------------------------------
// Función a futuro: Tal vez introducir algún criterio. 
bool tiempo_valido(void)
{
    return sincronizado;
}
//----------------------------------------------------
uint32_t timestamp(void)
{
    struct timespec ts; // Declaración del struct
    // Si está sincronizado y puede medir tiempo interno:
    if (sincronizado && sys_clock_gettime(SYS_CLOCK_REALTIME, &ts)==0){
        return (uint32_t)ts.tv_sec; // Tiempo real
    }
    // Si todavía no está sincronizado, usa segundos desde arranque
    return (uint32_t)(k_uptime_get()/MSEC_PER_SEC);
}