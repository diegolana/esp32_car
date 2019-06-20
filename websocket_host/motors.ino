
// use first channel of 16 channels (started from zero)
#define LEDC_CHANNEL_0     0
#define LEDC_CHANNEL_1     1
#define LEDC_CHANNEL_5     5
#define LEDC_CHANNEL_6     6

// use 13 bit precission for LEDC timer
#define LEDC_TIMER_13_BIT  13

// use 5000 Hz as a LEDC base frequency
#define LEDC_BASE_FREQ     5000

// fade LED PIN (replace with LED_BUILTIN constant for built-in LED)
#define MOTOR_RIGTH_PIN 27
#define MOTOR_RIGTH_PIN_2 14
#define MOTOR_RIGTH_ENABLE 12
#define MOTOR_LEFT_PIN 25
#define MOTOR_LEFT_PIN_2 26
#define MOTOR_LEFT_ENABLE 33

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
  pinMode(MOTOR_RIGTH_PIN, OUTPUT);
  pinMode(MOTOR_RIGTH_PIN_2, OUTPUT);
  pinMode(MOTOR_RIGTH_ENABLE, OUTPUT);
  
  pinMode(MOTOR_LEFT_PIN_2, OUTPUT);
  pinMode(MOTOR_RIGTH_ENABLE, OUTPUT);
  pinMode(MOTOR_LEFT_ENABLE, OUTPUT);
  
  digitalWrite(MOTOR_RIGTH_ENABLE, HIGH);
  digitalWrite(MOTOR_LEFT_ENABLE, HIGH);
   
  ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
  ledcAttachPin(MOTOR_RIGTH_PIN, LEDC_CHANNEL_0);
  
  ledcSetup(LEDC_CHANNEL_1, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
  ledcAttachPin(MOTOR_RIGTH_PIN_2, LEDC_CHANNEL_1);

  ledcSetup(LEDC_CHANNEL_5, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
  ledcAttachPin(MOTOR_LEFT_PIN, LEDC_CHANNEL_5);

  ledcSetup(LEDC_CHANNEL_6, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
  ledcAttachPin(MOTOR_LEFT_PIN_2, LEDC_CHANNEL_6);

}

void analogMotorRight(int value) {
  if (value < 1) {
      value = value * -1;
      ledcAnalogWrite(LEDC_CHANNEL_0, 0);
      ledcAnalogWrite(LEDC_CHANNEL_1, value);
  } else {
      ledcAnalogWrite(LEDC_CHANNEL_0, value);
      ledcAnalogWrite(LEDC_CHANNEL_1, 0);
  }

  Serial.print("Motor Right -> ");
  Serial.println(value);
  //delay(30);
}

void analogMotorLeft(int value) {
  if (value < 1) {
    value = value * -1;
    ledcAnalogWrite(LEDC_CHANNEL_5, 0);
    ledcAnalogWrite(LEDC_CHANNEL_6, value);
  } else {
    ledcAnalogWrite(LEDC_CHANNEL_5, value);    
    ledcAnalogWrite(LEDC_CHANNEL_6, 0);
  }

  Serial.print("Motor Left -> ");
  Serial.println(value);
  //delay(30);
}
