#include <FastLED_NeoMatrix.h>
#include <FastLED.h>

#define PIN 5
#define POTPIN 0
#define BRIGHTNESS 255

#define mw 8
#define mh 8
#define NumLEDS mw*mh

#define NUMMATRIX (mw*mh)
CRGB leds[NUMMATRIX];

FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(leds, 8, 8, 1, 1,
    NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
    NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE +
    NEO_TILE_TOP + NEO_TILE_LEFT +  NEO_TILE_PROGRESSIVE);

#define LED_BLACK           0

#define LED_RED_VERYLOW     (3 <<  11)
#define LED_RED_LOW         (7 <<  11)
#define LED_RED_MEDIUM      (15 << 11)
#define LED_RED_HIGH        (31 << 11)

#define LED_GREEN_VERYLOW   (1 <<  5)
#define LED_GREEN_LOW       (15 << 5)
#define LED_GREEN_MEDIUM    (31 << 5)
#define LED_GREEN_HIGH      (63 << 5)

#define LED_BLUE_VERYLOW    3
#define LED_BLUE_LOW        7
#define LED_BLUE_MEDIUM     15
#define LED_BLUE_HIGH       31

#define LED_ORANGE_VERYLOW  (LED_RED_VERYLOW + LED_GREEN_VERYLOW)
#define LED_ORANGE_LOW      (LED_RED_LOW     + LED_GREEN_LOW)
#define LED_ORANGE_MEDIUM   (LED_RED_MEDIUM  + LED_GREEN_MEDIUM)
#define LED_ORANGE_HIGH     (LED_RED_HIGH    + LED_GREEN_HIGH)

#define LED_PURPLE_VERYLOW  (LED_RED_VERYLOW + LED_BLUE_VERYLOW)
#define LED_PURPLE_LOW      (LED_RED_LOW     + LED_BLUE_LOW)
#define LED_PURPLE_MEDIUM   (LED_RED_MEDIUM  + LED_BLUE_MEDIUM)
#define LED_PURPLE_HIGH     (LED_RED_HIGH    + LED_BLUE_HIGH)

#define LED_CYAN_VERYLOW    (LED_GREEN_VERYLOW + LED_BLUE_VERYLOW)
#define LED_CYAN_LOW        (LED_GREEN_LOW     + LED_BLUE_LOW)
#define LED_CYAN_MEDIUM     (LED_GREEN_MEDIUM  + LED_BLUE_MEDIUM)
#define LED_CYAN_HIGH       (LED_GREEN_HIGH    + LED_BLUE_HIGH)

#define LED_WHITE_VERYLOW   (LED_RED_VERYLOW + LED_GREEN_VERYLOW + LED_BLUE_VERYLOW)
#define LED_WHITE_LOW       (LED_RED_LOW     + LED_GREEN_LOW     + LED_BLUE_LOW)
#define LED_WHITE_MEDIUM    (LED_RED_MEDIUM  + LED_GREEN_MEDIUM  + LED_BLUE_MEDIUM)
#define LED_WHITE_HIGH      (LED_RED_HIGH    + LED_GREEN_HIGH    + LED_BLUE_HIGH)

int colour1 = 0;
int colour2 = 0;
int colour3 = 0;
int colour4 = 0;
int colour = 0;
int fxnumber = 0;
int lastfx = 1;
int lastdash = 1;
int fxdir = 0;
int x = 0;
int y = 0;
int z = 0;
int frameno = 0;
int dashheight = random(6);
int patterndelay = 100;
int effectduration = 12;
int firstrun = 1;

void setup() {
  delay(1000);
//  Serial.begin(115200);
  pickcolourset();
  matrix->begin();
  matrix->setBrightness(BRIGHTNESS);
  FastLED.addLeds<WS2812B, PIN>(  leds, NUMMATRIX  ).setCorrection(TypicalLEDStrip);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 4000);  
}

typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { squarechase1,
                                squares,
                                vlinesup,
                                hdash,
                                growingbeams,
                                randomdots,
                                hlinesl,
                                beamscanner1,
                                hcrossover,
                                discolight,
                                rotatingblocks1,
                                vdash2,
                                hlines,
                                explode,
                                dashbackv,
                                randomsquares,
                                squarechase2,
                                rsquares,
                                vlinesdn,
                                vdash,
                                beamscanner2,
                                flashingsquares,
                                hlinesr,
                                shrinkingbeams,
                                dashbackh,
                                vcrossover,
                                randomcircles,
                                vlines,
                                hdash2,
                                rotatingblocks2
                              };

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

