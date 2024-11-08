#include <Key.h>
#include <Keypad.h>

// Configuración del teclado 4x4
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; // Pines de las filas
byte colPins[COLS] = {5, 4, 3, 2}; // Pines de las columnas

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);
// Variables que serán ingresadas
String variable1 = "";
String variable2 = "";

// Estados del menú
enum MenuState {
  MENU_MAIN,
  INSERT_VAR1,
  INSERT_VAR2,
  SEND_DATA
};

MenuState currentState = MENU_MAIN;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.backlight();
  showMainMenu();
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    switch (currentState) {
      case MENU_MAIN:
        handleMainMenu(key);
        break;
      case INSERT_VAR1:
        handleInsertVar1(key);
        break;
      case INSERT_VAR2:
        handleInsertVar2(key);
        break;
      case SEND_DATA:
        sendData();
        break;
    }
  }
}

void showMainMenu() {
  lcd.clear();
  lcd.print("1:Peso");
  lcd.setCursor(0, 1);
  lcd.print("2:Tiempo3:Enviar");
  currentState = MENU_MAIN;
}

void handleMainMenu(char key) {
  switch (key) {
    case '1':
      lcd.clear();
      lcd.print("Insertar Peso[gr]:");
      variable1 = "";
      currentState = INSERT_VAR1;
      break;
    case '2':
      lcd.clear();
      lcd.print("Insertar Tiempo [min]:");
      variable2 = "";
      currentState = INSERT_VAR2;
      break;
    case '3':
      currentState = SEND_DATA;
      break;
    default:
      // Si se presiona una tecla inválida
      lcd.setCursor(0, 1);
      lcd.print("Opcion invalida");
      delay(1000);
      showMainMenu();
      break;
  }
}

void handleInsertVar1(char key) {
  if (key == '#') {
    // Completar la entrada y volver al menú principal
    showMainMenu();
  } else if (key != '*') {
    variable1 += key;
    lcd.setCursor(0, 1);
    lcd.print(variable1);
  }
}

void handleInsertVar2(char key) {
  if (key == '#') {
    // Completar la entrada y volver al menú principal
    showMainMenu();
  } else if (key != '*') {
    variable2 += key;
    lcd.setCursor(0, 1);
    lcd.print(variable2);
  }
}

void sendData() {
  lcd.clear();
  lcd.print("Enviando datos...");
  Serial.println(variable1);
  Serial.println(variable2);
  delay(2000);
  showMainMenu();  // Volver al menú principal
}