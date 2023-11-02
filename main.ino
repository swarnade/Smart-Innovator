
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSerif9pt7b.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);





const char* ssid = "Swarnadeep SP";
const char* password = "12345678";


WiFiServer server(80);


String header;



String OutputRelay1 = "off";
String OutputRelay2 = "off";


const int outrelay1 = 26;
const int outrelay2 = 4;

 int Led1=0;
 int Led2=0;



unsigned long currentTime = millis();

unsigned long previousTime = 0; 

const long timeoutTime = 2000;


void disp(int Led1 ,int Led2)
{
              if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
              {
                Serial.println(F("SSD1306 allocation failed"));
                for (;;)
                  ;
              } 
              display.clearDisplay();

              if(Led1==0 && Led2==0)
              {

              //Condition When Both Led Are In In-Active
              display.setFont(&FreeSerif9pt7b);
              display.clearDisplay();
              display.setTextSize(1);
              display.setTextColor(WHITE);
              display.setCursor(0, 20);
              display.println("Relay 1 Is Off");
              display.display();
              display.setCursor(0, 55);
              display.println("Relay 2 Is Off");
              display.display();
              }

              else if(Led1==0 && Led2==1)
              {

              //Condition When Led1 Is Closed And Led2 Is Open
              display.setFont(&FreeSerif9pt7b);
              display.clearDisplay();
              display.setTextSize(1);
              display.setTextColor(WHITE);
              display.setCursor(0, 20);
              display.println("Relay 1 Is Off");
              display.display();
              display.setCursor(0, 55);
              display.println("Relay 2 Is On");
              display.display();
              }





              else if(Led1==1 && Led2==0)
              {

              //Condition When Led1 Is Open And Led2 Is Closed
              display.setFont(&FreeSerif9pt7b);
              display.clearDisplay();
              display.setTextSize(1);
              display.setTextColor(WHITE);
              display.setCursor(0, 20);
              display.println("Relay 1 Is On");
              display.display();
              display.setCursor(0, 55);
              display.println("Relay 2 Is Off");
              display.display();
              }




              else if(Led1==1 && Led2==1)
              {

              //Condition When Both Led Are Open
              display.setFont(&FreeSerif9pt7b);
              display.clearDisplay();
              display.setTextSize(1);
              display.setTextColor(WHITE);
              display.setCursor(0, 20);
              display.println("Relay 1 Is On");
              display.display();
              display.setCursor(0, 55);
              display.println("Relay 2 Is On");
              display.display();
              }
}





void setup() {
  Serial.begin(115200);

  pinMode(outrelay1, OUTPUT);
  pinMode(outrelay2, OUTPUT);

  digitalWrite(outrelay1, HIGH);
  digitalWrite(outrelay2, HIGH);


  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  
}

void loop(){
  WiFiClient client = server.available();   

  if (client) {                             
    currentTime = millis();
    previousTime = currentTime;
    // Client Connected
    Serial.println("New Client.");          
    String currentLine = "";                
    while (client.connected() && currentTime - previousTime <= timeoutTime) { 
      currentTime = millis();
      if (client.available()) {             
        char c = client.read();             
        Serial.write(c);                    
        header += c;
        if (c == '\n') {                   
         
          if (currentLine.length() == 0) {

            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            

            if (header.indexOf("GET /26/on") >= 0) {
              Serial.println("Relay 1 on");
              OutputRelay1 = "on";
              digitalWrite(outrelay1, LOW);
              Led1=1;
            }

            else if (header.indexOf("GET /26/off") >= 0) {
              Serial.println("Relay 1 off");
              OutputRelay1 = "off";
              digitalWrite(outrelay1, HIGH);
              Led1=0;
            }

            else if (header.indexOf("GET /27/on") >= 0) {
              Serial.println("Relay 2 on");
              OutputRelay2 = "on";
              digitalWrite(outrelay2, LOW);
              Led2=1;
            }

            else if (header.indexOf("GET /27/off") >= 0) {
              Serial.println("Relay 2 off");
              OutputRelay2 = "off";
              digitalWrite(outrelay2, HIGH);
              Led2=0;
            }
            
            disp(Led1,Led2);
            

            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");

            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: black; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555546;}</style></head>");
            

            client.println("<body><h1>ESP32 Web Server</h1>");
            
 
            client.println("<p>Relay 1 - State " + OutputRelay1 + "</p>");
     
            if (OutputRelay1=="off") {
              client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
 
            client.println("<p>Relay 2 - State " + OutputRelay2 + "</p>");
      
            if (OutputRelay2=="off") {
              client.println("<p><a href=\"/27/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/27/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");

            

            client.println();

            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') { 
          currentLine += c;      
        }
      }
    }

    header = "";

    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