int totalpatterns = ARRAY_SIZE ( gPatterns);
int fxlist[ARRAY_SIZE ( gPatterns)];
int colourlist[7];

void loop() {
  if (firstrun == 1) {
    for (int j=0;j<totalpatterns;j++){
      fxlist[j]=j;
    }
    for (int j=0;j<7;j++){
      colourlist[j]=j;
    }
    randomSeed(analogRead(A3));
    Serial.print("FastLED ");
    Serial.print(mh);
    Serial.print(" x ");
    Serial.print(mw);
    Serial.print(" Matrix Initialized with ");
    Serial.print(totalpatterns);
    Serial.print(" Patterns Loaded");
//    Serial.end();
    firstrun = 0;
    nexteffect();
  }
  EVERY_N_MILLIS_I(thisTimer,100){
    patterndelay=analogRead(POTPIN)/4;
    thisTimer.setPeriod(patterndelay);  
    matrix->clear();
    gPatterns[fxlist[fxnumber]]();
    matrix->show();
  }
  EVERY_N_SECONDS(effectduration) {
    nexteffect();
  }
}

void squares()
{

  if (x < 3) x++; else x = 0;
  if (z < 6)z = z + 2; else {
    z = 0;
    pickcolourset();
  }
  matrix->drawRect(x, x, 8 - z, 8 - z, colour1);
  delay(patterndelay);
}

void rsquares()
{
  if (x > 0) x--; else x = 3;
  if (z < 8) z = z + 2; else {
    z = 2;
    pickcolourset();
  }
  matrix->drawRect(x, x, z, z, colour1);
  delay(patterndelay);
}

void hlines() {
  switch (fxdir) {
    case 0:
      matrix->drawRect(x, 0, 2, 8, colour1);
      if (x > 0) x--;
      else {
        x = 0;
        fxdir = 1;
      }
      break;
    case 1:
      matrix->drawRect(x, 0, 2, 8, colour1);
      if (x < 6) x++;
      else {
        x = 6;
        pickcolourset();
        fxdir = 0;
      }
      break;
  }
}

void vlines() {
  switch (fxdir) {
    case 0:
      matrix->drawRect(0, y, 8, 2, colour1);
      if (y > 0) y--;
      else {
        y = 0;
        fxdir = 1;
      }
      break;
    case 1:
      matrix->drawRect(0, y, 8, 2, colour1);
      if (y < 6) y++;
      else {
        y = 6;
        pickcolourset();
        fxdir = 0;
      }
      break;
  }
}

void hlinesl() {
  if (x >= 0) x--;
  else {
    x = 8;
    pickcolourset();
  }
  switch (x){
    default:
      matrix->drawRect(x, 0, 2, 8, colour1);
    break;
    case -1:
      matrix->drawRect(0, 0, 1, 8, colour1);
    break;
    case 8:
      matrix->drawRect(x, 0, 1, 8, colour1);
    break;
  }
    delay(patterndelay);
}

void hlinesr() {
  if (x < 7) x++;
  else {
    x = -1;
    pickcolourset();
  }
  switch (x){
    default:
      matrix->drawRect(x, 0, 2, 8, colour1);
    break;
    case -1:
      matrix->drawRect(0, 0, 1, 8, colour1);
    break;
    case 8:
      matrix->drawRect(x, 0, 1, 8, colour1);
    break;
  }
    delay(patterndelay);
}

void vlinesup()
{
  if (y < 7) y++;
  else {
    y = -1;
    pickcolourset();
  }

  switch (y){
    default:
      matrix->drawRect(0, y, 8, 2, colour1);
    break;
    case -1:
      matrix->drawRect(0, 0, 8, 1, colour1);
    break;
    case 8:
      matrix->drawRect(0, y, 8, 1, colour1);
    break;
  }  
    delay(patterndelay);
}

void vlinesdn()
{
  if (y > 0) y--;
  else {
    y = 8;
    pickcolourset();
  }
  switch (y){
    default:
      matrix->drawRect(0, y, 8, 2, colour1);
    break;
    case 0:
      matrix->drawRect(0, 0, 8, 1, colour1);
    break;
    case 8:
      matrix->drawRect(0, y, 8, 1, colour1);
    break;
  }  
}

void vdash() {
  if (y < 7) y++;
  else {
    y = -1;
    pickdashheight();
    pickcolourset();
  }
  switch (y){
    default:
      matrix->drawRect(dashheight, y, 2, 2, colour1);
    break;
    case -1:
      matrix->drawRect(dashheight, 0, 2, 1, colour1);
    break;
    case 8:
      matrix->drawRect(dashheight, y, 2, 1, colour1);
    break;
  }
    delay(patterndelay);
}

