# FUENTES — Librerías, código externo y uso de Inteligencia Artificial

Declaración de todo el material externo usado en el proyecto **Estacionamiento Inteligente UAI** (TEI201, Grupo 1 Sección 4). Las líneas indicadas corresponden a `firmware/codigosensorwifi/codigosensorwifi.ino` (versión del Avance 3).

---

## 1. Librerías utilizadas

Las tres librerías vienen incluidas en el paquete de placas **esp32 de Espressif Systems para Arduino IDE** (arduino-esp32). No se instaló ninguna librería de terceros.

| Librería | Versión | Uso en el proyecto | Fuente |
|---|---|---|---|
| `WiFi.h` | Incluida en el paquete esp32 instalado* | Crea la red WiFi propia del ESP32 en modo punto de acceso (`WiFi.mode(WIFI_AP)`, `WiFi.softAP(...)`, líneas 998–1002) | https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi |
| `WebServer.h` | Incluida en el paquete esp32 instalado* | Servidor HTTP en el puerto 80: rutas `/`, `/estado`, `/historial` y `/descargar` (líneas 1011–1031) | https://github.com/espressif/arduino-esp32/tree/master/libraries/WebServer |
| `LittleFS.h` | Incluida en el paquete esp32 instalado* | Sistema de archivos en la memoria flash: guarda `historial.csv` y `sesion.txt` (funciones `iniciarAlmacenamiento`, `guardarRegistro`, `enviarHistorial`, `descargarCSV`) | https://github.com/espressif/arduino-esp32/tree/master/libraries/LittleFS |

\* Paquete usado para compilar: **"ESP32 boards"** instalado desde el Gestor de placas de Arduino IDE (arduino-esp32 de Espressif). El número de versión exacto no quedó registrado.

El dashboard **no usa librerías web externas** (ni Chart.js ni CDN): el gráfico se dibuja con la API `canvas` estándar del navegador, porque el celular conectado a la red del ESP32 no tiene internet.

---

## 2. Código externo adaptado

No se copiaron fragmentos de código desde GitHub, Stack Overflow ni tutoriales. Las funciones de las librerías se usaron siguiendo su documentación oficial (enlaces de la tabla anterior). La fórmula de distancia del HC-SR04 (`distancia = duracion * 0.0343 / 2`, línea 699) es el cálculo estándar del sensor: 0,0343 cm/µs es la velocidad aproximada del sonido y se divide por 2 porque el pulso va y vuelve.

---

## 3. Uso de Inteligencia Artificial

Se usaron dos herramientas: **ChatGPT** (OpenAI) y **Claude** (Anthropic). No se conservan los prompts textuales, por lo que se describe el propósito de cada solicitud sin inventar citas.

### 3.1 Firmware del ESP32 — Avance 2 (versión con display)
- **Herramienta:** ChatGPT (octubre 2026).
- **Uso:** generar la base del programa: medición con el HC-SR04, red WiFi en modo punto de acceso, página web con LIBRE/OCUPADO y control de un segmento del display.
- **Adaptación:** el equipo ajustó los pines al montaje real (TRIG 18, ECHO 19, display 25), fijó el umbral de 15 cm para la maqueta y resolvió en el hardware la conexión del display y del divisor de voltaje.
- **Verificación:** prueba en vivo en el Avance 2 (capturas en `iteraciones/iteracion_2/evidencias_v2/`).
- Versión archivada: `iteraciones/iteracion_2/codigo_v2/`.

### 3.2 Firmware del ESP32 — Avance 3 (dashboard + almacenamiento)
- **Herramienta:** ChatGPT (noviembre 2026).
- **Uso:**
  - Dashboard web con indicadores y gráfico (HTML/CSS/JavaScript dentro de `pagina[]`, líneas 56–526).
  - Estadísticas de ocupación (`actualizarEstadisticas`, líneas 741–757; `enviarEstado`, líneas 764–802).
  - Almacenamiento en LittleFS y exportación CSV (`iniciarAlmacenamiento` 533–598, `guardarRegistro` 605–660, `enviarHistorial` 809–930, `descargarCSV` 937–978).
  - Diagnóstico del error «Almacenamiento no disponible».
