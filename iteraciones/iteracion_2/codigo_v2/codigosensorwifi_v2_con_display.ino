#include <WiFi.h>
#include <WebServer.h>

// Pines del circuito
#define TRIG 18
#define ECHO 19
#define DISPLAY 25

// Configuración Wi-Fi
const char* ssid = "Estacionamiento_UAI";
const char* password = "parking123";

// Servidor web
WebServer server(80);

// Variables
float distancia = 0;
bool ocupado = false;
bool lecturaValida = false;

// Página web
const char pagina[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="es">

<head>
  <meta charset="UTF-8">
  <meta name="viewport"
        content="width=device-width, initial-scale=1">

  <title>Estacionamiento UAI</title>

  <style>
    body {
      font-family: Arial;
      text-align: center;
      background: #f1f5f9;
      padding: 30px;
    }

    h1 {
      color: #1e293b;
    }

    .card {
      background: white;
      padding: 30px;
      border-radius: 15px;
      max-width: 400px;
      margin: auto;
      box-shadow: 0 4px 15px #ccc;
    }

    #estado {
      font-size: 28px;
      font-weight: bold;
      margin: 25px 0;
    }

    #distancia {
      color: #64748b;
    }
  </style>
</head>

<body>

  <div class="card">

    <h1>Estacionamiento UAI</h1>

    <h2>Estado del estacionamiento</h2>

    <div id="estado">Cargando...</div>

    <p id="distancia"></p>

  </div>

  <script>

    async function actualizar() {

      try {

        const respuesta = await fetch('/estado');
        const datos = await respuesta.json();

        const estado = document.getElementById("estado");

        if (!datos.valida) {

          estado.innerHTML = "SIN LECTURA";
          estado.style.color = "orange";

        } else if (datos.ocupado) {

          estado.innerHTML = "OCUPADO";
          estado.style.color = "red";

        } else {

          estado.innerHTML = "LIBRE";
          estado.style.color = "green";

        }

        document.getElementById("distancia").innerHTML =
          datos.valida
            ? "Distancia: " + datos.distancia.toFixed(1) + " cm"
            : "Sensor sin lectura válida";

      } catch (error) {

        document.getElementById("estado").innerHTML =
          "SIN CONEXIÓN";

      }

    }

    setInterval(actualizar, 1000);

    actualizar();

  </script>

</body>
</html>
)rawliteral";

// Entregar página web
void paginaPrincipal() {
  server.send_P(200, "text/html", pagina);
}

// Entregar estado del sensor
void enviarEstado() {

  String json = "{";

  json += "\"ocupado\":";
  json += (ocupado ? "true" : "false");

  json += ",\"valida\":";
  json += (lecturaValida ? "true" : "false");

  json += ",\"distancia\":";
  json += String(distancia, 1);

  json += "}";

  server.send(200, "application/json", json);
}

void setup() {

  Serial.begin(115200);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(DISPLAY, OUTPUT);

  digitalWrite(TRIG, LOW);
  digitalWrite(DISPLAY, LOW);

  // Crear red Wi-Fi
  WiFi.mode(WIFI_AP);

  WiFi.softAP(ssid, password);

  Serial.println("Red Wi-Fi creada");
  Serial.print("Direccion IP: ");
  Serial.println(WiFi.softAPIP());

  // Configurar servidor
  server.on("/", paginaPrincipal);
  server.on("/estado", enviarEstado);

  server.begin();

  Serial.println("Servidor web iniciado");
}

void loop() {

  // Atender solicitudes del celular
  server.handleClient();

  // Medir distancia
  digitalWrite(TRIG, LOW);
  delayMicroseconds(5);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long tiempo = pulseIn(ECHO, HIGH, 30000);

  if (tiempo == 0) {

    lecturaValida = false;
    ocupado = false;

  } else {

    lecturaValida = true;

    distancia = tiempo * 0.0343 / 2;

    ocupado = (distancia < 15);
  }

  // Controlar display
  if (ocupado) {
    digitalWrite(DISPLAY, HIGH);
  } else {
    digitalWrite(DISPLAY, LOW);
  }

  // Mostrar datos
  Serial.print("Distancia: ");

  if (lecturaValida) {

    Serial.print(distancia);
    Serial.print(" cm - ");

    Serial.println(ocupado ? "OCUPADO" : "LIBRE");

  } else {

    Serial.println("Sin lectura");
  }

  delay(200);
}
