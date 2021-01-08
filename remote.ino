#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include "RTClib.h"

LiquidCrystal_I2C lcd(0x27,20,4); //alamat i2c lcd 0x27
RTC_DS1307 RTC; //deklarasi rtc ds1307
File myFile; //baca file dengan nama myFile
char nama[10]; //deklarasi opensd
char data[10];
char simpan[10];
long kode; //kode dosen
long kodex;
long tinggi;
long tinggix;
char customKey; //deklarasi keypad
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {9,8,7,6};
byte colPins[COLS] = {5,4,3,2};
Keypad customKeypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup(){
  Serial.begin(38400); //baudrate bluetooth
  
  lcd.begin();
  lcd.noCursor();
  Wire.begin();
  RTC.begin();
  if(!RTC.begin()){
    while(1);
  }
  if(!RTC.isrunning()){
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  
  menu();
}

void loop(){
  //kosong
}

void jam(){ //baca RTC
  DateTime now = RTC.now();
  lcd.setCursor(0,0);
  lcd.print(now.day());
  lcd.print('/');
  lcd.print(now.month());
  lcd.print('/');
  lcd.print(now.year());
  lcd.setCursor(12,0);
  if(now.hour()<10){
    lcd.print("0");
  }
  lcd.print(now.hour(),DEC);
  lcd.print(":");
  if(now.minute()<10){
    lcd.print("0");
  }
  lcd.print(now.minute(),DEC);
  lcd.print(":");
  if(now.second()<10){
    lcd.print("0");
  }
  lcd.print(now.second(),DEC);
  delay(1000);
}

void menuManual(){ //menu manual dengan dengan remote
  lcd.setCursor(0,1);
  lcd.print("Tekan 'A'/'B'/'C'");
  lcd.setCursor(0,2);
  lcd.print(" ");
  lcd.setCursor(0,3);
  lcd.print("Menu Utama 'D'");
  customKey = customKeypad.getKey();
  if(customKey == 'A'){
    lcd.clear();
    delay(1000);
    lcd.setCursor(0,1);
    lcd.print("naik");
    Serial.write('1'); //naik, serial komunikasi bluetooth
  }
  if(customKey == 'B'){
    lcd.clear();
    delay(1000);
    lcd.setCursor(0,1);
    lcd.print("turun");
    Serial.write('2'); //turun
  }
  if(customKey == 'C'){
    lcd.clear();
    delay(1000);
    lcd.setCursor(0,1);
    lcd.print("stop");
    Serial.write('0'); //stop
  }
  if(customKey == 'D'){
    lcd.clear();
    delay(1000);
    menu();
  }
  menuManual();
}

void opensd(){ //buka data meja dari microsd
  lcd.clear();
  delay(1000);
  while(1){
    customKey = customKeypad.getKey();
    lcd.setCursor(0,0);
    lcd.print("Pencarian");
    lcd.setCursor(0,3);
    lcd.print("Buka tekan B");
    lcd.setCursor(0,1);
    lcd.print(">Kode Dosen=");
            
    if(customKey >= '0' && customKey <= '9'){
      kode = kode * 10 + (customKey - '0');
      lcd.print(kode);
    }
    if(customKey == 'B'){
      lcd.clear();
      delay(1000);
      lcd.setCursor(0,2);
      while(!Serial){
        ;
      }
      if(!SD.begin(10)){
        while(1);
      }
      sprintf(nama, "%d.txt", kode);
      myFile = SD.open(nama); //mengambil nama file dari data txt kode
      if(myFile){
        char data;
        while((data = myFile.read()) > 0){
          Serial.write(data);
          lcd.print(char(data));
        }
        myFile.close();
      }
      else{
        lcd.setCursor(0,2);
        lcd.print("data tidak ada");
      }
    }
    if(customKey == 'D'){
      kode = 0;
      lcd.clear();
      delay(1000);
      menu();
    }
  }
}

void kodeDosen(){ //pengaturan kode dosen
  while(1){
    customKey = customKeypad.getKey();
    lcd.setCursor(0,1);
    lcd.print("->1. Kode Dosen=");
    if(customKey >= '0' && customKey <= '9'){
      kodex = kodex * 10 + (customKey - '0');
      lcd.print(kodex);
    }
    if(customKey == 'A'){
      lcd.clear();
      delay(1000);
      kodex;
      return;
    }
    lcd.setCursor(0,2);
    lcd.print("2. Tinggi Meja=");
    lcd.print(tinggi);
  }
  kodeDosen();
}

void tinggiMeja(){ //pengaturan tinggi meja
  while(1){
    lcd.setCursor(0,1);
    lcd.print("1. Kode Dosen=");
    lcd.print(kodex);

    customKey = customKeypad.getKey();
    lcd.setCursor(0,2);
    lcd.print("->2. Tinggi Meja=");
    if(customKey >= '0' && customKey <= '9'){
      tinggix = tinggix * 10 + (customKey - '0');
      lcd.print(tinggix);
    }
    if(customKey == 'A'){
      lcd.clear();
      delay(1000);
      tinggix;
      return;
    }
  }
}

void savesd(){ //simpan kode dosen dan ketinggian meja
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("saving.....");
  delay(3000);
  while (!Serial) {
    ;
  }
  if (!SD.begin(10)) {
    return;
  }
  sprintf(simpan,"%d.txt",kodex);
  myFile = SD.open(simpan, FILE_WRITE); //menulis file baru
  if (myFile) {
    myFile.print(tinggix); //mencetak data ketinggian meja
    myFile.close();
  }
  else{
  }
  lcd.setCursor(0,0);
  lcd.print("DONE..");
  delay(1000);
  lcd.clear();
}

void settingBaru(){ //menyimpan data baru
  while(2){
    lcd.setCursor(0,1);
    lcd.print("1. Kode Dosen=");
    lcd.print(kodex);
    lcd.setCursor(0,2);
    lcd.print("2. Tinggi Meja=");
    lcd.print(tinggix);
    lcd.setCursor(0,3);
    lcd.print("Simpan Data (C)");

    customKey = customKeypad.getKey();
    if(customKey == 'C'){
      if (tinggix < 70 || tinggix > 100){
        lcd.clear();
        delay(1000);
        lcd.setCursor(0,0);
        lcd.print("tinggi > 100");
        lcd.setCursor(0,1);
        lcd.print("tinggi < 70");
        delay(3000);
        kodex=0;
        tinggix=0;
        lcd.clear();
        delay(500);
        settingBaru();
      }
      if (tinggix >= 70 || tinggix <= 100){
        lcd.clear();
        delay(1000);
        savesd();
        lcd.clear();
        delay(500);
        kodex=0;
        tinggix=0;
        menu();
      }
    }
    if(customKey == 'D'){
      kodex=0;
      tinggix=0;
      lcd.clear();
      delay(1000);
      lcd.clear();
      menu();
    }
    if(customKey == 'A'){
      lcd.clear();
      delay(1000);
      kodeDosen();
      tinggiMeja();
     }
  }
}

void menu(){ //menu utama
  while(1){
    lcd.setCursor(0,0);
    jam();
    lcd.setCursor(0,1);
    lcd.print("Pilih Menu");
    lcd.setCursor(0,2);
    lcd.print("1. Manual");
    lcd.setCursor(0,3);
    lcd.print("2. Otomatis");
    customKey = customKeypad.getKey();
    if(customKey == '1'){
      Serial.write('a');
      lcd.clear();
      delay(1000);
      menuManual();
    }
    if(customKey == '2'){
      Serial.write('b');
      lcd.clear();
      delay(1000);
      lcd.setCursor(0,1);
      lcd.print("1. Pengguna Lama");
      lcd.setCursor(0,2);
      lcd.print("2. Pengguna Baru");
      lcd.setCursor(0,3);
      lcd.print("Tekan 1 / 2");
      while(1){
        customKey = customKeypad.getKey();
        if(customKey == '1'){
          opensd();
        }
        if(customKey == '2'){
          lcd.clear();
          delay(1000);
          settingBaru();
        }
      }
    }
  }
  menu();
}
