//Pines analogicos
#define sensorTemperatura A5
#define sensorHumedad A4  //Higrometro

//Pines digitales
#define pulsadorMarcha 11
#define pulsadorParada 12
#define pulsadorSilenciar 13
#define alarmaLed 17
#define ledFallaHumidificador 16
#define ledFallaCalentador 15

#define humidificador 7
#define alarmaSonora 14
#define ventilador 8
#define extractor 9
#define calentador 10
#define ArreglarCalentador 1
#define ArreglarHumidificador 0

#define motorVentilador 2
#define motorExtractor 3
#define ledTest 5

#define PTVN 2
#define PTEX 3
#define LVENT 4
#define LEXT 5

//Conversiones (todo en bits)
int grado_30 = 61;  // Celsius -> Bit
int grado_80 = 164;
int grado_82 = 168;
int grado_83 = 170;
int grado_85 = 174;

int humedad_60 = 614;  //  %Humedad -> Bit
int humedad_55 = 563;
int humedad_53 = 543;
int humedad_52 = 532;

//Estados
enum estadosDeParada { SinFalla,
                    ConFalla};
estadosDeParada estadoParada = SinFalla;

enum estadosDeConFalla {Puls1,
                        PulsMid,
                        Puls2};
estadosDeConFalla estadoFalla = Puls1;

enum EstadosGenerales { Parada,
                        Marcha,};
EstadosGenerales estadoActual = Parada;

enum EstadoCalentador { TiempoCLR,
                        EjecucionCLR,
                        EjecucionCLR2};
EstadoCalentador estadoActualCalentador = TiempoCLR;

enum EstadoExtractor { EjecucionEXT,
                       EjecucionEXT2};
EstadoExtractor estadoActualExtractor = EjecucionEXT;

enum EstadoHumidificador { TiempoHMD,
                           EjecucionHMD,
                           EjecucionHMD2};
EstadoHumidificador estadoActualHumidificador = TiempoHMD;

//estados intermitentes
enum estadosDeLedVentilador {apagadoV,
                            prendidoV};
estadosDeLedVentilador intermitenteVentilador = prendidoV;

enum estadosDeLedExtractor {apagadoE,
                            prendidoE};
estadosDeLedExtractor intermitenteExtractor = prendidoE;

enum estadosDeLedHumidificador {apagadoH,
                                prendidoH};
estadosDeLedHumidificador intermitenteHumidificador = prendidoH;

enum estadosDeLedCalentador {apagadoC,
                             prendidoC};
estadosDeLedCalentador intermitenteCalentador = prendidoC;

//Variables de tiempo
const unsigned long tiempoMaximo = 2100000;  // Tiempo maximo de espera de 35 minutos = 2100000
unsigned short sistemaConFalla = 0;
unsigned long timerCLR = 0;
unsigned long timerEXT = 0;
unsigned long timerHMD = 0;
unsigned long espera = 0;
unsigned long tiempoParada = 0;
unsigned mutearAlarma = 0;

//timers leds
unsigned long IntermLVENT = 0;
unsigned long IntermLEXT = 0;
unsigned long IntermLHUMID = 0;
unsigned long IntermLCALENT = 0;

//Variables de control
bool FallaCLRBool = false;
bool FallaHMDBool = false;
bool botPresionado = LOW;
bool proteccionVentilador = false;
bool proteccionExtractor = false;

void setup() {
  //Entradas
  pinMode(pulsadorMarcha, INPUT);
  pinMode(pulsadorParada, INPUT);
  pinMode(pulsadorSilenciar, INPUT);
  pinMode(sensorTemperatura, INPUT);
  pinMode(sensorHumedad, INPUT);
  pinMode(ArreglarCalentador, INPUT);
  pinMode(ArreglarHumidificador, INPUT);
  pinMode(PTVN, INPUT);
  pinMode(PTEX, INPUT);

  //Salidas
  pinMode(ledFallaHumidificador, OUTPUT);
  pinMode(ledFallaCalentador, OUTPUT);
  pinMode(alarmaLed, OUTPUT);
  pinMode(ventilador, OUTPUT);
  pinMode(extractor, OUTPUT);
  pinMode(calentador, OUTPUT);
  pinMode(humidificador, OUTPUT);
  pinMode(alarmaSonora, OUTPUT);
  pinMode(LVENT, OUTPUT);
  pinMode(LEXT, OUTPUT);
  
  
  //Empiezan los actuadores apagados
  digitalWrite(alarmaLed, LOW);
  digitalWrite(ventilador, LOW);
  digitalWrite(extractor, LOW);
  digitalWrite(calentador, LOW);
  digitalWrite(humidificador, LOW);
}

