#include <Keypad.h>

#define ROWS 4
#define COLS 3
#define KEY_LENGTH 5

// Posição do algarismo
int Position = 0;

// Caracteres do teclado
char Keys[ROWS][COLS] =
{
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'},
    {'*','0','#'}  
};

// Pinos de saída das linhas do teclado
byte RowPins[ROWS] = {2,3,4,5};

// Pinos de saída das colunas do teclado
byte ColPins[COLS] = {6,7,8};

// Instância do objeto que representa o teclado
Keypad keypad = Keypad(makeKeymap(Keys), RowPins, ColPins, ROWS, COLS);

// Output do circuito
int BuzzerOutput = 13;

// Input do circuito
int TemperatureSensor = A0;

// Temperaturas
float TempMax = 40.0;
float TempMin = 0.0;
float TempCur = 20.0;

// Status do buzzer
int BuzzStatus = 0; // 0 livre, 1 ativado

// Executa o buzzer dados os parâmetros
void buzz(int targetPin) 
{
    if (BuzzStatus == 0)
    {
        BuzzStatus = 1;

        digitalWrite(targetPin, HIGH);
        delay(1000);
        digitalWrite(targetPin, LOW);
    
        BuzzStatus = 0;
    }
}

void setup() 
{
    pinMode(BuzzerOutput, OUTPUT);
}

float decodeChar(char c)
{
    switch (c)
    {
        case '0': return 0.0; break;
        case '1': return 1.0; break;
        case '2': return 2.0; break;
        case '3': return 3.0; break;
        case '4': return 4.0; break;
        case '5': return 5.0; break;
        case '6': return 6.0; break;
        case '7': return 7.0; break;
        case '8': return 8.0; break;
        case '9': return 9.0; break;
        default: return '\0';
    }
}

// Main loop do circuito
void loop() 
{
    // Captura a tecla pressionada
    char key = keypad.getKey();

    if (key == '*' || key == '#')
    {    
        Position = 0;
        TempCur = 20.0;
    }

    if (key != NO_KEY && key >= '0' && key <= '9')
    {
        if (Position == 0)
        {
            // Primeiro algarismo
            TempCur = decodeChar(key);
          	Position++;
        }
        else if (Position == 1)
        {
            // Segundo algarismo
            TempCur = TempCur * 10.0 + decodeChar(key);
          	Position = 0;
        }

    }

    if (TempCur > TempMax || TempCur < TempMin)
    {
        Position = 0;
        TempCur = 20.0;
    }

    float environmentTemperature = (float(analogRead(TemperatureSensor))*5/(1023))/0.01;

  	environmentTemperature = (environmentTemperature - 32) / 1.8;
  
    if (environmentTemperature >= TempCur)
    {
        buzz(BuzzerOutput);
    }

    delay(100);
}
