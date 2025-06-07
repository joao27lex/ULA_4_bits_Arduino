const int ledPins[5] = {8, 9, 10, 11, 12}; // D8-D11: bits do resultado, D12: carry out

String entrada = "";
uint8_t x = 0;
uint8_t y = 0;
uint8_t f = 0;
uint8_t resultado = 0;
bool carryOut = false;
int estado = 0; // 0 = F, 1 = X, 2 = Y

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 5; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  Serial.println("===== ULA 4 BITS (Entrada binária via Serial) =====");
  Serial.println("Digite a função F (3 bits binários, ex: 011):");
}

uint8_t binStringToInt(String bin) {
  uint8_t val = 0;
  for (int i = 0; i < bin.length(); i++) {
    val = (val << 1) | (bin.charAt(i) - '0');
  }
  return val;
}

void loop() {
  if (Serial.available()) {
    entrada = Serial.readStringUntil('\n');
    entrada.trim();

    if (estado == 0) {
      if (entrada.length() != 3) {
        Serial.println("Erro: F deve ter 3 bits. Tente novamente:");
        return;
      }
      f = binStringToInt(entrada);
      estado = 1;
      Serial.println("Agora digite o valor de X (4 bits binários, ex: 0101):");
    }

    else if (estado == 1) {
      if (entrada.length() != 4) {
        Serial.println("Erro: X deve ter 4 bits. Tente novamente:");
        return;
      }
      x = binStringToInt(entrada);
      estado = 2;
      Serial.println("Agora digite o valor de Y (4 bits binários, ex: 0011):");
    }

    else if (estado == 2) {
      if (entrada.length() != 4) {
        Serial.println("Erro: Y deve ter 4 bits. Tente novamente:");
        return;
      }
      y = binStringToInt(entrada);
      carryOut = false;
      resultado = 0;

      switch (f) {
        case 0b000:
          resultado = x & y;
          break;

        case 0b001:
          resultado = x | y;
          break;

        case 0b010:
          resultado = ~y & 0x0F;
          break;

        case 0b011: {
          int soma = x + y;
          resultado = soma & 0x0F;
          carryOut = soma > 15;
          break;
        }
        
        case 0b100: {
          int sub = x - y;
          resultado = sub & 0x0F;
          carryOut = sub < 0;
          break;
        }

        case 0b101: {
          int mult = x * y;
          resultado = mult & 0x0F;
          carryOut = mult > 15;
          break;
        }

        case 0b110:
          if (y == 0) {
            Serial.println("Erro: divisão por zero");
            estado = 0;
            Serial.println("Digite a função F (3 bits binários, ex: 011):");
            return;
          }
          resultado = x / y;
          break;

        default:
          Serial.println("Função inválida.");
          estado = 0;
          Serial.println("Digite a função F (3 bits binários, ex: 011):");
          return;
      }

      // Atualizar LEDs
      for (int i = 0; i < 4; i++) {
        digitalWrite(ledPins[i], (resultado >> i) & 1);
      }
      digitalWrite(ledPins[4], carryOut);

      // Atualizar Terminal
      Serial.println("----- RESULTADO -----");
      Serial.print("Função (F): "); Serial.println(f, BIN);
      Serial.print("X = "); Serial.println(x, BIN);
      Serial.print("Y = "); Serial.println(y, BIN);
      Serial.print("Resultado (bin) = ");
      for (int i = 3; i >= 0; i--) {
        Serial.print((resultado >> i) & 1);
      }
      Serial.println();
      Serial.print("Carry Out = "); Serial.println(carryOut ? "1" : "0");
      Serial.println("---------------------");

      // Reiniciar
      estado = 0;
      Serial.println("Digite a próxima função F (3 bits binários):");
    }
  }
}
