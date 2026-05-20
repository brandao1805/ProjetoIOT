# Relatório Técnico — Sistema de Controle de Acesso para Cofre Inteligente

**Disciplina:** CCM520 — Internet das Coisas  
**Projeto:** Locker Inteligente com Feedback Multissensorial  
**Plataforma:** Arduino Uno  

---

## 1. Objetivo

O projeto tem como objetivo projetar e implementar um sistema embarcado de controle de acesso para um compartimento seguro, simulando um cofre inteligente.

O sistema deve ser capaz de gerenciar a entrada de uma senha pelo usuário, validar o acesso, acionar uma trava física representada por servomotores e monitorar condições de segurança do ambiente por meio de sensores.

Além disso, o projeto busca integrar múltiplos periféricos ao Arduino Uno, como display LCD, botões, potenciômetro, LEDs, buzzer, sensor LDR e servomotores, utilizando lógica de estados e interrupção externa para reset/emergência.

---

## 2. Descrição Geral do Sistema

O sistema foi desenvolvido em linguagem C/C++ para Arduino e simula o funcionamento de um cofre eletrônico.

A senha é inserida por meio de um potenciômetro. O usuário gira o potenciômetro para escolher cada número e confirma o dígito utilizando um botão. Após a inserção dos três dígitos, o sistema compara a senha digitada com a senha correta armazenada no código.

A senha configurada no projeto é:

```txt
253
```

Caso a senha esteja correta, o sistema libera o acesso, acende o LED verde e movimenta os servomotores para abrir o cofre. Caso a senha esteja incorreta, o LED vermelho acende, o buzzer toca e o sistema contabiliza uma tentativa errada.

Após três tentativas incorretas, o sistema entra em estado de bloqueio temporário.

O projeto também conta com um sensor LDR para monitoramento de luz. Se o cofre estiver fechado e o sensor detectar luminosidade indevida, o sistema aciona um alerta com LED vermelho, buzzer e mensagem no LCD.

---

## 3. Recursos de Hardware

### 3.1 Microcontrolador

Foi utilizado o **Arduino Uno**, responsável por controlar todas as entradas, saídas, sensores e atuadores do projeto.

---

### 3.2 Periféricos Utilizados

| Periférico | Pino(s) | Função no Projeto |
|---|---|---|
| LCD 16x2 | 7, 6, 5, 4, A2, A3 | Interface visual para orientar o usuário |
| Potenciômetro | A0 | Entrada analógica para seleção dos números da senha |
| LDR | A1 | Sensor de luminosidade para detecção de abertura indevida |
| Botão Confirmar | D13 | Confirma o início e cada dígito da senha |
| Botão Fechar | D12 | Fecha o cofre após abertura autorizada |
| Botão Reset | D2 | Reset/emergência por interrupção externa |
| Buzzer | D10 | Feedback sonoro de erro, bloqueio e alerta |
| LED Verde | D8 | Indica acesso liberado/cofre aberto |
| LED Vermelho | D9 | Indica erro, bloqueio ou alerta |
| Servo Motor 1 | A4 | Simula uma parte da trava do cofre |
| Servo Motor 2 | A5 | Simula outra parte da trava do cofre |

---

### 3.3 Justificativa dos Componentes

O **LCD 16x2** foi utilizado para guiar o usuário durante todo o processo, exibindo mensagens como cofre fechado, senha, erro, abertura, bloqueio e reset.

O **potenciômetro** foi escolhido como método de entrada de senha por permitir a leitura de valores analógicos, demonstrando o uso de `analogRead()` e da função `map()`.

Os **botões** permitem confirmar os dígitos, fechar o cofre e reiniciar o sistema.

Os **servomotores** simulam a trava física do cofre, movimentando-se para posições diferentes quando o cofre está fechado ou aberto.

O **LDR** foi utilizado como sensor de segurança, permitindo detectar luminosidade dentro do cofre quando ele deveria estar fechado.

Os **LEDs** fornecem feedback visual simples e direto: verde para acesso liberado e vermelho para erro, alerta ou bloqueio.

O **buzzer** adiciona feedback sonoro para situações de erro e segurança.

---

## 4. Recursos de Software e Bibliotecas

O projeto utiliza duas bibliotecas principais da Arduino IDE:

```cpp
#include <Servo.h>
#include <LiquidCrystal.h>
```

### 4.1 Biblioteca Servo.h

A biblioteca `Servo.h` é utilizada para controlar os dois servomotores do projeto. Ela permite definir a posição angular dos servos por meio da função `write()`.

Exemplo:

```cpp
motor1.write(90);
motor2.write(90);
```

No projeto, os servos são usados para representar o estado físico da trava do cofre.

---

### 4.2 Biblioteca LiquidCrystal.h

