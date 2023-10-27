
/*
 * Datalogueur avec thermocouple MAX31856 Afficheur LCD Enregistrement SD et horloge temps réel DS3231
 * 
 *  **** I2C  *****
 * 
 *  * ###Brochage pour affichage LCD##
 * Gnd->Gnd Haut fil jaune 
 * Vcc->5V  milieu fil vert
 * SDA A4 fil gris pin 20 Mega
 * SCL A5 fil blanc pin 21 Mega
 * 
 * **** SPI ****
 * 
 *  * ###Connection with DS3231 Horloge Temps réel ###
 * Vcc->5V
 * Gns->Gnd
 * SCL->pin A5 fil jaune  pin 21 Mega
 * SDA-> pin A4 fil orange pin 20 Mega 
 * 
 * 
SPI: CS, DI, DO, CLK pour le brochage de la carte
     6,  8,  7,  5

* branchement MAX31856 SPI
* // Use software SPI: CS, DI, DO, CLK
//Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(10, 11, 12, 13);
// use hardware SPI, just pass in the CS pin

fil rouge VCC
fil noir GND

fil vert DI patte 8
fil bleu DO patte 7
fil violet CLK patte 5
fil gris CS TC1 patte 9
fil blanc CS TC2 patte 3
fil orange CS TC3 patte 10
fil jaune CS TC4 patte 2


Use software SPI: CS, DI, DO, CLK pour le brochage de la carte
Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(10, 11, 12, 13) port utilisé pour le SPI;
use hardware SPI, just pass in the CS pin n'est obligatoire que CS posistion 10
Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(10);

 * 
 *  
 *  Branchement SD card
 *  VCC rouge
 *  GND brun
 * Jaune 11 MOSI - pin 11  pin 51 mega jaune
 * Orange 12 MISO - pin 12 pin 50 mega orange
 * vert 13 CLK - pin 13 pin 52 SCK vert
 * bleue  4 CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN) select bleu
 * 
 * 
fil blanc DI patte 51
fil noir DO patte 50
fil gris CLK patte 52
fil violet CS TC1 patte 4
 * 
 *  The circuit:
 * connect TM1637 N°1 pin CLK to Arduino fil noir 24
 * connect TM1637 N°1 pin DIO to Arduino fil blanc 22 
 * 
 * connect TM1637 N°2 pin CLK to Arduino fil bleu 28
 * connect TM1637 N°2 pin DIO to Arduino fil violet 26 
 * 
 * connect TM1637 N°3 pin CLK to Arduino fil vert 32
 * connect TM1637 N°3 pin DIO to Arduino fil bleu 30 
 * 
 * connect TM1637 N°4 pin CLK to Arduino fil vert 36
 * connect TM1637 N°4 pin DIO to Arduino fil jaune 34 
 * 
 * 
 * connect TM1637 N°4 pin GND to Arduino pin GND bleu
 * 
 * //TM1637Display display1 = TM1637Display(CLK, DIO); clk orange 24 dio jaune 22
TM1637Display display1 = TM1637Display(24, 22);
TM1637Display display2 = TM1637Display(28, 26);
TM1637Display display3 = TM1637Display(32, 30);
TM1637Display display4 = TM1637Display(36, 34);
 * 
 * 
 * 
 * Anémometre 
 * Bleu GND
 * Brun A0
 * 
 * LED 
 * 
 * fil Vert pin 42 led jaune 
 * fil Bleu pin 44 led rouge
 * Fil noir GND
 */


// Thermo
#include <Adafruit_MAX31856.h>

// décalaration de l'obget mathermo et envoie du brochage SPI dans l'ordre CS, DI, DO, CLK
Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(2, 8, 7, 5);
Adafruit_MAX31856 maxthermoA = Adafruit_MAX31856(10, 8, 7, 5);
Adafruit_MAX31856 maxthermoB = Adafruit_MAX31856(9, 8, 7, 5);
Adafruit_MAX31856 maxthermoC = Adafruit_MAX31856(3, 8, 7, 5);


// décalaration de l'obget mathermo et envoie du brochage SPI dans l'ordre CS, DI, DO, CLK
/*Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(3, 8, 7, 5);
Adafruit_MAX31856 maxthermoA = Adafruit_MAX31856(6, 8, 7, 5);
Adafruit_MAX31856 maxthermoB = Adafruit_MAX31856(10, 8, 7, 5);
Adafruit_MAX31856 maxthermoC = Adafruit_MAX31856(2, 8, 7, 5); */



