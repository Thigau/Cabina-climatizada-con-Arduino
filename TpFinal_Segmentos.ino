// Pines analógicos
#define sensorTemp A5
#define sensorHum A4

// Pines digitales para los segmentos
#define pinCero 0
#define pinUno 1
#define pinDos 2
#define pinTres 3
#define pinCuatro 4
#define pinCinco 5
#define pinSeis 6
#define pinSiete 7


// Pines para los cátodos
#define catodo_10_hum 8
#define catodo_1_hum 9
#define catodo_10_temp 10
#define catodo_1_temp 11

// Segmentos (a-g y DP)
const int pins[] = {pinCero, pinUno, pinDos, pinTres, pinCuatro, pinCinco, pinSeis, pinSiete};

// Tabla de segmentos para los números 0-9
const char n[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

// Variables globales
unsigned long previousMillis = 0; // Marca de tiempo anterior
const unsigned long interval = 5; // Intervalo para multiplexado (ms)
int currentDigit = 0; // Índice del dígito activo (0 a 3)

unsigned proteccionTermica = 0;

bool FallaVENTBool = false;
bool FallaEXTBool = false;

void setup() {
  // Configurar pines analógicos como entrada
  pinMode(sensorTemp, INPUT);
  pinMode(sensorHum, INPUT);

  // Configurar pines de segmentos como salida
  for (int i = 0; i < 8; i++) {
    pinMode(pins[i], OUTPUT);
  }

  // Configurar pines de cátodos como salida
  pinMode(catodo_10_hum, OUTPUT);
  pinMode(catodo_1_hum, OUTPUT);
  pinMode(catodo_10_temp, OUTPUT);
  pinMode(catodo_1_temp, OUTPUT);
}

void loop() {
  // Leer y ajustar valores de los sensores
  float temp = 0.487 * analogRead(sensorTemp) + 0.3; // Fórmula para temperatura
  float hum = 0.098 * analogRead(sensorHum) - 0.14;  // Fórmula para humedad

  // Convertir a enteros (solo dos dígitos)
  int tempInt = constrain((int)temp, 0, 99); // Asegurarse de que esté entre 0-99
  int humInt = constrain((int)hum, 0, 99);

  // Obtener los dígitos individuales
  int digits[] = {
    tempInt / 10,  // Decenas de temperatura
    tempInt % 10,  // Unidades de temperatura
    humInt / 10,   // Decenas de humedad
    humInt % 10    // Unidades de humedad
  };

  // Multiplexar los dígitos utilizando millis()
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Mostrar el siguiente dígito
    mostrarNumero(digits[currentDigit], currentDigit);

    // Cambiar al siguiente dígito
    currentDigit = (currentDigit + 1) % 4;
  }
}

// Función para mostrar un dígito en un cátodo específico
void mostrarNumero(int numero, int digitIndex) {
  // Apagar todos los cátodos antes de cambiar
  digitalWrite(catodo_10_hum, LOW);
  digitalWrite(catodo_1_hum, LOW);
  digitalWrite(catodo_10_temp, LOW);
  digitalWrite(catodo_1_temp, LOW);

  // Encender los segmentos correspondientes al número
  for (int i = 0; i < 8; i++) {
    digitalWrite(pins[i], (n[numero] >> i) & 0x01);
  }

  // Encender el cátodo correspondiente al dígito actual
  switch (digitIndex) {
    case 0: digitalWrite(catodo_10_temp, HIGH); break;
    case 1: digitalWrite(catodo_1_temp, HIGH); break;
    case 2: digitalWrite(catodo_10_hum, HIGH); break;
    case 3: digitalWrite(catodo_1_hum, HIGH); break;
  }
}
