# Hardware - Documentación

## Contenido
- `esquemas/` — Diagrama de conexiones del prototipo v2 (ESP32 + HC-SR04 + display)
- `bom/` — Lista de materiales
- `fotos/` — Fotografías del prototipo armado

---

## Componentes Principales
1. **ESP32 Dev Module (38 pines)** — Microcontrolador: lee el sensor, decide si el espacio está ocupado, controla el indicador y crea la red WiFi + página web.
2. **Sensor ultrasónico HC-SR04** — Mide la distancia al vehículo (rango 2–400 cm según su hoja de datos).
3. **Display de 7 segmentos Kingbright DA56-11HDB** — Se usa **un segmento como indicador**: encendido = OCUPADO.
4. **Divisor de voltaje (3 × 10 kΩ)** — Protege la entrada del ESP32: el pin ECHO del sensor entrega 5 V y el ESP32 acepta 3,3 V.
5. **Power bank 5 V (5000 mAh)** — Alimentación por USB, sin depender de un computador.

## Pasos de Ensamblaje
1. Conectar VCC del HC-SR04 al pin 5V del ESP32 y GND a GND (tierra común).
2. Conectar TRIG del HC-SR04 al GPIO18.
3. Conectar ECHO del HC-SR04 al GPIO19 **a través del divisor**: 10 kΩ entre ECHO y GPIO19, y 20 kΩ (2 × 10 kΩ en serie) entre GPIO19 y GND.
4. Conectar GPIO25 al ánodo común del display; el pin del segmento usado va a GND a través de una resistencia de 10 kΩ.
5. Alimentar el ESP32 por USB (power bank o computador).

## Precauciones
- ⚠️ **Nunca conectar ECHO directo al ESP32**: sin el divisor, los 5 V pueden dañar el pin.
- ⚠️ El botón BOOT de nuestro ESP32 está defectuoso: para cargar código se hace un puente temporal entre GPIO0 y GND, y se retira al ejecutar.
- ⚠️ Fijar protoboards y cables antes de transportar o presentar (los Dupont se sueltan con facilidad).

---

## Especificaciones Técnicas
**Alimentación:** 5 V por USB (power bank de 5000 mAh)  
**Conectividad:** WiFi del ESP32 en modo punto de acceso (red propia, IP 192.168.4.1)  
**Umbral de detección:** < 15 cm = OCUPADO (ajustado para la maqueta)

---

## Troubleshooting

| Problema | Posible Causa | Solución |
|----------|---------------|----------|
| No carga el código al ESP32 | Botón BOOT defectuoso | Puente temporal GPIO0–GND durante la carga |
| El segmento del display no enciende | Conexión fuera de fila en el protoboard | Revisar fila del pin y reubicar la resistencia (ver `iteraciones/iteracion_2`) |
| La página muestra "SIN LECTURA" | El sensor no recibe eco (objeto fuera de rango o cable suelto) | Revisar cables TRIG/ECHO y el divisor |
| La página muestra "SIN CONEXIÓN" | El celular se desconectó de la red del ESP32 | Reconectar a la red Estacionamiento_UAI |
