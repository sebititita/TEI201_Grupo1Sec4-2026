#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>

// ==========================================
// ESTACIONAMIENTO INTELIGENTE UAI
// AVANCE 3 - PROTOTIPO IoT
// ==========================================

// ---------- PINES ----------
#define TRIG 18
#define ECHO 19

// ---------- CONFIGURACION ----------
const float UMBRAL_OCUPADO = 15.0;

const unsigned long INTERVALO_MEDICION = 200;
const unsigned long INTERVALO_GUARDADO = 10000;

// ---------- WIFI ----------
const char* ssid = "Estacionamiento_UAI";
const char* password = "parking123";

WebServer server(80);

// ---------- SENSOR ----------
float distancia = 0;

bool ocupado = false;
bool lecturaValida = false;
bool primeraMedicion = true;

// ---------- ESTADISTICAS ----------
unsigned long tiempoOcupado = 0;
unsigned long tiempoObservado = 0;
unsigned long numeroOcupaciones = 0;

unsigned long ultimaMedicion = 0;
unsigned long ultimoGuardado = 0;

// ---------- ALMACENAMIENTO ----------
const char* archivoCSV = "/historial.csv";

bool almacenamientoOK = false;

uint32_t sesion = 0;

// Limite para evitar llenar la memoria
const size_t MAX_CSV = 180000;


// ==========================================
// PAGINA WEB
// ==========================================

const char pagina[] PROGMEM = R"rawliteral(

<!DOCTYPE html>
<html lang="es">

<head>

<meta charset="UTF-8">

<meta name="viewport"
content="width=device-width, initial-scale=1">

<title>Estacionamiento Inteligente UAI</title>

<style>

* {
  box-sizing: border-box;
}

body {
  font-family: Arial, sans-serif;
  background: #f1f5f9;
  margin: 0;
  padding: 20px;
  text-align: center;
  color: #1e293b;
}

.contenedor {
  max-width: 900px;
  margin: auto;
}

h1 {
  color: #1e3a8a;
  margin-bottom: 5px;
}

.subtitulo {
  color: #64748b;
  margin-bottom: 25px;
}

.card {
  background: white;
  padding: 25px;
  margin-bottom: 18px;
  border-radius: 15px;
  box-shadow: 0 3px 12px #00000015;
}

#estado {
  font-size: 36px;
  font-weight: bold;
  margin: 20px 0;
}

.grid {
  display: grid;
  grid-template-columns:
  repeat(auto-fit, minmax(180px, 1fr));
  gap: 15px;
}

.valor {
  font-size: 32px;
  font-weight: bold;
  color: #1d4ed8;
  margin-top: 10px;
}

.etiqueta {
  font-size: 15px;
  color: #64748b;
}

canvas {
  width: 100%;
  height: auto;
  background: #f8fafc;
  border-radius: 8px;
}

.boton {
  display: inline-block;
  padding: 14px 22px;
  background: #1d4ed8;
  color: white;
  text-decoration: none;
  border-radius: 9px;
  margin-top: 15px;
}

.nota {
  color: #64748b;
  font-size: 13px;
}

</style>

</head>

<body>

<div class="contenedor">

<h1>ESTACIONAMIENTO INTELIGENTE UAI</h1>

<p class="subtitulo">
Sistema de monitoreo de disponibilidad
</p>

<div class="card">

<h2>Estado actual</h2>

<div id="estado">Cargando...</div>

<p id="distancia"></p>

</div>


<div class="grid">

<div class="card">

<div class="etiqueta">
Espacios disponibles
</div>

<div class="valor" id="disponibles">
--
</div>

<div class="nota">
De 1 estacionamiento monitoreado
</div>

</div>


<div class="card">

<div class="etiqueta">
Porcentaje de ocupación
</div>

<div class="valor" id="porcentaje">
--
</div>

</div>


<div class="card">

<div class="etiqueta">
Tiempo ocupado
</div>

<div class="valor" id="tiempo">
--
</div>

</div>


<div class="card">

<div class="etiqueta">
Número de ocupaciones
</div>

<div class="valor" id="ocupaciones">
--
</div>

</div>

</div>


<div class="card">

<h2>Historial de ocupación</h2>

<canvas id="grafico"
width="700" height="250"></canvas>

<p class="nota">
1 = Ocupado | 0 = Libre
</p>

<p id="mensajeHistorial"></p>

<a href="/descargar" class="boton">
Descargar historial CSV
</a>

</div>


<div class="card">

<h3>Información del sistema</h3>

<p>
Sensor: HC-SR04
</p>

<p>
Microcontrolador: ESP32
</p>

