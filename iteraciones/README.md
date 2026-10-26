# Historial de Iteraciones

## Estructura
```
iteraciones/
├── iteracion_1/    # Concepto inicial (Avance #1)
├── iteracion_2/    # Prototipo Alpha IoT (Avance #2)  ← esta entrega
└── iteracion_3/    # Versión final (Avance #3) — pendiente
```

`comparativa_v1_v2_diseno3d.png` muestra lado a lado el modelo 3D inicial y el actual.

---

## Tabla Comparativa de Iteraciones

| Aspecto | v1 (Avance #1) | v2 (Avance #2) | v3 |
|---------|----|----|----|
| **Fecha** | 05/10/2026 | 19/10/2026 | Pendiente |
| **Sensores** | Idea: sensor de movimiento en el piso → cambiado a ultrasónico | 1 × HC-SR04 funcionando (2° sensor adquirido, no integrado) | 2 × HC-SR04 |
| **Actuadores** | Display / LEDs (concepto) | 1 segmento del display = OCUPADO | Por definir |
| **Conectividad** | Ninguna | WiFi propia del ESP32 + página web en el celular | WiFi del campus / página central |
| **Encapsulado** | ❌ (bandeja abierta en el modelo) | Modelo 3D conceptual (caja cerrada) | Finalizado |
| **Testing** | — | Pruebas del equipo (ver `testing/`) | 5+ usuarios |

---

## Evolución del Diseño

### Versión 1 — Concepto inicial
**Fecha:** 05/10/2026 · **Estado:** concepto (Avance #1)
- Problema definido: demora para encontrar estacionamiento en el Campus Peñalolén (ODS 11).
- Idea inicial: un sensor de movimiento al centro de cada espacio, en el piso.
- Primer modelo 3D: muro con sensores por fuera y electrónica en una bandeja abierta detrás.

### Versión 2 — Prototipo Alpha IoT
**Fecha:** 19/10/2026 · **Estado:** prototipo funcional de 1 espacio

**Mejoras implementadas:**
- Sensor ultrasónico HC-SR04 montado en un muro trasero, apuntando al auto.
- ESP32 con red WiFi propia y página web que muestra LIBRE/OCUPADO y la distancia en tiempo real.
- Divisor de voltaje para proteger el ESP32.
- Encapsulado rediseñado como caja cerrada con medidas reales.

**Problemas pendientes:**
- Integrar el segundo sensor (faltan cables macho-hembra y acceso a pines del ESP32).
- Mostrar el número de espacios disponibles (hoy se usa un solo segmento como indicador).
- Registrar pruebas cuantitativas (precisión, estabilidad durante 5 minutos).

---

## Justificación de Cambios

### Hardware
**De v1 a v2:**
- Cambio: sensor de movimiento → sensor ultrasónico HC-SR04.
- Razón: un sensor de movimiento detecta cambios; un auto estacionado no se mueve y dejaría de detectarse. El ultrasónico mide distancia, esté el auto quieto o no.
- Cambio: sensor en el piso → sensor en un muro trasero.
- Razón: en el piso el auto lo pisaría.

### Software
**De v1 a v2:**
- Lectura del HC-SR04 con umbral de 15 cm y manejo de "sin lectura" (timeout).
- Servidor web en el ESP32 con actualización cada 1 segundo.

### Diseño 3D
**De v1 a v2:**
- Bandeja abierta de 22 mm de alto → caja cerrada de 60 mm de alto (en la bandeja no cabían los cables).
- Sensores completos por fuera del muro → sensores por dentro; solo asoman los transductores.
- Se agregaron tapa removible, ventilación y agujero para el USB.

---

## Incorporación de Feedback

### Feedback del profesor (correo previo al Avance #2) → Cambios en v2
1. Una conexión por USB a un computador no es factible para un estacionamiento con 50–100 espacios; el sistema debe tener lógica de expansión.
   - Acción tomada: se usó el ESP32 con WiFi y una página web, y se definió la escalabilidad por nodos (un ESP32 por grupo de espacios conectado a la red del campus). Requisito de instalación: cobertura WiFi.

---

## Lecciones Aprendidas

### Técnicas
- Proteger las entradas de 3,3 V del ESP32 cuando un sensor trabaja a 5 V.
- Probar cada componente por separado (sensor, display, WiFi) antes de integrarlos.

### De Proceso
- Diseñar el encapsulado con las medidas reales de los componentes, no con valores genéricos.

### De Trabajo en Equipo
- Coordinar antes los materiales (cables macho-hembra) para no limitar el alcance del prototipo.
