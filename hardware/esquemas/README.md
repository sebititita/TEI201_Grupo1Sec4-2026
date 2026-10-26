# Esquemas de Circuito

## Archivos
- `v2_diagrama_conexiones_esp32.png` — Diagrama de conexiones del prototipo v2 (1 espacio)

## Tabla de pines

| Componente | Pin del componente | Pin del ESP32 | Nota |
|---|---|---|---|
| HC-SR04 | VCC | 5V | Alimentación del sensor |
| HC-SR04 | GND | GND | Tierra común |
| HC-SR04 | TRIG | GPIO18 | Pulso de disparo (salida del ESP32) |
| HC-SR04 | ECHO | GPIO19 | **A través del divisor**: 10 kΩ ECHO→GPIO19, 20 kΩ GPIO19→GND |
| Display 7 seg. | Ánodo común | GPIO25 | HIGH = segmento encendido = OCUPADO |
| Display 7 seg. | Segmento usado | GND vía 10 kΩ | Limita la corriente del segmento |
| Power bank | USB | Conector USB del ESP32 | 5 V |

## Notas de Diseño
- **Divisor de voltaje:** el HC-SR04 trabaja a 5 V y su salida ECHO entrega 5 V, pero el ESP32 acepta 3,3 V. Con 10 kΩ y 20 kΩ: 5 V × 20k / (10k + 20k) ≈ 3,3 V.
- **Un solo segmento como indicador:** por falta de cables y salidas libres, se usa un segmento del display (encendido = ocupado) en vez de dibujar números. Mostrar el número de espacios disponibles queda para la versión final.
- **Dos protoboards:** el display no cabía en el protoboard principal, por eso va en un segundo protoboard transparente.
- **Segundo sensor (próxima versión):** pines propuestos TRIG → GPIO32 y ECHO → GPIO33, con su propio divisor de voltaje. Pendiente de confirmar en el montaje.