<p>
Comunicación: Wi-Fi
</p>

<p>
Umbral de detección: 15 cm
</p>

<p class="nota">
Las estadísticas acumuladas corresponden
a la sesión actual.
</p>

</div>

</div>


<script>

// ==========================================
// ACTUALIZAR INDICADORES
// ==========================================

async function actualizarEstado() {

  try {

    const respuesta = await fetch("/estado");

    if (!respuesta.ok) {
      throw new Error("Error HTTP");
    }

    const datos = await respuesta.json();

    const estado =
      document.getElementById("estado");

    const disponibles =
      document.getElementById("disponibles");

    if (!datos.valida) {

      estado.innerText = "SIN LECTURA";
      estado.style.color = "#d97706";

      disponibles.innerText = "--";

    }

    else if (datos.ocupado) {

      estado.innerText = "OCUPADO";
      estado.style.color = "#dc2626";

      disponibles.innerText = "0";

    }

    else {

      estado.innerText = "LIBRE";
      estado.style.color = "#16a34a";

      disponibles.innerText = "1";

    }

    document.getElementById("distancia")
      .innerText = datos.valida
      ? "Distancia: " +
        datos.distancia.toFixed(1) + " cm"
      : "No existe una medición válida";

    document.getElementById("porcentaje")
      .innerText =
      datos.porcentaje.toFixed(1) + "%";

    document.getElementById("tiempo")
      .innerText =
      (datos.tiempoOcupado / 60)
      .toFixed(1) + " min";

    document.getElementById("ocupaciones")
      .innerText = datos.ocupaciones;

  }

  catch(error) {

    document.getElementById("estado")
      .innerText = "SIN CONEXIÓN";

    document.getElementById("estado")
      .style.color = "#dc2626";

  }

}


// ==========================================
// GRAFICO HISTORICO
// ==========================================

async function actualizarGrafico() {

  try {

    const respuesta = await fetch("/historial");

    if (!respuesta.ok) {
      throw new Error("Error HTTP");
    }

    const datos = await respuesta.json();

    const canvas =
      document.getElementById("grafico");

    const ctx = canvas.getContext("2d");

    const ancho = canvas.width;
    const alto = canvas.height;

    ctx.clearRect(0, 0, ancho, alto);

    // Fondo
    ctx.fillStyle = "#f8fafc";
    ctx.fillRect(0, 0, ancho, alto);

    // Ejes
    ctx.strokeStyle = "#94a3b8";
    ctx.lineWidth = 1;

    ctx.beginPath();
    ctx.moveTo(45, 20);
    ctx.lineTo(45, 205);
    ctx.lineTo(ancho - 20, 205);
    ctx.stroke();

    ctx.fillStyle = "#334155";
    ctx.font = "13px Arial";

    ctx.fillText("1", 20, 55);
    ctx.fillText("0", 20, 190);

    if (datos.length === 0) {

      ctx.fillText(
        "Todavía no hay registros",
        230, 120
      );

      return;
    }

    const espacio = ancho - 80;

    const paso = datos.length > 1
      ? espacio / (datos.length - 1)
      : 0;

    // Dibujar líneas entre registros
    for (let i = 1; i < datos.length; i++) {

      const anterior = datos[i - 1];
      const actual = datos[i];

      if (
        anterior.estado < 0 ||
        actual.estado < 0 ||
        anterior.sesion !== actual.sesion
      ) {
        continue;
      }

      const x1 = 45 + (i - 1) * paso;
      const x2 = 45 + i * paso;

      const y1 =
        anterior.estado === 1 ? 50 : 185;

      const y2 =
        actual.estado === 1 ? 50 : 185;

      ctx.strokeStyle = "#2563eb";
      ctx.lineWidth = 2;

      ctx.beginPath();
      ctx.moveTo(x1, y1);
      ctx.lineTo(x2, y2);
      ctx.stroke();

    }

    // Dibujar puntos
    datos.forEach((dato, i) => {

      if (dato.estado < 0) return;

      const x = 45 + i * paso;

      const y =
        dato.estado === 1 ? 50 : 185;

      ctx.fillStyle =
        dato.estado === 1
        ? "#dc2626"
        : "#16a34a";

      ctx.beginPath();

      ctx.arc(x, y, 4, 0, Math.PI * 2);

      ctx.fill();

    });

    document.getElementById("mensajeHistorial")
      .innerText =
      "Mostrando los últimos " +
      datos.length + " registros";

  }

  catch(error) {

    document.getElementById("mensajeHistorial")
      .innerText =
      "No se pudo cargar el historial";

  }

}