A biblioteca `LiquidCrystal.h` é utilizada para controlar o display LCD 16x2 em modo paralelo.

A inicialização usada no projeto foi:

```cpp
LiquidCrystal tela(7, 6, 5, 4, A2, A3);
```

O LCD é responsável por exibir mensagens de orientação ao usuário.

---

### 4.3 Funções Arduino Utilizadas

| Função | Uso no Projeto |
|---|---|
| `pinMode()` | Configuração dos pinos como entrada ou saída |
| `digitalRead()` | Leitura dos botões |
| `digitalWrite()` | Controle dos LEDs |
| `analogRead()` | Leitura do potenciômetro e do LDR |
| `map()` | Conversão do valor analógico do potenciômetro para número da senha |
| `tone()` | Acionamento do buzzer |
| `noTone()` | Desligamento do buzzer |
| `attachInterrupt()` | Configuração da interrupção externa do botão reset |
| `delay()` | Pequenos tempos de espera entre ações |
| `Serial.println()` | Exibição da leitura do LDR no Monitor Serial |

---

## 5. Funcionalidades Implementadas

### 5.1 Interface Visual com LCD

O display LCD é usado para orientar o usuário em todas as etapas do funcionamento.

As principais mensagens exibidas são:

```txt
COFRE FECHADO
APERTAR CONFIRMAR
```

```txt
SENHA:
Numero: X
```

```txt
SENHA CORRETA
ABRINDO...
```

```txt
COFRE ABERTO
APERTE FECHAR
```

```txt
SENHA INCORRETA
TENTATIVA: X
```

```txt
SISTEMA
BLOQUEADO
```

```txt
LUZ DETECTADA
TRAVADO
```

```txt
RESETANDO
SISTEMA...
```

O LCD torna o funcionamento do sistema mais claro e facilita a interação com o usuário.

---

### 5.2 Entrada de Dados via Potenciômetro

A senha é digitada por meio do potenciômetro conectado ao pino A0.

O valor lido pelo potenciômetro varia de 0 a 1023. Esse valor é convertido para uma faixa menor usando a função `map()`:

```cpp
numero = analogRead(potenciometro);
numero = map(numero, 0, 1023, 0, 6);
```

No código atual, o número escolhido varia de 0 a 6. Isso é suficiente para a senha definida, pois a senha correta utiliza os valores 2, 5 e 3.

Cada número é confirmado pelo botão conectado ao pino D13.

Após a confirmação de cada dígito, o LCD exibe um asterisco `*`, indicando que o número foi registrado.

---

### 5.3 Validação da Senha

A senha correta está armazenada em três variáveis:

```cpp
int senha1 = 2;
int senha2 = 5;
int senha3 = 3;
```

Os números digitados pelo usuário são armazenados em:

```cpp
int digito1;
int digito2;
int digito3;
```

A validação é feita por comparação direta:

```cpp
if (digito1 == senha1 &&
    digito2 == senha2 &&
    digito3 == senha3) {
    // senha correta
}
```

Se todos os dígitos estiverem corretos, o sistema libera o acesso. Caso contrário, registra uma tentativa incorreta.

---

### 5.4 Abertura e Fechamento do Cofre

O projeto utiliza dois servomotores para representar a trava do cofre.

Quando o cofre está fechado, os dois servos ficam na posição:

```cpp
motor1.write(90);
motor2.write(90);
```

Quando a senha está correta, o sistema abre o cofre com:

```cpp
motor1.write(180);
motor2.write(0);
```

Essas posições simulam o movimento da trava em sentidos opostos.

Após a abertura, o sistema aguarda o botão de fechar. Quando o botão é pressionado, os servos retornam à posição inicial, simulando o fechamento do cofre.

---

### 5.5 Feedback Visual por LEDs

O projeto utiliza dois LEDs:

| LED | Significado |
|---|---|
| LED Verde | Acesso liberado / cofre aberto |
| LED Vermelho | Senha incorreta / bloqueio / alerta |

Quando a senha está correta:

```cpp
digitalWrite(ledVerde, HIGH);
digitalWrite(ledVermelho, LOW);
```

Quando há erro ou alerta:

```cpp
digitalWrite(ledVerde, LOW);
digitalWrite(ledVermelho, HIGH);
```

Essa sinalização facilita a identificação visual do estado do sistema.

---

### 5.6 Feedback Sonoro com Buzzer

O buzzer é utilizado em situações de erro, bloqueio e alerta.

Quando a senha está incorreta, o buzzer toca em 1000 Hz:

```cpp
tone(piezo, 1000);
```

Quando o LDR detecta luz indevida, o buzzer toca em 1500 Hz:

```cpp
tone(piezo, 1500);
```

O buzzer é desligado com:

```cpp
noTone(piezo);
```

O uso de frequências diferentes ajuda a diferenciar erro de senha e alerta de segurança.

