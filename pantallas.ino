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

void drawHomeScreen() {

  currentScreen = HOME;

  tft.fillScreen(ILI9341_BLACK);

  drawHeader("MiniOS - Inicio");

  drawButton(20, 50, 120, 50,
             ILI9341_BLUE,
             "ARCHIVOS",
             ILI9341_WHITE,
             2);

  drawButton(180, 50, 120, 50,
             ILI9341_GREEN,
             "NOTAS",
             ILI9341_WHITE,
             2);

  drawButton(20, 120, 120, 50,
             ILI9341_MAGENTA,
             "SONIDO",
             ILI9341_WHITE,
             2);

  drawButton(180, 120, 120, 50,
             ILI9341_ORANGE,
             "INFO",
             ILI9341_WHITE,
             2);

  drawButton(90, 180, 140, 40,
             ILI9341_CYAN,
             "LECTURA",
             ILI9341_BLACK,
             2);
}