/*
Hygrométrie et température
Contrôle de ventilation/chauffage (fonction thermostat)
Librairies à télécharger et à décompresser dans le répertoire arduino/libraries : 
*Librairie Adafruit pour le DHT22  https://github.com/adafruit/DHT-sensor-library
*/
//Inclusion des bibliothèques
#include "DHT.h" //Bibliothèque DHT
#include <SPI.h> //Bibliothèque SPI -> communication SPI
#include <TFT.h> //Bibliothèque pour l'écran TFT

//Définition des broches de l'écran TFT
#define SCLK 13  
#define MOSI 11 
#define CS   10
#define DC   9
#define RESET  8

TFT TFTscreen = TFT(CS, DC, RESET);

// Définition des pin DHT22
// Pin 1 (a gauche) +5V
// Pin 2 data DHTPIN -> pin 4 Arduino
// Placer une résistance de 10K entre les Pins 1 et 2
// Pin 4 Masse
#define DHTPIN 4
DHT dht(DHTPIN, DHT22);

// Définition des consignes :
const int intTimePause = 3000; // Actualisation des valeurs toutes les 3 secondes
int relai_humi = 7; // Relier le pin IN1 du relai au pin 7 de l'Arduino
int cons_humi = 53; // Consigne d'humidité
int relai_temp = 3; // Si installation d'un radiateur, relier le pin IN2 du relai au pin 3 de l'Arduino
int cons_temp = 26; // Consigne température

void setup(void) {

  Serial.begin(9600);
  Serial.print("Hello !");
  Serial.println("DHTxx test!");
  pinMode (relai_humi,OUTPUT);
  pinMode (relai_temp,OUTPUT);  // À décommenter pour activer le relai radiateur
  
  Serial.begin(9600);
  dht.begin();

  TFTscreen.begin();

  TFTscreen.background(255, 255, 255);
  TFTscreen.stroke(0,0,255);
  TFTscreen.setTextSize(2);


}

void loop() {
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();


  TFTscreen.println("Humidite :");
  TFTscreen.println(h);

  TFTscreen.println("Temp : ");
  TFTscreen.println(t);

  // Commande des relais
  // Humidité : mise en marche/arrêt de la ventilation
  if (h >= (cons_humi))  // Si la mesure h est > ou + à la consigne +2.00 : allumage extracteur
  {
    digitalWrite (relai_humi,LOW);   // Si la mesure h est < à la consigne +1.00 : arrêt extracteur
    TFTscreen.println("Ventil : ON");
  }
  else
  if (h < (cons_humi))
  {
    digitalWrite (relai_humi,HIGH);
    TFTscreen.println("Ventil : OFF");
  }
 

  // Température : mise en marche/arrêt du radiateur  // Décommenter toutes les lignes ci dessous
  if (t >= (cons_temp)) // Si la mesure t est > ou = à la consigne +2.00 : allumage chauffage
  {
    digitalWrite (relai_temp,LOW);
    TFTscreen.println("Rad : OFF");
  }
  else
  if (t < (cons_temp)) // Si la mesure t est < à la consigne -1.00 : arrêt chauffage
  {
    digitalWrite (relai_temp,HIGH);
    TFTscreen.println("Rad : ON");
  }
  delay (intTimePause);

  TFTscreen.setCursor(0,0);
  TFTscreen.setTextColor(ST7735_BLUE);
  TFTscreen.fillScreen(ST7735_WHITE); //Efface l'écran en blanc

  
  }//Fin loop