---

### 5.7 Sistema de Bloqueio por Tentativas

O sistema possui um contador de tentativas:

```cpp
int tentativas = 0;
const int limiteTentativas = 3;
```

A cada senha incorreta, o contador é incrementado:

```cpp
tentativas++;
```

Quando o número de tentativas chega a 3, a função `bloquearSistema()` é chamada.

Durante o bloqueio:

- O estado muda para `BLOQUEADO`;
- Os servos permanecem fechados;
- O LED vermelho acende;
- O buzzer toca;
- O LCD mostra a mensagem de bloqueio;
- O sistema aguarda 5 segundos;
- As tentativas são zeradas.

Trecho principal:

```cpp
if (tentativas >= limiteTentativas) {
  bloquearSistema();
}
```

Esse comportamento representa uma proteção contra tentativas repetidas de acesso indevido.

---

### 5.8 Monitoramento de Segurança com LDR

O sensor LDR está conectado ao pino A1 e é utilizado para detectar luz no interior do cofre.

A leitura é feita com:

```cpp
int luminosidade = analogRead(ldr);
```

O limite configurado no projeto é:

```cpp
const int limiteLDR = 80;
```

Se a leitura for menor que esse valor, o sistema interpreta que houve entrada de luz indevida:

```cpp
if (luminosidade < limiteLDR) {
  estadoAtual = ALERTA;
}
```

Durante o estado de alerta:

- O cofre permanece fechado;
- O LED vermelho acende;
- O buzzer toca;
- O LCD mostra `LUZ DETECTADA` e `TRAVADO`.

Um detalhe importante do projeto é que o LDR não gera alerta quando o cofre foi aberto corretamente pela senha. Isso é controlado pela variável:

```cpp
bool cofreAberto = false;
```

Dentro da função `verificarLDR()`, há a seguinte verificação:

```cpp
if (cofreAberto == true) {
  return;
}
```

Isso evita falso alarme quando o usuário abre o cofre corretamente.

---

### 5.9 Interrupção de Hardware para Reset

O botão de reset está conectado ao pino D2, que permite o uso de interrupção externa no Arduino Uno.

A configuração da interrupção é feita no `setup()`:

```cpp
attachInterrupt(
  digitalPinToInterrupt(botaoReset),
  acionarReset,
  FALLING
);
```

O botão foi configurado com resistor interno de pull-up:

```cpp
pinMode(botaoReset, INPUT_PULLUP);
```

Por isso, a interrupção ocorre na borda de descida, ou seja, quando o sinal muda de HIGH para LOW ao pressionar o botão.

A função de interrupção é:

```cpp
void acionarReset() {
  resetarSistema = true;
}
```

A variável `resetarSistema` foi declarada como `volatile`:

```cpp
volatile bool resetarSistema = false;
```

Isso é necessário porque ela pode ser alterada fora do fluxo principal do programa, dentro da interrupção.

A função de interrupção não executa diretamente o reset completo. Ela apenas altera a flag. O reset completo é tratado no fluxo principal do código pela função `resetarCofre()`.

Essa abordagem é mais segura, pois evita executar comandos longos dentro da interrupção.

---

## 6. Máquina de Estados

O projeto utiliza uma enumeração para representar os estados do sistema:

```cpp
enum Estado {
  TRANCADO,
  DIGITANDO,
  VALIDANDO,
  ABERTO,
  ERRO,
  BLOQUEADO,
  ALERTA
};
```

A variável que guarda o estado atual é:

```cpp
Estado estadoAtual = TRANCADO;
```

Os estados implementados são:

| Estado | Descrição |
|---|---|
| `TRANCADO` | Cofre fechado, aguardando o usuário |
| `DIGITANDO` | Usuário está escolhendo os números da senha |
| `VALIDANDO` | Sistema está comparando a senha digitada |
| `ABERTO` | Cofre aberto após senha correta |
| `ERRO` | Senha digitada incorretamente |
| `BLOQUEADO` | Sistema bloqueado após três erros |
| `ALERTA` | LDR detectou luz indevida |

---

## 7. Fluxo de Funcionamento do Software

```txt
[INÍCIO]
    |
    v
[CONFIGURAÇÃO DO SISTEMA]
    |
    v
[COFRE TRANCADO]
    |
    | Botão confirmar pressionado
    v
[DIGITAÇÃO DA SENHA]
    |
    | 3 dígitos confirmados
    v
[VALIDAÇÃO DA SENHA]
    |
    | Senha correta
    v
[COFRE ABERTO]
    |
    | Botão fechar pressionado
    v
[COFRE TRANCADO]


[VALIDAÇÃO DA SENHA]
    |
    | Senha incorreta
    v
[ERRO]
    |
    | Menos de 3 tentativas
    v
[COFRE TRANCADO]


[ERRO]
    |
    | 3 tentativas incorretas
    v
[BLOQUEADO]
    |
    v
[COFRE TRANCADO]


[COFRE TRANCADO]
    |
    | LDR detecta luz indevida
    v
[ALERTA]
    |
    v
[COFRE TRANCADO]


[QUALQUER ESTADO]
    |
    | Botão reset pressionado
    v
[RESET DO SISTEMA]
    |
    v
[COFRE TRANCADO]
```

