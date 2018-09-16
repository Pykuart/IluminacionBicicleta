// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      6

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYCONST   300
#define PINFRENO       4
#define PININTDER      3
#define PININTIZQ      2

int delayval = DELAYCONST / NUMPIXELS; // delay for half a second

int intensidad = 127;

uint32_t colorLuzFreno = pixels.Color(intensidad *2, 0, 0);
uint32_t colorLuzPosicion = pixels.Color(intensidad/2, 0, 0);
uint32_t colorLuzIntermitente = pixels.Color(intensidad, intensidad/2, 0);
uint32_t colorLuzApagada = pixels.Color(0, 0, 0);
uint32_t colorLuzBlanca = pixels.Color(intensidad, intensidad, intensidad);
uint32_t background;

void setup() {

  limpiarLuces();
  background = colorLuzApagada;
  pinMode(PINFRENO, INPUT);
  pinMode(PININTDER, INPUT);
  pinMode(PININTIZQ, INPUT);
}

void loop() {

  int sensorPinFreno = digitalRead(PINFRENO);
  delay(5);
  int sensorPinIntDer = digitalRead(PININTDER);
  delay(5);
  int sensorPinIntIzq = digitalRead(PININTIZQ);
  delay(5);
  
  background = colorLuzPosicion;
  if(sensorPinFreno == HIGH){
    background = colorLuzFreno;
  }

  if(sensorPinIntDer == HIGH){
    intermitenteDerecha();
  }else if(sensorPinIntIzq == HIGH){
    intermitenteIzquierda();
  }else{
    luzDeFondo();
  }
  delay(500);
}

/**
 * Marca las luces con color de posicion
 */
void luzDeFondo() {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, background); // Moderately bright green color.
  }
  pixels.show(); // This sends the updated pixel color to the hardware.
}

/**
 * Marca las luces con color de freno
 */
void luzFreno() {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, colorLuzFreno); // Moderately bright green color.
  }
  pixels.show(); // This sends the updated pixel color to the hardware.
}

/**
 * activa las luces con la intermitencia a la derecha
 */
void intermitenteDerecha() {
  long milliseconds = millis() + 5000;
  boolean esFondo = false;
  uint32_t color = colorLuzApagada;
  while (millis() < milliseconds) {
    for (int i = (NUMPIXELS/2); i < NUMPIXELS; i++) {
      color = colorLuzIntermitente;
      if (esFondo) {
        color = background;
      }
      pixels.setPixelColor(i, color); // Moderately bright green color.
      pixels.show(); // This sends the updated pixel color to the hardware.
      delay(delayval);
    }
    esFondo = !esFondo;
  }
  color = background;
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, color); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(delayval);
  }
}

/**
 * Activa la luces con la intermitencia a la izquierda
 */
void intermitenteIzquierda() {
  long milliseconds = millis() + 5000;
  boolean esFondo = false;
  uint32_t color = background;
  while (millis() < milliseconds) {
    for (int i = (NUMPIXELS/2)-1; i >= 0 ; i--) {
      color = colorLuzIntermitente;
      if (esFondo) {
        color = background;
      }
      pixels.setPixelColor(i, color); // Moderately bright green color.
      pixels.show(); // This sends the updated pixel color to the hardware.
      delay(delayval);
    }
    esFondo = !esFondo;
  }
  color = background;
  for (int i = NUMPIXELS; i >= 0 ; i--) {
    pixels.setPixelColor(i, color); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(delayval);
  }
}

void limpiarLuces() {
  pixels.begin(); // This initializes the NeoPixel library.
  pixels.show(); // This sends the updated pixel color to the hardware.
}

