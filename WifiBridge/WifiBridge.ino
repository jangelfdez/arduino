#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

#define RXpin 20
#define TXpin 21

#define LED_BUILTIN 8

//const char *ssid = "test-serie";
//const char *password = NULL;

const char* ssid = "LaCasaDomotica";             // Change this to your WiFi SSID
const char* password = "";  // Change this to your WiFi password

String blind_status = "Subida";
String garage_status = "Cerrada";
String light_status = "ON";
String movement_status = "OFF";

WiFiServer server(80);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  Serial0.begin(115200, SERIAL_8N1, RXpin, TXpin);


  Serial.println();
  Serial.println("******************************************************");
  Serial.print("Connecting to ");
  Serial.println(ssid);

  //WiFi.begin(ssid, password);

  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  //   Serial.print(".");
  // }

  // Serial.println("");
  // Serial.println("WiFi connected");
  // Serial.println("IP address: ");
  // Serial.println(WiFi.localIP());

  if (!WiFi.softAP(ssid, password)) {
    Serial.println("Soft AP creation failed.");
    while (1)
      ;
  }
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();


  Serial.println("Server started");
}

void loop() {

  WiFiClient client = server.accept();  // listen for incoming clients

  if (client) {  // if you get a client,
    //Serial.println("New Client.");  // print a message out the serial port
    String currentLine = "";      // make a String to hold incoming data from the client
    while (client.connected()) {  // loop while the client's connected
      if (client.available()) {   // if there's bytes to read from the client,
        char c = client.read();   // read a byte, then
        //Serial.write(c);
        if (c == '\n') {  // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // // and a content-type so the client knows what's coming, then a blank line:
            // client.println("HTTP/1.1 200 OK");
            // client.println("Content-type:text/html");
            // client.println();

            // // the content of the HTTP response follows the header:
            // client.print("Click <a href=\"/0\">here</a> to Read Garage Door Status.<br>");
            // client.print("Click <a href=\"/1\">here</a> to Open Garage Door.<br>");
            // client.print("Click <a href=\"/2\">here</a> to Close Garage Door.<br>");
            // client.print("Click <a href=\"/3\">here</a> to Read House Temperature.<br>");
            // client.print("Click <a href=\"/4\">here</a> to Read House Humidity.<br>");
            // client.print("Click <a href=\"/5\">here</a> to Read Movement Sensor Status.<br>");
            // client.print("Click <a href=\"/6\">here</a> to Read Outside Lights Status.<br>");

            // // The HTTP response ends with another blank line:
            // client.println();
            // // break out of the while loop:
            break;
          } else {  // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /0")) {
          Serial.println("0 [Read Garage Door Status]");
          Serial0.write("0");
          delay(500);
          String incoming_command = Serial0.readString();
          Serial.println("RX Command > " + incoming_command);
          
          Serial.println("0");
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();

          // the content of the HTTP response follows the header:
          client.print(incoming_command);

          // The HTTP response ends with another blank line:
          client.println();
        }
        if (currentLine.endsWith("GET /1")) {
          Serial.println("1 [Open Garage Door]");
          Serial0.write("1");  // print it out the serial monitor
          delay(500);
          String incoming_command = Serial0.readString();
          Serial.println("RX Command > " + incoming_command);
  
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();

          // the content of the HTTP response follows the header:
          client.print(incoming_command);

          // The HTTP response ends with another blank line:
          client.println();
        }
        if (currentLine.endsWith("GET /2")) {
          Serial.println("2 [Close Garage Door]");

          Serial0.write("2");  // print it out the serial monitor 
          delay(500);
          String incoming_command = Serial0.readString();
          Serial.println("RX Command > " + incoming_command);

          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();

          // the content of the HTTP response follows the header:
          client.print(incoming_command);

          // The HTTP response ends with another blank line:
          client.println();
        }
        if (currentLine.endsWith("GET /3")) {
          Serial.println("3 [Read House Temperature]");
          Serial0.write("3");  // print it out the serial monitor
          delay(500);
          String incoming_command = Serial0.readString();
          Serial.println("RX Command > " + incoming_command);

          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();

          // the content of the HTTP response follows the header:
          client.print(incoming_command);

          // The HTTP response ends with another blank line:
          client.println();
        }
        if (currentLine.endsWith("GET /4")) {
          Serial.println("4 [Read House Humidity]");

          Serial0.write("4");  // print it out the serial monitor
          delay(500);
          String incoming_command = Serial0.readString();
          Serial.println("RX Command > " + incoming_command);
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();

          // the content of the HTTP response follows the header:
          client.print(incoming_command);

          // The HTTP response ends with another blank line:
          client.println();
        }
        if (currentLine.endsWith("GET /5")) {
          Serial.println("5 [Read Movement Sensor Status]");

          Serial0.write("5");  // print it out the serial monitor
          delay(500);
          String incoming_command = Serial0.readString();
          Serial.println("RX Command > " + incoming_command);
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();

          // the content of the HTTP response follows the header:
          client.print(incoming_command);

          // The HTTP response ends with another blank line:
          client.println();
        }
        if (currentLine.endsWith("GET /6")) {
          Serial.println("6 [Read Outside Lights Status]");

          Serial0.write("6");  // print it out the serial monitor
          delay(500);
          String incoming_command = Serial0.readString();
          Serial.println("RX Command > " + incoming_command);
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();

          // the content of the HTTP response follows the header:
          client.print(incoming_command);

          // The HTTP response ends with another blank line:
          client.println();
        }
        if (currentLine.endsWith("GET /7")) {
          Serial.println("7 [Read Blind Status]");

          Serial0.write("7");  // print it out the serial monitor
          delay(500);
          String incoming_command = Serial0.readString();
          Serial.println("RX Command > " + incoming_command);
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();

          // the content of the HTTP response follows the header:
          client.print(incoming_command);

          // The HTTP response ends with another blank line:
          client.println();
        }
        if (currentLine.endsWith("GET /8")) {
          Serial.println("8 [Open Blind]");

          Serial0.write("8");  // print it out the serial monitor
          delay(500);
          String incoming_command = Serial0.readString();
          Serial.println("RX Command > " + incoming_command);
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();

          // the content of the HTTP response follows the header:
          client.print(incoming_command);

          // The HTTP response ends with another blank line:
          client.println();
        }
        if (currentLine.endsWith("GET /9")) {
          Serial.println("9 [Close Blind]");

          Serial0.write("9");  // print it out the serial monitor
          delay(500);
          String incoming_command = Serial0.readString();
          Serial.println("RX Command > " + incoming_command);

          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();

          // the content of the HTTP response follows the header:
          client.print(incoming_command);

          // The HTTP response ends with another blank line:
          client.println();
        }
      }
    }
    // close the connection:
    client.stop();
    //Serial.println("Client Disconnected.");
  }
}
