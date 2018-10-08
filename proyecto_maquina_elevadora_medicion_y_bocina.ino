
#include <NewPing.h>

/*Aqui se configuran los pines donde debemos conectar el sensor*/
#define TRIGGER_PIN  12
#define ECHO_PIN     11
#define MAX_DISTANCE 200

/*Crear el objeto de la clase NewPing*/
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

unsigned long tiempoPrendidoBocina1 = 0;
unsigned long resetMillis1 = 0;
unsigned long avanceTiempo1 = 0;
unsigned long tiempoPrendidoBocina2 = 0;
unsigned long resetMillis2 = 0;
unsigned long avanceTiempo2 = 0;
unsigned long tiempoPrendidoBocina3 = 0;
unsigned long resetMillis3 = 0;
unsigned long avanceTiempo3 = 0;

int altura_maxima_bajada=50;
int altura_intermedia_1_bajada=35;
int altura_intermedia_2_bajada=20;
int altura_minima_bajada=7;

int duracionBocina = 500;



void setup() {
 /*relé1 bocina*/
  pinMode(5, OUTPUT);
  digitalWrite(5,HIGH);

  /*relé2 final de carrera*/
  pinMode(10, OUTPUT);
  digitalWrite(10,HIGH);                                                                                                                                                
  /*entrada de señal indicadora de bajada*/
  pinMode(3, INPUT_PULLUP);
  /*Entrada de señal indicadora de subida*/
  pinMode(4,INPUT_PULLUP);

  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,LOW);
  /*alimentacion para contacto indicador de baja*/

  Serial.begin(9600);
}

void loop() {

  tiempoPrendidoBocina1 = millis();
  avanceTiempo1 = tiempoPrendidoBocina1 - resetMillis1;

  tiempoPrendidoBocina2 = millis();
  avanceTiempo2 = tiempoPrendidoBocina2 - resetMillis2;

  tiempoPrendidoBocina3 = millis();
  avanceTiempo3 = tiempoPrendidoBocina3 - resetMillis3;

  int uS = sonar.ping_median();

  int medidaCM = uS / US_ROUNDTRIP_CM;

  if(medidaCM==0){
   digitalWrite(LED_BUILTIN,HIGH);
  }else{
    digitalWrite(LED_BUILTIN,LOW);
  }

  if (!digitalRead(3)) {

    
    sonarBocinaBajada ( medidaCM, tiempoPrendidoBocina1,
                        tiempoPrendidoBocina2,
                        tiempoPrendidoBocina3,
                        avanceTiempo1,
                        avanceTiempo2, 
                        avanceTiempo3 );

  }
else if(medidaCM>=altura_minima_bajada ){
  digitalWrite(10,HIGH);
  digitalWrite(5,HIGH);
}
else{
  digitalWrite(5,HIGH);
}

  
  Serial.print("Distancia: ");
  Serial.println(medidaCM);
  

  
}

/**********************************************************************************************************************************************************************************/

void sonarBocinaBajada (int medidaCM,
                        unsigned long tiempoPrendidoBocina1,
                        unsigned long tiempoPrendidoBocina2,
                        unsigned long tiempoPrendidoBocina3,
                        unsigned long avanceTiempo1,
                        unsigned long avanceTiempo2,
                        unsigned long avanceTiempo3 ) {
                          

  if (!digitalRead(10)) {
    digitalWrite(5, HIGH);
  }

  if (medidaCM < altura_maxima_bajada && medidaCM >= altura_intermedia_1_bajada) {
    resetMillis2 = tiempoPrendidoBocina2;
    resetMillis3 = tiempoPrendidoBocina3;
    if (avanceTiempo1 <= duracionBocina ) {

      digitalWrite(5, LOW);
      digitalWrite(10,HIGH);
    }
    else if (avanceTiempo1 > duracionBocina && avanceTiempo1 <= duracionBocina+1500) {

      digitalWrite(5, HIGH);
      digitalWrite(10,HIGH);

    } else {
      resetMillis1 = tiempoPrendidoBocina1;
    }
  }
  else if (medidaCM < altura_intermedia_1_bajada && medidaCM >= altura_intermedia_2_bajada) {

    resetMillis1 = tiempoPrendidoBocina1;
    resetMillis3 = tiempoPrendidoBocina3;

    if (avanceTiempo2 <= duracionBocina ) {

      digitalWrite(5, LOW);
      digitalWrite(10,HIGH);
    }
    else if (avanceTiempo2 > duracionBocina && avanceTiempo2 <= duracionBocina+500) {

      digitalWrite(5, HIGH);
      digitalWrite(10,HIGH);
      
    } else {
      resetMillis2 = tiempoPrendidoBocina2;
    }
  } else if (medidaCM < altura_intermedia_2_bajada && medidaCM >= altura_minima_bajada ) {
    resetMillis1 = tiempoPrendidoBocina1;
    resetMillis2 = tiempoPrendidoBocina2;
    digitalWrite(5, LOW);
    digitalWrite(10,HIGH);
  }
  else if (medidaCM < altura_minima_bajada&&medidaCM>0) {
    digitalWrite(5, HIGH);
    digitalWrite(10, LOW);
    resetMillis1 = tiempoPrendidoBocina1;
    resetMillis2 = tiempoPrendidoBocina2;
    resetMillis3 = tiempoPrendidoBocina3;
  }
  else {

    digitalWrite(5, HIGH);
    digitalWrite(10, HIGH);
    resetMillis1 = tiempoPrendidoBocina1;
    resetMillis2 = tiempoPrendidoBocina2;
    resetMillis3 = tiempoPrendidoBocina3;
  }
}

void sonarBocinaSubida (int medidaCM,
                        unsigned long tiempoPrendidoBocina1,
                        unsigned long tiempoPrendidoBocina2,
                        unsigned long tiempoPrendidoBocina3,
                        unsigned long avanceTiempo1,
                        unsigned long avanceTiempo2,
                        unsigned long avanceTiempo3 ) {

  if (digitalRead(10)) {
    digitalWrite(5, HIGH);
  }

  if (medidaCM < 100 ) {
    digitalWrite(5, LOW);
    digitalWrite(10, LOW);

  }

  else {

    digitalWrite(5, HIGH);
    digitalWrite(10, LOW);
    resetMillis1 = tiempoPrendidoBocina1;
    resetMillis2 = tiempoPrendidoBocina2;
    resetMillis3 = tiempoPrendidoBocina3;
  }
}
