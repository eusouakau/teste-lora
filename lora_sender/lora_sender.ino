//inclusão de bibliotecas
#include <SoftwareSerial.h> //biblioteca para comunicação de Porta Serial
#include "heltec.h" //bibliotaca do Lora 32 Heltec
#define BAND 915E6  //Escolha da frequência da banda. 915Mhz é o padrão no Brasil

//definição pinos Rx e Tx
#define pinRx 36
#define pinTx 37

SoftwareSerial lora2(pinRx, pinTx); //inicialização da comunicação serial nas portal Rx e Tx definidas acima

//definição de diversar variáveis globais
String packet;
float currentTemp = 0;
int count = 0;
String LoRaMessage = "";

//função para enviar os pacotes pelo lora
void sendPacket();
void sendPacket()
{
  LoRa.beginPacket();
  LoRa.print("Temperatura: ");
  LoRa.print(currentTemp);
  LoRa.print(" ºC");
  LoRa.print("\n");
  LoRa.print("Pacote: ");
  LoRa.print(count);
  LoRa.endPacket();
}

//função de inicialização
void setup()
{
  //inicialização da comunicação serial (monitor e porta)
  Serial.begin(115200);
  lora2.begin(115200);
  
  pinMode(LED,OUTPUT); //inicializa o LED

  //Inicialização do display 
  Heltec.begin(true /*Habilita o Display*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Habilita debug Serial*/, true /*Habilita o PABOOST*/, BAND /*Frequência BAND*/);
  Heltec.display->init();
  Heltec.display->setFont(ArialMT_Plain_16);
  Heltec.display->clear();
  Heltec.display->drawString(33, 5, "Iniciado");
  Heltec.display->drawString(10, 30, "com Sucesso!");
  Heltec.display->display();
  delay(1000);
}

//função principal em loop
void loop() { 

    //variaveis para pegar os dados da porta serial
    float a;
    byte dataArray[4] = {
      ((uint8_t*)&a)[0],
      ((uint8_t*)&a)[1],
      ((uint8_t*)&a)[2],
      ((uint8_t*)&a)[3]
    };
    
    ((uint8_t*)&a)[0] = dataArray[0];
    ((uint8_t*)&a)[1] = dataArray[1];
    ((uint8_t*)&a)[2] = dataArray[2];
    ((uint8_t*)&a)[3] = dataArray[3];

    //se houver dados pelo lora, le eles e atribui para currentTemp
  if (lora2.available()){
    dataArray[0] = lora2.read();
    dataArray[1] = lora2.read();
    dataArray[2] = lora2.read();
    dataArray[3] = lora2.read();

    ((uint8_t*)&a)[0] = dataArray[0];
    ((uint8_t*)&a)[1] = dataArray[1];
    ((uint8_t*)&a)[2] = dataArray[2];
    ((uint8_t*)&a)[3] = dataArray[3];

    currentTemp = a;
  
  }

  //Escreve as informações no display, definindo a fonte, alinhamento, posição, etc
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_16);
  
  Heltec.display->drawString(30, 5, "Enviando");
  Heltec.display->drawString(22, 30, String(currentTemp, 2));
  Heltec.display->drawString(78, 30, "°C");
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(22, 50, "Pacote: ");
  Heltec.display->setTextAlignment(TEXT_ALIGN_RIGHT);
  Heltec.display->drawString(95, 50, String(count));
  Heltec.display->display();

  sendPacket(); //Envia temperatura pelo lora
  
  count++; //aumenta em 1 o contador de pacotes
  
  delay(1000); //delay de 1 segundo antes de repetir o loop
  
}
