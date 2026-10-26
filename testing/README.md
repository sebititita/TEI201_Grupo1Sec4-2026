# Testing y Validación

## Estado en el Avance #2
En esta etapa se realizaron **pruebas funcionales del equipo** (no con usuarios externos). El testing con 5+ usuarios corresponde al Avance #3.

## Pruebas realizadas (v2)
| Prueba | Cómo se hizo | Resultado |
|---|---|---|
| Lectura del sensor | Monitor Serie (115200 baudios) acercando y alejando un objeto | Lecturas de distancia consistentes; se fijó el umbral en 15 cm |
| Indicador físico | Objeto a menos de 15 cm / retirado | Segmento del display encendido (ocupado) / apagado (libre) |
| Conectividad | Celular conectado a la red `Estacionamiento_UAI`, página en 192.168.4.1 | La página muestra LIBRE (50,1 cm) y OCUPADO (5,0 cm), actualizándose sola |
| Alimentación portátil | ESP32 conectado a power bank 5000 mAh | Funcionó alimentado solo por la power bank, sin computador |

## Evidencias
`evidencias/fotos/`:
- `v2_pagina_web_LIBRE.png` — Estado LIBRE, distancia 50,1 cm
- `v2_pagina_web_OCUPADO.png` — Estado OCUPADO, distancia 5,0 cm

## Pendiente (Avance #3)
- Tabla de precisión (distancia real vs. medida) y número de repeticiones
- Prueba de estabilidad de 5+ minutos registrada
- Testing con 5+ usuarios del grupo objetivo (estudiantes que llegan en auto)
