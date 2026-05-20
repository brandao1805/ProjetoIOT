# Locker Inteligente com Feedback Multissensorial 🔐

Projeto final da disciplina **CCM520 – Internet das Coisas**, desenvolvido com Arduino Uno.

O projeto consiste em um sistema de controle de acesso para um locker/cofre inteligente. O sistema permite que o usuário digite uma senha utilizando um potenciômetro e um botão de confirmação. Caso a senha esteja correta, o cofre é aberto por meio de servomotores. Caso a senha esteja incorreta, o sistema informa o erro no LCD, aciona LED vermelho e buzzer. Após três tentativas incorretas, o sistema entra em estado de bloqueio.

Além disso, o projeto possui monitoramento de segurança por meio de um sensor LDR, que detecta incidência de luz indevida quando o cofre está fechado. O sistema também possui um botão de reset/emergência utilizando interrupção externa.

---

## 👤 Integrantes

- Lucas de Almeida Brandão - RA: 22.124.066-6
- Henrique Gaiardoni Sant'Anna - RA: 22.124.009-6

---

## 📖 Sumário

- [Visão Geral](#visão-geral)
- [Funcionamento do Sistema](#funcionamento-do-sistema)
- [Senha e Entrada de Dados](#senha-e-entrada-de-dados)
- [Sistema de Bloqueio](#sistema-de-bloqueio)
- [Monitoramento de Segurança com LDR](#monitoramento-de-segurança-com-ldr)
- [Interrupção de Reset](#interrupção-de-reset)
- [Máquina de Estados](#máquina-de-estados)
- [Componentes Utilizados](#componentes-utilizados)
- [Mapeamento de Pinos](#mapeamento-de-pinos)
- [Como Reproduzir](#como-reproduzir)
- [Testes Realizados](#testes-realizados)
- [Conclusão](#conclusão)

---

## 🔍 Visão Geral

O sistema simula um **cofre inteligente** controlado por Arduino Uno.

O usuário insere uma senha de **3 dígitos** utilizando um potenciômetro e um botão de confirmação. O Arduino valida a senha e, caso ela esteja correta, aciona dois servomotores responsáveis por simular a abertura do cofre.

O sistema também possui recursos de segurança, como bloqueio após três tentativas incorretas, alerta sonoro com buzzer, LEDs de indicação e monitoramento de luminosidade com sensor LDR.

Funcionalidades principais:

- Entrada de senha via potenciômetro;
- Confirmação dos dígitos por botão;
- Senha padrão: `253`;
- Validação de acesso;
- Abertura e fechamento por dois servomotores;
- LCD 16x2 para orientação do usuário;
- LED verde para acesso liberado;
- LED vermelho para erro, alerta ou bloqueio;
- Buzzer para feedback sonoro;
- Bloqueio após 3 tentativas incorretas;
- Monitoramento de luz com LDR;
- Reset/emergência por interrupção externa.

---

## 🖥️ Funcionamento do Sistema

### Tela Inicial

Ao ligar o sistema, o LCD exibe o estado inicial do cofre:

```txt
COFRE FECHADO
APERTAR CONFIRMAR
```

Nesse estado, o cofre está fechado, os servos ficam na posição travada, os LEDs permanecem apagados e o sistema aguarda o usuário pressionar o botão de confirmação.

---

## 🔒 Senha e Entrada de Dados

A senha é composta por três dígitos. A senha correta configurada no código é:

```txt
253
```

A entrada dos dígitos funciona da seguinte forma:

1. O usuário pressiona o botão **Confirmar** para iniciar.
2. O LCD mostra a tela de senha.
3. O usuário gira o potenciômetro para escolher um número.
4. O número atual aparece no LCD.
5. O usuário pressiona o botão **Confirmar** para salvar o dígito.
6. O sistema mostra um `*` no LCD para indicar que o dígito foi registrado.
7. O processo se repete até completar os três dígitos.

Durante a digitação, o LCD exibe algo semelhante a:

```txt
SENHA: *
Numero: 2
```

Depois que os três números são digitados, o sistema entra no estado de validação.

---

## 🔓 Senha Correta

Quando a senha digitada é igual a `253`, o sistema libera o acesso.

Ações realizadas:

- O LCD mostra `SENHA CORRETA`;
- O LED verde acende;
- O LED vermelho permanece apagado;
- O buzzer fica desligado;
- Os dois servomotores se movimentam para abrir o cofre;
- O sistema mostra a mensagem `COFRE ABERTO`.

Mensagem exibida:

```txt
SENHA CORRETA
ABRINDO...
```

Depois:

```txt
COFRE ABERTO
APERTE FECHAR
```

Após a abertura, o sistema aguarda o botão **Fechar** ser pressionado. Quando isso acontece, os servos retornam para a posição inicial, simulando o fechamento do cofre.

---

## Senha Incorreta

Quando a senha digitada está errada, o sistema informa a falha ao usuário.

Ações realizadas:

- O LCD mostra `SENHA INCORRETA`;
- O LED vermelho acende;
- O buzzer toca;
- O contador de tentativas é incrementado;
- O LED verde permanece apagado.

Exemplo de mensagem:

```txt
SENHA INCORRETA
TENTATIVA: 1
```

O usuário pode tentar novamente até atingir o limite de três tentativas incorretas.

---

## 🚫 Sistema de Bloqueio

O sistema possui bloqueio automático após **3 tentativas incorretas**.

Quando o limite é atingido:

- O estado do sistema muda para `BLOQUEADO`;
- Os servomotores permanecem na posição fechada;
- O LED vermelho acende;
- O buzzer é acionado;
- O LCD mostra a mensagem de bloqueio;
- O sistema fica bloqueado temporariamente;
- Depois, as tentativas são zeradas e uma nova tentativa é permitida.

Mensagem exibida:

```txt
SISTEMA
BLOQUEADO
```

Depois do tempo de bloqueio, o LCD mostra:

```txt
TENTE NOVAMENTE
```

Esse comportamento representa uma proteção contra múltiplas tentativas de acesso não autorizado.

---

## 🚨 Monitoramento de Segurança com LDR

O projeto utiliza um sensor **LDR** para monitorar a presença de luz indevida no cofre.

A lógica utilizada é:

- Se o cofre estiver fechado e o LDR detectar luz, o sistema interpreta como possível tentativa de abertura indevida.
- O sistema entra no estado de alerta.
- O buzzer toca.
- O LED vermelho acende.
- Os servos permanecem na posição fechada.
- O LCD informa que houve detecção de luz.

Mensagem exibida:

```txt
LUZ DETECTADA
TRAVADO
```

O limite de leitura do LDR está definido no código por meio da constante:

```cpp
const int limiteLDR = 80;
```

Se necessário, esse valor pode ser ajustado de acordo com a iluminação do ambiente.

O sistema também evita falso alerta: quando o cofre é aberto corretamente pela senha, a variável `cofreAberto` impede que o LDR dispare o alarme durante a abertura autorizada.

---

## ⚠️ Interrupção de Reset

O projeto possui um botão de reset/emergência conectado ao pino 2 do Arduino Uno.

Esse botão utiliza interrupção externa por meio da função:

```cpp
attachInterrupt(
  digitalPinToInterrupt(botaoReset),
  acionarReset,
  FALLING
);
```

Quando o botão é pressionado, a variável `resetarSistema` é ativada. Em seguida, o sistema executa a função `resetarCofre()`.

O reset realiza as seguintes ações:

- Fecha os servomotores;
- Apaga LED verde e LED vermelho;
- Desliga o buzzer;
- Zera o contador de tentativas;
- Define o estado como `TRANCADO`;
- Exibe mensagem de reset no LCD.

Mensagem exibida:

```txt
RESETANDO
SISTEMA...
```

Depois:

```txt
SISTEMA OK
```

---

## ⚙️ Máquina de Estados

O código utiliza uma lógica baseada em estados para organizar o funcionamento do cofre.

Estados definidos no código:

| Estado | Descrição |
|---|---|
| `TRANCADO` | Cofre fechado, aguardando interação do usuário |
| `DIGITANDO` | Usuário está escolhendo os dígitos da senha |
| `VALIDANDO` | Sistema compara a senha digitada com a senha correta |
| `ABERTO` | Cofre aberto após senha correta |
| `ERRO` | Senha digitada incorretamente |
| `BLOQUEADO` | Sistema bloqueado após três erros |
| `ALERTA` | LDR detectou luz indevida |

Fluxo simplificado:

```txt
TRANCADO
   |
   v
DIGITANDO
   |
   v
VALIDANDO
   | senha correta
   v
ABERTO
   |
   v
TRANCADO

VALIDANDO
   | senha incorreta
   v
ERRO
   |
   | 3 tentativas incorretas
   v
BLOQUEADO
   |
   v
TRANCADO

TRANCADO
   | luz detectada pelo LDR
   v
ALERTA
   |
   v
TRANCADO

RESET / EMERGÊNCIA
   -> TRANCADO
```

---

## 🧩 Componentes Utilizados

| Componente | Função |
|---|---|
| Arduino Uno | Controlador principal do sistema |
| Display LCD 16x2 | Interface visual com o usuário |
| Potenciômetro | Seleção dos números da senha |
| Botão Confirmar | Confirma início e dígitos da senha |
| Botão Fechar | Fecha o cofre após abertura |
| Botão Reset | Reinicia o sistema por interrupção |
| 2 Servomotores | Simulam a trava de abertura e fechamento |
| LDR | Detecta luminosidade indevida |
| Buzzer | Emite alertas sonoros |
| LED Verde | Indica acesso liberado |
| LED Vermelho | Indica erro, bloqueio ou alerta |
| Resistores | Proteção e funcionamento dos componentes |
| Protoboard | Montagem do circuito |
| Jumpers | Conexões elétricas |

---

## 🗺️ Mapeamento de Pinos

| Componente | Pino no Arduino |
|---|---|
| LCD RS | 7 |
| LCD E | 6 |
| LCD D4 | 5 |
| LCD D5 | 4 |
| LCD D6 | A2 |
| LCD D7 | A3 |
| Botão Confirmar | 13 |
| Botão Fechar | 12 |
| Botão Reset/Emergência | 2 |
| Potenciômetro | A0 |
| LDR | A1 |
| Buzzer | 10 |
| LED Verde | 8 |
| LED Vermelho | 9 |
| Servo 1 | A4 |
| Servo 2 | A5 |

---

## 📚 Bibliotecas Utilizadas

O projeto utiliza duas bibliotecas padrão da Arduino IDE:

```cpp
#include <Servo.h>
#include <LiquidCrystal.h>
```

A biblioteca `Servo.h` é usada para controlar os servomotores.

A biblioteca `LiquidCrystal.h` é usada para controlar o display LCD 16x2.

---

## ▶️ Como Reproduzir

### Requisitos de Software

- Arduino IDE 1.8+ ou 2.x;
- Biblioteca `Servo.h`;
- Biblioteca `LiquidCrystal.h`;
- Placa Arduino Uno configurada na IDE.

As bibliotecas utilizadas já são normalmente incluídas por padrão na Arduino IDE.

---

### 👣 Passos

1. Monte o circuito conforme o diagrama elétrico do projeto.
2. Conecte o Arduino Uno ao computador.
3. Abra o arquivo do projeto na Arduino IDE.
4. Caso necessário, salve o arquivo com extensão `.ino`.
5. Selecione a placa:

```txt
Ferramentas > Placa > Arduino Uno
```

6. Selecione a porta correta:

```txt
Ferramentas > Porta
```

7. Clique em **Carregar** para enviar o código ao Arduino.
8. Após o upload, o LCD deverá exibir a tela inicial.
9. Pressione o botão **Confirmar** para iniciar a digitação da senha.
10. Use o potenciômetro para escolher os dígitos.
11. Digite a senha `253` para abrir o cofre.

---

## 🚀 Como Usar

### 🔐 Para abrir o cofre

1. Aguarde a mensagem `COFRE FECHADO`.
2. Aperte o botão **Confirmar**.
3. Gire o potenciômetro até o número desejado.
4. Aperte **Confirmar** para registrar o dígito.
5. Repita até completar três dígitos.
6. Digite `253`.
7. O cofre será aberto.

---

### 🔒 Para fechar o cofre

1. Após a mensagem `COFRE ABERTO`, pressione o botão **Fechar**.
2. Os servos voltarão para a posição fechada.
3. O sistema retornará ao estado inicial.

---

### ⬅️ Para resetar o sistema

1. Pressione o botão **Reset/Emergência**.
2. O sistema fechará os servos.
3. As tentativas serão zeradas.
4. O LCD mostrará a mensagem de reset.
5. O sistema voltará ao estado `TRANCADO`.

---

## 🧪 Testes Realizados

| Teste | Resultado Esperado | Resultado Obtido |
|---|---|---|
| Ligar o sistema | LCD mostra cofre fechado | Funcionou |
| Digitar senha correta `253` | Cofre abre e LED verde acende | Funcionou |
| Digitar senha incorreta | LED vermelho acende e buzzer toca | Funcionou |
| Errar senha três vezes | Sistema entra em bloqueio temporário | Funcionou |
| Pressionar botão fechar | Cofre fecha | Funcionou |
| Pressionar botão reset | Sistema reinicia | Funcionou |
| Detectar luz com o cofre fechado | Sistema entra em alerta | Funcionou |
| Abrir cofre corretamente com luz ambiente | LDR não dispara alerta falso | Funcionou |

---

## 🛠️ Explicação das Principais Funções

### `setup()`

Inicializa os componentes principais do sistema: LCD, servos, botões, LEDs, buzzer, comunicação serial e interrupção externa.

---

### `loop()`

Controla o fluxo principal do sistema. Exibe a tela inicial, aguarda o botão de confirmação, recebe a senha, valida os dígitos e decide se o cofre deve abrir ou se deve indicar erro.

---

### `verificarLDR()`

Realiza a leitura do sensor LDR. Caso detecte luz indevida com o cofre fechado, ativa o estado de alerta, acendendo o LED vermelho, tocando o buzzer e exibindo mensagem no LCD.

---

### `bloquearSistema()`

Executada quando o usuário erra a senha três vezes. O sistema aciona LED vermelho, buzzer e mensagem de bloqueio no LCD.

---

### `resetarCofre()`

Reinicia o sistema. Fecha os servos, apaga LEDs, desliga o buzzer, zera tentativas e retorna ao estado inicial.

---

### `escolherNumero()`

Lê o valor do potenciômetro e converte a leitura analógica para um número utilizado na senha. Cada número é confirmado pelo botão de confirmação.

---

### `esperarBotao()`

Aguarda o botão ser pressionado e depois solto, evitando que um único clique seja lido várias vezes.

---

## Calibração do LDR

O valor do LDR pode variar conforme o ambiente. Por isso, o código imprime a leitura do sensor no Monitor Serial:

```cpp
Serial.println(luminosidade);
```

Com base nesses valores, é possível ajustar a constante:

```cpp
const int limiteLDR = 80;
```

Se o alerta estiver disparando com muita facilidade ou não estiver disparando, basta alterar esse valor e reenviar o código para o Arduino.

---

## 🎬 Conclusão

O projeto implementa um cofre inteligente com controle de acesso por senha e monitoramento de segurança.

A solução integra entradas digitais, entrada analógica, sensor de luminosidade, display LCD, LEDs, buzzer e servomotores. Além disso, utiliza lógica de estados e interrupção externa para reset/emergência.

Com isso, o sistema atende ao objetivo de criar um locker inteligente capaz de validar acesso, representar estados físicos do cofre e reagir a possíveis situações de segurança.
