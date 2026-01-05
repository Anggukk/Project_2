#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11
#define LED_PIN 8

DHT dht(DHTPIN,DHTTYPE);

float TEMP_LIMIT=28.0;
float HUM_LIMIT=70.0;

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  delay(2000);

  float hum=dht.readHumidity();
  float temp=dht.readTemperature();

  if(isnan(hum)||isnan(temp)){
    Serial.println("ERROR");
    return;
  }

  bool warning =(temp>=TEMP_LIMIT || hum>=HUM_LIMIT);

  digitalWrite(LED_PIN, warning?HIGH:LOW);

  Serial.print(temp);
  Serial.print(",");
  Serial.print(hum);
  Serial.print(",");
  Serial.println(warning?"WARNING":"NORMAL");
}
