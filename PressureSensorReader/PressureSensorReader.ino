#include <Wire.h>

int pr_sensor_pin = A5 ; // ADC[0] on Micro or D23
int sense_val = 0 ; // initialize value

int sen_addr = 24 ; // 0x18 for sensor, look at data sheet for address
int sen_payload = 7 ; // 7 byte payload, info below
int sen_ready_pin = 4 ; // D4 on Micro
/*
  Data payload info
1 - status byte
2-4 pressure output (24 bits)
5-7 compensated temperature (24 bits)

Pin 3 - EOC
  - is high when a measurement and calculation has been completed
  and the data is ready to be clocked out.
  We read from D4 in this program.
*/

void setup() {
  
  // Join i2c
  Wire.begin() ;  // address as optional argument
  pinMode(sen_ready_pin,  INPUT) ;

  // write to serial monitor
  Serial.begin(9600) ;
}

void loop() {

  if (digitalRead(sen_ready_pin)) {
    Wire.requestFrom(sen_addr, sen_payload) ;

    while (Wire.available()) {
      char stat = Wire.read() ; // status byte
      /*
      0 - 1 internal math saturation has occured
      1 - Always 0
      2 - 0 integrity passed, 1 - integrity failed
      3 - Always 0
      4 - Always 0
      5 - 1 device is busy
      6 - device is powered (1) not powered (0)
      7 - Always 0
      */
      char pd1 = Wire.read() ;  // pressure data <23:16>
      char pd2 = Wire.read() ;  // pressure data <15:8>
      char pd3 = Wire.read() ;  // pressure data <7:0>
      
      char td1 = Wire.read() ;  // pressure data <23:16>
      char td2 = Wire.read() ;  // pressure data <15:8>
      char td3 = Wire.read() ;  // pressure data <7:0>
      Serial.print("I2C out:") ;
      Serial.print(stat) ;
      Serial.print(pd1) ;
      Serial.print(pd2) ;
      Serial.print(pd3) ;
      Serial.print(td1) ;
      Serial.print(td2) ;
      Serial.print(td3) ;
      Serial.print("\n") ;
    } // end while Wire.available()
  } // end if digital_read

  sense_val = analogRead(pr_sensor_pin) ;
  Serial.print("Analog out:") ;
  Serial.println(sense_val) ;

  delay(500) ;

}
