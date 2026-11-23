# Firmware — ESP32

Código definitivo del Avance 3: [`codigosensorwifi/codigosensorwifi.ino`](codigosensorwifi/codigosensorwifi.ino). La versión anterior, con el display, está en `iteraciones/iteracion_2/codigo_v2/`.

## Qué hace

1. Mide la distancia con el **HC-SR04** cada **200 ms**.
2. Decide el estado:

   | Condición | Estado |
   |---|---|
   | distancia < 15 cm | **OCUPADO** |
   | distancia ≥ 15 cm | **LIBRE** |
   | sin eco (`pulseIn` = 0) | **SIN LECTURA** |

3. Calcula estadísticas de la sesión:
   - % del tiempo ocupado;
   - tiempo ocupado;
   - número de ocupaciones.
4. Guarda registros en **`/historial.csv`** (LittleFS, memoria flash) cada **10 s** y en **cada cambio de estado**.
5. Crea la red WiFi **`Estacionamiento_UAI`** y sirve el **dashboard** en `http://192.168.4.1`.

## Parámetros (inicio del archivo)

| Constante | Valor | Para qué |
|---|---|---|
| `TRIG` / `ECHO` | 18 / 19 | Pines del sensor (ECHO pasa por un divisor de voltaje) |
| `UMBRAL_OCUPADO` | 15.0 cm | Distancia bajo la cual se considera ocupado (escala maqueta) |
| `INTERVALO_MEDICION` | 200 ms | Cada cuánto se mide |
| `INTERVALO_GUARDADO` | 10 000 ms | Registro periódico en el CSV |
| `MAX_CSV` | 180 000 bytes | Límite del archivo para no llenar la partición |
| `ssid` / `password` | `Estacionamiento_UAI` / `parking123` | Red WiFi propia |

## Funciones principales

| Función | Líneas | Descripción |
|---|---|---|
| `iniciarAlmacenamiento()` | 533–598 | Monta LittleFS (`begin(true)` formatea si no puede montar), aumenta el número de sesión en `/sesion.txt` y crea el CSV con encabezado si no existe |
| `guardarRegistro()` | 605–660 | Agrega una línea `sesion,segundos,distancia_cm,estado` al CSV |
| `medirDistancia()` | 667–734 | Pulso de 10 µs en TRIG, mide el eco con `pulseIn` (timeout 30 ms), calcula `distancia = duración × 0,0343 / 2`, cuenta ocupaciones y guarda si cambió el estado |
| `actualizarEstadisticas()` | 741–757 | Suma tiempo observado y tiempo ocupado (solo con lecturas válidas) |
| `enviarEstado()` | 764–802 | Responde `/estado` con JSON: `ocupado`, `valida`, `distancia`, `porcentaje`, `tiempoOcupado` (s), `ocupaciones` |
| `enviarHistorial()` | 809–930 | Responde `/historial` con los últimos 60 registros (1 = ocupado, 0 = libre, −1 = sin lectura) |
| `descargarCSV()` | 937–978 | Responde `/descargar` con el archivo `historial.csv` |
| `setup()` / `loop()` | 985–1094 | Inicializa todo; el loop atiende la web y mide sin bloquear (`millis()`) |

## Dashboard (página web dentro del código, líneas 56–526)

- **Estado actual:** LIBRE (verde), OCUPADO (rojo) o SIN LECTURA (naranjo), con la distancia en cm.
- **Indicadores:**
  - espacios disponibles (1 o 0 de 1 monitoreado);
  - % de ocupación;
  - tiempo ocupado (min);
  - número de ocupaciones.
- **Gráfico:** historial de los últimos 60 registros, dibujado con `canvas` y sin librerías externas.
- **Botón:** descarga del CSV.
- **Actualización:** estado cada **1 s**, gráfico cada **10 s**.

## Endpoints

| Ruta | Respuesta |
|---|---|
| `/` | Página del dashboard (HTML) |
| `/estado` | JSON con el estado e indicadores |
| `/historial` | JSON con los últimos 60 registros |
| `/descargar` | Archivo `historial.csv` |

## Cómo cargarlo

Ver la sección **Cargar el firmware** del [README principal](../README.md#cargar-el-firmware). Placa **ESP32 Dev Module**, esquema de particiones con partición de archivos (ej. *Default 4MB with spiffs*), monitor serie a 115200 baudios. Si el botón BOOT no funciona, puentear GPIO0–GND durante la carga y retirarlo después.

## Librerías

`WiFi.h`, `WebServer.h` y `LittleFS.h`, todas incluidas en el paquete esp32 de Espressif. Detalle y declaración de uso de IA en [`FUENTES.md`](../FUENTES.md).
