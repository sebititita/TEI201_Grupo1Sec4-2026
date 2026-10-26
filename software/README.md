# Software - Documentación

## Contenido
- `src/codigosensorwifi/codigosensorwifi.ino` — Código del ESP32 (v2)
- `librerias/` — Librerías utilizadas
- `docs/` — Funcionamiento, manual de instalación y operación, imágenes del código

---

## Stack Tecnológico

**Plataforma:** ESP32 Dev Module  
**Lenguaje:** C/C++ (Arduino) + HTML/CSS/JavaScript para la página web  
**IDE:** Arduino IDE 2.x  
**Librerías principales:**
- `WiFi.h` — incluida en el paquete ESP32 (crea la red WiFi)
- `WebServer.h` — incluida en el paquete ESP32 (sirve la página web)

---

## Instalación y Configuración

### 1. Instalar Arduino IDE
Descargar Arduino IDE 2.x desde https://www.arduino.cc/en/software

### 2. Configurar ESP32
En Arduino IDE: *Herramientas → Placa → Gestor de placas*, buscar **esp32** (Espressif Systems) e instalar. Luego seleccionar la placa **ESP32 Dev Module** y el puerto correspondiente.

### 3. Instalar Librerías
No requiere librerías externas: `WiFi.h` y `WebServer.h` vienen con el paquete ESP32.

### 4. Cargar Código
1. Abrir `src/codigosensorwifi/codigosensorwifi.ino`
2. Seleccionar placa y puerto
3. Compilar y subir (si el botón BOOT no responde, hacer puente GPIO0–GND durante la carga)
4. Abrir el Monitor Serie a **115200 baudios** para ver la IP y las lecturas

---

## Configuración de Parámetros

```cpp
// Pines
#define TRIG 18
#define ECHO 19
#define DISPLAY 25

// Red WiFi creada por el ESP32
const char* ssid = "Estacionamiento_UAI";
const char* password = "parking123";

// Umbral de ocupación (en loop): ocupado = (distancia < 15);
```

---

## Arquitectura del Software

```
loop() cada ~200 ms
  ├─ server.handleClient()      → atiende al celular
  ├─ Pulso TRIG 10 µs → pulseIn(ECHO)  → tiempo de eco
  ├─ distancia = tiempo × 0.0343 / 2   (cm)
  ├─ ocupado = distancia < 15
  ├─ GPIO25 HIGH/LOW            → segmento del display
  └─ Serial.print(...)          → registro en Monitor Serie

Celular (navegador) cada 1 s
  └─ GET /estado → JSON {"ocupado":…, "valida":…, "distancia":…}
       → muestra LIBRE (verde) / OCUPADO (rojo) / SIN LECTURA (naranjo)
```

### Módulos Principales
1. **Inicialización** (`setup`) — Pines, red WiFi en modo punto de acceso, rutas del servidor
2. **Lectura de Sensores** — Pulso de disparo y medición del eco con `pulseIn` (timeout 30 ms)
3. **Procesamiento** — Conversión a centímetros y decisión ocupado/libre con umbral de 15 cm
4. **Actuación** — Encendido/apagado del segmento del display (GPIO25)
5. **Comunicación** — Página web (`/`) y estado en JSON (`/estado`) por WiFi

---

## API / Endpoints

| Endpoint | Método | Descripción |
|----------|--------|-------------|
| `/` | GET | Página web del estado del estacionamiento |
| `/estado` | GET | Estado en JSON: `ocupado` (bool), `valida` (bool), `distancia` (cm) |
