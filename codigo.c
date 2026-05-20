// Biblioteca para controlar os servomotores
#include <Servo.h>

// Biblioteca para controlar o display LCD 16x2
#include <LiquidCrystal.h>

// Criação dos dois objetos Servo
// motor1 e motor2 representam a trava física do cofre
Servo motor1;
Servo motor2;


// Configuração dos pinos do LCD
LiquidCrystal tela(7, 6, 5, 4, A2, A3);

// Botão usado para confirmar:
#define confirmar 13

// Botão usado para fechar o cofre depois que ele foi aberto
#define fechar 12

// Botão de reset/emergência
// Está no pino 2 porque esse pino aceita interrupção externa no Arduino Uno
#define botaoReset 2

// Potenciômetro usado para escolher os números da senha
#define potenciometro A0

// LDR usado para detectar luz indevida dentro do cofre
#define ldr A1

// Buzzer usado para alertas sonoros
#define piezo 10

// LED verde indica acesso liberado/cofre aberto
#define ledVerde 8

// LED vermelho indica erro, bloqueio ou alerta
#define ledVermelho 9

// A senha correta é 253
int senha1 = 2;
int senha2 = 5;
int senha3 = 3;

// Armazena quantas vezes o usuário errou a senha
int tentativas = 0;

// Número máximo de tentativas antes de bloquear o sistema
const int limiteTentativas = 3;

// Essa variável indica se o cofre foi aberto corretamente pela senha
// Ela é importante para impedir que o LDR dispare alerta quando o cofre está aberto de forma autorizada
bool cofreAberto = false;

// Valor limite para detectar luz
// Se a leitura do LDR for menor que esse valor, o sistema entende que houve incidência de luz indevida
const int limiteLDR = 80;

// Variável alterada pela interrupção
// "volatile" é usado porque essa variável pode mudar fora do fluxo normal do programa, dentro da função de interrupção
volatile bool resetarSistema = false;

// Enumeração com os possíveis estados do cofre
enum Estado {
  TRANCADO,    // Cofre fechado, aguardando o usuário
  DIGITANDO,  // Usuário está digitando a senha
  VALIDANDO,  // Sistema está comparando a senha digitada
  ABERTO,     // Cofre aberto após senha correta
  ERRO,       // Senha incorreta
  BLOQUEADO,  // Sistema bloqueado após 3 erros
  ALERTA      // Alerta de segurança ativado pelo LDR
};

// Estado inicial do sistema
Estado estadoAtual = TRANCADO;

// Essa função é chamada automaticamente quando o botão de reset é apertado
// Ela não executa o reset diretamente, apenas ativa uma flag
// Isso é uma boa prática em interrupções, pois evita código longo dentro dela
void acionarReset() {
  resetarSistema = true;
}

void setup() {

  // Inicializa a comunicação serial, mostra os valores do LDR no monitor serial
  Serial.begin(9600);

  // Inicializa o LCD com 16 colunas e 2 linhas
  tela.begin(16, 2);
  tela.clear();

  // Liga os servos nos pinos analógicos A4 e A5, também podem ser usados como pinos digitais
  motor1.attach(A4);
  motor2.attach(A5);

  // Posição inicial dos servos, 90 graus representa o cofre fechado
  motor1.write(90);
  motor2.write(90);

  // Configuração dos botões principais como entrada
  pinMode(confirmar, INPUT);
  pinMode(fechar, INPUT);

  // Configuração do botão de reset com resistor interno de pull-up
  // Nesse modo, o botão deve levar o pino ao GND quando pressionado
  pinMode(botaoReset, INPUT_PULLUP);

  // Configuração das saídas
  pinMode(piezo, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledVermelho, OUTPUT);

  // Garante que os LEDs iniciem apagados
  digitalWrite(ledVerde, LOW);
  digitalWrite(ledVermelho, LOW);

  // Garante que o buzzer inicie desligado
  noTone(piezo);

  // Configura a interrupção externa no botão de reset
  attachInterrupt(
    digitalPinToInterrupt(botaoReset), // associa o pino 2 à interrupção
    acionarReset, // função chamada quando a interrupção ocorre
    FALLING // dispara quando o sinal muda de HIGH para LOW
  );
}


