#include <Wire.h> //komunikasi sda &scl
#include <LiquidCrystal_I2C.h> //library i2c lcd

LiquidCrystal_I2C lcd(0x27,16,2); //deklarasi i2c lcd 16x2

#define Pin_D1_L  4 //lpwm
#define Pin_D2_L  5 //rpwm
#define Pin_E_L   6 //pwm enable

const int buttonUp = 22; //deklarasi push button
const int buttonDown = 24;
const int buttonStop = 26;
int nilaiTombol1, nilaiTombol2, nilaiTombol3;

#define trigPin1 13 //deklarasi sensor jarak
#define echoPin1 12
#define trigPin2 10
#define echoPin2 11

long duration;
long duration1;
int distance;
int distance1;

char state; // pilihan menu manual di remote
char menu; // pilihan menu dari menu
long data; // data dari remote

void setup() {
  // put your setup code here, to run once:
  Serial.begin(38400);
  
  lcd.begin();
  lcd.noCursor();
  lcd.clear();

  pinMode(trigPin1, OUTPUT); // inisialisasi sensor jarak
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  pinMode(buttonUp, INPUT);  // inisialisasi button
  pinMode(buttonDown, INPUT);
  pinMode(buttonStop, INPUT);
  
  pinMode(Pin_D1_L, OUTPUT); // inisialisasi motor
  pinMode(Pin_D2_L, OUTPUT);
  pinMode(Pin_E_L, OUTPUT);

  analogWrite(Pin_E_L, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  //  pembacaan ketinggian meja dengan sensor hc-sr04
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration = pulseIn(echoPin1, HIGH);
  distance = ((duration / 2) * 0.0343) + 7.05; // 4.5 diambil dari jarak sensor 1 dari permukaan bawah meja
  
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration1 = pulseIn(echoPin2, HIGH);
  distance1 = ((duration1 / 2) * 0.0343) + 4.5; // 4.5 diambil dari jarak sensor 2 dari permukaan bawah meja
  
  lcd.setCursor(0,0); //  jarak sensor 1
  lcd.print("Sensor1= ");
  lcd.setCursor(0,1); //  jarak sensor 2
  lcd.print("Sensor2= ");
  lcd.setCursor(12,1);
  lcd.print("cm");
  lcd.setCursor(12,0);
  lcd.print("cm");
  lcd.setCursor(9,0);
  lcd.print(distance);
  lcd.setCursor(9,1);
  lcd.print(distance1);
  //delay(500);

  nilaiTombol1 = digitalRead(buttonUp);
  nilaiTombol2 = digitalRead(buttonDown);
  nilaiTombol3 = digitalRead(buttonStop);

  if(nilaiTombol1 == 1){
    delay(1000);
    motor_cw();
  }
  if(nilaiTombol2 == 1){
    delay(1000);
    motor_ccw();
  }
  if(nilaiTombol3 == 1){
    delay(1000);
    motor_stop();
  }
  //------------------------------------------------------------
  if(Serial.available() > 0){
    menu = Serial.read();
    Serial.println(menu);
  }
  if(menu == 'a'){
    manual();
  }
  if(menu == 'b'){
    otomatis();
  }
}

void manual(){
  if(Serial.available() > 0){
    state = Serial.read();
    Serial.println(state);
  }
  if(state == '1'){
    //delay(1000);
    analogWrite(Pin_E_L, 255); //0-255
    digitalWrite(Pin_D1_L, LOW);
    digitalWrite(Pin_D2_L, HIGH);
  }
  if(state == '2'){
    //delay(1000);
    analogWrite(Pin_E_L, 255);
    digitalWrite(Pin_D1_L, HIGH);
    digitalWrite(Pin_D2_L, LOW);
  }
  if(state == '0'){
    delay(1000);
    digitalWrite(Pin_D1_L, LOW);
    digitalWrite(Pin_D2_L, LOW);
    analogWrite(Pin_E_L, 0);
  }
  manual();
}

void otomatis(){
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration = pulseIn(echoPin1, HIGH);
  distance = ((duration / 2) * 0.0343) + 7.05; // 4.5 diambil dari jarak sensor 1 dari permukaan bawah meja
  
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration1 = pulseIn(echoPin2, HIGH);
  distance1 = ((duration1 / 2) * 0.0343) + 4.5; // 4.5 diambil dari jarak sensor 2 dari permukaan bawah meja
  
  lcd.setCursor(0,0); //  jarak sensor 1
  lcd.print("Sensor1= ");
  lcd.setCursor(0,1); //  jarak sensor 2
  lcd.print("Sensor2= ");
  lcd.setCursor(12,1);
  lcd.print("cm");
  lcd.setCursor(12,0);
  lcd.print("cm");
  lcd.setCursor(9,0);
  lcd.print(distance);
  lcd.setCursor(9,1);
  lcd.print(distance1);
  
  if(Serial.available() > 0){
    data = Serial.parseInt();
    Serial.print(data);
  }
  if(data > 69){
    if(data > distance1){
      for(int a = distance1; a <= distance1; a++){
        //int a = distance1;
        if(a >= data-1.5){
          delay(10);
          digitalWrite(Pin_D1_L, LOW);
          digitalWrite(Pin_D2_L, LOW);
          analogWrite(Pin_E_L, 0);
        }else{
          delay(10);
          //Serial.println(distance1);
          analogWrite(Pin_E_L, 255); //0-255
          digitalWrite(Pin_D1_L, LOW);
          digitalWrite(Pin_D2_L, HIGH);
        }
      }
    }
    if(data < distance){
      for(int b = distance; b >= distance; b--){
        if(b <= data+1.5){
          delay(10);
          digitalWrite(Pin_D1_L, LOW);
          digitalWrite(Pin_D2_L, LOW);
          analogWrite(Pin_E_L, 0);
        }else{
          delay(10);
          analogWrite(Pin_E_L, 255);
          digitalWrite(Pin_D1_L, HIGH);
          digitalWrite(Pin_D2_L, LOW);
        }
      }
    }
  }
  otomatis();
}

void motor_ccw(){
  analogWrite(Pin_E_L, 255);
  digitalWrite(Pin_D1_L, HIGH);
  digitalWrite(Pin_D2_L, LOW);
}

void motor_cw(){
  analogWrite(Pin_E_L, 255); //0-255
  digitalWrite(Pin_D1_L, LOW);
  digitalWrite(Pin_D2_L, HIGH);
}

void motor_stop(){
  digitalWrite(Pin_D1_L, LOW);
  digitalWrite(Pin_D2_L, LOW);
  analogWrite(Pin_E_L, 0);
}