void vdash2() {
  if (y > 0) y--;
  else {
    y = 8;
    pickdashheight();
    pickcolourset();
  }
  switch (y){
    default:
  matrix->drawRect(dashheight, y, 2, 2, colour1);
    break;
    case 0:
  matrix->drawRect(dashheight, 0, 2, 1, colour1);
    break;
    case 8:
  matrix->drawRect(dashheight, y, 2, 1, colour1);
    break;
  }
}

void hdash() {
  if (x < 7) x++;
  else {
    x = -1;
    pickdashheight();
    pickcolourset();
  }
    switch (x){
    default:
      matrix->drawRect(x, dashheight, 2, 2, colour1);
    break;
    case -1:
      matrix->drawRect(0, dashheight, 1, 2, colour1);
    break;
    case 8:
      matrix->drawRect(x, dashheight, 1, 2, colour1);
    break;
  }
}

void hdash2() {
  if (x > 0) x--;
  else {
    x = 7;
    pickdashheight();
    pickcolourset();
  }
    switch (x){
    default:
      matrix->drawRect(x, dashheight, 2, 2, colour1);
    break;
    case 0:
      matrix->drawRect(0, dashheight, 1, 2, colour1);
    break;
  }
}

void dashbackv() {
  switch (fxdir) {
    case 0:
      if (y < 6) y++;
      else {
        y = 6;
        pickdashheight();
        fxdir = 1;
      }
      matrix->drawRect(dashheight, y, 2, 2, colour1);
      break;
    case 1:
      if (y > 0) y--;
      else {
        y = 0;
        pickdashheight();
        pickcolourset();
        fxdir = 0;
      }
      matrix->drawRect(dashheight, y, 2, 2, colour1);
      break;
  }
}

void dashbackh() {
  switch (fxdir) {
    case 0:
      if (x < 6) x++;
      else {
        x = 6;
        pickdashheight();
        fxdir = 1;
      }
      matrix->drawRect(x, dashheight, 2, 2, colour1);
      break;
    case 1:
      if (x > 0) x--;
      else {
        x = 0;
        pickdashheight();
        pickcolourset();
        fxdir = 0;
      }
      matrix->drawRect(x, dashheight, 2, 2, colour1);
      break;
  }
}

void randomdots() {
  matrix->drawCircle(random(8), random(8), 0, colour1);
  matrix->drawCircle(random(8), random(8), 0, colour2);
  matrix->drawCircle(random(8), random(8), 0, colour3);
  matrix->drawCircle(random(8), random(8), 0, colour4);
  pickcolourset();
}

void randomcircles() {
  matrix->drawCircle(random(7), random(7), 1, colour1);
  pickcolourset();
}

void randomsquares() {
  matrix->drawRect(random(7), random(7), 2, 2, colour1);
  pickcolourset();
}

void flashingsquares() {
  switch (random(4)) {
    case 0:
      matrix->drawRect(0, 0, 2, 2, colour1);
      break;
    case 1:
      matrix->drawRect(6, 0, 2, 2, colour1);
      break;
    case 2:
      matrix->drawRect(0, 6, 2, 2, colour1);
      break;
    case 3:
      matrix->drawRect(6, 6, 2, 2, colour1);
      break;
  }
  pickcolourset();
  matrix->fillRect(2, 2, 4, 4, colour1);
  pickcolourset();
}

void discolight() {
  matrix->fillRect(0, 0, 4, 4, colour1);
  matrix->fillRect(0, 4, 4, 4, colour2);
  matrix->fillRect(4, 0, 4, 4, colour3);
  matrix->fillRect(4, 4, 4, 4, colour4);
  pickcolourset();
}

void vcrossover() {
  switch (fxdir) {
    case 0:
      if (y > 0) y--; else {
        pickcolourset();
        fxdir = 1;
      }
      break;
    case 1:
      if (y < 6) y++; else {
        pickcolourset();
        fxdir = 0;
      }
      break;
  }
  matrix->drawRect(0, y, 2, 2, colour1);
  matrix->drawRect(4, y, 2, 2, colour2);
  matrix->drawRect(2, 6 - y, 2, 2, colour3);
  matrix->drawRect(6, 6 - y, 2, 2, colour4);
}

