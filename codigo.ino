#include <SPI.h>
#include <SD.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <XPT2046_Touchscreen.h>
#include <DHT.h>

// -------------------------
// TFT
// -------------------------
#define TFT_CS   15
#define TFT_DC   2
#define TFT_RST  4
Adafruit_ILI9341 tft(TFT_CS, TFT_DC, TFT_RST);

// -------------------------
// TOUCH
// -------------------------
#define TOUCH_CS   21
#define TOUCH_IRQ  22
XPT2046_Touchscreen ts(TOUCH_CS, TOUCH_IRQ);

// -------------------------
#define SD_CS 5
#define BUZZER_PIN 27

// -------------------------
// DHT11
// -------------------------
#define DHTPIN 14
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// -------------------------
#define TS_MINX 200
#define TS_MAXX 3800
#define TS_MINY 200
#define TS_MAXY 3800

// -------------------------
enum ScreenState {
  BOOT, HOME, FILES, NOTES, SOUND, INFO, READING
};

ScreenState currentScreen = BOOT;

bool sdOk = false;
unsigned long lastReadTime = 0;

// -------------------------
bool inRect(int x, int y, int rx, int ry, int rw, int rh) {
  return (x >= rx && x <= (rx + rw) && y >= ry && y <= (ry + rh));
}

void beepShort() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(80);
  digitalWrite(BUZZER_PIN, LOW);
}

// -------------------------
void drawButton(int x, int y, int w, int h, uint16_t color, const char* text, uint16_t txtColor, int textSize) {
  tft.fillRoundRect(x, y, w, h, 8, color);
  tft.drawRoundRect(x, y, w, h, 8, ILI9341_WHITE);

  tft.setTextColor(txtColor);
  tft.setTextSize(textSize);

  int16_t x1, y1;
  uint16_t tw, th;
  tft.getTextBounds(text, 0, 0, &x1, &y1, &tw, &th);

  tft.setCursor(x + (w - tw) / 2, y + (h - th) / 2);
  tft.print(text);
}

void drawHeader(const char* title) {
  tft.fillRect(0, 0, 320, 30, ILI9341_DARKCYAN);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 8);
  tft.print(title);
}

void drawBackButton() {
  drawButton(10, 200, 80, 30, ILI9341_RED, "VOLVER", ILI9341_WHITE, 1);
}

// -------------------------
void drawBootScreen() {
  tft.fillScreen(ILI9341_BLACK);

  tft.setTextColor(ILI9341_CYAN);
  tft.setTextSize(3);
  tft.setCursor(60, 60);
  tft.print("MiniOS");

  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.setCursor(40, 120);
  tft.print("ESP32 Mini Computer");

  tft.setTextSize(1);
  tft.setCursor(75, 170);
  tft.print("Cargando interfaz...");
}

// -------------------------
void drawHomeScreen() {
  currentScreen = HOME;
  tft.fillScreen(ILI9341_BLACK);
  drawHeader("MiniOS - Inicio");

  drawButton(20, 50, 120, 50, ILI9341_BLUE, "ARCHIVOS", ILI9341_WHITE, 2);
  drawButton(180, 50, 120, 50, ILI9341_GREEN, "NOTAS", ILI9341_WHITE, 2);
  drawButton(20, 120, 120, 50, ILI9341_MAGENTA, "SONIDO", ILI9341_WHITE, 2);
  drawButton(180, 120, 120, 50, ILI9341_ORANGE, "INFO", ILI9341_WHITE, 2);
  drawButton(90, 180, 140, 40, ILI9341_CYAN, "LECTURA", ILI9341_BLACK, 2);
}

// -------------------------
void drawFilesScreen() {
  currentScreen = FILES;
  tft.fillScreen(ILI9341_BLACK);
  drawHeader("Explorador SD");
  drawBackButton();
}

void drawNotesScreen() {
  currentScreen = NOTES;
  tft.fillScreen(ILI9341_BLACK);
  drawHeader("Notas");
  drawBackButton();
}

