void drawReadingScreen() {

  currentScreen = READING;

  tft.fillScreen(ILI9341_BLACK);

  drawHeader("Sensor DHT11");

  drawBackButton();

  lastReadTime = 0;
}

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