// LCD
#include <Wire.h>
#include <LiquidCrystal_I2C.h> // pour télécharger la librairie: croquis - include library - manage libraries - LiquidCrystal_I2C.h de Frank de Brabander
LiquidCrystal_I2C lcd(0x27,20,4); //écran LCD 0x27 avec 16 colonnes et 2 lignes (ligne 0 et ligne 1)  

// Horloge
#include <DS3231.h> //Library for RTC module horloge temps réel
// Init the DS3231 using the hardware interface
DS3231  rtc(SDA, SCL);

// SD card
#include <SPI.h>
#include <SD.h>
// CS pour le carte SD
const byte chipSelect = 4;

// Affichage Led
#include <TM1637Display.h>
// Create a display object of type TM1637Display
//TM1637Display display1 = TM1637Display(CLK, DIO); clk orange 24 dio jaune 22
TM1637Display display1 = TM1637Display(24, 22);
TM1637Display display2 = TM1637Display(28, 26);
TM1637Display display3 = TM1637Display(32, 30);
TM1637Display display4 = TM1637Display(36, 34);



// BME
#include <Adafruit_BME280.h>  
#define adresseI2CduBME280                0x76            // Adresse I2C du BME280 (0x76, dans mon cas, ce qui est souvent la valeur par défaut)
#define pressionAuNiveauDeLaMerEnHpa      1024.90         // https://fr.wikipedia.org/wiki/Pression_atmospherique (1013.25 hPa en moyenne, valeur "par défaut")
//#define delaiRafraichissementAffichage    1500            // Délai de rafraîchissement de l'affichage (en millisecondes)

// Instanciation de la librairie BME280
Adafruit_BME280 bme;

float alti;

// Anémometre

float tension;
float vitesse;

// variable température temps réel et avant
int templu;  
int templuA;  
int templuB;  
int templuC;  

int templuamb;
int templuambA;
int templuambB;
int templuambC;

//int templuavant ;
//int number ;
//int valeur ; 
int deltatps;

int tchs1 ;
int tchs2 ;
int tchs3 ;
int tchs4 ;
int tcerr ;
int sderr ;

// initialisation 

void setup() {

       
    // début de la communication du port série 
    Serial.begin(9600);
    
// initialitsion Premier thermocouple PREMIER CS 6 ****************************** 1
  while (!Serial) delay(10);
//  Serial.println("MAX31856 test thermocouple");

// test de fonctionnement du thermocouple 
  if (!maxthermo.begin()) {
    Serial.println("Ne peux pas initiliser le thermocouple. 1");
   while (1) delay(10);
  }

  // définir le type de thermcouple dans mon cas c'est S 
maxthermo.setThermocoupleType(MAX31856_TCTYPE_S); 
  
// définittion de type de conversion 
 maxthermo.setConversionMode(MAX31856_CONTINUOUS);

// filtre du 50hz sur le thermocouple
  maxthermo.setNoiseFilter(MAX31856_NOISE_FILTER_50HZ); 


// initialitsion A thermocouple DEUXIEME  CS 3 *********** 2
  while (!Serial) delay(10);
//  Serial.println("MAX31856 test thermocouple");

// test de fonctionnement du thermocouple 
  if (!maxthermoA.begin()) {
    Serial.println("Ne peux pas initiliser le thermocouple. 2");
   while (1) delay(10);
  }

  // définir le type de thermcouple dans mon cas c'est S
maxthermoA.setThermocoupleType(MAX31856_TCTYPE_S); 
// définittion de type de conversion 
 maxthermoA.setConversionMode(MAX31856_CONTINUOUS);
// filtre du 50hz sur le thermocouple
  maxthermoA.setNoiseFilter(MAX31856_NOISE_FILTER_50HZ); 


// initialitsion A thermocouple TROISIEME  CS 2 *********** 3
  while (!Serial) delay(10);
//  Serial.println("MAX31856 test thermocouple");

// test de fonctionnement du thermocouple 
  if (!maxthermoB.begin()) {
    Serial.println("Ne peux pas initiliser le thermocouple. 3");
   while (1) delay(10);
  }

  // définir le type de thermcouple dans mon cas c'est S
maxthermoB.setThermocoupleType(MAX31856_TCTYPE_S); 
// définittion de type de conversion 
 maxthermoB.setConversionMode(MAX31856_CONTINUOUS);
// filtre du 50hz sur le thermocouple
  maxthermoB.setNoiseFilter(MAX31856_NOISE_FILTER_50HZ); 


// initialitsion A thermocouple QUATRIEME  CS 1 *********** 4
  while (!Serial) delay(10);
//  Serial.println("MAX31856 test thermocouple");

// test de fonctionnement du thermocouple 
  if (!maxthermoC.begin()) {
    Serial.println("Ne peux pas initiliser le thermocouple. 4");
   while (1) delay(10);
  }

  // définir le type de thermcouple dans mon cas c'est S
maxthermoC.setThermocoupleType(MAX31856_TCTYPE_S); 
// définittion de type de conversion 
 maxthermoC.setConversionMode(MAX31856_CONTINUOUS);
// filtre du 50hz sur le thermocouple
  maxthermoC.setNoiseFilter(MAX31856_NOISE_FILTER_50HZ); 


// FIN des 4 thermocouple

// initialisation LCD
  lcd.begin();
  lcd.cursor_on();
  lcd.backlight(); // noBacklight() pour désactiver le retro éclairage
  lcd.setCursor(0,0); // écriture du texte sur la ligne 1 colonne 5 (centré)
  lcd.print("DEBUT !"); // texte "hello" sur ligne 1 colonne 5 


// initialise horloge
  Initialize_RTC();

// initialise transfert vers Excel
  Initialize_PlxDaq();
// Initialisation SD Card
  Initialize_SDcard();


if(!bme.begin(adresseI2CduBME280)) {
    Serial.println(F("> PB BME…"));
  //  while(1);                              // Arrêt du programme, en cas d'échec de l'initialisation
  } else {
  //  Serial.println(F("> OK BME !")); 
  }
  Serial.println();


/* traitement des led */ 
pinMode(42,OUTPUT);
pinMode(44,OUTPUT);


tchs1 = 0 ;
tchs2 = 0;
tchs3 = 0;
tchs4 = 0;

} // fin boucle initialisation /** fin void setup **/