// LOOP PRINCIPAL

void loop() {

  // Se o botão de reset foi acionado, reinicia o sistema
  if (resetarSistema) {
    resetarCofre();
    return;
  }

  // Verifica se há luz indevida no cofre enquanto ele está fechado
  verificarLDR();

  // Variáveis que armazenam os três números digitados pelo usuário
  int digito1 = 0;
  int digito2 = 0;
  int digito3 = 0;

  // Estado inicial a cada ciclo
  estadoAtual = TRANCADO;

  // Garante que o sistema comece considerando o cofre fechado
  cofreAberto = false;

  // Mostra a tela inicial
  tela.clear();

  tela.setCursor(0, 0);
  tela.print("COFRE FECHADO");

  tela.setCursor(0, 1);
  tela.print("APERTAR CONFIRMAR");

  // Apaga os LEDs e desliga o buzzer
  digitalWrite(ledVerde, LOW);
  digitalWrite(ledVermelho, LOW);

  noTone(piezo);

  // Garante que os servos fiquem na posição fechada
  motor1.write(90);
  motor2.write(90);

  // Aguarda o usuário apertar o botão confirmar
  esperarBotao(confirmar);

  // Após esperar o botão, verifica se houve reset
  if (resetarSistema) {
    resetarCofre();
    return;
  }

  // O sistema entra no estado de digitação
  estadoAtual = DIGITANDO;

  // Prepara o LCD para mostrar a senha sendo digitada
  tela.clear();

  tela.setCursor(0, 0);
  tela.print("SENHA:");

  // O usuário escolhe cada número usando o potenciômetro
  // e confirma cada dígito com o botão confirmar
  digito1 = escolherNumero(0);
  digito2 = escolherNumero(1);
  digito3 = escolherNumero(2);

  // Verifica novamente se houve reset durante a digitação
  if (resetarSistema) {
    resetarCofre();
    return;
  }

  // O sistema entra no estado de validação
  estadoAtual = VALIDANDO;

  // VERIFICAÇÃO DA SENHA

  if (digito1 == senha1 &&
      digito2 == senha2 &&
      digito3 == senha3) {

    // Se a senha está correta, zera as tentativas
    tentativas = 0;

    // Atualiza o estado para aberto
    estadoAtual = ABERTO;

    // Marca que o cofre foi aberto corretamente
    cofreAberto = true;

    // Mensagem de senha correta
    tela.clear();

    tela.setCursor(0, 0);
    tela.print("SENHA CORRETA");

    tela.setCursor(0, 1);
    tela.print("ABRINDO...");

    // LED verde aceso indica acesso liberado
    digitalWrite(ledVerde, HIGH);

    // LED vermelho apagado indica que não há erro
    digitalWrite(ledVermelho, LOW);

    // Buzzer desligado
    noTone(piezo);

    // Movimento dos servos para abrir a trava
    // Um servo vai para 180 graus e o outro para 0 graus
    // Isso simula a abertura da trava em sentidos opostos
    motor1.write(180);
    motor2.write(0);

    delay(1000);

    // Mostra que o cofre está aberto
    tela.clear();

    tela.setCursor(0, 0);
    tela.print("COFRE ABERTO");

    tela.setCursor(0, 1);
    tela.print("APERTE FECHAR");

    // Aguarda o botão de fechar
    esperarBotao(fechar);

    // Se o reset foi acionado enquanto o cofre estava aberto
    if (resetarSistema) {
      resetarCofre();
      return;
    }

    // Mensagem de fechamento
    tela.clear();

    tela.setCursor(0, 0);
    tela.print("FECHANDO...");

    // Retorna os servos para a posição fechada
    motor1.write(90);
    motor2.write(90);

    // Atualiza o estado do cofre
    cofreAberto = false;
    estadoAtual = TRANCADO;

    // Apaga o LED verde
    digitalWrite(ledVerde, LOW);

    delay(1000);

  } else {

    // CASO A SENHA ESTEJA INCORRETA
    
    // Atualiza o estado para erro
    estadoAtual = ERRO;

    // Soma uma tentativa errada
    tentativas++;

    // Mostra erro no LCD
    tela.clear();

    tela.setCursor(0, 0);
    tela.print("SENHA INCORRETA");

    tela.setCursor(0, 1);
    tela.print("TENTATIVA: ");
    tela.print(tentativas);

    // LED verde apagado
    digitalWrite(ledVerde, LOW);

    // LED vermelho aceso indica erro
    digitalWrite(ledVermelho, HIGH);

    // Buzzer toca em 1000 Hz para indicar erro
    tone(piezo, 1000);

    delay(2000);

    // Desliga buzzer e LED vermelho
    noTone(piezo);

    digitalWrite(ledVermelho, LOW);

    delay(500);

    // Se chegou no limite de tentativas, bloqueia o sistema
    if (tentativas >= limiteTentativas) {
      bloquearSistema();
    }
  }
}