//Funciones
int leerTemperatura() {
  return analogRead(sensorTemperatura);
}

int leerHumedad() {
  return analogRead(sensorHumedad);
}

void loop(){

  bool botn2 = digitalRead(pulsadorParada);

  if(digitalRead(pulsadorSilenciar) == HIGH){
    mutearAlarma = 1;
  }

  if(digitalRead(PTVN) == HIGH){
    proteccionVentilador = true;

    switch (intermitenteVentilador) {
      case prendidoV:
        digitalWrite(LVENT, HIGH);
        IntermLVENT = millis();
        intermitenteVentilador = apagadoV;
      break;

      case apagadoV:
        if(millis() - IntermLVENT >= 500){
          digitalWrite(LVENT, LOW);
        }

        if(millis() - IntermLVENT >= 1000){
          intermitenteVentilador = prendidoV;
        }
      break;
    }
  }
  else{
    proteccionVentilador = false;
    digitalWrite(LVENT, LOW);
  }

  if(digitalRead(PTEX) == HIGH){
    proteccionExtractor = true;
    switch(intermitenteExtractor){
      case prendidoE:
        digitalWrite(LEXT, HIGH);
        IntermLEXT = millis();
        intermitenteExtractor = apagadoE;
      break;

      case apagadoE:
        if(millis() - IntermLEXT >= 500){
          digitalWrite(LEXT, LOW);
        }

        if(millis() - IntermLEXT >= 1000){
          intermitenteExtractor = prendidoE;
        }      
      break;
    }
  }
  else{
    proteccionExtractor = false;
    digitalWrite(LEXT, LOW);
  }

  if(digitalRead(pulsadorParada) == HIGH && sistemaConFalla == 0){
    estadoActual = Parada;
  }

  if(digitalRead(ArreglarHumidificador) == HIGH && (millis() - espera >= 3000)){
    FallaHMDBool = false;
  }

  if(digitalRead(ArreglarCalentador) == HIGH && (millis() - espera >= 3000)){
    FallaCLRBool = false;
  }

  if (FallaCLRBool == true){
    switch(intermitenteCalentador){
      case prendidoC:
        digitalWrite(ledFallaCalentador, HIGH);
        IntermLCALENT = millis();
        intermitenteCalentador = apagadoC;
      break;

      case apagadoC:
        if(millis() - IntermLCALENT >= 500){
          digitalWrite(ledFallaCalentador, LOW);
        }

        if(millis() - IntermLCALENT >= 1000){
          intermitenteCalentador = prendidoC;
        }
      break;
    }
  }
  else if (FallaCLRBool == false) {
    digitalWrite(ledFallaCalentador, LOW);
  }

  if (FallaHMDBool == true){
    switch (intermitenteHumidificador){
      case prendidoH:
        IntermLHUMID = millis();
        digitalWrite(ledFallaHumidificador, HIGH);
        intermitenteHumidificador = apagadoH;
      break;

      case apagadoH:
        if(millis() - IntermLHUMID >= 500){
          digitalWrite(ledFallaHumidificador, LOW);
        }

        if(millis() - IntermLHUMID >= 1000){
          intermitenteHumidificador = prendidoH;
        }    
      break;
    }
  }
  else if (FallaHMDBool == false){
    digitalWrite(ledFallaHumidificador, LOW);
  }

  if(millis() - espera >= 3000){
    if (FallaCLRBool == true || FallaHMDBool == true){
      estadoActual = Parada;
      estadoParada = ConFalla;
      digitalWrite(alarmaLed, HIGH);
      sistemaConFalla = 1;
    }
  }

  if(proteccionVentilador == true || proteccionExtractor == true){
    estadoActual = Parada;
    estadoParada = ConFalla;
    digitalWrite(alarmaLed, HIGH);
  }

  switch(estadoActual){
    case Parada:
        digitalWrite(ventilador, LOW);
        digitalWrite(extractor, LOW);
        digitalWrite(calentador, LOW);
        digitalWrite(humidificador, LOW);
        estadoActualCalentador = TiempoCLR;
        estadoActualHumidificador = TiempoHMD;
        estadoActualExtractor = EjecucionEXT;

      switch (estadoParada) {
        case SinFalla:
          if(digitalRead(pulsadorMarcha) == HIGH){
            digitalWrite(ventilador, HIGH);
            estadoActual = Marcha;
          }
        break;

        case ConFalla:

          if(mutearAlarma == 0){
            digitalWrite(alarmaSonora, HIGH);
          } 
          else if(mutearAlarma == 1){
            digitalWrite(alarmaSonora, LOW);
          }

          switch (estadoFalla){
            case Puls1:
              if (botn2 == HIGH){
                estadoFalla = PulsMid;
              }
            break;

            case PulsMid:
              if (botn2 == LOW){
                estadoFalla = Puls2;
              }
            break;

            case Puls2:
              if (botn2 == HIGH){
                estadoActualCalentador = TiempoCLR;
                estadoActualHumidificador = TiempoHMD;
                estadoActualExtractor = EjecucionEXT;
                sistemaConFalla = 0;
                estadoParada = SinFalla;
                digitalWrite(alarmaLed, LOW);
                mutearAlarma = 0;
              }
            break;
          }
        break;
      }
    break;

    case Marcha:
      
      int temperatura = leerTemperatura();
      int humedad = leerHumedad();

      switch (estadoActualCalentador) {
        case TiempoCLR:
          timerCLR = millis();
          estadoActualCalentador = EjecucionCLR;
        break;
        
        case EjecucionCLR:
          if (temperatura <= grado_80 && (millis() - timerCLR <= 5000)) {
              digitalWrite(calentador, HIGH);
              timerCLR = millis();
              estadoActualCalentador = EjecucionCLR2;
          }
          else if ((millis() - timerCLR > 5000) && temperatura <= grado_80 && FallaCLRBool == false) { //Falla en prender
            FallaCLRBool = true;
            espera = millis();
          }
          else if (temperatura > grado_85 && (millis() - timerCLR > 5000) && FallaCLRBool == false){ //Falla en apagar
            FallaCLRBool = true;
            espera = millis();
          }
        break;

        case EjecucionCLR2:
          if (temperatura >= grado_83 && (millis() - timerCLR <= 5000)){
            digitalWrite(calentador, LOW);
            estadoActualCalentador = EjecucionCLR;
            timerCLR = millis();
          }
          else if ((millis() - timerCLR > 5000) && FallaCLRBool == false) { //Falla en apagar
            FallaCLRBool = true;
            espera = millis();
          }
        break;
      }

      switch (estadoActualHumidificador){
        case TiempoHMD:
          timerHMD = millis();
          estadoActualHumidificador = EjecucionHMD;
        break;

        case EjecucionHMD:
          if(humedad <= humedad_53 && (millis() - timerHMD <= 5000)){
            digitalWrite(humidificador, HIGH);
            timerHMD = millis();
            estadoActualHumidificador = EjecucionHMD2;
          }
          else if (millis() - timerHMD > 5000){ //Falla 
            FallaHMDBool = true;
          }
        break;

        case EjecucionHMD2:
          if(humedad >= humedad_60 && (millis() - timerHMD <= 5000)){
            digitalWrite(humidificador, LOW);
            estadoActualHumidificador = EjecucionHMD;
            timerHMD = millis();
          }
          else if (millis() - timerHMD > 5000){ //Falla 
            FallaHMDBool = true;
          }
        break;
      }

      switch (estadoActualExtractor) {
        
        case EjecucionEXT:
          if (temperatura >= grado_85) {
              digitalWrite(extractor, HIGH);
              estadoActualExtractor = EjecucionEXT2;
          }
        break;

        case EjecucionEXT2:
          if (temperatura <= grado_82){
            digitalWrite(extractor, LOW);
            estadoActualExtractor = EjecucionEXT;
          }
        break;
      }

    break;

  }

botPresionado = botn2;

}
