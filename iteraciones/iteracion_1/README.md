# Iteración 1 - Concepto Inicial

## Información General
**Fecha:** 05/10/2026  
**Versión:** v1.0  
**Estado:** Concepto  
**Avance asociado:** Avance #1

---

## Descripción
Definición del problema (demora para estacionar en el Campus Peñalolén, ODS 11), investigación del contexto y propuesta conceptual: un sensor por espacio que indique en tiempo real si está libre u ocupado.

## Archivos en esta Carpeta
- `v1_referencia_conceptual.jpg` — Imagen de referencia usada para la idea inicial (3 espacios, sensores en un muro, display con espacios disponibles).
- `v1_modelo3d_inicial.step` — Primer modelo 3D (2 espacios, sensores por fuera del muro, bandeja de electrónica abierta).
- `v1_render_modelo_inicial.png` — Render del primer modelo.

## Problemas Identificados

1. **Sensor de movimiento**
   - Descripción: la primera idea fue un sensor de movimiento en el centro del espacio.
   - Causa: un auto estacionado no se mueve, así que el sensor dejaría de detectarlo; además, en el piso el auto lo pisaría.
   - Solución propuesta: sensor ultrasónico en un muro trasero (implementado en v2).

2. **Modelo 3D sin encapsulado real**
   - Descripción: la electrónica quedaba en una bandeja abierta de 22 mm de alto.
   - Causa: no se habían considerado las medidas reales ni los cables.
   - Solución propuesta: caja cerrada con tapa, diseñada con las medidas de los componentes (implementado en v2).

## Plan para Iteración 2
- [x] Armar el circuito con ESP32 y HC-SR04
- [x] Agregar conectividad para mostrar el estado fuera de la maqueta
- [x] Rediseñar el encapsulado
