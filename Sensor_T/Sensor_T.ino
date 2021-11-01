#include <OneWire.h>
#include <DallasTemperature.h>

const byte OC1A_PIN = 9;
const byte OC1B_PIN = 10;

const int TEMP_UPPER_PECERA_1 = 24;  //Valor maximo de temperatura, cambiar a gusto
const int TEMP_LOWER_PECERA_1 = 23;  //Valor minimo de temperatura, cambiar a gusto
const int TEMP_UPPER_PECERA_2 = 25;  //Valor maximo de temperatura, cambiar a gusto
const int TEMP_LOWER_PECERA_2 = 24;  //Valor minimo de temperatura, cambiar a gusto

OneWire ourWire1(2);                //Se establece el pin 2  como bus OneWire
OneWire ourWire2(3);                //Se establece el pin 3  como bus OneWire
DallasTemperature sensors1(&ourWire1); //Se declara una variable u objeto para nuestro sensor1
DallasTemperature sensors2(&ourWire2); //Se declara una variable u objeto para nuestro sensor2

void setupTimer1() {
  //Set PWM frequency to about 25khz on pins 9,10 (timer 1 mode 10, no prescale, count to 960)
  TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
  TCCR1B = (1 << CS10) | (1 << WGM13);
  ICR1 = 960;
  OCR1A = 0;
  OCR1B = 0;
}

//PWM Cooler 1
void setPWM1A(float f) {
  f = f < 0 ? 0 : f > 1 ? 1 : f;
  OCR1A = (uint16_t)(960 * f);
}

//PWM Cooler 2
void setPWM1B(float f) {
  f = f < 0 ? 0 : f > 1 ? 1 : f;
  OCR1B = (uint16_t)(960 * f);
}

void setup() {
  pinMode(OC1A_PIN, OUTPUT); //Set OUTPUT pin cooler 1
  pinMode(OC1B_PIN, OUTPUT); //Set OUTPUT pin cooler 2
  // Clear Timer1 control and count registers
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  setupTimer1();
  Serial.begin(9600);
  sensors1.begin();   //Se inicia el sensor 1
  sensors2.begin();   //Se inicia el sensor 2
  // Para mas info sobre registros ver: https://lascosasdedani.com/tutorial-pwm-arduino-para-generar-senales-pwm
}

void loop() {
  //--------------LECTURA TERMOMETRO 1----------------------
  sensors1.requestTemperatures();   //Se envía el comando para leer la temperatura
  float temp1 = sensors1.getTempCByIndex(0); //Se obtiene la temperatura en ºC del sensor 1
  
  //--------------LECTURA TERMOMETRO 2----------------------
  sensors2.requestTemperatures();   //Se envía el comando para leer la temperatura
  float temp2 = sensors2.getTempCByIndex(0); //Se obtiene la temperatura en ºC del sensor 2

  //------------SENSOR 1--------------------
  if (isnan(temp1))
  {
    Serial.println("Conecta el sensor 1 pajero!");
  }
  else
  {
    if (temp1 > TEMP_UPPER_PECERA_1)
    {
      Serial.println("Prender cooler 1");
      setPWM1A(1.0f);
      delay(5);
    }
    else if (temp1 <= TEMP_LOWER_PECERA_1)
    {
      Serial.println("Apagar cooler 1");
      setPWM1A(0.0f);
      delay(5);
    }
  }
  
  //------------SENSOR 2--------------------
  if (isnan(temp2))
  {
    Serial.println("Conecta el sensor 2 pajero!");
  }
  else
  {
    if (temp2 > TEMP_UPPER_PECERA_2)
    {
      Serial.println("Prender cooler 2");
      setPWM1B(1.0f);
      delay(5);
    }
    else if (temp2 <= TEMP_LOWER_PECERA_2)
    {
      Serial.println("Apagar cooler 2");
      setPWM1B(0.0f);
      delay(5);
    }
  }
  
  delay(100);
}