- **Adaptación hecha por el equipo:**
  - Se eliminó todo el control del display (GPIO25): la salida pasó a ser solo el dashboard.
  - Se cambió `LittleFS.begin(false)` por `LittleFS.begin(true)` (línea 535) para que la partición se formatee si no se puede montar; así se resolvió el error de almacenamiento.
  - Se mantuvieron los pines y el umbral del Avance 2 para no cambiar lo que ya funcionaba.
- **Comprensión:**
  - `loop()` mide cada 200 ms sin bloquear el servidor (usa `millis()` en vez de `delay` largos).
  - `actualizarEstadisticas` suma el tiempo transcurrido entre mediciones al tiempo observado y, si el espacio estaba ocupado, al tiempo ocupado; el porcentaje es tiempo ocupado / tiempo observado × 100.
  - Una "ocupación" se cuenta cuando el estado pasa a OCUPADO.
  - El CSV se escribe cada 10 s y en cada cambio de estado.
  - `sesion.txt` guarda un contador que aumenta en cada encendido para distinguir sesiones.
  - `/historial` lee el CSV y devuelve los últimos 60 registros con un arreglo circular.
- **Verificación:** 20 pruebas de detección, pruebas de distancia, tiempo de respuesta, descarga del CSV y reinicio del ESP32 (ver `testing/`).

### 3.3 Documentación, diseño y diagramas
- **Herramienta:** Claude (septiembre a noviembre 2026).
- **Uso:**
  - **Avance 1:** apoyo en la búsqueda y verificación de fuentes del benchmark de otras universidades.
  - **Avance 2:**
    - Estructura de los README del repositorio.
    - Diagrama de conexiones v2.
    - Imágenes del código.
    - Modelo 3D conceptual del encapsulado (STEP y renders).
  - **Avance 3:**
    - Diseño paramétrico de la maqueta MDF y generación de los archivos de corte láser (`diseño-3d/corte_laser/generador_maqueta.py`, DXF), ajustado a las medidas reales del power bank, la protoboard y el ESP32 que entregó el equipo.
    - Gemelo 3D en STEP.
    - Planos técnicos (`diseño-3d/planos.pdf`).
    - Esquemático eléctrico (`hardware/esquematico.pdf`).
    - Modelo de la versión real (`diseño-3d/version_real/`).
    - Redacción de README, protocolo y resultados a partir de los datos medidos por el equipo.
    - Este archivo.
- **Adaptación:** el equipo tomó las medidas reales de los componentes, eligió el diseño (poste con caja del sensor y caja central), cortó y armó las piezas en el DEC y confirmó que todo cupiera. Los resultados de pruebas son mediciones del equipo; la IA no generó datos.
- **Verificación:** las piezas cortadas encajaron y los componentes cupieron en la caja; el esquemático corresponde al montaje real (TRIG 18, ECHO 19, divisor de 3 × 10 kΩ).

### 3.4 Modelo 3D inicial (Avance 1)
- **Herramienta:** ChatGPT (octubre 2026).
- **Uso:** primer modelo conceptual del muro con sensores (`iteraciones/iteracion_1/v1_modelo3d_inicial.step`).
- **Adaptación:** el equipo lo usó solo como referencia y lo reemplazó por los modelos v2 y v3.

### 3.5 Imágenes ilustrativas de la presentación
- **Herramienta:** ChatGPT (noviembre 2026).
- **Uso:** infografías explicativas para las diapositivas.
- **Advertencia:** son representaciones ilustrativas, no evidencia experimental. Algunas mostraban pines o resistencias distintos al montaje real; la referencia válida es `hardware/esquematico.pdf`.
