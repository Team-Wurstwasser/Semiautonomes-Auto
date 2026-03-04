#include <Servo.h>
#include <NewPing.h>
 
#define TRIG_PIN_01 11
#define ECHO_PIN_01 10
#define MAX_DIST_01 200
 
NewPing sonar_01(TRIG_PIN_01, ECHO_PIN_01, MAX_DIST_01);
 
Servo servo1;
int position1 = 0;
int radar[18];
int radarpos = 0;
int distance;
 
char richtung = 'x';
const int led1 = 12;
const int led2 = 13;
 
//Reifenansteuerrung
const int reifenkontrolle_1_01 = 8;
const int reifenkontrolle_1_02 = 7;
const int ena = 5;
const int reifenkontrolle_2_03 = 4;
const int reifenkontrolle_2_04 = 2;
const int enb = 6;
const int Geschwindigkeit_reifen_01 = 200;
const int Geschwindigkeit_riefen_02 = 200;
const int Areal_Links = 0;  
const int Areal_Mitte = 0;
const int Areal_Rechts = 0;
 
int modus = 0; // Manuelle eingabe nötig 1 / 0
 
void setup(){
  servo1.attach(9);
  Serial.begin(9600);
 
  pinMode(reifenkontrolle_1_01, OUTPUT);
  pinMode(reifenkontrolle_1_02, OUTPUT);
  pinMode(ena, OUTPUT);
  pinMode(reifenkontrolle_2_03, OUTPUT);
  pinMode(reifenkontrolle_2_04, OUTPUT);
  pinMode(enb, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
 
}
 
void loop(){
  if(modus == 0){
    //Automatisch Gesteuertes Programm
    Scan();
    Areal_Bestimmung();
    Richtung_Bestimmung();
    Fahrt();
    Serial_Output();
  }
  else if(modus == 1){
    //Benutzer Gesteuertes Programm
    //1.Wert: Zeit in ms; 2.Wert: Geschwindigkeit in %
    Vorwaerts(300, 100);
    Links(1000, 50);
    Rueckwaerts(250, 75);
    Rechts(300, 100);
    Links(600, 50);
    Vorwaerts(750, 25);
    Stopp(500);
    Rueckwaerts(25, 100);
    Vorwaerts(25, 100);
    Rueckwaerts(25, 100);
    Vorwaerts(25, 100);
    Rueckwaerts(25, 100);
    Vorwaerts(25, 100);
    Rueckwaerts(25, 100);
    Vorwaerts(25, 100);
    Rueckwaerts(25, 100);
    Vorwaerts(25, 100);
    Stopp(2000);
  }
 
}
 
void Scan(){
  // Servo fuer Radar Steuerrung
  while (position1 < 180)
  {
    servo1.write(position1);
    radarpos = position1 / 10;
    delay(50);
    position1+=10;
    distance = sonar_01.ping_cm();
    radar[radarpos] = distance;
  }
 
  while (position1 > 0)
  {
    servo1.write(position1);
    radarpos = position1 / 10;
    delay(50);
    position1-=10;
    distance = sonar_01.ping_cm();
    radar[radarpos] = distance;
 
  }
}
 
void Areal_Bestimmung(){
  //Berechnung Lenkung
 
 Areal_Rechts = 255;
 Areal_Mitte = 255;
 Areal_Links = 255;
  int i;
    for (i = 0; i <= 5; i++){
      if(radar[i] <= Areal_Rechts && radar[i] != 0){      
        Areal_Rechts  = radar[i];
      }
    }
    for (i = 6; i <= 11; i++){
      if(radar[i] <= Areal_Mitte && radar[i] != 0){      
        Areal_Mitte  = radar[i];
      }
    }
    for (i = 12; i <= 17; i++){
      if(radar[i] <= Areal_Links && radar[i] != 0){      
        Areal_Links  = radar[i];
      }
    }
}
 
void Richtung_Bestimmung(){
  if (Areal_Mitte <= 20 && Areal_Rechts <= 40 && Areal_Links <= 40){
    richtung = 'b';
  }  
  else if (Areal_Mitte >= Areal_Links && Areal_Mitte >= Areal_Rechts){
    richtung = 'v';
  }
  else if (Areal_Rechts > Areal_Links && Areal_Rechts > Areal_Mitte){
    richtung = 'r';
  }
  else if (Areal_Links > Areal_Rechts && Areal_Links > Areal_Mitte){
    richtung = 'l';
  }
  else {
    richtung = 'x';
  }
}
 
void Fahrt(){
  if (richtung == 'v'){
    Serial.println("Vorwärts");
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    // Fahre Vorwärts
    Vorwaerts(500, 100);
  }
  else if (richtung == 'r'){
    Serial.println("Rechts");
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
    // Fahre Rechts
    Rechts(300, 100);
    Vorwaerts(200, 100);
  }
  else if (richtung == 'l'){
    Serial.println("Links");
    digitalWrite(led1, LOW);
    digitalWrite(led2, HIGH);
    // Fahre Links
    Links(300, 100);
    Vorwaerts(200, 100);
  }
  else if (richtung == 'b'){
    Serial.println("Rückwärts");
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    // Fahre Rückwärts
    Rueckwaerts(500, 100);
  }
  else {
   
  }
  Stopp(0);
}
 
void Vorwaerts(int x, int y){
  analogWrite(ena, y*2.25);
  analogWrite(enb, y*2.55);
  digitalWrite(reifenkontrolle_1_01, LOW);
  digitalWrite(reifenkontrolle_1_02, HIGH);
  digitalWrite(reifenkontrolle_2_03, LOW);
  digitalWrite(reifenkontrolle_2_04, HIGH);
  delay(x);
}
 
void Rechts(int x, int y){
  analogWrite(ena, y*2.25);
  analogWrite(enb, y*2.55);
  digitalWrite(reifenkontrolle_1_01, HIGH);
  digitalWrite(reifenkontrolle_1_02, LOW);
  digitalWrite(reifenkontrolle_2_03, LOW);
  digitalWrite(reifenkontrolle_2_04, HIGH);
  delay(x);
}
 
void Links(int x, int y){
  analogWrite(ena, y*2.25);
  analogWrite(enb, y*2.55);
  digitalWrite(reifenkontrolle_1_01, LOW);
  digitalWrite(reifenkontrolle_1_02, HIGH);
  digitalWrite(reifenkontrolle_2_03, HIGH);
  digitalWrite(reifenkontrolle_2_04, LOW);
  delay(x);
}
 
void Rueckwaerts(int x, int y){
  analogWrite(ena, y*2.25);
  analogWrite(enb, y*2.55);
  digitalWrite(reifenkontrolle_1_01, HIGH);
  digitalWrite(reifenkontrolle_1_02, LOW);
  digitalWrite(reifenkontrolle_2_03, HIGH);
  digitalWrite(reifenkontrolle_2_04, LOW);
  delay(x);
}
 
void Stopp(int x){
  digitalWrite(reifenkontrolle_1_01, LOW);
  digitalWrite(reifenkontrolle_1_02, LOW);
  digitalWrite(reifenkontrolle_2_03, LOW);
  digitalWrite(reifenkontrolle_2_04, LOW);
  delay(x);
}
 
void Serial_Output() {
  const char* labels[] = {"Areal_Rechts", "Areal_Mitte", "Areal_Links"};
  
  for (int i = 0; i < 3; i++) {
    Serial.print(labels[i]);
    Serial.println(":");
    
    for (int j = 0; j < 6; j++) {
      Serial.print(radar[i * 6 + j]);
      if (j < 5) Serial.print(", ");
    }
    Serial.println("\n");
  }
  Serial.print("\n\n");
}
