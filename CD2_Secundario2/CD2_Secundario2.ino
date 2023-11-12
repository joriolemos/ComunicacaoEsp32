//  *********************************************
//                    ESCRAVO 2
//  *********************************************
#include <SoftwareSerial.h>
SoftwareSerial Mestre(4, 5); // RX, TX

// Pino Entrada - Selecao Escravo
int PinFault = 10;
int LED = 13;

// Variaveis Globais
char Flag = '0';
String readString;

struct HDLCFrame {
  char flag[8];         
  char address[8];      
  char control[16];     
  char information[10]; 
  char fcs[32];        
};

 HDLCFrame frame;

void CreateFrame (int Fault) { 

  //**********************************//
  //*********** Monta Flag ***********//  
  for (int i = 0; i < 8; i++) {
    frame.flag[i] = '0';      // Preenchendo com '0'  
  }  
  //**********************************//
  //*********** Monta Endereço  *****//
  for (int i = 0; i < 8; i++) {
    frame.address[i] = '0';     // Preenchendo com '0'     
  } 
  //**********************************//
  //*********** Monta Controle  *****//  
  for (int i = 0; i < 16; i++) {
    frame.control[i] = '0';     // Preenchendo com '0'    
  }  
  //**********************************//
  //********** Monta Informação *****//  
  for (int i = 0; i < 9; i++) {
    frame.information[i] = '0';  // Preenchendo com '0'
  }
  frame.information[9] = '\0';  // Adicionando o caractere nulo no final  
  //**********************************//
  //************* Monta fcs *********//
  
  for (int i = 0; i < 32; i++) {
    frame.fcs[i] = '0';
    }
  
  if(Fault == 0){
    frame.fcs[7] = '0';	 
  }else{
    frame.fcs[7] = '1';    
  }    
  frame.fcs[31] = '\0';  // Adicionando o caractere nulo no final

  //*********** Imprime Frame - Envia para Info. Mestre ***********//
  Mestre.print(frame.flag);
  //Mestre.print(frame.address);
  //Mestre.print(frame.control);
  //Mestre.print(frame.information);
  //Mestre.print(frame.fcs);
}

int GenerateFault()
{

  int Fault = digitalRead(PinFault);
  return Fault;

}

  
void setup() {  
  //SerialPort.begin(9600); 
  Serial.begin(9600);
  Mestre.begin(9600);
  pinMode(PinFault, INPUT);
  pinMode(LED,OUTPUT);
}

void loop() { 

  //Serial.println("loop");
  //Recebe Informações Mestre
  Serial.println("Aguarda Envio  Mestre!");  
  while (Mestre.available()){
    readString = Mestre.readString();
  }
  //Recebe Flag
  if(readString[3] == '1'){
    
    Serial.println();
    Serial.print("Frame Recebido do Mestre: ");
    Serial.print(readString);
    delay(3000);     
    //Recebe Flag - Direcionada para Escravo
    if (readString[10] == '1') {
      int GenerateFault1 = GenerateFault();

      Serial.println();
      Serial.println("Frame Enviado para o Mestre: ");

      digitalWrite(LED, HIGH);
      CreateFrame(GenerateFault1);
      delay(3000);
      //Recebe Informacao - Liga - Desliga Led

      if (readString[33] == '0') {
        digitalWrite(LED, LOW);
      }else{
        digitalWrite(LED, HIGH);   
      }
    } else
    {
      //Escravo N Selecionado.
      digitalWrite(LED, LOW);
    }
  }
  readString = " ";

}