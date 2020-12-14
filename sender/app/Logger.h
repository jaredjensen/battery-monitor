#include "Config.h"

class Logger
{
public:
  void init()
  {
#ifdef DEBUG
    Serial.begin(9600);
    while (!Serial);
#endif
  }

  void print(char text[])
  {
#ifdef DEBUG
    Serial.println(text);
#endif
  }

  void print(float val)
  {
#ifdef DEBUG
    Serial.println(val);
#endif
  }

  void print(int val)
  {
#ifdef DEBUG
    Serial.println(val);
#endif
  }
};