---

## 8. Estrutura do Código

```txt
projeto.ino
|
├── setup()
|   └── Inicializa LCD, servos, botões, LEDs, buzzer e interrupção
|
├── loop()
|   └── Controla o fluxo principal do sistema
|
├── acionarReset()
|   └── Função chamada pela interrupção do botão reset
|
├── verificarLDR()
|   └── Lê o sensor LDR e aciona alerta se houver luz indevida
|
├── bloquearSistema()
|   └── Bloqueia o sistema após três tentativas incorretas
|
├── resetarCofre()
|   └── Reinicia o sistema, fecha os servos e zera tentativas
|
├── escolherNumero()
|   └── Lê o potenciômetro e registra cada número da senha
|
└── esperarBotao()
    └── Aguarda o pressionamento e soltura de um botão
```

---

## 9. Trechos Relevantes do Código

### 9.1 Definição da Senha

```cpp
int senha1 = 2;
int senha2 = 5;
int senha3 = 3;
```

---

### 9.2 Mapeamento do Potenciômetro

```cpp
numero = analogRead(potenciometro);
numero = map(numero, 0, 1023, 0, 6);
```

---

### 9.3 Validação da Senha

```cpp
if (digito1 == senha1 &&
    digito2 == senha2 &&
    digito3 == senha3) {
    // acesso liberado
}
```

---

### 9.4 Bloqueio por Tentativas

```cpp
tentativas++;

if (tentativas >= limiteTentativas) {
  bloquearSistema();
}
```

---

### 9.5 Verificação do LDR

```cpp
int luminosidade = analogRead(ldr);

if (luminosidade < limiteLDR) {
  estadoAtual = ALERTA;
}
```

---

### 9.6 Interrupção de Reset

```cpp
attachInterrupt(
  digitalPinToInterrupt(botaoReset),
  acionarReset,
  FALLING
);
```

---

### 9.7 Função de Interrupção

```cpp
void acionarReset() {
  resetarSistema = true;
}
```

---

## 10. Testes Realizados

| Teste | Resultado Esperado | Resultado Obtido |
|---|---|---|
| Ligar o sistema | LCD mostra cofre fechado | Funcionou |
| Pressionar botão confirmar | Sistema inicia entrada da senha | Funcionou |
| Girar potenciômetro | LCD mostra o número selecionado | Funcionou |
| Confirmar três dígitos corretos | Sistema valida a senha | Funcionou |
| Digitar senha `253` | Cofre abre, LED verde acende | Funcionou |
| Pressionar botão fechar | Cofre fecha e LED verde apaga | Funcionou |
| Digitar senha incorreta | LED vermelho acende e buzzer toca | Funcionou |
| Errar senha três vezes | Sistema bloqueia temporariamente | Funcionou |
| Detectar luz com cofre fechado | Sistema entra em alerta | Funcionou |
| Abrir o cofre corretamente | LDR não dispara falso alerta | Funcionou |
| Pressionar reset | Sistema reinicia | Funcionou |

---

## 11. Possíveis Limitações

Apesar de funcional, o projeto possui algumas limitações:

- A senha é fixa no código e não pode ser alterada pelo usuário durante a execução.
- O sistema não possui armazenamento em memória não volátil, como EEPROM.
- O bloqueio após três erros é temporário, não permanente.
- A leitura do potenciômetro pode variar um pouco dependendo da posição física e da estabilidade elétrica.
- O limite do LDR pode precisar de calibração conforme a iluminação do ambiente.
- A trava física é apenas simulada por servomotores.

---

## 12. Conclusão

O projeto desenvolvido implementa um sistema de controle de acesso para um cofre inteligente utilizando Arduino Uno.

A solução integra diferentes tipos de periféricos, incluindo entradas digitais, entrada analógica, sensor de luminosidade, display LCD, LEDs, buzzer e servomotores.

O sistema permite a entrada de senha pelo usuário, valida o acesso, abre e fecha o cofre, indica estados visualmente, emite alertas sonoros, bloqueia após tentativas incorretas e detecta possíveis situações de violação por meio do LDR.

Além disso, o uso de interrupção externa para reset/emergência demonstra a aplicação de recursos importantes de sistemas embarcados.

Dessa forma, o projeto atende aos objetivos propostos, simulando um locker inteligente com feedback multissensorial e lógica de controle baseada em estados.
