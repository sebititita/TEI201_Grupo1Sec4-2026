# Archivos Fusion 360

## `modelo_v2_ESP32.step`
Ensamble completo del prototipo v2 (formato STEP, se abre directamente en Fusion 360 con *Archivo → Abrir*). Cada pieza tiene nombre propio y aparece como componente separado.

### Componentes del ensamble
```
Estacionamiento_v3_ESP32
├── 01_Base
├── 02_Encapsulado_muro_frontal (agujeros sensores + ventana display)
├── 03_Encapsulado_lateral_izq
├── 04_Encapsulado_lateral_der_con_pasacable_USB
├── 05_Encapsulado_trasera_con_ventilacion
├── 06_Tapa_removible_atornillada
├── Poste_tapa_1..4
├── HC_SR04_P1 / HC_SR04_P2 (placa, emisor, receptor, pines)
├── Display_7seg (cuerpo, frente, pines)
├── ESP32 (placa, módulo WiFi, conector USB, headers, separadores)
├── Protoboard_400 + resistencias
├── Lineas, topes de rueda e identificación P1/P2
└── Auto_referencia_HotWheels (solo referencia de escala)
```

## Versiones

| Versión | Fecha | Cambios Principales |
|---------|-------|---------------------|
| v2.0 | 19/10/2026 | Caja cerrada con tapa, sensores por dentro del muro, medidas reales, ESP32 con agujero USB |
| v1.0 | 05/10/2026 | Primer modelo: muro con sensores por fuera y bandeja de electrónica abierta (ver `iteraciones/iteracion_1/`) |

**Pendiente:** guardar el ensamble como `.f3d` con historial (timeline) para el Avance #3.
