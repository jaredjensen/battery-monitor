class LED
{
public:
  int OFF = 0;
  int RED = 1;
  int GREEN = 2;
  int BLUE = 3;
  int flashInterval = 200;

  LED(int redPin, int greenPin, int bluePin)
  {
    // Create the bitmasks
    RED = 1 << (redPin - 2);
    GREEN = 1 << (greenPin - 2);
    BLUE = 1 << (bluePin - 2);

    // Set pins for output
    PORT->Group[PORTA].DIRSET.reg = RED | GREEN | BLUE;
  }

  void setColor(int color)
  {
    PORT->Group[PORTA].OUTCLR.reg = RED | GREEN | BLUE;
    PORT->Group[PORTA].OUTSET.reg = color;
  }

  void flash(int color, int count) {
    for (int i = 0; i < count; i++) {
      setColor(color);
      delay(flashInterval);
      setColor(OFF);
      if (i < count - 1) {
        delay(flashInterval);
      }
    }
  }
};