void hcrossover() {
  switch (fxdir) {
    case 0:
      if (x > 0) x--; else {
        pickcolourset();
        fxdir = 1;
      }
      break;
    case 1:
      if (x < 6) x++; else {
        pickcolourset();
        fxdir = 0;
      }
      break;
  }
  matrix->drawRect(x, 0, 2, 2, colour1);
  matrix->drawRect(x, 4, 2, 2, colour2);
  matrix->drawRect(6 - x, 2, 2, 2, colour3);
  matrix->drawRect(6 - x, 6, 2, 2, colour4);
}

void explode() {
  if (frameno < 5)frameno = frameno + 1; else {
    frameno = 0;
    pickcolourset();
  }
  switch (frameno) {
    case 0:
      matrix->fillRect(3, 3, 2, 2, colour1);
      pickcolourset();
      break;
    case 1:
      matrix->fillRect(2, 2, 2, 2, colour1);
      matrix->fillRect(4, 2, 2, 2, colour2);
      matrix->fillRect(2, 4, 2, 2, colour3);
      matrix->fillRect(4, 4, 2, 2, colour4);
      break;
    case 2:
      matrix->fillRect(1, 1, 2, 2, colour1);
      matrix->fillRect(5, 1, 2, 2, colour2);
      matrix->fillRect(1, 5, 2, 2, colour3);
      matrix->fillRect(5, 5, 2, 2, colour4);
      break;
    case 3:
      matrix->fillRect(0, 0, 2, 2, colour1);
      matrix->fillRect(6, 6, 2, 2, colour2);
      matrix->fillRect(0, 6, 2, 2, colour3);
      matrix->fillRect(6, 0, 2, 2, colour4);
      break;
    case 4:
      matrix->fillRect(1, 1, 2, 2, colour1);
      matrix->fillRect(5, 1, 2, 2, colour2);
      matrix->fillRect(1, 5, 2, 2, colour3);
      matrix->fillRect(5, 5, 2, 2, colour4);
      break;
    case 5:
      matrix->fillRect(2, 2, 2, 2, colour1);
      matrix->fillRect(4, 2, 2, 2, colour2);
      matrix->fillRect(2, 4, 2, 2, colour3);
      matrix->fillRect(4, 4, 2, 2, colour4);
      break;
  }
    delay(patterndelay);
}

void rotatingblocks1() {
  if (frameno < 3) frameno++; else {
    frameno = 0;
    pickcolourset();
  }
  switch (frameno) {
    case 0:
      matrix->fillRect(0, 0, 2, 2, colour1);
      matrix->fillRect(6, 0, 2, 2, colour2);
      matrix->fillRect(0, 6, 2, 2, colour3);
      matrix->fillRect(6, 6, 2, 2, colour4);
      break;
    case 1:
      matrix->fillRect(2, 0, 2, 2, colour1);
      matrix->fillRect(4, 0, 2, 2, colour2);
      matrix->fillRect(2, 6, 2, 2, colour3);
      matrix->fillRect(4, 6, 2, 2, colour4);
      break;
    case 2:
      matrix->fillRect(2, 2, 2, 2, colour1);
      matrix->fillRect(4, 2, 2, 2, colour2);
      matrix->fillRect(2, 4, 2, 2, colour3);
      matrix->fillRect(4, 4, 2, 2, colour4);
      break;
    case 3:
      matrix->fillRect(0, 2, 2, 2, colour1);
      matrix->fillRect(6, 2, 2, 2, colour2);
      matrix->fillRect(0, 4, 2, 2, colour3);
      matrix->fillRect(6, 4, 2, 2, colour4);
      break;
  }
}

void rotatingblocks2() {
  if (frameno < 3) frameno++; else {
    frameno = 0;
    pickcolourset();
  }
  switch (frameno) {
    case 3:
      matrix->fillRect(0, 0, 2, 2, colour1);
      matrix->fillRect(6, 0, 2, 2, colour2);
      matrix->fillRect(0, 6, 2, 2, colour3);
      matrix->fillRect(6, 6, 2, 2, colour4);
      break;
    case 2:
      matrix->fillRect(2, 0, 2, 2, colour1);
      matrix->fillRect(4, 0, 2, 2, colour2);
      matrix->fillRect(2, 6, 2, 2, colour3);
      matrix->fillRect(4, 6, 2, 2, colour4);
      break;
    case 1:
      matrix->fillRect(2, 2, 2, 2, colour1);
      matrix->fillRect(4, 2, 2, 2, colour2);
      matrix->fillRect(2, 4, 2, 2, colour3);
      matrix->fillRect(4, 4, 2, 2, colour4);
      break;
    case 0:
      matrix->fillRect(0, 2, 2, 2, colour1);
      matrix->fillRect(6, 2, 2, 2, colour2);
      matrix->fillRect(0, 4, 2, 2, colour3);
      matrix->fillRect(6, 4, 2, 2, colour4);
      break;
  }
}

