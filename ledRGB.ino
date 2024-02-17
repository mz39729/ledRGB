#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
//#include <DNSServer.h>
#include <ESP8266mDNS.h>

const char *ssid = "*****";
const char *password = "******";

//DNSServer dnsServer;
//const byte DNS_PORT = 53;

int pinRojo = D3;  // Define el pin para el componente rojo del LED RGB
int pinVerde = D2; // Define el pin para el componente verde del LED RGB
int pinAzul = D4;  // Define el pin para el componente azul del LED RGB
bool ledEncendido = false;

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);

  // Inicializa los pines del LED RGB como salidas
  pinMode(pinRojo, OUTPUT);
  pinMode(pinVerde, OUTPUT);
  pinMode(pinAzul, OUTPUT);

  // Conéctate a la red Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a la red WiFi...");
  }
  Serial.println("Conectado a la red WiFi");

  // Configura y comienza el servidor DNS
  //dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
  //dnsServer.addDomain("ledrgn.local". WiFi.softAPIP());
  
  Serial.print("Dirección IP asignada: ");
  Serial.println(WiFi.localIP());

  if(!MDNS.begin("ledrgb")){
    Serial.println("Error mDNS");
    while(1){
      delay(1000);
      }
    }

  // Maneja las solicitudes HTTP
  server.on("/", HTTP_GET, handleRoot);
  server.on("/color", HTTP_POST, handleColor);
  server.on("/encender", HTTP_GET, handleEncender);
  server.on("/apagar", HTTP_GET, handleApagar);

  server.begin();

  MDNS.addService("http","tcp",80);

}

void loop() {
#if defined(ESP8266)
  MDNS.update();
#endif
  
  // Captura cualquier solicitud DNS
  //dnsServer.processNextRequest();
  server.handleClient();
}