void drawSoundScreen() {
  currentScreen = SOUND;
  tft.fillScreen(ILI9341_BLACK);
  drawHeader("Sonido");
  drawBackButton();

  drawButton(60, 70, 200, 60, ILI9341_CYAN, "BEEP", ILI9341_BLACK, 3);
}

void drawInfoScreen() {
  currentScreen = INFO;
  tft.fillScreen(ILI9341_BLACK);
  drawHeader("Info");
  drawBackButton();
}

// -------------------------
void drawReadingScreen() {
  currentScreen = READING;
  tft.fillScreen(ILI9341_BLACK);
  drawHeader("Sensor DHT11");
  drawBackButton();

  lastReadTime = 0; // fuerza actualización inmediata
}

// -------------------------
void updateReading() {
  float temp = dht.readTemperature();

  tft.fillRect(0, 40, 320, 140, ILI9341_BLACK);

  if (isnan(temp)) {
    tft.setTextColor(ILI9341_RED);
    tft.setTextSize(2);
    tft.setCursor(40, 100);
    tft.print("Error lectura");
    return;
  }

  tft.setTextSize(3);
  tft.setCursor(40, 70);

  if (temp < 18) {
    tft.setTextColor(ILI9341_CYAN);
    tft.print("FRIO");
  } else if (temp <= 28) {
    tft.setTextColor(ILI9341_GREEN);
    tft.print("TEMPLADO");
  } else {
    tft.setTextColor(ILI9341_RED);
    tft.print("CALIENTE");
  }

  tft.setCursor(40, 120);
  tft.setTextSize(2);
  tft.print("Temp: ");
  tft.print(temp);
  tft.print(" C");
}

// -------------------------
bool getTouch(int &x, int &y) {
  if (!ts.touched()) return false;

  TS_Point p = ts.getPoint();

  x = map(p.x, TS_MINX, TS_MAXX, 0, 320);
  y = map(p.y, TS_MINY, TS_MAXY, 0, 240);

  x = constrain(x, 0, 319);
  y = constrain(y, 0, 239);

  return true;
}

// -------------------------
void handleHomeTouch(int x, int y) {
  if (inRect(x, y, 20, 50, 120, 50)) drawFilesScreen();
  else if (inRect(x, y, 180, 50, 120, 50)) drawNotesScreen();
  else if (inRect(x, y, 20, 120, 120, 50)) drawSoundScreen();
  else if (inRect(x, y, 180, 120, 120, 50)) drawInfoScreen();
  else if (inRect(x, y, 90, 180, 140, 40)) drawReadingScreen();
}

void handleBackTouch(int x, int y) {
  if (inRect(x, y, 10, 200, 80, 30)) drawHomeScreen();
}

void handleSoundTouch(int x, int y) {
  handleBackTouch(x, y);

  if (inRect(x, y, 60, 70, 200, 60)) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(200);
    digitalWrite(BUZZER_PIN, LOW);
  }
}

// -------------------------
void setup() {
  Serial.begin(115200);

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  tft.begin();
  tft.setRotation(0);

  ts.begin();
  ts.setRotation(-1); // NO TOCAR

  dht.begin();

  drawBootScreen();

  if (SD.begin(SD_CS)) sdOk = true;

  delay(2000);
  drawHomeScreen();
}

// -------------------------
void loop() {
  int x, y;

  if (getTouch(x, y)) {

    switch (currentScreen) {

      case HOME:
        handleHomeTouch(x, y);
        break;

      case FILES:
      case NOTES:
      case INFO:
        handleBackTouch(x, y);
        break;

      case SOUND:
        handleSoundTouch(x, y);
        break;

      case READING:
        handleBackTouch(x, y);
        break;
    }

    delay(220);
  }

  // 👇 ACTUALIZACIÓN AUTOMÁTICA
  if (currentScreen == READING) {
    if (millis() - lastReadTime > 2000) {
      updateReading();
      lastReadTime = millis();
    }
  }
}