bool getTouch(int &x, int &y) {

  if (!ts.touched()) return false;

  TS_Point p = ts.getPoint();

  x = map(p.x, TS_MINX, TS_MAXX, 0, 320);
  y = map(p.y, TS_MINY, TS_MAXY, 0, 240);

  x = constrain(x, 0, 319);
  y = constrain(y, 0, 239);

  return true;
}

void handleHomeTouch(int x, int y) {

  if (inRect(x, y, 20, 50, 120, 50))
    drawFilesScreen();

  else if (inRect(x, y, 180, 50, 120, 50))
    drawNotesScreen();

  else if (inRect(x, y, 20, 120, 120, 50))
    drawSoundScreen();

  else if (inRect(x, y, 180, 120, 120, 50))
    drawInfoScreen();

  else if (inRect(x, y, 90, 180, 140, 40))
    drawReadingScreen();
}