void handleRoot() {
  String html = "<!DOCTYPE html><html lang='es'><head>";
html += "<meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'>";
html += "      <style>";
html += "        div {width: 50px;height: 50px;margin: 5px;cursor: pointer;}";
html += "      </style>";
html += "      <title>LED RGB</title> <link href='https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/css/bootstrap.min.css' rel='stylesheet' integrity='sha384-EVSTQN3/azprG1Anm3QDgpJLIm9Nao0Yz1ztcQTwFspd3yD65VohhpuuCOmLASjC' crossorigin='anonymous'>";
html += "    </head><body><div class='container' style='width: 90%;'>";
html += "      <h1>Control de LED RGB</h1>";
html += "      <form id='colorForm' action='/color' method='post'>";
html += "           <label for='Rojo' class='form-label text-danger'>Rojo</label>";
html += "           <input type='range' class='form-range'  id='rojo' name='rojo' min='0' max='255'><br>";
html += "           <label for='Verde' class='form-label text-success'>Verde</label>";
html += "           <input type='range' class='form-range' id='verde' name='verde' min='0' max='255'><br>";
html += "           <label for='Azul' class='form-label text-primary'>Azul</label>";
html += "           <input type='range' class='form-range' id='azul' name='azul' min='0' max='255'><br>";
html += "           <input type='submit' value='Cambiar Color' class='btn-primary btn'><br><br>";
html += "    </form>";
html += "    <button onclick='apagar()'  class='btn btn-primary'>Encender</button>";
html += "    <button onclick='encender()' class='btn btn-danger'>Apagar</button><br><br>";
html += "    <div class='row' style='width: 100%; display: flex;''>";
html += "       <div id='1' style='background-color:rgb(0, 204, 255)'></div>";
html += "       <div id='2' style='background-color:rgb(255, 162, 0)'></div>";
html += "       <div id='3' style='background-color:rgb(255, 0, 255)'></div>";
html += "       <div id='4' style='background-color:rgb(179, 255, 0)'></div>";
html += "       <div id='5' style='background-color:rgb(153, 0, 255)'></div>";
html += "       <div id='6' style='background-color:rgb(255, 0, 98)'></div>";
html += "    </div></div>";
html += "   <script>";
html += "     document.addEventListener('DOMContentLoaded', function () {";
html += "  var form = document.getElementById('colorForm');";
html += "var divs = document.querySelectorAll('div');";
html += "var rojo;var verde;var azul;";
html += "var rojoStored = localStorage.getItem('rojo');";
html += "var verdeStored = localStorage.getItem('verde');";
html += "var azulStored = localStorage.getItem('azul');";
html += "function colorDiv(elemento) {";
html += "  var idDelDiv = elemento.id;";
html += "  if (idDelDiv === '1') {";
html += "    rojo = 0; verde = 204; azul = 255;console.log(rojo+' '+verde+' '+azul);";
html += "  localStorage.setItem('rojo', rojo);";
html += " localStorage.setItem('verde', verde);";
html += " localStorage.setItem('azul', azul);";
html += "} else if (idDelDiv === '2') {";
html += "  rojo = 255; verde = 165; azul = 0;console.log(rojo+' '+verde+' '+azul);";
html += "  localStorage.setItem('rojo', rojo);";
html += "  localStorage.setItem('verde', verde);";
html += "  localStorage.setItem('azul', azul);";
html += "} else if (idDelDiv === '3') {";
html += "  rojo = 255; verde = 0; azul = 255;console.log(rojo+' '+verde+' '+azul);";
html += "  localStorage.setItem('rojo', rojo);";
html += "  localStorage.setItem('verde', verde);";
html += " localStorage.setItem('azul', azul);";
html += "} else if (idDelDiv === '4') {";
html += "  rojo = 179; verde = 255; azul = 0;console.log(rojo+' '+verde+' '+azul);";
html += "  localStorage.setItem('rojo', rojo);";
html += "  localStorage.setItem('verde', verde);";
html += "  localStorage.setItem('azul', azul);";
html += "} else if (idDelDiv === '5') {";
html += "  rojo = 153; verde = 0; azul = 255;console.log(rojo+' '+verde+' '+azul);";
html += " localStorage.setItem('rojo', rojo);";
html += " localStorage.setItem('verde', verde);";
html += "  localStorage.setItem('azul', azul);";
html += " } else if (idDelDiv === '6') {";
html += "   rojo = 255; verde = 0; azul = 98;console.log(rojo+' '+verde+' '+azul);";
html += " localStorage.setItem('rojo', rojo);";
html += "  localStorage.setItem('verde', verde);";
html += "  localStorage.setItem('azul', azul);}}";
html += "divs.forEach(function (div) {";
html += "div.addEventListener('click', function () {";
html += "colorDiv(this);});});";
html += "form.addEventListener('submit', function () {";
html += " rojo = document.getElementById('rojo').value;";
html += " verde = document.getElementById('verde').value;";
html += " azul = document.getElementById('azul').value;";
html += " localStorage.setItem('rojo', rojo);";
html += " localStorage.setItem('verde', verde);";
html += "  localStorage.setItem('azul', azul);";
html += " });";
html += " if (rojoStored) document.getElementById('rojo').value = rojoStored;";
html += " if (verdeStored) document.getElementById('verde').value = verdeStored;";
html += " if (azulStored) document.getElementById('azul').value = azulStored;";
html += "});";
html += "function encender() {";
html += "  var xhttp = new XMLHttpRequest();";
html += "  xhttp.open('GET', '/encender', true);";
html += "  xhttp.send();";
html += "}";
html += "function apagar() {";
html += "  var xhttp = new XMLHttpRequest();";
html += "  xhttp.open('GET', '/apagar', true);";
html += "  xhttp.send();";
html += "}";
html += "   </script></body></html>";
  server.send(200, "text/html", html);
}

void handleColor() {
  if (server.hasArg("rojo") && server.hasArg("verde") && server.hasArg("azul")) {
    int rojo = server.arg("rojo").toInt();
    int verde = server.arg("verde").toInt();
    int azul = server.arg("azul").toInt();

    analogWrite(pinRojo, rojo);
    analogWrite(pinVerde, verde);
    analogWrite(pinAzul, azul);

    server.sendHeader("Location", "/", true);
    server.send(302, "text/plain", "");
  } else {
    server.send(400, "text/plain", "Parámetros faltantes");
  }
}
void handleEncender() {
  ledEncendido = true;
  digitalWrite(pinRojo, HIGH);
  digitalWrite(pinVerde, HIGH);
  digitalWrite(pinAzul, HIGH);
  server.send(200, "text/plain", "LED encendido");
}

void handleApagar() {
  ledEncendido = false;
  digitalWrite(pinRojo, LOW);
  digitalWrite(pinVerde, LOW);
  digitalWrite(pinAzul, LOW);
  server.send(200, "text/plain", "LED apagado");
}