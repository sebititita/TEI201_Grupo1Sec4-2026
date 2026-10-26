# Documentación de Software

## Imágenes del código
`imagenes_codigo/` contiene el código completo en 3 imágenes (1920×1080) usadas en la presentación del Avance #2:
1. `codigo_esp32_1de3.png` — Librerías, pines, red WiFi y página web (HTML)
2. `codigo_esp32_2de3.png` — Actualización de la página y envío del estado (JSON)
3. `codigo_esp32_3de3.png` — `setup()` y `loop()`: medición, decisión y actuación

## Diagrama de flujo (lógica principal)

```
          ┌──────────────┐
          │   Inicio     │
          └──────┬───────┘
                 ▼
   Configurar pines + crear red WiFi + iniciar servidor
                 ▼
   ┌──────────────────────────────┐
   │ loop: atender al celular     │◄────────────┐
   └──────────────┬───────────────┘             │
                  ▼                             │
       Disparar TRIG y medir ECHO               │
                  ▼                             │
         ¿Hubo eco (tiempo > 0)?                │
          │ No               │ Sí               │
          ▼                  ▼                  │
     SIN LECTURA     distancia = t·0,0343/2     │
     display OFF             ▼                  │
          │          ¿distancia < 15 cm?        │
          │           │ Sí        │ No          │
          │           ▼           ▼             │
          │       OCUPADO       LIBRE           │
          │     display ON   display OFF        │
          └───────────┴───────────┴──► esperar 200 ms
```

---

## Manual de Instalación
Ver `software/README.md` (instalar paquete ESP32 en Arduino IDE, seleccionar *ESP32 Dev Module* y subir el sketch).

---

## Manual de Operación

### Encendido del Sistema
1. Conectar el ESP32 a la power bank (o al computador) por USB.
2. Esperar unos segundos a que aparezca la red WiFi **Estacionamiento_UAI**.

### Operación Normal
1. Conectar el celular a la red **Estacionamiento_UAI** (clave `parking123`).
2. Abrir el navegador en **http://192.168.4.1**.
3. Colocar un objeto (auto) frente al sensor: a menos de 15 cm la página muestra **OCUPADO** en rojo y se enciende el segmento del display; al retirarlo muestra **LIBRE** en verde.

### Apagado del Sistema
1. Desconectar el cable USB del ESP32.

---

## Troubleshooting

**Problema 1: El celular no ve la red WiFi**
- Causa probable: el ESP32 no está alimentado o el código no se cargó.
- Solución: revisar el cable USB y el Monitor Serie (debe decir "Red Wi-Fi creada").

**Problema 2: La página dice "SIN LECTURA"**
- Causa probable: el sensor no recibe eco (objeto fuera de rango o cable TRIG/ECHO suelto).
- Solución: revisar las conexiones y el divisor de voltaje del pin ECHO.

**Problema 3: La página dice "SIN CONEXIÓN"**
- Causa probable: el celular se desconectó de la red del ESP32 o se cambió a datos móviles.
- Solución: volver a conectarse a Estacionamiento_UAI y recargar la página.
