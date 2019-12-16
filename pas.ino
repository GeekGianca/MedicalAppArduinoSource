#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//pin para la sonda DQ
const byte pinDatosDQ = 9;
//Instancias
OneWire oneWireObjeto(pinDatosDQ);
DallasTemperature sensorDS18B20(&oneWireObjeto);
//Configuracion Bluethoot
SoftwareSerial bt_conexion(2, 3); // RX | TX
//ritmo pulso cardiaco
int pulso = 0;
//frecuencia respiratoria
float frecuencia_valor = 0;
//Variables a enviar por bluetooth
String datosEnviar = "";
void setup()
{
  pinMode(13, OUTPUT);
  pinMode(A5, INPUT);
  pinMode(11, OUTPUT);
  digitalWrite(13, HIGH);
  bt_conexion.begin(9600);
  sensorDS18B20.begin();
  Serial.begin(9600);
  bt_conexion.println("Modulo Conectado");
  bt_conexion.print("#");
}


void loop()
{
  Serial.println("Enviando comando a los sensores");
  sensorDS18B20.requestTemperatures();
  Serial.println("Temp available: ");
  Serial.println(sensorDS18B20.getTempCByIndex(0));
  Serial.println("Obtieniendo pulso cardiaco");
  pulso = analogRead(A5);
  if(pulso >= 200){
    digitalWrite(11, HIGH);
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(11, LOW);
    digitalWrite(13, LOW);
  }
  delay(50);
  Serial.println(pulso);
  Serial.println("Obtieniendo frecuencia respiratoria");
  float sensor_volt;
  float RS_air; //  Get the value of RS via in a clear air
  float R0;  // Get the value of R0 via in Alcohol
  float sensorValue;
    /*--- Get a average data by testing 100 times ---*/
  float sensorV = analogRead(A0);
  for(int x = 0 ; x < 100 ; x++){
      sensorValue = sensorValue + analogRead(A0);
  }
  sensorValue = sensorValue/100.0;
  Serial.print(analogRead(A0));
  Serial.print("valor---");
  Serial.println(sensorValue);
    /*-----------------------------------------------*/
  sensor_volt = sensorValue/1024*5.0;
  RS_air = (5.0-sensor_volt)/sensor_volt; // omit *RL
  R0 = RS_air/60.0; // The ratio of RS/R0 is 60 in a clear air from Graph (Found using WebPlotDigitizer)
  Serial.print("sensor_volt = ");
  Serial.print(sensor_volt);
  Serial.println("V");
  Serial.print("RS = ");
  Serial.println(RS_air);
  Serial.print("R0 = ");
  Serial.println(R0);
  delay(1000);
  if (bt_conexion.available())
  {
    datosEnviar = ""+String(sensorDS18B20.getTempCByIndex(0))+","+pulso+","+sensorV+","+RS_air+","+R0+"#";
    bt_conexion.println(datosEnviar);
    Serial.println(datosEnviar);
    /*char VarChar;

    VarChar = bt_conexion.read();

    if (VarChar == '1')
    {
      digitalWrite(13, HIGH);
      delay(100);
      bt_conexion.print("Riego Encendido");
      Serial.println("Riego Encendido");
      bt_conexion.print("#");
    }
    if (VarChar == '0')
    {
      digitalWrite(13, LOW);
      delay(100);
      bt_conexion.print("Riego Apagado#");
      Serial.println("Riego Apagado#");
    }*/
  }
}
