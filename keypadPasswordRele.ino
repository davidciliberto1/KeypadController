#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x3F, 16, 4);
// variables de botones
char tecla;
char clave[5];
char claveMaestra[5] = "5555"; // Set contraseña
byte indice = 0;
int menu = 0;
int ledRojo = 6;
int ledVerde = 7;
int rele = 14;
const byte ROWS = 4; // 4 filas
const byte COLS = 4; // 3 columnas
char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};
byte rowPins[ROWS] = {16, 17, 18, 19}; //conecta pines filas al keypad
byte colPins[COLS] = {2, 3, 4, 5};     //conecta pines colunmas al keypadd

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup()
{
  pinMode(rele, OUTPUT);    // pines
  pinMode(ledRojo, OUTPUT); // pines
  pinMode(ledVerde, OUTPUT);
  digitalWrite(rele, LOW);    // apaga pines
  digitalWrite(ledRojo, LOW); // apaga pines
  digitalWrite(ledVerde, LOW);
  Serial.begin(9600);
  lcd.begin();
  while (!Serial)
    ;
  Serial.println("blawdehlwefhj"); //inicia display
  Wire.begin();
  lcd.setCursor(0, 2); // imprime clave en display
  lcd.print("ERDGE");
  displayCodigoEntrada(); // pantalla entrada
  updateMenu();           // inicia menu
}

void loop()
{

  tecla = keypad.getKey(); // tecla
  if (tecla)
  {
    clave[indice] = tecla; // ancla tecla a indice
    indice++;              // corre el indice un digito
    Serial.println(tecla);
    lcd.setCursor(indice, 2); // imprime clave en display
    lcd.print(tecla);
  }
  if (!digitalRead(tecla))
  { // manejo menu
    menu == tecla;
    updateMenu();
  }
  if (indice == 4)
  { // tope de input
    if (!strcmp(clave, claveMaestra))
    { //compara clave ingresada con clave maestra
      Serial.println("correcta");
      displayCorrecto();            // devuelve display correcto
      digitalWrite(ledVerde, HIGH); //  enciende LED de verificacion correcto
      digitalWrite(rele, HIGH);     // conecta Relay
      delay(1500);                  // espera 2.5 segundos
      digitalWrite(ledVerde, LOW);  // apaga LED de verificacion
      displayCodigoEntrada();       // vuelve a display de entrada
      indice = 0;                   //
    }
    else
    { // devuelve display incorrecto
      Serial.println("incorrecta");
      displayIncorrecto();
      digitalWrite(ledRojo, HIGH);
      digitalWrite(rele, LOW);

      delay(1500);
      digitalWrite(ledRojo, LOW);
      displayCodigoEntrada();
      indice = 0;
    }
  }
}

void displayCodigoEntrada()
{
  lcd.clear();
  lcd.setCursor(0, 0); // display pagina entrada
  lcd.print("Introduzca clave");
  lcd.setCursor(0, 1);
  lcd.print("Clave: ****");
}
void displayCorrecto()
{
  lcd.clear();
  lcd.setCursor(0, 0); // display contraseña correcta
  lcd.print("****************");
  lcd.setCursor(0, 1);
  lcd.print("    Correcto!   ");
  lcd.setCursor(0, 2);
  lcd.print(" Codigo Valido! ");
  lcd.setCursor(0, 3);
  lcd.print("****************");
}
void displayIncorrecto()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("----------------");
  lcd.setCursor(0, 1); // display contraseña incorrecta
  lcd.print("   Incorrecto!  ");
  lcd.setCursor(0, 2);
  lcd.print("Codigo Invalido!");
  lcd.setCursor(0, 3);
  lcd.print("----------------");
}
