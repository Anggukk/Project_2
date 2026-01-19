#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11
#define LDR_PIN A0
#define BUTTON_PIN 7
#define BUZZER_PIN 6
#define LED_GREEN 8
#define LED_BLUE 9
#define LED_RED 10

DHT dht(DHTPIN,DHTTYPE);

float TEMP_GOOD=26.0;
float TEMP_DANGER=30.0;
float HUM_GOOD=60.0;
float HUM_DANGER=75.0;

int LIGHT_THRESHOULD=300;

bool autoMode=true;
bool lastButtonState=LOW;

String level="GOOD";

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(LDR_PIN, INPUT);
  pinMode(BUTTON_PIN,INPUT_PULLUP);
  pinMode(BUZZER_PIN,OUTPUT);

  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  digitalWrite(BUZZER_PIN,LOW);
}

void loop() {
  delay(500);

  bool buttonState=digitalRead(BUTTON_PIN);
  if(buttonState==LOW && lastButtonState==HIGH){
    autoMode=!autoMode;
    delay(300);
  }
  lastButtonState=buttonState;

  float hum=dht.readHumidity();
  float temp=dht.readTemperature();
  int light=analogRead(LDR_PIN);

  if(isnan(hum)||isnan(temp)){
    Serial.println("ERROR");
    return;
  }

  if(temp>=TEMP_DANGER || hum>HUM_DANGER){
    level="DANGER";
  }
  else if (temp>=TEMP_GOOD || hum>=HUM_GOOD) { 
    level="CAUTION";
  }
  else{
    level="GOOD";
  }

  if(autoMode){
    if(level=="GOOD"){
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_BLUE, LOW);
      digitalWrite(LED_RED, LOW);
      digitalWrite(BUZZER_PIN, LOW);
    }
    else if (level=="CAUTION") {
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_BLUE, HIGH);
      digitalWrite(LED_RED, LOW);
      digitalWrite(BUZZER_PIN, LOW);
    }
    else if(level=="DANGER"){
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_BLUE, LOW);
      digitalWrite(LED_RED, HIGH);
      digitalWrite(BUZZER_PIN, HIGH);
    }

    // green led를 조명으로 활용
    if(light<LIGHT_THRESHOULD){
      digitalWrite(LED_GREEN, HIGH);
    }
  }

  Serial.print(temp);
  Serial.print(",");
  Serial.print(hum);
  Serial.print(",");
  Serial.print(light);
  Serial.print(",");
  Serial.print(autoMode?"AUTO":"MANUAL");
  Serial.print(",");
  Serial.println(level);
}
