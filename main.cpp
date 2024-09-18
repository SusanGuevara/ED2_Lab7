/* 
Universidad Del Valle de Guatemala
Facultad de Ingeniería
Departamento de Electrónica, Mecatrónica y Biomédica
Electrónica Digital 2

Susan Daniela Guevara Catalán - 22519

Laboratorio No. 7 - UART

Parte 1: Función que enciende un LED y lo mantiene encendido por cierto tiempo.
Parte 2: Muestra en la terminal serial un menú con 2 opciones:
                    1. Lectura ADC
                    2. Controlar LEDs
         La opción “Lectura ADC” la escoge el usurio ingresando un “1” en la terminal serial y la opción “Controlar LEDs” ingresando un “2”. 
         Si el usuario ingresa una opción diferente, se muestra un mensaje de error (Opcion invalida) y vuelve a desplegar el menú.
Parte 3: Opción 1 (Lectura ADC): Realiza una lectura del canal analógico y despliega el valor de voltaje obtenido.
Parte 4: Opción 2 (Controlar LEDs): Muestra un mensaje solicitando el LED a encender y el tiempo en milisegundos que debe permanecer encendido. 
         El usuario debe indicar esos parámetros separados por coma y sin espacios (Ej: “1,500” -> encender LED1 por 500ms).
          - Si el usuario ingresa un valor de LED inválido o si el tiempo no es un número, muestra un mensaje de error y solicitar el comando nuevamente.
          - Luego de interpretar el comando, llama a la función de la parte 1 y enciende el LED.
         Al finalizar el tiempo, vuelva a desplegar el menú.
*/

/************************ Importación de librerías *******************************/
#include <Arduino.h>

/************************ Declaración de variables *******************************/
#define Pot 34 // Pin del potenciometro
// Pin de las LEDs
#define Led1 13 // Pin del LED verde
#define Led2 14 // Pin del LED amarillo
#define Led3 26 // Pin del LED rojo
int opcion; // Variable en la que el usuario escoge la opción del menú
float valorAnalogico, voltaje; // Guardan el valor medido del potenciometro.
String entrada; // Guarda el valor del indicador y el tiempo para la opción 2.
int comaIndex; // Guarda la posición de la coma, la cual separa el valor del indicador y del tiempo.
int led; // Guarda el número de LED que el usuario desea encender.
String tiempoStr; // Guarda el tiempo como string para validar si es numérico.
int tiempo; // Guarda el tiempo en ms que el usuario desea que el LED permanezca encendido. 

/************************ Prototipo de funciones *******************************/
// Función que enciende las LEDs por un tiempo determinado. 
void EncenderLED(int indicador, int tiempo);
/************************ Configuración de pines *******************************/
void setup(){
  // Empieza la conexión serial
  Serial.begin(115200);
  // Definición de las leds como salidas
  pinMode(Led1, OUTPUT);
  pinMode(Led2, OUTPUT);
  pinMode(Led3, OUTPUT);
}

void loop(){
  bool entradaValida = false;  // Variable para comprobar la validez de la entrada.
  bool esNumero = true; // Variable que valida que el tiempo es un numero.

  // Menú desplegable en el monitor serial
  Serial.println("\nEscriba el número de una opción del menú");
  Serial.println("1. Lectura ADC");
  Serial.println("2. Controlar LEDs");

  // Esperar hasta que el usuario ingrese algo en el monitor serial
  while (Serial.available() == 0) {
    // Espera a que haya datos disponibles en el puerto serial
  }

  // Lee la opción que el usuario escogió
  opcion = Serial.parseInt();  // Lee un número entero desde el puerto serial

  // Evaluación de la opción ingresada
  switch (opcion) {
    case 1:
      Serial.println("Opción 1 seleccionada: Lectura ADC");
      valorAnalogico = analogRead(Pot);
      voltaje = (valorAnalogico / 4095.0) * 3.3; // Suponiendo Vref = 3.3V y resolución de 12 bits
      Serial.print("Voltaje = ");
      Serial.println(voltaje);
      break;
    case 2:
      while (!entradaValida) {  // Repetir hasta que la entrada sea válida
        Serial.println("Opción 2 seleccionada: Controlar LEDs");
        Serial.println("Ingrese el número de LED y el tiempo en ms separados por coma (Ej: 1,500):");

        // Esperar hasta que el usuario ingrese el formato "LED,Tiempo"
        while (Serial.available() == 0) {
          // Espera a que haya datos disponibles en el puerto serial
        }

        // Leer la cadena completa hasta que encuentre un salto de línea
        entrada = Serial.readStringUntil('\n');

        // Encontrar la posición de la coma
        comaIndex = entrada.indexOf(',');

        // Verificar si la coma existe
        if (comaIndex == -1) {
          Serial.println("Formato inválido. Asegúrese de usar el formato correcto (Ej: 1,500).");
          continue;  // Pedir entrada nuevamente
        }

        // Extraer el número del LED (antes de la coma) y el tiempo (después de la coma)
        led = entrada.substring(0, comaIndex).toInt();
        tiempoStr = entrada.substring(comaIndex + 1);

        // Validar que el tiempo no contenga letras (solo dígitos)
        esNumero = true;
        for (int i = 0; i < tiempoStr.length(); i++) {
          if (!isDigit(tiempoStr.charAt(i))) {
            esNumero = false;
            break;
          }
        }

        if (!esNumero) {
          Serial.println("Error: El tiempo debe ser un número válido.");
          continue;  // Pedir entrada nuevamente si no es un número válido
        }

        // Convertir el tiempo a entero
        tiempo = tiempoStr.toInt();

        if (led < 1 || led > 3) {
          Serial.println("Error: LED inválido. Ingrese un número de LED entre 1 y 3.");
        } else if (tiempo <= 0) {
          Serial.println("Error: El tiempo debe ser un número mayor a 0.");
        } else {
          EncenderLED(led, tiempo);
          entradaValida = true;
        }
      }
      break;
    default:
      Serial.println("Opción inválida, por favor ingrese una opción válida.");
      break;
  }
}

/************************ Definición de funciones *******************************/

void EncenderLED(int indicador, int tiempo){
  switch (indicador) {
    case 1:
      digitalWrite(Led1, HIGH);
      vTaskDelay(tiempo / portTICK_PERIOD_MS);
      digitalWrite(Led1, LOW);
      break;
    case 2:
      digitalWrite(Led2, HIGH);
      vTaskDelay(tiempo / portTICK_PERIOD_MS);
      digitalWrite(Led2, LOW);
      break;
    case 3:
      digitalWrite(Led3, HIGH);
      vTaskDelay(tiempo / portTICK_PERIOD_MS);
      digitalWrite(Led3, LOW);
      break;
  }
}
