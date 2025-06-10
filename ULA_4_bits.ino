const int ledPins[5] = {8, 9, 10, 11, 12}; // D8-D11: saídas dos 4 bits do resultado, D12: carry out (se houve estouro)

String entrada = "";       // armazena temporariamente o valor digitado no terminal serial
uint8_t f = 0;             // função (F), 3 bits que determinam a operação da ULA
uint8_t x = 0;             // valor de entrada X (4 bits)
uint8_t y = 0;             // valor de entrada Y (4 bits)
uint8_t resultado = 0;     // resultado da operação
bool carryOut = false;     // indica se houve carry em soma ou multiplicação
int estado = 0;            // controla o fluxo: 0 = esperando F, 1 = esperando X, 2 = esperando Y

void setup() {
  Serial.begin(9600); // inicia comunicação serial a 9600 bps

  // configura os pinos dos LEDs como saída
  for (int i = 0; i < 5; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

  // mensagens iniciais no terminal
  Serial.println("===== ULA 4 BITS (Entrada binária via Serial) =====");
  exibirOpcoes(); // mostra as opções de operações disponíveis
}

// função auxiliar que converte uma string binária para um número inteiro
uint8_t binStringToInt(String bin) {
  uint8_t val = 0;
  for (int i = 0; i < bin.length(); i++) {
    val = (val << 1) | (bin.charAt(i) - '0'); // desloca à esquerda e adiciona novo bit
  }
  return val;
}

// função que exibe as operações disponíveis
void exibirOpcoes() {
  Serial.println("Digite a função F (3 bits binários). Opções:");
  Serial.println("000 -> AND");
  Serial.println("001 -> OR");
  Serial.println("010 -> NOT Y");
  Serial.println("011 -> XOR");
  Serial.println("100 -> Soma");
  Serial.println("101 -> Subtração");
  Serial.println("110 -> Multiplicação");
  Serial.println("111 -> Divisão");
}

void loop() {
  if (Serial.available()) {
    entrada = Serial.readStringUntil('\n');
    entrada.trim();

    // =====================
    // ESTADO 0: Leitura da função (F)
    // =====================
    if (estado == 0) {
      if (entrada.length() != 3) {
        Serial.println("Erro: F deve ter 3 bits. Tente novamente:");
        return;
      }
      f = binStringToInt(entrada);

      estado = 1;
      Serial.println("Agora digite o valor de X (4 bits binários, ex: 0101):");
    }

    // =====================
    // ESTADO 1: Leitura de X
    // =====================
    else if (estado == 1) {
      if (entrada.length() != 4) {
        Serial.println("Erro: X deve ter 4 bits. Tente novamente:");
        return;
      }
      x = binStringToInt(entrada);
      estado = 2;
      Serial.println("Agora digite o valor de Y (4 bits binários, ex: 0011):");
    }

    // =====================
    // ESTADO 2: Leitura de Y e execução da operação
    // =====================
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
          Serial.println("Você escolheu a operação AND");
          resultado = x & y;
          break;

        case 0b001:
          Serial.println("Você escolheu a operação OR");
          resultado = x | y;
          break;

        case 0b010:
          Serial.println("Você escolheu a operação NOT Y");
          resultado = ~y & 0x0F;
          break;

        case 0b011:
          Serial.println("Você escolheu a operação XOR");
          resultado = x ^ y;
          break;

        case 0b100: {
          Serial.println("Você escolheu a operação SOMA");
          int soma = x + y;
          resultado = soma & 0x0F;
          carryOut = soma > 15;
          break;
        }

        case 0b101: {
          Serial.println("Você escolheu a operação SUBTRAÇÃO");
          int sub = x - y;
          resultado = sub & 0x0F;
          break;
        }

        case 0b110: {
          Serial.println("Você escolheu a operação MULTIPLICAÇÃO");
          int mult = x * y;
          resultado = mult & 0x0F;
          carryOut = mult > 15;
          break;
        }

        case 0b111:
          if (y == 0) {
            Serial.println("Erro: divisão por zero");
            estado = 0;
            exibirOpcoes();
            return;
          }
          Serial.println("Você escolheu a operação DIVISÃO");
          resultado = x / y;
          break;

        default:
          Serial.println("Função inválida.");
          estado = 0;
          exibirOpcoes();
          return;
      }


      // Atualiza LEDs
      for (int i = 0; i < 4; i++) {
        digitalWrite(ledPins[i], (resultado >> i) & 1);
      }
      digitalWrite(ledPins[4], carryOut);

      // Exibe resultado no terminal
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

      // Reinicia fluxo
      estado = 0;
      exibirOpcoes();
    }
  }
}