// ************** boucle principal ARDUINO ************************

void loop() {

// Test thermocouple 
  Serial.print("Cold Junction Temp: ");
  Serial.println(maxthermo.readCJTemperature());

  Serial.print("Cold Junction Temp: ");
  Serial.println(maxthermoA.readCJTemperature());

  Serial.print("Cold Junction Temp: ");
  Serial.println(maxthermoB.readCJTemperature()); 

  Serial.print("Cold Junction Temp: ");
  Serial.println(maxthermoC.readCJTemperature());  

  
  Serial.print("Thermocouple Temp: ");
  Serial.println(maxthermo.readThermocoupleTemperature());

  Serial.print("Thermocouple Temp: ");
  Serial.println(maxthermoA.readThermocoupleTemperature());

  Serial.print("Thermocouple Temp: ");
  Serial.println(maxthermoB.readThermocoupleTemperature()); 

  Serial.print("Thermocouple Temp: ");
  Serial.println(maxthermoC.readThermocoupleTemperature());  

 
  // Check and print any faults
  uint8_t fault = maxthermo.readFault();
  if (fault) {
  
    if (fault & MAX31856_FAULT_CJRANGE) Serial.println("Cold Junction Range Fault"); 
    if (fault & MAX31856_FAULT_TCRANGE) Serial.println("Thermocouple Range Fault");
    if (fault & MAX31856_FAULT_CJHIGH)  Serial.println("Cold Junction High Fault");
    if (fault & MAX31856_FAULT_CJLOW)   Serial.println("Cold Junction Low Fault");
    if (fault & MAX31856_FAULT_TCHIGH)  Serial.println("Thermocouple High Fault");
    if (fault & MAX31856_FAULT_TCLOW)   Serial.println("Thermocouple Low Fault");
    if (fault & MAX31856_FAULT_OVUV)    Serial.println("Over/Under Voltage Fault");
    if (fault & MAX31856_FAULT_OPEN)    Serial.println("Thermocouple Open Fault");
    tchs1 = 1;
    
  }

if (not fault) { 
  tchs1 = 0;
}


 uint8_t faultA = maxthermoA.readFault();
  if (faultA) {

    if (faultA & MAX31856_FAULT_CJRANGE) Serial.println("Cold Junction Range Fault"); 
    if (faultA & MAX31856_FAULT_TCRANGE) Serial.println("Thermocouple Range Fault");
    if (faultA & MAX31856_FAULT_CJHIGH)  Serial.println("Cold Junction High Fault");
    if (faultA & MAX31856_FAULT_CJLOW)   Serial.println("Cold Junction Low Fault");
    if (faultA & MAX31856_FAULT_TCHIGH)  Serial.println("Thermocouple High Fault");
    if (faultA & MAX31856_FAULT_TCLOW)   Serial.println("Thermocouple Low Fault");
    if (faultA & MAX31856_FAULT_OVUV)    Serial.println("Over/Under Voltage Fault");
    if (faultA & MAX31856_FAULT_OPEN)    Serial.println("Thermocouple Open Fault");
    tchs2 = 1;
    
  }

if (not faultA) { 
  tchs2 = 0;
}


 uint8_t faultB = maxthermoB.readFault();
  if (faultB) {
    
    if (faultB & MAX31856_FAULT_CJRANGE) Serial.println("Cold Junction Range Fault"); 
    if (faultB & MAX31856_FAULT_TCRANGE) Serial.println("Thermocouple Range Fault");
    if (faultB & MAX31856_FAULT_CJHIGH)  Serial.println("Cold Junction High Fault");
    if (faultB & MAX31856_FAULT_CJLOW)   Serial.println("Cold Junction Low Fault");
    if (faultB & MAX31856_FAULT_TCHIGH)  Serial.println("Thermocouple High Fault");
    if (faultB & MAX31856_FAULT_TCLOW)   Serial.println("Thermocouple Low Fault");
    if (faultB & MAX31856_FAULT_OVUV)    Serial.println("Over/Under Voltage Fault");
    if (faultB & MAX31856_FAULT_OPEN)    Serial.println("Thermocouple Open Fault");
    if (maxthermoB.readThermocoupleTemperature() < 1 or maxthermoB.readThermocoupleTemperature() > 1700 ) {
    tchs3 = 1;
    }
    
  }
    if (not faultB) { 
     tchs3 = 0;
    }
    if (maxthermoB.readThermocoupleTemperature() > 1 and maxthermoB.readThermocoupleTemperature() < 1600 ) {
    tchs3 = 0;
    }

uint8_t faultC = maxthermoC.readFault();
  if (faultC) {

    if (faultC & MAX31856_FAULT_CJRANGE) Serial.println("Cold Junction Range Fault"); 
    if (faultC & MAX31856_FAULT_TCRANGE) Serial.println("Thermocouple Range Fault");
    if (faultC & MAX31856_FAULT_CJHIGH)  Serial.println("Cold Junction High Fault");
    if (faultC & MAX31856_FAULT_CJLOW)   Serial.println("Cold Junction Low Fault");
    if (faultC & MAX31856_FAULT_TCHIGH)  Serial.println("Thermocouple High Fault");
    if (faultC & MAX31856_FAULT_TCLOW)   Serial.println("Thermocouple Low Fault");
    if (faultC & MAX31856_FAULT_OVUV)    Serial.println("Over/Under Voltage Fault");
    if (faultC & MAX31856_FAULT_OPEN)    Serial.println("Thermocouple Open Fault");
    tchs4 = 1;
    
  }

if (not faultC) { 
  tchs4 = 0;
}



  // vérification conversion compléte et lire la température thermocouple 1
 if (maxthermo.conversionComplete()) {
      // sauvegarde de la température
   if (tchs1 == 0) {
        templu= maxthermo.readThermocoupleTemperature();
        templuamb = maxthermo.readCJTemperature() ;
   } else {
   templu = 0;
   templuamb = 0;}
        
 } else {
   Serial.println("Conversion incomplète! thermo 1");
   
   templu = 0;
   templuamb = 0;
   tchs1 = 1;
 }

  // vérification conversion compléte et lire la température thermocouple 2
 if (maxthermoA.conversionComplete()) {
      // sauvegarde de la température
    if (tchs2 == 0) {
         templuA= maxthermoA.readThermocoupleTemperature();
         templuambA = maxthermoA.readCJTemperature() ;
    } else {
    templuA = 0;
    templuambA = 0;}

     
 } else {
   Serial.println("Conversion incomplète! thermo 2");
   templuA = 0;
   templuambA =0 ;
    tchs2 = 1;
 }

  // vérification conversion compléte et lire la température thermocouple 3
 if (maxthermoB.conversionComplete()) {
// sauvegarde de la température
   if (tchs3 == 0) {
   templuB= maxthermoB.readThermocoupleTemperature();
   templuambB = maxthermoB.readCJTemperature() ;
   } else {
    templuB = 0;
    templuambB = 0;}
     
 } else {
   Serial.println("Conversion incomplète! thermo 3");
   templuB = 0;
   templuambB =0 ;
    tchs3 = 1;
 }

  // vérification conversion compléte et lire la température thermocouple 4
 if (maxthermoC.conversionComplete()) {
// sauvegarde de la température

   if (tchs4 == 0) {
   templuC= maxthermoC.readThermocoupleTemperature();
   templuambC = maxthermoC.readCJTemperature() ;
    } else {
    templuC = 0;
    templuambC = 0;}
     
 } else {
   Serial.println("Conversion incomplète! thermo 4");
   templuC = 0;
   templuambC =0 ;
    tchs4 = 1;
 }




// BME
 alti = bme.readAltitude(pressionAuNiveauDeLaMerEnHpa) ;

// Anémometre 
 int valeurLue = analogRead(A0);
 tension = (float)valeurLue * 5 / 1023;
 vitesse = ((float)valeurLue * 5 / 1023) / 0.009 ;
 
 lcd.clear();
   // affichage LCD de la température
    lcd.setCursor(0, 0); 
    lcd.print(rtc.getDateStr()); 
    lcd.print(" "); 
    lcd.print(rtc.getTimeStr()); 
    
    lcd.setCursor(0,1 );
    lcd.print("T1:");
    lcd.print(tchs1);
    lcd.print(" T2:");
    lcd.print(tchs2); 
    lcd.print(" T3:");
    lcd.print(tchs3); 
    lcd.print(" T4:");
    lcd.print(tchs4); 
          
    lcd.setCursor(0,2 );
    lcd.print("T"); 
    lcd.print(bme.readTemperature());
    lcd.print(" Hu "); 
    lcd.print(bme.readHumidity());
    
    lcd.setCursor(0,3 );
  
    lcd.print("P"); 
    lcd.print(bme.readPressure() / 100.0F);   
    lcd.print(" A"); 
    lcd.print(alti);
     
    lcd.print(" V"); 
    lcd.print(vitesse,0); 

  // Envoie sur carte SD et Excel
  if (deltatps == 50) { // on ecrit toute les 3 = 180 x1000 minutes sur la carte 

    Write_PlxDaq();
    Write_SDcard(); 

    
    deltatps = 0;
    }

    deltatps = deltatps +1 ;

    

// Test Horloge
  Serial.print(rtc.getDOWStr());
  Serial.print(" ");
  
  // Send date
  Serial.print(rtc.getDateStr());
  Serial.print(" -- ");

  // Send time
  Serial.println(rtc.getTimeStr());


 // Set the brightness to 5 (0=dimmest 7=brightest)
  display1.setBrightness(5);
  display1.showNumberDec(templu); 

// Set the brightness to 5 (0=dimmest 7=brightest)
  display2.setBrightness(5);
  display2.showNumberDec(templuA); 

// Set the brightness to 5 (0=dimmest 7=brightest)
  display3.setBrightness(5);
  display3.showNumberDec(templuB); 


// Set the brightness to 5 (0=dimmest 7=brightest)
  display4.setBrightness(5);
  display4.showNumberDec(templuC); 

tcerr = tchs1 + tchs2 + tchs3 + tchs4 ;
  if ( tcerr > 0 ) { 
  
   digitalWrite(44, HIGH); 
  }
else {
   digitalWrite(44, LOW); 
  }
 
// lcd.clear();
 
  delay(2000); // délais de la boucle = une seconde 
  

// sortie

 
} // fin loop

