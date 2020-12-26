class LED
{
public:
  int OFF = 0;
  int RED = 1;
  int GREEN = 2;
  int BLUE = 3;
  int flashInterval;

  void init(int rRed, int rGreen, int rBlue, int interval)
  {
    flashInterval = interval;
    
    // Create the bitmasks
    RED = 1 << rRed;
    GREEN = 1 << rGreen;
    BLUE = 1 << rBlue;

    // Set registers for output
    PORT->Group[PORTA].DIRSET.reg = RED | GREEN | BLUE;
  }

  void setColor(int color)
  {
    turnOff();
    PORT->Group[PORTA].OUTSET.reg = color;
  }

  void turnOff()
  {
    PORT->Group[PORTA].OUTCLR.reg = RED | GREEN | BLUE;
  }

  void flash(int color, int count)
  {
    for (int i = 0; i < count; i++)
    {
      setColor(color);
      delay(flashInterval);
      turnOff();
      if (i < count - 1)
      {
        delay(flashInterval);
      }
    }
  }

  void sequence(int color1, int color2)
  {
    flash(color1, 1);
    delay(flashInterval);
    flash(color2, 1);
  }

  void sequence(int color1, int color2, int color3)
  {
    flash(color1, 1);
    delay(flashInterval);
    flash(color2, 1);
    delay(flashInterval);
    flash(color3, 1);
  }
};
