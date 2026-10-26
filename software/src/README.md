# Código Fuente

## Archivos Principales

### `codigosensorwifi/codigosensorwifi.ino`
Código del ESP32 para la versión v2 (1 espacio). Está dentro de una carpeta con el mismo nombre porque Arduino IDE lo exige para abrir el sketch.

**Estructura:**
- **Configuración:** pines (TRIG 18, ECHO 19, DISPLAY 25), nombre y clave de la red WiFi
- **Página web:** HTML/CSS/JS guardado en memoria (`PROGMEM`); se actualiza sola cada 1 segundo consultando `/estado`
- **`paginaPrincipal()`:** entrega la página web
- **`enviarEstado()`:** entrega el estado del sensor en formato JSON
- **`setup()`:** configura pines, crea la red WiFi (`WiFi.softAP`) e inicia el servidor
- **`loop()`:** atiende al celular, mide la distancia, decide ocupado/libre, controla el display e imprime en el Monitor Serie

### Variables y funciones clave
| Nombre | Tipo | Función |
|---|---|---|
| `distancia` | float | Última distancia medida (cm) |
| `ocupado` | bool | `true` si la distancia es menor a 15 cm |
| `lecturaValida` | bool | `false` si el sensor no recibió eco (timeout) |
| `pulseIn(ECHO, HIGH, 30000)` | función | Mide el tiempo del eco; devuelve 0 si pasan 30 ms sin respuesta |
| `tiempo * 0.0343 / 2` | cálculo | Convierte el tiempo a cm (velocidad del sonido 0,0343 cm/µs; ida y vuelta) |

### Monitor Serial
- Baudrate: 115200
- Mensajes: `Red Wi-Fi creada`, `Direccion IP: 192.168.4.1`, `Servidor web iniciado`, `Distancia: X cm - OCUPADO/LIBRE`, `Sin lectura`
