# Envío BLE mínimo — XIAO nRF52840

Firmware para probar la transmisión hacia una aplicación sin tener todavía el
sensor. No lee hardware, no usa GPIO y no guarda nada: solo anuncia y notifica.

## Qué hace

1. Se anuncia como `XIAO-FLOW` (conectable).
2. Cuando la app habilita las notificaciones, envía un paquete por segundo con
   caudal y temperatura simulados (una onda suave alrededor de 5.00 ml/min y
   25.00 °C).

## Servicio GATT

| | UUID |
| --- | --- |
| Servicio | `f1a70001-9b4c-4f3e-8b6d-1c2a3d4e5f60` |
| Datos (notify) | `f1a70002-9b4c-4f3e-8b6d-1c2a3d4e5f60` |

Paquete de 10 bytes, little-endian:

| Offset | Campo | Tipo | Escala |
| --- | --- | --- | --- |
| 0 | `seq` | `uint16` | número de secuencia, detecta notificaciones perdidas |
| 2 | `ts` | `uint32` | segundos desde el arranque |
| 6 | `flow` | `int16` | centésimas de ml/min (`500` = 5.00 ml/min) |
| 8 | `temp` | `int16` | centésimas de °C (`2500` = 25.00 °C) |

En Python: `struct.unpack("<HIhh", data)`.

## Compilar y grabar

Verificado con nRF Connect SDK v3.4.0 (Zephyr 4.4): 150 KB de flash, 33 KB de RAM.

```sh
west build -p -b xiao_ble/nrf52840 xiao_flow_sim
```

Variante Sense: `-b xiao_ble/nrf52840/sense`.

El XIAO trae el bootloader de Adafruit, no hace falta depurador:

1. Doble pulsación rápida del reset → aparece una unidad USB (`XIAO-SENSE` / `XIAO_BOOT`).
2. Copiar `build/xiao_flow_sim/zephyr/zephyr.uf2` a esa unidad; la placa se reinicia sola.

Los logs salen por el puerto USB CDC.

## Probar con nRF Connect for Mobile

1. Scan → conectar a `XIAO-FLOW`.
2. Abrir el servicio `f1a70001-...` y pulsar el icono de flechas (notificaciones)
   en la característica `f1a70002-...`.
3. Llega un valor de 10 bytes por segundo. nRF Connect lo muestra en hexadecimal:
   por ejemplo `0A 00 2C 00 00 00 F4 01 C4 09` = seq 10, ts 44 s, flujo 5.00
   ml/min, temp 25.00 °C.
