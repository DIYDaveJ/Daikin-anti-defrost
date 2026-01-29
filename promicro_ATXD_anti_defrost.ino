/*
  Daikin ATXD anti defrost
  V1.0
  29 Jan. 2026
*/

#define serial_output 0
#define adc_at_zero 350  //DAC value corresponding to 1 C (20k NTC with 33k series resistor)

uint16_t max_evap;
uint16_t temp_evap;
uint16_t temp_evap_fake;
uint8_t val_pwm_evap = 80; //PWM value to set ~ 0 C
uint8_t i = 0;

uint32_t last_read = 0;


uint16_t ringBuffer[256];
uint8_t buffInd = 0;



void setup() {


// initialize serial communication at 9600 bits per second:
#if serial_output
  Serial.begin(9600);
#endif
  pinMode(10, INPUT);  // sets the pin as input

  TCCR1B = TCCR1B & B11111000 | B00000001;  // set timer 1 divisor to 1 for PWM frequency of 31372.55Hz
  last_read = millis();
}

// the loop routine runs over and over again forever:
void loop() {


  if ((millis() - last_read) > 900 && i == 0) {  //triggers 100ms before readout
    pinMode(10, INPUT);                          // sets the pin as input
  }

  if ((millis() - last_read) > 1000) {  //triggers every second
    last_read = millis();

    if (i == 0) {
      //only when i==0 (every 8 s)
      temp_evap = analogRead(A0);         //read real evap. temp,
      ringBuffer[buffInd++] = temp_evap;  //store in buffer, increment buffInd

      max_evap = 0;                    //initial value of target_evap
      for (int j = 0; j < 256; j++) {  //cycle through the ringBuffer to find the maximum value
        if (ringBuffer[j] > max_evap) {
          max_evap = ringBuffer[j];
        }
      }

      if (max_evap < adc_at_zero) { //if evaporator temperature is below zero C, use PWM fake temperature
        pinMode(10, OUTPUT);  // sets the pin to output

      } else { //disable pwm signal when evaporator is above zero C (warm outside or defrosting)
        pinMode(10, INPUT);  // sets the pin to input (high impedance)
        val_pwm_evap = 80; //the value to start from when IO becomes output
      }

#if serial_output
      Serial.print("Evap:");
      Serial.print(temp_evap);
      Serial.print(",");
      Serial.print("Evap_max:");
      Serial.print(max_evap);
      Serial.print(",");
      Serial.print("Evap_fake:");
      Serial.print(temp_evap_fake);
      Serial.print(",");
      Serial.print("val_pwm_evap:");
      Serial.println(val_pwm_evap);

#endif
    }

    if (max_evap < adc_at_zero) {
      temp_evap_fake = analogRead(A0);  //read fake evap temp

      if (temp_evap_fake < max_evap) {
        val_pwm_evap++;
      } else if (temp_evap_fake > max_evap) {
        val_pwm_evap--;
      }

      analogWrite(10, val_pwm_evap);
    }


    i++;
    if (i > 4) {
      i = 0;
    }
  }
}


