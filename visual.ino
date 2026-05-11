void drawButton(int x, int y, int w, int h,
                uint16_t color,
                const char* text,
                uint16_t txtColor,
                int textSize) {

  tft.fillRoundRect(x, y, w, h, 8, color);
  tft.drawRoundRect(x, y, w, h, 8, ILI9341_WHITE);

  tft.setTextColor(txtColor);
  tft.setTextSize(textSize);

  int16_t x1, y1;
  uint16_t tw, th;

  tft.getTextBounds(text, 0, 0, &x1, &y1, &tw, &th);

  tft.setCursor(x + (w - tw) / 2,
                y + (h - th) / 2);

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

  drawButton(10, 200, 80, 30,
             ILI9341_RED,
             "VOLVER",
             ILI9341_WHITE,
             1);
}