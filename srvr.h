/**
  ******************************************************************************
  * @file    srvr.h
  * @author  Waveshare Team
  * @version V1.0.0
  * @date    23-January-2018
  * @brief   ESP8266 WiFi server.
  *          This file provides firmware functions:
  *           + Sending web page of the tool to a client's browser
  *           + Uploading images from client part by part
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include <ESP8266WiFi.h>// ESP8266 and WiFi classes
#include "buff.h"       // POST request data accumulator
#include "epd.h"        // e-Paper driver

#include "scripts.h"    // JavaScript code
#include "css.h"        // Cascading Style Sheets
#include "html.h"       // HTML page of the tool
#include <WiFiClient.h>

/* SSID and password of your WiFi net ----------------------------------------*/
const char* ssid     = "SnoopyNet";//"your ssid";
const char* password = "aircottage";//"your password";

/* Server and IP address ------------------------------------------------------*/
WiFiServer server(80); // Wifi server exemplar using port 80
IPAddress myIP;       // IP address in your local wifi net

/* The 'index' page flag ------------------------------------------------------*/
bool isIndexPage = true; // true : GET  request, client needs 'index' page;
                         // false: POST request, server sends empty page.

WiFiClient nvrsrv;

ADC_MODE(ADC_VCC);

uint64_t deepSleepTimeInS = 3600*2;
void pingNVR() {

  float volts = ESP.getVcc();                                                                                                                 
  Serial.printf("The internal VCC reads %1.2f volts\n", volts / 1000); 
  Serial.printf("LED_BUILTIN: %d\n", LED_BUILTIN); 
  //pinMode(16, OUTPUT);      
  //digitalWrite(16,HIGH);
  
  int count=0;
  Serial.println("Ping NVR starts");
  if (!nvrsrv.connect("nvrnas.cottage", 2323)) {
    Serial.println("Connection to nvrsrv.cottage:23 failed.");
    return;
  }
  char voltageString[20];
  sprintf(voltageString, "%1.2f\n", volts/1000);
  nvrsrv.write(voltageString, strlen(voltageString));
  String ip = myIP.toString();
  const char *cIP = ip.c_str();
  nvrsrv.write(cIP, strlen(cIP));
  nvrsrv.write("\n", 1);
  bool sawVoltage = false;
  while (nvrsrv.connected() && ++count < 4) {
    Serial.println("read a line");
    String line = nvrsrv.readStringUntil('\n');
    if (sawVoltage) {
      sscanf(line.c_str(), "%d", &deepSleepTimeInS);
      char message[256];
      sprintf(message, "Time delay should be %ds", deepSleepTimeInS);
      Serial.println(message);
    }
    Serial.println(line);
    sawVoltage = line.startsWith("Voltage");
    Serial.println("done line");
  }
}
/* Server initialization ------------------------------------------------------*/
void Srvr__setup() 
{  
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    // Applying SSID and password
    WiFi.begin(ssid, password); 

    // Waiting the connection to a router
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.print(".");
    }

    // Connection is complete
    Serial.println("");
    Serial.println("WiFi connected");

    // Start the server
    server.begin();
    Serial.println("Server started");

    // Show obtained IP address in local Wifi net
    Serial.println(myIP = WiFi.localIP());
    pingNVR();
}

/* Sending a script to the client's browser ------------------------------------*/
void Srvr__file(WiFiClient client, int fileIndex)
{
    // Print log message: sending of script file
    Serial.println("Script");

    // Clear data stream of the 'client'
    client.flush();

    // Sent to the 'client' the header describing the type of data.
    // In this case 'Content-Type' is 'text/javascript'
    client.print("HTTP/1.1 200 OK\r\nContent-Type: text/javascript\r\n\r\n");

    // Choose the index of script
    // (ESP8266 can't to send all of code by one file
    // and needs split it on a few parts)
    switch (fileIndex)
    {
        case 0: sendJS_A(client); break;
        case 1: sendJS_B(client); break;
        case 2: sendJS_C(client); break;
        case 3: sendJS_D(client); break;
    }
    
    // Print log message: the end of request processing
    delay(1);
    Serial.println(">>>");  
}