// FUNÇÃO PARA VERIFICAR O LDR

void verificarLDR() {

  // Se o reset foi acionado, reinicia o sistema
  if (resetarSistema) {
    resetarCofre();
    return;
  }

  // Se o cofre foi aberto pela senha correta, o LDR não deve gerar alerta
  // Isso evita que o sistema dispare alarme quando o usuário abrir o cofre corretamente e a luz entrar naturalmente
  if (cofreAberto == true) {
    return;
  }

  // Faz a leitura analógica do LDR
  int luminosidade = analogRead(ldr);

  // Mostra o valor da luminosidade no monitor serial, ajuda a calibrar o limiteLDR
  Serial.println(luminosidade);

  // Se a luminosidade passar do limite definido, o sistema entende que houve uma abertura indevida ou entrada de luz no cofre
  if (luminosidade < limiteLDR) {

    // Atualiza o estado para alerta
    estadoAtual = ALERTA;

    // Fecha imediatamente os servos
    motor1.write(90);
    motor2.write(90);

    // Garante que o sistema considere o cofre fechado
    cofreAberto = false;

    // LED verde apagado
    digitalWrite(ledVerde, LOW);

    // LED vermelho aceso indica alerta
    digitalWrite(ledVermelho, HIGH);

    // Buzzer toca em frequência maior para indicar alerta
    tone(piezo, 1500);

    // Mensagem de alerta no LCD
    tela.clear();

    tela.setCursor(0, 0);
    tela.print("LUZ DETECTADA");

    tela.setCursor(0, 1);
    tela.print("TRAVADO");

    // Enquanto a luz continuar sendo detectada, o sistema permanece travado
    while (analogRead(ldr) < limiteLDR) {

      // Mesmo dentro do alerta, o reset continua funcionando
      if (resetarSistema) {
        resetarCofre();
        return;
      }

      delay(50);
    }

    // Quando a condição de alerta termina, desliga buzzer e LED vermelho
    noTone(piezo);

    digitalWrite(ledVermelho, LOW);

    // Mostra que o sistema voltou ao normal
    tela.clear();

    tela.setCursor(0, 0);
    tela.print("SISTEMA OK");

    // Volta para o estado trancado
    estadoAtual = TRANCADO;

    delay(1000);
  }
}


// FUNÇÃO PARA BLOQUEAR O SISTEMA APÓS 3 ERROS

void bloquearSistema() {

  // Atualiza o estado para bloqueado
  estadoAtual = BLOQUEADO;

  // Mantém o cofre fechado
  motor1.write(90);
  motor2.write(90);

  // Garante que o sistema considere o cofre fechado
  cofreAberto = false;

  // LED verde apagado
  digitalWrite(ledVerde, LOW);

  // LED vermelho aceso durante o bloqueio
  digitalWrite(ledVermelho, HIGH);

  // Mensagem no LCD
  tela.clear();

  tela.setCursor(0, 0);
  tela.print("SISTEMA");

  tela.setCursor(0, 1);
  tela.print("BLOQUEADO");

  // Buzzer toca durante o bloqueio
  tone(piezo, 1000);

  // Sistema fica bloqueado por 5 segundos
  delay(5000);

  // Desliga buzzer e LED vermelho
  noTone(piezo);

  digitalWrite(ledVermelho, LOW);

  // Zera as tentativas após o bloqueio
  tentativas = 0;

  // Mensagem para o usuário tentar novamente
  tela.clear();

  tela.setCursor(0, 0);
  tela.print("TENTE NOVAMENTE");

  delay(1500);

  // Volta ao estado trancado
  estadoAtual = TRANCADO;
}


