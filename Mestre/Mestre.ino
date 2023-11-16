#include <HardwareSerial.h>

HardwareSerial Escravo1(1);
HardwareSerial Escravo2(2);

int PinSend = 9;
int PinSel = 10;

char Flag = '0';
char LigaLed = '0';
String readString = " ";
int temp = 0;
int EscravoRetain = 0;
int SlaveSel1 = 0;

struct HDLCFrame {
  char flag[8];         
  char address[8];      
  char control[16];     
  char information[10]; 
  char fcs[32];         
};

HDLCFrame frame;

void CreateFrame (char Flag, int Escravo, char LigaLed) { 

  for (int i = 0; i < 8; i++) {
    frame.flag[i] = '0'; 
  }  

  frame.flag[3] = Flag;

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

  for (int i = 0; i < 16; i++) {
    frame.control[i] = '0';    
  }  
  
  for (int i = 0; i < 10; i++) {
    frame.information[i] = '0';  // Preenchendo com '0'
  }
  frame.information[9] = LigaLed;

  for (int i = 0; i < 32; i++) {
    frame.fcs[i] = '0';
  }
  frame.fcs[32] = '\0';  // Adicionando o caractere nulo no final

  Escravo1.write(frame.flag);
  Escravo1.println();
  
  Escravo2.write(frame.flag);
  Escravo2.println();
}

int SlaveSel()
{
  int Escravo = digitalRead(PinSel);  

  if(Escravo == 1 & EscravoRetain == 0){
    Serial.println("### Escravo 2 - Selecionado ###");
    EscravoRetain = 1;
  }else{
    if(Escravo == 1){
      Serial.println("### Escravo 1 - Selecionado ###");
      EscravoRetain = 0;
    }
  }
  delay(500);

  return EscravoRetain;
}
  
void setup() {  
  Serial.begin(115200);
  Escravo1.begin(115200, SERIAL_8N1, 16, 17); // RX, TX
  Escravo2.begin(115200, SERIAL_8N1, 18, 19); // RX, TX
  pinMode(PinSel, INPUT);
  pinMode(PinSend, INPUT);  
}

void loop() {

  SlaveSel1 = SlaveSel(); 

  if(digitalRead(PinSend) == 1){ 

    Flag = '1';
    LigaLed = '1'; 
    Serial.print("Frame Enviado para o  Escravos!");
    Serial.println();
    CreateFrame(Flag,SlaveSel1,LigaLed);


    if(SlaveSel1 == 0){      

      Serial.print("### Aguarda Retorno Escravo 01! ###");
      delay(2000);

      while (Escravo1.available()){
        delay(1);   
        if(Escravo1.available()>0){      
          readString = Escravo1.readString();
        }
      }

    }

    else{

      Serial.print("### Aguarda Retorno Escravo 02! ###");
      delay(2000);

      while (Escravo2.available()){
        delay(1);   
        if(Escravo2.available()>0){      
          readString = Escravo2.readString();
        }
      }
    }    

    if(readString != " "){
      Serial.println();
      Serial.print("##### Recebido Escravo ####### ");  
      Serial.print(readString);
      Serial.print(" #####");
    }
    else{
      Serial.println();
      Serial.print("##### Falha - Não recebido Informacao Escravo! ####### ");    
    } 

    LigaLed = '0';
    CreateFrame(Flag,SlaveSel1,LigaLed);  
    delay(1000);

    if(readString[62] == '0'){         
      Serial.println();
      Serial.print("##### Comunicacao OK - Fim Comunicacao #####");
      Serial.println();  

    }else{
      Serial.println();
      Serial.print("##### Fault - Erro Retorno Escravo!!!!! #####");
      Serial.println();

    }
    temp = 0;
    readString = " ";
    
  }else{ 
    if(digitalRead(PinSend) == 0 & temp == 0){
      Serial.println("Pressione Botao Enviar"); 
      temp = 1;
      Flag = '0';
      LigaLed = '0';
      readString = " ";
    }   
  }
}