// ==========================================
// ACTUALIZACION AUTOMATICA
// ==========================================

setInterval(actualizarEstado, 1000);

setInterval(actualizarGrafico, 10000);

actualizarEstado();
actualizarGrafico();

</script>

</body>                     
</html>

)rawliteral";
                                         

// ==========================================
// INICIAR ALMACENAMIENTO
// ==========================================

void iniciarAlmacenamiento() {

  almacenamientoOK = LittleFS.begin(true);

  if (!almacenamientoOK) {

    Serial.println(
      "ERROR: LittleFS no pudo iniciarse"
    );

    return;
  }

  // Recuperar número de sesión

  if (LittleFS.exists("/sesion.txt")) {

    File f = LittleFS.open(
      "/sesion.txt", FILE_READ
    );

    if (f) {

      sesion = f.parseInt();
      f.close();

    }

  }

  sesion++;

  File s = LittleFS.open(
    "/sesion.txt", FILE_WRITE
  );

  if (s) {

    s.println(sesion);
    s.close();

  }

  // Crear CSV si no existe

  if (!LittleFS.exists(archivoCSV)) {

    File f = LittleFS.open(
      archivoCSV, FILE_WRITE
    );

    if (f) {

      f.println(
        "sesion,segundos,distancia_cm,estado"
      );

      f.close();

    }

  }

  Serial.println("LittleFS iniciado");

}


// ==========================================
// GUARDAR DATOS
// ==========================================

void guardarRegistro() {

  if (!almacenamientoOK) return;

  File f = LittleFS.open(
    archivoCSV, FILE_APPEND
  );

  if (!f) return;

  if (f.size() >= MAX_CSV) {

    Serial.println(
      "Historial lleno. Descargar CSV."
    );

    f.close();
    return;

  }

  f.print(sesion);
  f.print(",");

  f.print(millis() / 1000);
  f.print(",");

  if (lecturaValida) {

    f.print(distancia, 2);

  }

  f.print(",");

  if (!lecturaValida) {

    f.println("SIN_LECTURA");

  }

  else if (ocupado) {

    f.println("OCUPADO");

  }

  else {

    f.println("LIBRE");

  }

  f.close();

}


// ==========================================
// MEDIR DISTANCIA
// ==========================================

void medirDistancia() {

  bool ocupadoAnterior = ocupado;
  bool validaAnterior = lecturaValida;

  // Pulso ultrasónico

  digitalWrite(TRIG, LOW);
  delayMicroseconds(5);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG, LOW);

  long duracion = pulseIn(
    ECHO, HIGH, 30000
  );

  // Validar medición

  if (duracion == 0) {

    lecturaValida = false;
    ocupado = false;

  }

  else {

    lecturaValida = true;

    distancia = duracion * 0.0343 / 2.0;

    ocupado =
      distancia < UMBRAL_OCUPADO;

  }

  // Contabilizar ocupaciones

  if (
    lecturaValida &&
    ocupado &&
    (primeraMedicion ||
     !ocupadoAnterior ||
     !validaAnterior)
  ) {

    numeroOcupaciones++;

  }

  // Guardar cambios

  if (
    primeraMedicion ||
    ocupado != ocupadoAnterior ||
    lecturaValida != validaAnterior
  ) {

    guardarRegistro();

  }

  primeraMedicion = false;

}


// ==========================================
// ESTADISTICAS DE OCUPACION
// ==========================================

void actualizarEstadisticas(
  unsigned long tiempoTranscurrido,
  bool estadoAnterior,
  bool lecturaAnterior
) {

  if (!lecturaAnterior) return;

  tiempoObservado += tiempoTranscurrido;

  if (estadoAnterior) {

    tiempoOcupado += tiempoTranscurrido;

  }

}


// ==========================================
// ENVIAR ESTADO AL CELULAR
// ==========================================

void enviarEstado() {

  float porcentaje = 0;

  if (tiempoObservado > 0) {

    porcentaje =
      100.0 * tiempoOcupado /
      tiempoObservado;

  }

  String json = "{";

  json += "\"ocupado\":";
  json += (ocupado ? "true" : "false");

  json += ",\"valida\":";
  json += (lecturaValida ? "true" : "false");

  json += ",\"distancia\":";
  json += String(distancia, 2);

  json += ",\"porcentaje\":";
  json += String(porcentaje, 2);

  json += ",\"tiempoOcupado\":";
  json += String(tiempoOcupado / 1000);

  json += ",\"ocupaciones\":";
  json += String(numeroOcupaciones);

  json += "}";

  server.send(
    200, "application/json", json
  );

}


