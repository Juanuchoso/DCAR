
// Definición de pines del L298N
const int motorPin1 = 6; // IN1
const int motorPin2 = 7; // IN2
const int enablePin = 5; // ENA (para controlar la velocidad con PWM)
const int conversor=26;
// Variables para almacenar los datos recibidos y tiempos
String receivedData1 = "";
String receivedData2 = "";
unsigned long tiempoEncendido = 0;        // Tiempo que el motor estará encendido (en milisegundos)
unsigned long tiempoEspera = 0;           // Tiempo de espera entre ciclos (en milisegundos)
unsigned long tiempoAnterior = 0; // Almacena el último tiempo de cambio
bool motorEncendido = false;     // Estado del motor
bool datosRecibidos = false;     // Estado de recepción de datos

void setup() {
  // Inicialización de los pines del motor
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);

  // Inicialización de la comunicación serial
  Serial.begin(9600);
}

void loop() {
  // Verificar si hay datos disponibles en el puerto serial
  if (Serial.available() > 0) {
    receivedData1 = Serial.readStringUntil('\n'); 
    delay(10);
    receivedData2 = Serial.readStringUntil('\n'); 

    tiempoEncendido = receivedData1.toInt()*conversor;      
    tiempoEspera = receivedData2.toInt()*60000;    


    Serial.print(analogRead(enablePin));

    datosRecibidos = true;
    motorEncendido = true;
    tiempoAnterior = millis();
    startMotor(); 
  }

  if (datosRecibidos) {
    unsigned long tiempoActual = millis();
    Serial.print("Tiempo actual: ");
    Serial.println(tiempoActual);
    Serial.print("Tiempo anterior: ");
    Serial.println(tiempoAnterior);

    if (motorEncendido && (tiempoActual - tiempoAnterior >= tiempoEncendido)) {
      stopMotor();
      motorEncendido = false;
      tiempoAnterior = millis();
    } else if (!motorEncendido && (tiempoActual - tiempoAnterior >= tiempoEspera)) {
      startMotor();
      motorEncendido = true;
      tiempoAnterior = millis();
    }
  }
}


void startMotor() {
  digitalWrite(motorPin1, HIGH); // Establece la dirección
  digitalWrite(motorPin2, LOW);
  analogWrite(enablePin, 200);   // Configura la velocidad al máximo (PWM)
  Serial.println("Motor activado");
}

void stopMotor() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  analogWrite(enablePin, 0);     // Detiene el motor
  Serial.println("Motor detenido");
}
