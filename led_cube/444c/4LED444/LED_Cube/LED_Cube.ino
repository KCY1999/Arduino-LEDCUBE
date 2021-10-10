#define CUBE_SIZE 4
#define PLANE_SIZE CUBE_SIZE*CUBE_SIZE
#define PLANE_TIME 20
#define LED_TIME 200

int LEDPin[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, A0, A1};
int PlanePin[] = {A2 , A3, A4, A5};

void setup()
{
  int pin;
  for (pin = 0; pin < PLANE_SIZE; pin++) {
    pinMode(LEDPin[pin], OUTPUT);
    digitalWrite(LEDPin[pin], LOW);
  }
  for (pin = 0; pin < CUBE_SIZE; pin++) {
    pinMode(PlanePin[pin], OUTPUT);
    digitalWrite(PlanePin[pin], HIGH);
  }
}

void loop()
{
  for (int led = 0; led < PLANE_SIZE; led++)
  {
    digitalWrite(LEDPin[led], HIGH);
    for (int plane = 0; plane < CUBE_SIZE; plane++)
    {
      digitalWrite(PlanePin[plane], LOW);
      delay(LED_TIME);
      digitalWrite(PlanePin[plane], HIGH);
    }
    digitalWrite(LEDPin[led], LOW);
    delay(PLANE_TIME);
  }
}
