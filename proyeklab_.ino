#include <ESP8266Firebase.h> //Library untuk Firebase
#include <ESP8266WiFi.h> //Library untuk Wifi dari ESP8266
#include <DHT11.h> //Library untuk DHT11
#include <SoilHygrometer.h> //Library untuk Hygrometer
#include <Servo.h> //Library untuk Servo


#define _SSID "Pli"
#define _PASSWORD "WinterStar" //Inisialisasi Untuk Wifi
#define REFERENCE_URL "https://dhyhygro-default-rtdb.asia-southeast1.firebasedatabase.app/" //Inisialisasi Untuk Firebase
Firebase firebase(REFERENCE_URL);

DHT11 dht11(2); //Inisialisasi DHT11
SoilHygrometer hygrometer(A0); //Inisialisasi hygrometer
Servo myservo; //Inisialisasi Servo

//Inisialisasi variabel untuk sensor dan pin
int temperature = 0;
int humidity = 0;
int humid = 0;
int soilPin = A0;
int sensorValue = 0;
int pos = 90;
int buzzer = 12; 


void setup() {
  //Inisialisasi untuk Pin dari Sensor dan Aktuator
  Serial.begin(9600);
  pinMode(soilPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  myservo.attach(5);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(1000);

  //Status Untuk koneksi Wifi
  Serial.print("Connecting to ");
  Serial.print(_SSID);
  WiFi.begin(_SSID, _PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //status untuk koneksi Wifi
  Serial.println();
  Serial.print("Connected /");
  Serial.print("/");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  myservo.write(pos);                    
}

void loop() {
  //membaca inputan sensor
  int result = dht11.readTemperatureHumidity(temperature, humidity);
  sensorValue = analogRead(soilPin);

  //function untuk mengirim data ke database
  senData();
  
  //Logic untuk menggerakan servo
   if (temperature >= 30){
      myservo.write(pos = 0);
      delay(1000);
    }else {
      myservo.write(pos = 90);
      delay(1000);
    }
  //Logic untuk membunyikan buzzer
    if(sensorValue >= 120 || humid <= 60){
      tone(buzzer, 2000, 1000);   
      delay(325);               
      noTone(buzzer);           
      tone(buzzer, 1000, 1500);
    }else {
      noTone(buzzer);
    }
  delay(5000);
}

void senData(){
  //Melempar Data ke Database
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print("°C");
    firebase.setInt("/Temperature", temperature); 

    Serial.print(" Humidity: ");
    Serial.print(humidity);
    Serial.print("%");
    firebase.setInt("/Humidity", humidity);

    Serial.println(" Moist Value : ");
    Serial.print(sensorValue);
    Serial.println();
    firebase.setInt("/Moist", sensorValue);
}