// ==========================================
// CONSULTAR HISTORIAL
// ==========================================

void enviarHistorial() {

  if (!almacenamientoOK) {

    server.send(
      503, "application/json", "[]"
    );

    return;

  }

  File f = LittleFS.open(
    archivoCSV, FILE_READ
  );

  if (!f) {

    server.send(
      200, "application/json", "[]"
    );

    return;

  }

  const int MAX_REGISTROS = 60;

  String ultimos[MAX_REGISTROS];

  int cantidad = 0;
  int posicion = 0;

  // Omitir encabezado
  f.readStringUntil('\n');

  while (f.available()) {

    String linea =
      f.readStringUntil('\n');

    linea.trim();

    if (linea.length() == 0) continue;

    ultimos[posicion] = linea;

    posicion =
      (posicion + 1) % MAX_REGISTROS;

    if (cantidad < MAX_REGISTROS) {
      cantidad++;
    }

  }

  f.close();

  String json = "[";

  for (int i = 0; i < cantidad; i++) {

    int indice =
      (posicion - cantidad +
       i + MAX_REGISTROS)
      % MAX_REGISTROS;

    String linea = ultimos[indice];

    int p1 = linea.indexOf(',');
    int p2 = linea.indexOf(',', p1 + 1);
    int p3 = linea.indexOf(',', p2 + 1);

    if (
      p1 < 0 ||
      p2 < 0 ||
      p3 < 0
    ) continue;

    String ses =
      linea.substring(0, p1);

    String segundos =
      linea.substring(p1 + 1, p2);

    String estado =
      linea.substring(p3 + 1);

    int valor = -1;

    if (estado == "OCUPADO") {
      valor = 1;
    }

    if (estado == "LIBRE") {
      valor = 0;
    }

    if (json.length() > 1) {
      json += ",";
    }

    json += "{\"sesion\":";
    json += ses;

    json += ",\"segundos\":";
    json += segundos;

    json += ",\"estado\":";
    json += String(valor);

    json += "}";

  }

  json += "]";

  server.send(
    200, "application/json", json
  );

}


// ==========================================
// DESCARGAR ARCHIVO CSV
// ==========================================

void descargarCSV() {

  if (!almacenamientoOK) {

    server.send(
      503,
      "text/plain",
      "Almacenamiento no disponible"
    );

    return;

  }

  File f = LittleFS.open(
    archivoCSV, FILE_READ
  );

  if (!f) {

    server.send(
      404,
      "text/plain",
      "Archivo no encontrado"
    );

    return;

  }

  server.sendHeader(
    "Content-Disposition",
    "attachment; filename=historial.csv"
  );

  server.streamFile(
    f, "text/csv"
  );

  f.close();

}


// ==========================================
// SETUP
// ==========================================

void setup() {

  Serial.begin(115200);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  digitalWrite(TRIG, LOW);

  // Almacenamiento persistente
  iniciarAlmacenamiento();

  // Crear red Wi-Fi
  WiFi.mode(WIFI_AP);

  WiFi.softAP(
    ssid, password, 1, 0, 4
  );

  Serial.println("Wi-Fi iniciado");

  Serial.print("Direccion IP: ");
  Serial.println(WiFi.softAPIP());

  // Configurar rutas

  server.on("/", []() {

    server.send_P(
      200, "text/html", pagina
    );

  });

  server.on(
    "/estado", enviarEstado
  );

  server.on(
    "/historial", enviarHistorial
  );

  server.on(
    "/descargar", descargarCSV
  );

  server.begin();

  ultimaMedicion = millis();
  ultimoGuardado = millis();

  Serial.println(
    "Sistema iniciado correctamente"
  );

}


// ==========================================
// LOOP
// ==========================================

void loop() {

  // Atender solicitudes web
  server.handleClient();

  unsigned long ahora = millis();

  // Medición cada 200 ms
  if (
    ahora - ultimaMedicion >=
    INTERVALO_MEDICION
  ) {

    // Contabilizar el tiempo desde
    // la medición anterior

    unsigned long delta =
      ahora - ultimaMedicion;

    bool anteriorOcupado = ocupado;
    bool anteriorValida = lecturaValida;

    ultimaMedicion = ahora;

    actualizarEstadisticas(
      delta,
      anteriorOcupado,
      anteriorValida
    );

    medirDistancia();

  }

  // Guardar registro cada 10 segundos
  if (
    ahora - ultimoGuardado >=
    INTERVALO_GUARDADO
  ) {

    ultimoGuardado = ahora;

    guardarRegistro();

  }

  delay(2);

}