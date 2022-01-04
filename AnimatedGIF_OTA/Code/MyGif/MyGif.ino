// TFT_eSPI_memory
//
// Example sketch which shows how to display an
// animated GIF image stored in FLASH memory
//
// written by Larry Bank
// bitbank@pobox.com
//
// Adapted by Bodmer for the TFT_eSPI Arduino library:
// https://github.com/Bodmer/TFT_eSPI
//
// To display a GIF from memory, a single callback function
// must be provided - GIFDRAW
// This function is called after each scan line is decoded
// and is passed the 8-bit pixels, RGB565 palette and info
// about how and where to display the line. The palette entries
// can be in little-endian or big-endian order; this is specified
// in the begin() method.
//
// The AnimatedGIF class doesn't allocate or free any memory, but the
// instance data occupies about 22.5K of RAM.

//#define USE_DMA       // ESP32 ~1.25x single frame rendering performance boost for badgers.h
                        // Note: Do not use SPI DMA if reading GIF images from SPI SD card on same bus as TFT
  #define NORMAL_SPEED  // Comment out for rame rate for render speed test

// Load GIF library
#include <AnimatedGIF.h>
AnimatedGIF gif;


#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

// Example AnimatedGIF library images

// For ESP32 TTGO 135 X 240
//#include "firewatch.h"
//#include "family.h"
//#include "DragonFire.h"
//#include "darksouls.h"
//#include "gryffindor.h"
//#include "cozyfireplace.h"
//#include "campfire.h"
//#include "redDragon.h"
//#include "bonfire.h"
//#include "sakuraTree.h"
//#include "redSunset.h"
//#include "planet.h"
                                // ESP32 40MHz SPI single frame rendering performance
                                // Note: no DMA performance gain on smaller images or transparent pixel GIFs
//#define GIF_IMAGE firewatch   //  No DMA  63 fps, DMA:  71fps
//#define GIF_IMAGE family
//#define GIF_IMAGE DragonFire    
//#define GIF_IMAGE darksouls
//#define GIF_IMAGE gryffindor
//#define GIF_IMAGE cozyfireplace
//#define GIF_IMAGE campfire
//#define GIF_IMAGE redDragon
//#define GIF_IMAGE bonfire
//#define GIF_IMAGE sakuraTree
//#define GIF_IMAGE redSunset
//#define GIF_IMAGE planet
 

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////


// For ESP32 ST7789V 320 X 240
#include "family2.h"
//#include "planet2.h"
//#include "deer.h"
//#include "bonfire2.h"     
//#include "gryffindor2.h"   
//#include "darksouls2.h" 
//#include "spirit_animals.h"
//#include "hell_dragon.h"            

#define GIF_IMAGE family2
//#define GIF_IMAGE planet2
//#define GIF_IMAGE deer
//#define GIF_IMAGE bonfire2
//#define GIF_IMAGE gryffindor2
//#define GIF_IMAGE darksouls2
//#define GIF_IMAGE spirit_animals
//#define GIF_IMAGE hell_dragon


/////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////



// For ESP32 GC9A01 240 X 240 Round
//#include "landscape.h"
//#include "bird.h"
//#include "sleeping_fox.h"
//#include "sleeping_dragon.h"
//#include "water_bender.h"
//#include "planet3.h"
//#include "deer2.h"
     

//#define GIF_IMAGE landscape
//#define GIF_IMAGE bird
//#define GIF_IMAGE sleeping_fox
//#define GIF_IMAGE sleeping_dragon
//#define GIF_IMAGE water_bender
//#define GIF_IMAGE planet3
//#define GIF_IMAGE deer2



/////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////


#include <SPI.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

// Replace with your network credentials
const char* ssid = "Shahrukh13";
const char* password = "shahan2020";


bool wifi_state = false;

#define DEBOUNCE_TIME 250
volatile uint32_t DebounceTimer = 0;

void IRAM_ATTR buttonpressed() 
{
  if ( millis() - DEBOUNCE_TIME  >= DebounceTimer ) 
  {
    DebounceTimer = millis();
    wifi_state = ! wifi_state;
  }
}

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);

  pinMode(35,INPUT);
  attachInterrupt(35, buttonpressed, FALLING);
  tft.begin();
#ifdef USE_DMA
  tft.initDMA();
#endif
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  gif.begin(BIG_ENDIAN_PIXELS);
}

#ifdef NORMAL_SPEED // Render at rate that is GIF controlled

String IP;
int x=240;
int y=135;

void loop()
{
  if(wifi_state && WiFi.status() != WL_CONNECTED)
  {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("");
  
    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    IP = WiFi.localIP().toString();
    
    tft.setTextSize(1); // Any text size muliplier will work
    tft.setTextColor(TFT_RED, TFT_BLUE);
    tft.setTextDatum(TC_DATUM); // Top Centre is datum
                                // any datum could be used
    tft.setTextPadding(0); // Setting to zero switches off padding
    tft.drawString(ssid, 80, 0, 4);
    tft.drawString(IP, 80, 50, 4);
    delay(5000);
    
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(200, "text/plain", "Hi! I am ESP32.");
    });
  
    AsyncElegantOTA.begin(&server);    // Start ElegantOTA
    server.begin();
    Serial.println("HTTP server started");
    wifi_state = 0;
  }
  if(wifi_state == 1 && WiFi.status() == WL_CONNECTED)
  {
    IP = WiFi.localIP().toString();
    Serial.println(IP);

    tft.setTextSize(1); // Any text size muliplier will work
    tft.setTextColor(TFT_RED, TFT_BLUE);
    tft.setTextDatum(TC_DATUM); // Top Centre is datum
                                // any datum could be used
    tft.setTextPadding(0); // Setting to zero switches off padding
    tft.drawString(ssid, 80, 0, 4);
    tft.drawString(IP, 80, 50, 4);
    
    wifi_state = 0;
    delay(5000);
  }
  // Put your main code here, to run repeatedly:
  if (gif.open((uint8_t *)GIF_IMAGE, sizeof(GIF_IMAGE), GIFDraw))
  {
    Serial.printf("Successfully opened GIF; Canvas size = %d x %d\n", gif.getCanvasWidth(), gif.getCanvasHeight());
    tft.startWrite(); // The TFT chip slect is locked low
    while (gif.playFrame(true, NULL))
    {
      yield();
    }
    gif.close();
    tft.endWrite(); // Release TFT chip select for other SPI devices
  }
}
#else // Test maximum rendering speed
void loop()
{
  
}
#endif