// **********  Fin LOOP *****************

// objets

// transferts vers  Excel
void Write_PlxDaq()
  {
    Serial.print("DATA"); //always write "DATA" to Indicate the following as Data
    Serial.print(","); //Move to next column using a ","
    Serial.print("DATE"); //Store date on Excel
    Serial.print(","); //Move to next column using a ","
    Serial.print("TIME"); //Store date on Excel
    Serial.print(","); //Move to next column using a ","
    Serial.print(templu); 
    Serial.print(","); //Move to next column using a ","
    Serial.print(templuA); 
    Serial.print(","); //Move to next column using a ","
    Serial.print(templuB); 
    Serial.print(","); //Move to next column using a ","
    Serial.print(templuC); 
    Serial.print(","); //Move to next column using a ","
    Serial.print(bme.readTemperature());
    Serial.print(","); //Move to next column using a ","
    Serial.print(bme.readHumidity());
    Serial.print(","); //Move to next column using a ","
    Serial.print(bme.readPressure() / 100.0F);
    Serial.print(","); //Move to next column using a ","
    Serial.print(alti);
    Serial.print(","); //Move to next column using a ","
    Serial.print(vitesse, 3);
    Serial.print(","); //Move to next column using a ","
    Serial.print(tchs1); //Move to next column using a ","
    Serial.print(","); //Move to next column using a ","
    Serial.print(tchs2); //Move to next column using a ","
    Serial.print(","); //Move to next column using a ","
    Serial.print(tchs3); //Move to next column using a ","
    Serial.print(","); //Move to next column using a ","
    Serial.print(tchs4); //Move to next column using a ","
    Serial.print(","); //Move to next column using a ","
    Serial.println(); //End of Row move to next row
  }

