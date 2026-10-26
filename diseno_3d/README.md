# Diseño 3D - Documentación

## Concepto del encapsulado
Toda la electrónica queda **dentro de una caja detrás del muro del estacionamiento**. Hacia afuera solo se asoman los transductores del sensor (por agujeros en el muro) y el display (por una ventana). Así el auto nunca toca los componentes y el sistema queda protegido. En una instalación real, la caja iría montada frente a cada grupo de espacios.

El modelo v2 incluye: muro frontal, laterales, pared trasera, tapa removible, postes de fijación, **ESP32**, **protoboard**, **2 sensores HC-SR04** (versión de 2 espacios), **display de 7 segmentos**, resistencias, separadores y un auto Hot Wheels de referencia.

---

## Carpetas

#### `fusion360/`
`modelo_v2_ESP32.step` — Ensamble completo con todos los componentes (se abre en Fusion 360 con *Archivo → Abrir*).

#### `renders/`
- `01_vista_general_v2.png` — Encapsulado cerrado
- `02_interior_sin_tapa_v2.png` — Interior sin tapa: ubicación de ESP32, protoboard, sensores y display
- `03_corte_lateral_v2.png` — Corte lateral: altura del sensor y distancia al auto

#### `planos/`
Pendiente para el Avance #3 (ver medidas abajo).

---

## Especificaciones

**Software:** Autodesk Fusion 360 (modelo exportado en formato STEP)  
**Unidades:** Milímetros (mm)  
**Escala:** 1:1 (dimensiones de la maqueta)

**Dimensiones externas de la caja:** 190 mm × 75 mm × 63 mm (60 mm de paredes + 3 mm de tapa)  
**Interior útil:** 184 mm × 67 mm × 60 mm  
**Base de la maqueta:** 190 mm × 235 mm × 5 mm (2 espacios de 60 × 130 mm)  
**Material propuesto:** PLA para la maqueta (barato y fácil de imprimir); PETG para la versión en exterior (resiste mejor sol, calor y humedad)

| Elemento | Medida |
|---|---|
| Muro frontal | 5 mm de espesor |
| Laterales, pared trasera y tapa | 3 mm de espesor |
| Agujeros de los sensores | 4 × Ø17 mm (2 por sensor), 26 mm entre centros, centro a 12 mm del piso |
| Ventana del display | 13,4 × 19,8 mm |
| Agujero USB del ESP32 | 14 × 10 mm (lateral derecho) |
| Ventilación | 5 ranuras de 3 × 28 mm en la tapa y 6 ranuras de 4 × 20 mm en la pared trasera |
| Fijación de la tapa | 4 postes de 7 × 7 mm con tornillos M3 |
| ESP32 | Montado sobre separadores de 8 mm |
| Distancia auto–sensor | 40 mm con el auto contra el tope de rueda (el HC-SR04 mide desde 2 cm) |

Las medidas del HC-SR04 (45 × 20 mm, transductores Ø16 mm que sobresalen 12 mm), del protoboard (83 × 54 × 9 mm) y del módulo de display se tomaron midiendo nuestros componentes. Las medidas del auto Hot Wheels son estimadas (~75 × 32 × 25 mm).

---

## Consideraciones de Diseño

### Funcionales
- [x] Acceso a puertos USB/alimentación (agujero lateral alineado con el conector del ESP32)
- [x] Ventilación adecuada (ranuras en tapa y pared trasera)
- [x] Protección de componentes (sensor montado por dentro; solo asoman los transductores)
- [x] Fácil acceso para mantenimiento (tapa removible con 4 tornillos)
- [ ] Resistencia a condiciones ambientales (versión exterior en PETG con sellos: pendiente)

### Justificación de medidas
- **Altura de 60 mm:** permite que entren los cables Dupont conectados sobre el protoboard (en el modelo inicial la bandeja tenía 22 mm y no cabían).
- **Sensor a 12 mm del piso:** apunta de frente a un auto de juguete bajo.
- **Separación de 40 mm entre el auto y el sensor:** el HC-SR04 no mide bien a menos de 2 cm.