void squarechase1()
{
  if (x < 6) x++; else x = 0;
  if (y < 6) y++; else  {
    y = 0;
    pickcolourset();
  }
  matrix->fillRect(x, 0, 2, 2, colour1);
  matrix->fillRect(6 - x, 6, 2, 2, colour2);
  matrix->fillRect(0, 6 - y, 2, 2, colour3);
  matrix->fillRect(6, y, 2, 2, colour4);
}

void squarechase2()
{
  if (x > 0) x--; else x = 6;
  if (y > 0) y--;
  else  {
    y = 6;
    pickcolourset();
  }
  matrix->fillRect(x, 0, 2, 2, colour1);
  matrix->fillRect(6 - x, 6, 2, 2, colour2);
  matrix->fillRect(0, 6 - y, 2, 2, colour3);
  matrix->fillRect(6, y, 2, 2, colour4);
}

void beamscanner1()
{
  switch (fxdir) {
    case 0:
      if (x < 6) x++;
      else {
        if (y < 6) y++; else y = 0;
        fxdir = 1;
      }
      matrix->fillRect(x, y, 2, 2, colour1);
      break;
    case 1:
      if (x > 0) x--;
      else {
        if (y < 6) y++; else {
          y = 0;
          pickcolourset();
        }
        fxdir = 0;
      }
      matrix->fillRect(x, y, 2, 2, colour1);
      break;
  }
}

void beamscanner2()
{
  switch (fxdir) {
    case 0:
      if (x < 6) x++;
      else {
        if (y > 0) y--; else y = 6;
        fxdir = 1;
      }
      matrix->fillRect(x, y, 2, 2, colour1);
      break;
    case 1:
      if (x > 0) x--;
      else {
        if (y > 0) y--; else {
          y = 6;
          pickcolourset();
        }
        fxdir = 0;
      }
      matrix->fillRect(x, y, 2, 2, colour1);
      break;
  }
}

void growingbeams()
{
  if (x > 0) {
    x--;
    y--;
    z=z+2;
  }
  else {
    x = 3;
    y = 3;
    z = 2;
    pickcolourset();
  }
  matrix->drawRect( x, y, z, 1, colour1);
  matrix->drawRect( x, 7 - y, z, 1, colour1);
    delay(patterndelay);
}


void shrinkingbeams()
{
  if (x < 3 && y < 3 && z > 0) {
    x++;
    y++;
    z=z-2;
  }
  else {
    x = 0;
    y = 0;
    z = 8;
    pickcolourset();
  }
  matrix->drawRect( x, y, z, 1, colour1);
  matrix->drawRect( x, 7 - y, z, 1, colour1);
    delay(patterndelay);
}

void nexteffect() {
  if (fxnumber < totalpatterns-1) fxnumber++; else {
    fxnumber = 0;
    randomize(fxlist,totalpatterns);
  }
  pickcolourset();
  pickdashheight();
  z = 0;
  x = 0;
  y = 0;
}

void swap (int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void randomize ( int arr[], int n )
{
    // Start from the last element and swap one by one. We don't
    // need to run for the first element that's why i > 0
    for (int i = n-1; i > 0; i--)
    {
        int j = random(0, n);
        swap(&arr[i], &arr[j]);
    }
}

void pickdashheight() {
  lastdash = dashheight;
  pickdash:
  dashheight = random(6);
  if (dashheight == lastdash) goto pickdash;
}

void pickcolourset(){
    randomize(colourlist,7);
    colour1=colourswitch(colourlist[1]);
    colour2=colourswitch(colourlist[2]);
    colour3=colourswitch(colourlist[3]);
    colour4=colourswitch(colourlist[4]);
}

int colourswitch(int thiscolour){
    int newcolour;
    switch (thiscolour) {
    case 0:
      newcolour = LED_WHITE_HIGH;
      break;
    case 1:
      newcolour = LED_RED_HIGH;
      break;
    case 2:
      newcolour = LED_GREEN_HIGH;
      break;
    case 3:
      newcolour = LED_BLUE_HIGH;
      break;
    case 4:
      newcolour = LED_ORANGE_HIGH;
      break;
    case 5:
      newcolour = LED_PURPLE_HIGH;
      break;
    case 6:
      newcolour = LED_CYAN_HIGH;
      break;
    }
    return newcolour;
}