// initailisation vers Excel
void Initialize_PlxDaq()
{
Serial.println("CLEARDATA"); //clears up any data left from previous projects
Serial.println("LABEL,Date,Heure,T1,T2,T3,T4,T Amb, H BME, Pression, Altitude, Vent,E1,E2,E3,E4"); //always write LABEL, to indicate it as first line
}

// initialisation SD Card
void Initialize_SDcard()
{
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    sderr = 1 ;
    digitalWrite(42, HIGH); 
    
    return;
  }
   // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("LoggerCD.csv", FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println("D;H;T1;T2;T3;T4;TEMP;HUMI;PRESS;ALT;VENT;E1;E2;E3;E4"); //Write the first row of the excel file
    dataFile.close();
    sderr = 0 ;
    digitalWrite(42, LOW); 
    
  }
} // fin void initialise SD

// écrire sur carte SD
void Write_SDcard()
{
    // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("LoggerCD.csv", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.print(rtc.getDateStr()); //Store data on SD card
    dataFile.print(";"); //Move to next column using a ","
    dataFile.print(rtc.getTimeStr()); //Store data on SD card
    dataFile.print(";"); //Move to next column using a ","
    dataFile.print(templu); //Store data on SD card
    dataFile.print(";"); //Move to next column using a ","
    dataFile.print(templuA); //Store data on SD card
    dataFile.print(";"); //Move to next column using a ","
    dataFile.print(templuB); //Store data on SD card
    dataFile.print(";"); //Move to next column using a ","
    dataFile.print(templuC); //Store data on SD card
    dataFile.print(";"); //Move to next column using a ","
    dataFile.print(bme.readTemperature());
    dataFile.print(";"); //Move to next column using a ","
    dataFile.print(bme.readHumidity());
    dataFile.print(";"); //Move to next column using a ","
    dataFile.print(bme.readPressure() / 100.0F);
    dataFile.print(";"); //Move to next column using a ","
    dataFile.print(alti);
    dataFile.print(";"); //Move to next column using a ","
    dataFile.print(vitesse, 3);
    dataFile.print(";"); //Move to next column using a ","
    dataFile.print(tchs1);
    dataFile.print(";"); //Move to next column using a ","
    dataFile.print(tchs2);
    dataFile.print(";"); //Move to next column using a ","
    dataFile.print(tchs3);
    dataFile.print(";"); //Move to next column using a ","
    dataFile.print(tchs4);
    dataFile.println(); //End of Row move to next row
    dataFile.close(); //Close the file
 //   sderr = 0 ;
 //   digitalWrite(42, LOW); 
  }
  else
  Serial.println("Attention SD invalide");
//  sderr = 1 ;
//  digitalWrite(42, HIGH); 
}


void Initialize_RTC()
{
   // Initialize the rtc object
  rtc.begin();

//#### The following lines can be uncommented to set the date and time for the first time###  
/* */
//rtc.setDOW(MONDAY);     // Set Day-of-Week to SUNDAY 
//rtc.setTime(18, 58, 00);     // Set the time to 12:00:00 (24hr format)
//rtc.setDate(20, 10, 2023);   // Set the date to January 1st, 2014 
//Serial.println(" horloge ");
/* */
} 
