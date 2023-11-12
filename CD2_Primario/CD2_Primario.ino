//  *********************************************
//                    MESTRE
//  *********************************************
#include <SoftwareSerial.h>
SoftwareSerial Escravo1(2, 3); // RX, TX
SoftwareSerial Escravo2(4, 5); // RX, TX

// Pino Entrada - Selecao Escravo
int PinSend = 9;
int PinSel = 10;

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

void CreateFrame (char Flag, int Escravo) { 

  //**********************************//
  //*********** Monta Flag ***********//  
  for (int i = 0; i < 8; i++) {
    frame.flag[i] = '0'; 
  }  
  frame.flag[3] = Flag;
  frame.flag[8] = '\0';
   //**********************************//
  //*********** Monta Endereço  *****//
  for (int i = 0; i < 8; i++) {
    frame.address[i] = '0';
    
  }  
  if(Escravo == 0){
  	frame.address[0] = '1';
    frame.address[1] = '0';
   }
  else{
    frame.address[0] = '0';
    frame.address[1] = '1';
   }  
  //**********************************//
  //*********** Monta Controle  *****//
  
  for (int i = 0; i < 16; i++) {
    frame.control[i] = '0';    
  }  
  //**********************************//
  //********** Monta Informação *****//
  
  for (int i = 0; i < 10; i++) {
    frame.information[i] = '0';  // Preenchendo com '0'
  }
  frame.information[0] = Flag;
  frame.information[9] = '\0';  // Adicionando o caractere nulo no final  

  
  //**********************************//
  //************* Monta fcs *********//
  
  for (int i = 0; i < 32; i++) {
    frame.fcs[i] = '0';
    }
  frame.fcs[31] = '\0';  // Adicionando o caractere nulo no final

  //*********** Imprime Frame ***********//
  if(Escravo == 0){
    Escravo1.print(frame.flag);
    //*Escravo1.print(frame.address);
    //*Escravo1.print(frame.control);
    //*Escravo1.print(frame.information);
    //*Escravo1.print(frame.fcs);
  }else{
    Escravo2.print(frame.flag);
    //*Escravo2.print(frame.address);
    //*Escravo2.print(frame.control);
    //*Escravo2.print(frame.information);
    //*Escravo2.print(frame.fcs);
  }


}

int SlaveSel()
{
  int Escravo = digitalRead(PinSel);
  return Escravo;
}
  
void setup() {  
  Serial.begin(9600);
  Escravo1.begin(9600);
  Escravo2.begin(9600);
  pinMode(PinSel, INPUT);
  pinMode(PinSend, INPUT);
}

void loop() {

  int SlaveSel1 = SlaveSel();
  
  if(digitalRead(PinSend) == 1){
    
    Flag = '1'; 
    Serial.print("Frame Enviado para o  Escravo: ");
	  CreateFrame(Flag,SlaveSel1);
    Serial.println();

    if(SlaveSel1 == 0){
      //Recebe Informações Escravos
      Serial.print("Aguarda Retorno Escravo: ");
      while (Escravo1.available()){   
        readString = Escravo1.readString();  
    }}else{
      //Recebe Informações Escravos
      Serial.print("Aguarda Retorno Escravo: ");
      while (Escravo2.available()){   
        readString = Escravo2.readString();
    }}


    Serial.println();
    Serial.print("Recebido Escravo");  
    Serial.print(readString);    
    delay(3000);

    Flag = '0';

    if(readString[49] == 0){    
      Serial.println();
      Serial.print("Comunicacao OK ");
      Serial.println();     

    }else{
      Serial.println("Erro Retorno Escravo ");

    }
    Serial.print("Frame Enviado para o  Escravo: ");
	  CreateFrame(Flag,SlaveSel1);
    Serial.println();
    delay(3000);

    readString = " ";
    
  }else{
    delay(3000);
    //Flag = '1';
    //CreateFrame(Flag,SlaveSel1);
    Serial.println("Pressione Botao Enviar");
    
  }

  readString = " ";

}