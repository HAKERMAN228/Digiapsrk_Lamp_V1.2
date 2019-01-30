// ************************** НАСТРОЙКИ ***********************
#define PIXELPIN   0 // пин ленты 
#define BUTTONPIN  2 // пин кнопки
#define MAXFUN     8 // количество эфектов 
#define PIXEL_COUNT  8// количество светодиодов 
// ************************** ДЛЯ РАЗРАБОТЧИКОВ ***********************
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXELPIN, NEO_GRB + NEO_KHZ800);

int fun = 0;

uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
//****************************РЕЖИМЫ****************************************
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

//********************************************************************
void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
    if (buttonPressed()) return;
  }
}

//********************************************************************
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { 
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
    if (buttonPressed()) return;
  }
}

//********************************************************************
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    
        }
        strip.show();
       
        delay(wait);
       
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        
        }
    }
    if (buttonPressed()) return;
  }
}

//********************************************************************
void movingColor(uint8_t wait) {
  uint16_t i, j;

  colorWipe(strip.Color(0, 0, 0),0);
  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
//      strip.setPixelColor(i, Wheel((i+j) & 255));
      strip.setPixelColor(i, Wheel((j*4)%256));
      strip.show();
      delay(wait);
      if (buttonPressed()) return;
      strip.setPixelColor(i, strip.Color(0, 0, 0));
    }
  }
}

//********************************************************************
void psycho(uint8_t wait) {
  colorWipe(Wheel(random(255)),0);
  strip.show();
  delay(wait);
  buttonPressed();
}

//********************************************************************
void fadingColor(uint8_t wait) {
  uint16_t i;
  uint32_t c;

  c = Wheel(random(255));
  for(i=0; i<255; i++) {
    strip.setBrightness(i);
    colorWipe(c,0);
    strip.show();
    delay(wait);
    if (buttonPressed()) {
      strip.setBrightness(255);
      return;
    }
 }
  for(i=255; i>1; i--) {
    strip.setBrightness(i);
    colorWipe(c,0);
    strip.show();
    delay(wait);
    if (buttonPressed()) {
      strip.setBrightness(255);
      return;
    }
  }
}

//********************************************************************
void bouncingColor(uint8_t wait) {
  uint16_t i, j;
  uint32_t c;

  colorWipe(strip.Color(0, 0, 0),0);
  c = strip.Color(0, 0, 0);
  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.setPixelColor(strip.numPixels()-i, c);
      strip.show();
      delay(wait);
      strip.setPixelColor(i, strip.Color(0, 0, 0));
      strip.setPixelColor(strip.numPixels()-i, strip.Color(0, 0, 0));
      if (i%(strip.numPixels()/2) == 0)
        c = Wheel( random(255) % 255);
      if (buttonPressed()) return;
    }
  }
}

//********************************************************************
void white(void) {
  colorWipe(strip.Color(255, 255, 255),0);
  strip.show();
  buttonPressed();
}

//********************************************************************
void black(void) {
  colorWipe(strip.Color(0, 0, 0),0);
  strip.show();
  while (!buttonPressed());
}

//********************************************************************
boolean buttonPressed() {
  boolean pressed = !digitalRead(BUTTONPIN);
  if (pressed) {
    delay(10);
    pressed = !digitalRead(BUTTONPIN);
    if (pressed) {
      fun++;
      if (fun>MAXFUN) fun = 0;
      while (!digitalRead(BUTTONPIN));
    }  
  }
  return (pressed);
}

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  pinMode(BUTTONPIN, INPUT);
}

void loop() {
  switch (fun) {
  case 0:
      rainbow(20);
      break;
  case 1:
      rainbowCycle(20);
      break;
  case 2:
      theaterChaseRainbow(50);
      break;
  case 3:
      movingColor(20);
      break;
  case 4:
      bouncingColor(200);
      break;
  case 5:
      fadingColor(5);
      break;
  case 6:
      psycho(20);
      break;
  case 7:
      white();
      break;
  case 8:
      black();
      break;
  }
}
