#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>
#include <SPI.h>
#include <SD.h>

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

#define TS_MINX 180 //150
#define TS_MINY 200  //120
#define TS_MAXX 920 //920
#define TS_MAXY 940 //940

#define PENRADIUS 3

#define MINPRESSURE 10
#define MAXPRESSURE 1000

#define BUTTON_SIZE_X 145
#define BUTTON_SIZE_Y 40
#define BUTTON_START_X 10
#define BUTTON_START_Y 70
#define BUTTON_SPACE_X 10
#define BUTTON_SPACE_Y 5

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 150);

// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;

const int chipSelect = 4;









// ************************************
// ------------------------------------
//               SETUP
// ------------------------------------
// ************************************
void setup() {

  uint16_t identifier = initScreenDriver();

  startupScreen(identifier);

  initSD();

  main_Menue();

  Serial.begin(9600);

}







// ************************************
// ------------------------------------
//               MAIN
// ------------------------------------
// ************************************
void loop() {

  Screen_touched();


}









// ************************************
// ------------------------------------
//               FUNKTIONEN
// ------------------------------------
// ************************************



// --------------------------------------------
// Main Menue
// --------------------------------------------
void main_Menue() {

  tft.fillScreen(WHITE);

  menueTitle("Hauptmenue");

  stateLine(0, 0);

  menueButtons(6);

  textField(3, "Hallo TEST !!!");

}



// --------------------------------------------
// Textfeld
// --------------------------------------------
void textField(int textSize, String textMSG) {
  tft.drawRect(BUTTON_START_X, 25, BUTTON_SIZE_X * 2 + BUTTON_SPACE_X, BUTTON_SIZE_Y, BLUE);

  tft.setCursor(15, 35);
  tft.setTextColor(RED);  tft.setTextSize(textSize);
  tft.println(textMSG);
}



// --------------------------------------------
// Statusleiste
// --------------------------------------------
void stateLine(boolean back, int statMSG) {

  tft.fillRect(0, 207, 320, 33, WHITE);

  tft.drawFastHLine(0, 205, 320, BLACK);

  if (back = 0) {

    tft.fillRect(5, 210, 55, 25, BLACK);

    tft.setCursor(10, 220);
    tft.setTextColor(WHITE);  tft.setTextSize(1);
    tft.println("<--BACK");
  }

  //  if (statMSG > 0) {

  tft.setCursor(200, 220);
  tft.setTextColor(BLACK);  tft.setTextSize(1);
  tft.print("STATUS:");
  tft.println(statMSG);
  //  }

}



// --------------------------------------------
// MenueButtons
// --------------------------------------------
void menueButtons(float buttonCount) {

  int rowCount = (int)(buttonCount / 2 + .5);
  int count = 0;

  for (int row = 0; row < rowCount; row++) {

    for (int col = 0; col <= 1; col++) {

      if (count < buttonCount)
        tft.drawRect(BUTTON_START_X + (BUTTON_SPACE_X * col + BUTTON_SIZE_X * col), BUTTON_START_Y + (BUTTON_SPACE_Y * row + BUTTON_SIZE_Y * row), BUTTON_SIZE_X, BUTTON_SIZE_Y, BLACK);

      count++;
    }

  }

}



// --------------------------------------------
// MenueTitle
// --------------------------------------------
void menueTitle(String TitleText) {

  tft.setCursor(5, 5);
  tft.setTextColor(BLACK);  tft.setTextSize(2);
  tft.println("HAUPTMENUE");
  tft.drawFastHLine(0, 22, 320, BLACK);
}



// --------------------------------------------
// Init_SD
// --------------------------------------------
void initSD() {

  pinMode(SS, OUTPUT);

  card.init(SPI_HALF_SPEED, chipSelect);

  tft.fillScreen(WHITE);
  tft.setCursor(25, 90);
  tft.setTextColor(BLACK);  tft.setTextSize(2);
  tft.println("INIT SD - PLEAS WAIT");
  delay(1000);

  tft.fillScreen(WHITE);
  tft.setCursor(100, 80);
  tft.setTextColor(BLACK);  tft.setTextSize(2);
  tft.println("SD Card Type:");
  tft.setCursor(155, 110);
  tft.println(card.type());

  delay(2000);

}



// --------------------------------------------
// Screen_touched
// --------------------------------------------
int Screen_touched() {

  int button = 0;
  int but_x, but_y;

  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  //pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //pinMode(YM, OUTPUT);

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {

    // scale from 0->1023 to tft.width
    p.x = tft.height() - (map(p.x, TS_MINX, TS_MAXX, tft.height(), 0));
    p.y = tft.width() - (map(p.y, TS_MINY, TS_MAXY, tft.width(), 0));

    Serial.println(p.x);
    Serial.println(p.y);

// Buttons row
  for (int i = 0; i <= 2; i++) {

      if (p.x <= 75 + 45 * (i + 1) && p.x >= 75 + 45 * i)
        button = i, Serial.println(button);
  }

// Buttons col
  if (button > 0) {
  if (p.y <= 155 && p.y >= 10)
      button = button + 1;
    else if (p.y <= 310 && p.y >= 165)
      button = button + 4;
      Serial.println(button);
      }
  
// Back Button
  if (p.y <= 55 && p.x >= 220)
      button = 7;
      Serial.println(button);
      
    stateLine(1, button);
    delay(500);
  }

  return button;

}



// --------------------------------------------
// init Screendriver
// --------------------------------------------
uint16_t initScreenDriver() {

  uint16_t ChipID = tft.readID();

  Serial.begin(9600);

  tft.reset();

  tft.begin(ChipID);

  tft.setRotation(3);

  pinMode(13, OUTPUT);

  return ChipID;

}



// --------------------------------------------
// startup Screen
// --------------------------------------------
void startupScreen(uint16_t identifier) {

  tft.fillScreen(BLACK);
  tft.fillRect(20, 20, 280, 200, GREEN);
  tft.setCursor(70, 90);
  tft.setTextColor(RED);  tft.setTextSize(3);
  tft.println("GardenRobo");
  tft.setCursor(100, 125); tft.setTextSize(2);
  tft.setTextColor(BLUE);
  tft.print("Chip: ");
  tft.println(identifier, HEX);
  tft.setCursor(95, 150);
  tft.setTextColor(BLUE);
  tft.println("Made by UT");

  delay(2500);

  tft.fillRect(0, 0, 320, 240, WHITE);
}
