

const int Motor1 = 6; 
const int Motor2 = 7; 
const int ENA = 5; 
const int conversor=26;
// Variables para almacenar los datos recibidos y tiempos
String receivedData1 = "";
String receivedData2 = "";
unsigned long tiempoON = 0;        // Tiempo que el motor estará encendido (en milisegundos)
unsigned long tiempoWAIT = 0;           // Tiempo de espera entre ciclos (en milisegundos)
unsigned long tiempoBEFORE = 0; // Almacena el último tiempo de cambio
bool motorEncendido = false;     // Estado del motor
bool datosRecibidos = false;     // Estado de recepción de datos

void setup() {
  // Inicialización de los pines del motor
  pinMode(Motor1, OUTPUT);
  pinMode(Motor2, OUTPUT);
  pinMode(ENA, OUTPUT);

  // Inicialización de la comunicación serial
  Serial.begin(9600);
}

void loop() {
  // Verificar si hay datos disponibles en el puerto serial
  if (Serial.available() > 0) {
    receivedData1 = Serial.readStringUntil('\n'); 
    delay(10);
    receivedData2 = Serial.readStringUntil('\n'); 

    tiempoON = receivedData1.toInt()*conversor;      
    tiempoWAIT = receivedData2.toInt()*60000;    


    Serial.print(analogRead(ENA));

    datosRecibidos = true;
    motorEncendido = true;
    tiempoBEFORE = millis();
    startMotor(); 
  }

  if (datosRecibidos) {
    unsigned long tiempoActual = millis();
    Serial.print("Tiempo actual: ");
    Serial.println(tiempoActual);
    Serial.print("Tiempo anterior: ");
    Serial.println(tiempoBEFORE);

    if (motorEncendido && (tiempoActual - tiempoBEFORE >= tiempoON)) {
      stopMotor();
      motorEncendido = false;
      tiempoBEFORE = millis();
    } else if (!motorEncendido && (tiempoActual - tiempoBEFORE >= tiempoWAIT)) {
      startMotor();
      motorEncendido = true;
      tiempoBEFORE = millis();
    }
  }
}


void startMotor() {
  digitalWrite(Motor1, HIGH); // Establece la dirección
  digitalWrite(Motor2, LOW);
  analogWrite(ENA, 200);   // Configura la velocidad al máximo (PWM)
  Serial.println("Motor activado");
}

void stopMotor() {
  digitalWrite(Motor1, LOW);
  digitalWrite(Motor2, LOW);
  analogWrite(ENA, 0);     // Detiene el motor
  Serial.println("Motor desactivado");
}
