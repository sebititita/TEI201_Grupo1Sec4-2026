# Iteración 3 — Prototipo IoT completo (Avance #3)

**Fecha:** noviembre 2026 · **Estado:** prototipo funcional de 1 espacio, integrado en maqueta MDF

## Cambios respecto a la iteración 2

| Aspecto | Iteración 2 | Iteración 3 | Por qué |
|---|---|---|---|
| Salida | 1 segmento del display + página web simple | Solo dashboard web; **display retirado** (GPIO25 libre) | El dashboard entrega toda la información; el segmento no aportaba más y ocupaba pines y cableado |
| Datos | Solo en tiempo real | **Historial en `historial.csv`** (LittleFS), persistente al reiniciar, descargable | La rúbrica exige almacenamiento; el historial permite medir el uso del espacio |
| Información | LIBRE/OCUPADO + distancia | Espacios disponibles, % de ocupación, tiempo ocupado, n° de ocupaciones, gráfico | Convertir datos en información útil |
| Encapsulado | Modelo 3D conceptual (caja tras un muro) | **Maqueta MDF 5,5 mm cortada con láser**: poste, caja del sensor con visera, caja central con tapa encastrada | Construcción rápida en el DEC; tapas desmontables para reparación |
| Alimentación | Power bank | Power bank dentro de la caja central | Maqueta autónoma |

## Problemas y soluciones

| Problema | Solución |
|---|---|
| «Almacenamiento no disponible» al descargar el CSV | `LittleFS.begin(true)` para formatear la partición si no se puede montar |
| La primera caja central no dejaba espacio para el enchufe USB del power bank | Se midió el power bank (10 × 6,5 × 1,5 cm) y se rediseñó la caja (145 × 135 × 50 mm) |
| Las piezas no cabían en el MDF disponible (35 × 40 cm) | Caja central más compacta, poste de 160 → 140 mm, base cortada aparte |
| Botón BOOT dañado | Puente GPIO0–GND durante la carga |
| Lecturas atípicas puntuales del sensor | Identificado; mejora propuesta: mediana de varias lecturas + histéresis |

## Aprendizajes

- Medir los componentes reales **antes** de cortar: una sola medida (el power bank) obligó a rediseñar la caja.
- Una sola lectura del ultrasónico puede fallar. Para contar ocupaciones de forma confiable se necesita filtrar.
- El almacenamiento local (LittleFS) permite tener historial sin internet, pero sin fecha y hora reales.

Archivos de esta iteración: `firmware/`, `hardware/`, `diseño-3d/` y `testing/` en la raíz del repositorio.
