
// use first channel of 16 channels (started from zero)
#define LEDC_CHANNEL_0     0
#define LEDC_CHANNEL_5     5

// use 13 bit precission for LEDC timer
#define LEDC_TIMER_13_BIT  13

// use 5000 Hz as a LEDC base frequency
#define LEDC_BASE_FREQ     5000

// fade LED PIN (replace with LED_BUILTIN constant for built-in LED)
#define MOTOR_RIGTH_PIN 13
#define MOTOR_LEFT_PIN 23

int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by

// Arduino like analogWrite
// value has to be between 0 and valueMax
void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 255) {
  // calculate duty, 8191 from 2 ^ 13 - 1
  uint32_t duty = (8191 / valueMax) * min(value, valueMax);

  // write duty to LEDC
  ledcWrite(channel, duty);
}

void setupMotors() {
  // Setup timer and attach timer to a led pin
  ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
  ledcAttachPin(MOTOR_RIGTH_PIN, LEDC_CHANNEL_0);

  ledcSetup(LEDC_CHANNEL_5, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
  ledcAttachPin(MOTOR_LEFT_PIN, LEDC_CHANNEL_5);
}

void analogMotorRight(int value) {
  if (value < 1) {
    value = value * -1;
  }
  ledcAnalogWrite(LEDC_CHANNEL_0, value);
  Serial.print("Motor Right -> ");
  Serial.println(value);
  //delay(30);
}

void analogMotorLeft(int value) {
  if (value < 1) {
    value = value * -1;
  }
  ledcAnalogWrite(LEDC_CHANNEL_5, value);
  Serial.print("Motor Left -> ");
  Serial.println(value);
  //delay(30);
}