/* The server state observation loop -------------------------------------------*/
bool state_loop() 
{
    // Looking for a client trying to connect to the server
    WiFiClient client = server.available();   
    bool powerOff = false;

    // Exit if there is no any clients
    if (!client) return powerOff;

    // Print log message: the start of request processing
    Serial.print("<<<");

    // Waiting the client is ready to send data
    while(!client.available()) delay(1);

    // Set buffer's index to zero
    // It means the buffer is empty initially
    Buff__bufInd = 0;

    // Data transfer result (true - successful, false - failed)
    bool dataTrRes = false;

    // While the stream of 'client' has some data do...
    while (client.available())
    {
        // Read a character from 'client'
        int q = client.read();

 
        // Save it in the buffer and increment its index
        Buff__bufArr[Buff__bufInd++] = (byte)q;
        //Serial.println(q, HEX);

        // If the carachter means the end of line, then... 
        if ((q == 10) || (q == 13))
        {
            // Clean the buffer
            Buff__bufInd = 0; 
            Serial.println("CLEAR");
            continue;
        }

        // If data accumulated in buffer end with "styles.css", then
        if ((Buff__bufInd > 11) && Buff__signature(Buff__bufInd - 11, "/styles.css"))
        {
            // Print log message: sending of Cascading Style Sheets file
            Serial.println("Style"); 

            // Clear data stream of the 'client'
            client.flush();

            // Sent to the 'client' the header describing the type of data.
            // In this case 'Content-Type' is 'text/css'
            client.print("HTTP/1.1 200 OK\r\nContent-Type: text/css\r\n\r\n");

            // Send the Cascading Style Sheets file to the 'client'
            sendCSS(client);
            delay(1);

            // Print log message: the end of request processing
            Serial.println(">>>");
            return powerOff;
        }

        // If the buffer's length is larger, than 13 (length of '/processingX.js'), then...
        if (Buff__bufInd > 15)
        {
            // If the buffer's data end with '/processingA.js', then...
            if (Buff__signature(Buff__bufInd - 16, "/processingA.js"))
            {
                // Send the 1st part of code to the 'client'
                Srvr__file(client, 0);
                return powerOff;
            }

            // If the buffer's data end with '/processingB.js', then...
            if (Buff__signature(Buff__bufInd - 16, "/processingB.js"))
            {
                // Send the 2nd part of code to the 'client'
                Srvr__file(client, 1);
                return powerOff;
            }

            // If the buffer's data end with '/processingC.js', then...
            if (Buff__signature(Buff__bufInd - 16, "/processingC.js"))
            {
                // Send the 3rd part of code to the 'client'
                Srvr__file(client, 2);
                return powerOff;
            }

            // If the buffer's data end with '/uploading.js', then...
            if (Buff__signature(Buff__bufInd - 14, "/uploadingD.js"))
            {
                // Send the 4th part of code to the 'client'
                Srvr__file(client, 3);
                return powerOff;
            }
        }

        // If the buffer's length is larger, than 4 (length of command's name), then...
        if (Buff__bufInd > 4)
        {
            // It is probably POST request, no need to send the 'index' page
            isIndexPage = false;
       if (0 && q >= 'A' && q <= 'Z' && Buff__bufArr[Buff__bufInd-4] <= 'Z') {
            Buff__bufArr[Buff__bufInd] = 0;
            Serial.println("Buffer interpreted as POST:");
            Serial.println(Buff__bufArr);
        }

            // e-Paper driver initialization
            if (Buff__signature(Buff__bufInd - 5, "EPD")) 
            {
                // Getting of e-Paper's type
                EPD_dispIndex = Buff__getByte(Buff__bufInd - 2);

                // Print log message: initialization of e-Paper (e-Paper's type)
                Serial.printf("EPD %s", EPD_dispMass[EPD_dispIndex].title);

                // Initialization
                EPD_dispInit();
                dataTrRes = true;
                break; 
            }
            
            // Power off
            if (Buff__signature(Buff__bufInd - 4, "POFF")) {
              Serial.println("Power off.");
              dataTrRes = true;
              powerOff = true;
              break;
            }
            // Image loading
            if (Buff__signature(Buff__bufInd - 4, "LOAD"))
            {
                // Print log message: image loading
                Serial.println("LOAD");

                // Load data into the e-Paper 
                // if there is loading function for current channel (black or red)
                if (EPD_dispLoad != 0) {
                  Serial.println("Sent to dispLoad");
                  EPD_dispLoad(); 
                  Buff__bufInd = 0;
                }
                dataTrRes = true;    
                break;
            }
            
            // Select the next data channel
            if (Buff__signature(Buff__bufInd - 4, "NEXT"))
            {
                // Print log message: next data channel
                Serial.print("NEXT");

                // Instruction code for for writting data into 
                // e-Paper's memory
                int code = EPD_dispMass[EPD_dispIndex].next;

                // e-Paper '2.7' (index 7) needs inverting of image data bits
                EPD_invert = (EPD_dispIndex == 7);

                // If the instruction code isn't '-1', then...
                if (code != -1)
                {
                    // Print log message: instruction code
                    Serial.printf(" %d", code);

                    // Do the selection of the next data channel
                    EPD_SendCommand(code);
                    delay(2);
                }

                // Setup the function for loading choosen channel's data
                EPD_dispLoad = EPD_dispMass[EPD_dispIndex].chRd;
                dataTrRes = true;
                break;
            }
            
            // If the loading is complete, then...
            if (Buff__signature(Buff__bufInd - 4, "SHOW")) 
            {
                // Show results and Sleep
                EPD_dispMass[EPD_dispIndex].show();

                //Print log message: show
                Serial.print("SHOW");
                dataTrRes = true;
                break; 
            }

            // If the routine reaches this code,
            // it means the there is no known commands,
            // the server has to send the 'index' page
            isIndexPage = true;
        }
    }



    // Clear data stream of the 'client'
    client.flush();

    // Sent to the 'client' the header describing the type of data.
    // In this case 'Content-Type' is 'text/html'
    if (dataTrRes) {
      client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nAccess-Control-Allow-Origin: *\r\n\r\n");
    }
    else 
    {
      Serial.println("ERROR");
      Buff__bufArr[Buff__bufInd] = 0;
            Serial.println("Buffer that caused the error:");
            Serial.println(Buff__bufArr);
      client.print("HTTP/1.1 0 OK\r\nContent-Type: text/html\r\nAccess-Control-Allow-Origin: *\r\n\r\n");
    }

    // Send the 'index' page if it's needed
    if (isIndexPage) sendHtml(client, myIP); else client.print("Ok!");
    
    client.print("\r\n");
    delay(1);

    // Print log message: the end of request processing
    Serial.println(">>>");
    return powerOff;
}

void Srvr__loop() {
  if (state_loop()) {
    Serial.println("Power off Now.");
    delay(500);
    uint64_t t = deepSleepTimeInS * ((uint64_t)1000000);
    ESP.deepSleep(t, WAKE_RF_DEFAULT); 
  }
}
