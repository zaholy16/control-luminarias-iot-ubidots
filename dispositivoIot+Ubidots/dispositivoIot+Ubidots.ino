// This example retrieves last value of a variable from the Ubidots API
// using HTTP protocol.

/****************************************
 * Include Libraries
 ****************************************/

#include "Ubidots.h"


/****************************************
 * Define Constants
 ****************************************/

const char* UBIDOTS_TOKEN = "BBFF-gJsQsacCGSk8GFFQKkLuOzmm6AwxdM";                                     
const char* WIFI_SSID = "zaholy";                                         
const char* WIFI_PASS = "pelusatony";                                         
const char* DEVICE = "esp32-hihuitlan";  
const char* VAR_L1 = "luminary_1";     

Ubidots ubidots(UBIDOTS_TOKEN, UBI_HTTP);

#define pin_l1 25
#define pin_ldr 39
#define pin_pir 15
#define pin_led 5

/****************************************
 * Auxiliar Functions
 ****************************************/

// Put here your auxiliar functions

/****************************************
 * Main Functions
 ****************************************/

void setup() {
  Serial.begin(115200);
  ubidots.wifiConnect(WIFI_SSID, WIFI_PASS);
 
  pinMode(pin_l1, OUTPUT);
  pinMode(pin_led, OUTPUT);
  pinMode(pin_pir, INPUT);
  digitalWrite(pin_l1, LOW);
  digitalWrite(pin_led, LOW);

  for(int i = 0; i > 30; i++) //Utilizamos un for para calibrar el sensor depende del tipo de sensor que utilicemos va a cambiar el tiempo de calibración
  {
    delay(1000);
  }
}

void loop() {

  float value_l1 = ubidots.get(DEVICE, VAR_L1);
  int value_ldr = analogRead(pin_ldr);
  int value_pir = digitalRead(pin_pir);

  //Luminosidad
  int brightness = map(value_ldr, 0, 1000, 0, 100);

  Serial.print(("Brightness: ")); Serial.println(brightness);
  Serial.print("Value: "); Serial.println(value_l1);

  //Movimiento y luminosidad
  if(value_pir==1 && brightness > 360) //Si detecta que hay movimiento y poca luz manda activar el led
  {
    if(value_l1==1){ //Si se enciende manualmente, encender el led
      Serial.print("Movimiento Detectado!!");
      digitalWrite(pin_led, HIGH);
      Serial.print("Foco encendido!!");
      digitalWrite(pin_l1, LOW);
    }else{ //Si se apaga manualmente, apagar el led
      digitalWrite(pin_led, HIGH);
      digitalWrite(pin_l1, LOW);
    }
  }
  else //Si la condición anterior no se cumple manda apagar el led
  {
    Serial.println("No hay movimiento");
    digitalWrite(pin_led, LOW);
    Serial.print("Foco apagado!!");
    digitalWrite(pin_l1, HIGH);

    if(value_l1==1){
      digitalWrite(pin_l1, LOW);
    }else{
      digitalWrite(pin_l1, HIGH);
    }
  }

  ubidots.add("brightness", brightness);
  ubidots.add("movement", value_pir);

  bool bufferSent = false;
  bufferSent = ubidots.send(DEVICE);  // Will send data to a device label that matches the device Id

  if (bufferSent) {
    Serial.println("Values sent by the device");
  }
  
  delay(5000);
}
