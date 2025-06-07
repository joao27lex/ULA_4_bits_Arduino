# 🔢 ULA de 4 Bits com Arduino

Este projeto implementa uma Unidade Lógica e Aritmética (ULA) de 4 bits usando um microcontrolador Arduino. A ULA realiza operações lógicas e aritméticas sobre dois operandos binários de 4 bits (`X` e `Y`), com a operação selecionada por 3 bits de função (`F`). Os resultados são exibidos através de LEDs conectados aos pinos digitais do Arduino, simulando o comportamento real de uma ULA em hardware digital.

---

## 🎯 Funcionalidade

- Entradas `X`, `Y` e `F` são digitadas via **Serial Monitor** (em binário).
- O resultado da operação é exibido em **LEDs** conectados aos pinos digitais D8 a D12.
- A ULA suporta 7 operações selecionadas por 3 bits de controle (`F`).

---

## 🛠️ Operações suportadas

| F (função) | Operação        | Descrição                    |
|------------|------------------|------------------------------|
| `000`      | AND              | Bit a bit entre X e Y        |
| `001`      | OR               | Bit a bit entre X e Y        |
| `010`      | NOT Y            | Negação bit a bit de Y       |
| `011`      | Soma             | Soma de X e Y (carry detectado) |
| `100`      | Subtração        | X menos Y (carry detectado)  |
| `101`      | Multiplicação    | X vezes Y (carry detectado)  |
| `110`      | Divisão inteira  | X dividido por Y             |

---

## 🔌 Hardware utilizado

- 1 placa **Arduino Uno** ou similar
- 5 LEDs (para representar os bits do resultado e o carry-out)
- 5 resistores (220Ω ou similares)
- Protoboard e jumpers

### Mapeamento dos LEDs

| Pino Digital | Representa     |
|--------------|----------------|
| D8           | Bit 0 (LSB)     |
| D9           | Bit 1           |
| D10          | Bit 2           |
| D11          | Bit 3 (MSB)     |
| D12          | Carry Out       |

---

## 🧪 Como usar

1. Faça upload do código `.ino` para o Arduino.
2. Abra o **Monitor Serial** na IDE do Arduino com baud rate de `9600`.
3. Siga as instruções exibidas no terminal:
   - Digite 3 bits binários representando a operação (`F`).
   - Digite 4 bits binários para o operando `X`.
   - Digite 4 bits binários para o operando `Y`.
4. O resultado será:
   - Exibido no **terminal serial**, em binário.
   - Mostrado nos **LEDs** conectados aos pinos digitais (bits 0 a 3 + carry-out).

---

## 📋 Exemplo de uso

**Entrada digitada no Serial Monitor:**
- 011
- 0101
- 0011


**Saída exibida no terminal:**
- Função (F): 11
- X: 101
- Y: 11
- Resultado: 1000
- Carry Out: 0


Neste exemplo:
- A função `011` representa soma.
- `0101` (5) + `0011` (3) = `1000` (8).
- O LED do bit 3 acende, e os LEDs dos bits 2 a 0 permanecem apagados.
- Carry-out = 0 → LED D12 apagado.

---

## 🧠 Sobre o código

A entrada é controlada por uma variável `estado` que define qual dado o Arduino está esperando:
- `estado == 0`: recebe `F` (função)
- `estado == 1`: recebe `X`
- `estado == 2`: recebe `Y` e então executa a operação

A função `binStringToInt()` converte a entrada binária (em string) para inteiro, usando operações de bit shifting.

O resultado da operação é mascarado com `& 0x0F` para garantir que apenas os 4 bits menos significativos sejam usados (como em uma ULA real de 4 bits). O bit de **carry out** é ativado se houver overflow (por exemplo, na soma ou multiplicação).

