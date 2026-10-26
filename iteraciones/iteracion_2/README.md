# Iteración 2 - Prototipo Alpha IoT

## Información General
**Fecha:** 19/10/2026  
**Versión:** v2.0  
**Estado:** Prototipo funcional (1 espacio)  
**Avance asociado:** Avance #2

---

## Descripción
Prototipo con un sensor HC-SR04 y un ESP32 que detecta si hay un objeto a menos de 15 cm, enciende un segmento del display como indicador de OCUPADO y publica el estado en una página web a través de su propia red WiFi.

---

## Cambios Respecto a v1

### Hardware
**Agregado:**
- ESP32 Dev Module (microcontrolador con WiFi)
- Sensor ultrasónico HC-SR04
- Divisor de voltaje en ECHO (3 × 10 kΩ)
- Display de 7 segmentos en un segundo protoboard
- Power bank 5000 mAh

**Modificado:**
- Sensor de movimiento → ultrasónico — Razón: detectar autos detenidos.

### Software
**Nuevas funcionalidades:**
- Lectura de distancia y decisión ocupado/libre (umbral 15 cm)
- Red WiFi propia (`Estacionamiento_UAI`) y página web en `192.168.4.1`
- Estado en JSON (`/estado`) y actualización automática cada 1 segundo

### Diseño Mecánico
- Encapsulado rediseñado en 3D (ver `diseno_3d/`); no se imprimió (no requerido en el Avance #2).

---

## Problemas encontrados y soluciones (registro del armado)

| Problema | Solución |
|---|---|
| El botón BOOT del ESP32 estaba defectuoso y no se podía cargar el código | Puente temporal GPIO0–GND durante la carga; se retira al ejecutar |
| La salida ECHO del HC-SR04 entrega 5 V y el ESP32 acepta 3,3 V | Divisor de voltaje: 10 kΩ de ECHO a GPIO19 y 20 kΩ (2 × 10 kΩ) de GPIO19 a GND (≈ 3,3 V) |
| Había que definir cuándo un espacio está ocupado | Pruebas con el Monitor Serie acercando y alejando un objeto; se fijó el umbral en 15 cm |
| El display no cabía en el protoboard principal | Se montó en un segundo protoboard transparente |
| Un segmento del display no encendía | Se probaron los segmentos uno por uno; había una conexión fuera de fila y se reubicó la resistencia |
| No alcanzaban los cables para formar números | Se usa un solo segmento como indicador (encendido = ocupado) |
| El segmento estaba conectado fijo a 3V3 (siempre encendido) | Se pasó a controlarlo desde GPIO25 según la lectura del sensor |
| Solo se veían los datos por cable (Monitor Serie) | El ESP32 crea su propia red WiFi y muestra el estado en el celular |
| No se pudo integrar el segundo sensor | Faltan cables macho-hembra para los pines del ESP32; queda para v3 |

`v2_notas_armado.pdf` contiene el registro detallado del armado (escrito antes de agregar la conectividad WiFi).

---

## Resultados de Testing

### Funcionalidad Lograda
✅ Detección de objeto y decisión ocupado/libre  
✅ Indicador físico (segmento del display)  
✅ Página web en el celular por WiFi propia del ESP32  
⚠️ Segundo espacio: sensor adquirido, no integrado  
⚠️ Número de espacios disponibles: pendiente

### Evidencia
Capturas de la página web en `testing/evidencias/fotos/`: **LIBRE** con 50,1 cm y **OCUPADO** con 5,0 cm.

---

## Plan para Iteración 3 (Versión Final)

**Hardware:**
- [ ] Integrar el segundo HC-SR04 (TRIG GPIO32, ECHO GPIO33, con su propio divisor)
- [ ] Conectores seguros (cables macho-hembra o placa soldable)

**Software:**
- [ ] Leer ambos sensores en secuencia y calcular espacios disponibles (2 − ocupados)
- [ ] Mostrar el número en el display y en la página

**Diseño:**
- [ ] Guardar el modelo como `.f3d` y generar planos técnicos
- [ ] Evaluar impresión 3D del encapsulado

**Testing:**
- [ ] Prueba de estabilidad de 5+ minutos y precisión de las lecturas
- [ ] Testing con 5+ usuarios