// FUNÇÃO PARA RESETAR O COFRE

void resetarCofre() {

  // Fecha os servos
  motor1.write(90);
  motor2.write(90);

  // Reseta variáveis de controle
  cofreAberto = false;
  tentativas = 0;
  estadoAtual = TRANCADO;

  // Apaga LEDs
  digitalWrite(ledVerde, LOW);
  digitalWrite(ledVermelho, LOW);

  // Desliga buzzer
  noTone(piezo);

  // Mensagem de reset no LCD
  tela.clear();

  tela.setCursor(0, 0);
  tela.print("RESETANDO");

  tela.setCursor(0, 1);
  tela.print("SISTEMA...");

  delay(1000);

  // Mensagem de sistema normal
  tela.clear();

  tela.setCursor(0, 0);
  tela.print("SISTEMA OK");

  delay(1000);

  tela.clear();

  // Desativa a flag de reset
  resetarSistema = false;
}


// FUNÇÃO PARA ESCOLHER UM NÚMERO DA SENHA

// Essa função lê o valor do potenciômetro e transforma em um número.
// O parâmetro "posicao" indica qual dígito da senha está sendo preenchido:
// 0 = primeiro dígito
// 1 = segundo dígito
// 2 = terceiro dígito
int escolherNumero(int posicao) {

  // Número escolhido pelo usuário
  int numero = 0;

  // Guarda o número anterior para atualizar o LCD apenas quando mudar
  int numeroAnterior = -1;

  // Enquanto o botão confirmar não for pressionado, o usuário pode girar o potenciômetro e escolher o número
  while (digitalRead(confirmar) == LOW) {

    // Se o reset foi acionado, reinicia o sistema
    if (resetarSistema) {
      resetarCofre();
      return 0;
    }

    // Verifica o LDR durante a digitação
    verificarLDR();

    // Lê o valor analógico do potenciômetro
    numero = analogRead(potenciometro);

    // Converte o valor de 0 a 1023 para um número entre 0 e 6
    // Isso permite escolher os dígitos da senha
    numero = map(numero, 0, 1023, 0, 6);

    // Atualiza o LCD somente se o número mudou
    if (numero != numeroAnterior) {

      tela.setCursor(0, 1);
      tela.print("Numero: ");

      tela.print(numero);
      tela.print("      ");

      numeroAnterior = numero;
    }

    delay(100);
  }

  // Aguarda o usuário soltar o botão confirmar
  // Isso evita que o mesmo clique seja contado mais de uma vez
  while (digitalRead(confirmar) == HIGH) {

    // Se o reset foi acionado, reinicia o sistema
    if (resetarSistema) {
      resetarCofre();
      return 0;
    }

    // Continua verificando o LDR
    verificarLDR();

    delay(10);
  }

  // Mostra um asterisco na linha da senha para indicar que o dígito foi inserido
  // A posição muda conforme o dígito escolhido
  tela.setCursor(7 + posicao, 0);
  tela.print("*");

  delay(300);

  // Retorna o número escolhido
  return numero;
}


// FUNÇÃO PARA ESPERAR UM BOTÃO SER APERTADO

// Essa função espera o botão ser pressionado e depois solto.
// Ela é usada tanto para o botão confirmar quanto para o botão fechar.
void esperarBotao(int botao) {

  // Aguarda o botão ser pressionado
  while (digitalRead(botao) == LOW) {

    // Se o reset foi acionado, reinicia o sistema
    if (resetarSistema) {
      resetarCofre();
      return;
    }

    // Continua verificando o LDR enquanto espera
    verificarLDR();

    delay(10);
  }

  // Aguarda o botão ser solto
  while (digitalRead(botao) == HIGH) {

    // Se o reset foi acionado, reinicia o sistema
    if (resetarSistema) {
      resetarCofre();
      return;
    }

    // Continua verificando o LDR enquanto espera
    verificarLDR();

    delay(10);
  }

  // Pequeno atraso para evitar leitura duplicada do botão
  delay(200);
}
