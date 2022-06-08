//inclusão de bibliotecas
#include <SoftwareSerial.h> //biblioteca para comunicação de Porta Serial
#include <SparkFun_RHT03.h> //Biblioteca do sensor de Temperatura rht03

//definição pinos Rx e Tx
#define pinRx 7
#define pinTx 8

const int PINR = A0; //definição do pino do sensor de temperatura
RHT03 rht; //inicialização de uma variável pro leitor de temperatura

SoftwareSerial lora(pinRx, pinTx); //inicialização da comunicação serial nas portal Rx e Tx definidas acima

//função de inicialização
void setup() {
  //inicialização da comunicação serial (monitor e porta)
  Serial.begin(115200); 
  lora.begin(115200);
  rht.begin(PINR); //inicialização sensor de temperatura
  
}

//função principal em loop
void loop() {
 
    //atualizar o sensor para pegar dados
    int updateRet = rht.update();

    //ler temperatura em Celsius
    float latestTempC = rht.tempC();

    //conversão do float para 4 bytes
    //Porta Serial só passa dados em bytes, portanto é necessário essa conversão
    byte dataArray[4] = {
      ((uint8_t*)&latestTempC)[0],
      ((uint8_t*)&latestTempC)[1],
      ((uint8_t*)&latestTempC)[2],
      ((uint8_t*)&latestTempC)[3]
    };

    //passar os 4 bytes da temperatura pelo Serial
    lora.flush();
    lora.write(dataArray[0]);
    lora.write(dataArray[1]);
    lora.write(dataArray[2]);
    lora.write(dataArray[3]);
    
    // Imprimir a temperatura no monitor Serial 
    Serial.print(" Temp (C): " + String(latestTempC, 1) + " deg C");
  
  
  delay(1000); //delay de 1 segundo antes de repetir o loop

}
