//inclusão de bibliotecas
#include "heltec.h" //bibliotaca do Lora 32 Heltec
#define BAND 915E6 //Escolha da frequência da banda. 915Mhz é o padrão no Brasil

//definição de variáveis globais
String packSize = "--";
String packet ;

//Protótipo das funções
void LoRaDataPrint();
void cbk(int packetSize);

/*
  Nome da função: LoRaDataPrint
  objetivo: imprime a temperatura e tamanho do pacote recebido no display.
*/
void LoRaDataPrint(){
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(0 , 1 , "Recebendo "+ packSize + " bytes");
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(15, 15, packet);
  Heltec.display->display();
}

/*
  Nome da função: cbk
  recebe como parâmetos um inteiros (packetSize)
  objetivo: recebe a temperatura via LoRa e armazena na variável packet.
*/
void cbk(int packetSize) {
  packet ="";
  packSize = String(packetSize,DEC);
  for (int i = 0; i < packetSize; i++) {
    packet += (char) LoRa.read(); //Atribui um caractere por vez a váriavel packet 
  }
  LoRaDataPrint();
}

//função de inicialização
void setup()
{
  pinMode(LED,OUTPUT); //inicializa o LED
  
  Serial.begin(115200); //inicializa o monitor serial

  //Inicialização do display 
  Heltec.begin(true /*Habilita o Display*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Habilita debug Serial*/, true /*Habilita o PABOOST*/, BAND /*Frequência BAND*/);
  Heltec.display->init();  
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->clear();
  Heltec.display->drawString(10, 5, "Iniciado com Sucesso!");
  Heltec.display->drawString(10, 30, "Aguardando os dados...");
  Heltec.display->display();
  Serial.println("Iniciado com Sucesso!");
  Serial.println("Aguardando os dados...");
  delay(1000);
  
  LoRa.receive(); // Habilita o rádio LoRa para receber dados
}

//função principal em loop
void loop() {
  
  int packetSize = LoRa.parsePacket(); //recebe os pacotes
  if (packetSize) { //Verifica se há dados chegando via LoRa
    cbk(packetSize);
    digitalWrite(LED, HIGH);   // Liga o LED
    delay(500);                // Espera 500 milissegundos
    digitalWrite(LED, LOW);    // Desliiga o LED
    delay(500);                // Espera 500 milissegundos
    Serial.print("Recebendo a temperatura: ");
    Serial.print(packet); //Imprime no monitor serial a temperatura
  }
  
  delay(10); //delay de 1 centésimo de segundo antes de repetir o loop
}
