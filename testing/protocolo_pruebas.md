# Protocolo de pruebas — Avance 3

**Sistema probado:** ESP32 + HC-SR04 con el firmware de `firmware/codigosensorwifi/` (umbral de ocupado = 15 cm), montado en la maqueta MDF y alimentado con power bank.
**Fecha:** noviembre 2026 · **Responsable de testing:** Mariano Pedrosa (con el equipo).
**Acceso:** celular conectado a la red `Estacionamiento_UAI`, dashboard en `http://192.168.4.1`.

| # | Prueba | Objetivo | Procedimiento | Criterio de éxito (referencia para evaluar) |
|---|---|---|---|---|
| P1 | **Detección** | Verificar que el sistema clasifica bien LIBRE/OCUPADO | 1. Poner un objeto frente al sensor, dentro del espacio (a menos de 15 cm). 2. Retirarlo. 3. En cada caso, anotar lo que muestra el dashboard. Repetir **20 veces**. | ≥ 90 % de clasificaciones correctas |
| P2 | **Precisión de distancia** | Medir el error del HC-SR04 en el rango de la maqueta | Poner un objeto a distancias de referencia de **5, 10, 15, 20 y 30 cm** del sensor y anotar la distancia que muestra el dashboard. Error absoluto = \|medida − real\|. | Error ≤ 3 cm en todo el rango. Sin error en torno al umbral (≤ 15 cm). |
| P3 | **Tiempo de respuesta** | Estimar cuánto tarda el dashboard en reflejar un cambio | Poner o retirar el objeto y observar cuánto tarda el dashboard en cambiar de estado. | ≤ 2 s (el dashboard consulta el estado cada 1 s) |
| P4 | **Almacenamiento** | Verificar que el historial se guarda y se puede descargar | Dejar el sistema funcionando, provocar varios cambios de estado y descargar `historial.csv` con el botón del dashboard. Revisar que existan registros periódicos (cada 10 s) y en cada cambio. | El CSV se descarga y contiene registros coherentes con lo observado |
| P5 | **Persistencia** | Verificar que el historial sobrevive a un reinicio | Desconectar y reconectar la alimentación del ESP32. Descargar el CSV de nuevo. | Los registros anteriores siguen en el archivo y empieza una nueva sesión (`sesion` + 1) |

## Condiciones

- Prueba en interior, sobre la maqueta, con el sensor apuntando al espacio grabado.
- El umbral de 15 cm corresponde a la escala de la maqueta. En un estacionamiento real habría que recalibrarlo.
- Los datos y resultados están en [`resultados.md`](resultados.md) y en [`datos/`](datos/).
