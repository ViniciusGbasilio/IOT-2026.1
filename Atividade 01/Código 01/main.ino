const int LED1 = 4;
const int LED2 = 5;
const int LED3 = 6;
const int LED4 = 7;

void setup() {

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

}

void loop() {

  
  for(int contador = 0; contador < 16; contador++) {

    digitalWrite(LED1, contador % 2);
    digitalWrite(LED2, (contador / 2) % 2);
    digitalWrite(LED3, (contador / 4) % 2);
    digitalWrite(LED4, (contador / 8) % 2);

    delay(500);
  }


  digitalWrite(LED1, HIGH); delay(400); digitalWrite(LED1, LOW);
  digitalWrite(LED2, HIGH); delay(400); digitalWrite(LED2, LOW);
  digitalWrite(LED3, HIGH); delay(400); digitalWrite(LED3, LOW);
  digitalWrite(LED4, HIGH); delay(400); digitalWrite(LED4, LOW);


  digitalWrite(LED4, HIGH); delay(400); digitalWrite(LED4, LOW);
  digitalWrite(LED3, HIGH); delay(400); digitalWrite(LED3, LOW);
  digitalWrite(LED2, HIGH); delay(400); digitalWrite(LED2, LOW);
  digitalWrite(LED1, HIGH); delay(400); digitalWrite(LED1, LOW);

}