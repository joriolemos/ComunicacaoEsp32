#include <HardwareSerial.h>

HardwareSerial Mestre(1);

int PinFault = 10;
int LED = 13;

char Flag = '0';
String readString = " ";
int temp = 0;

struct HDLCFrame {
  char flag[8];         
  char address[8];      
  char control[16];     
  char information[10]; 
  char fcs[32];        
};

HDLCFrame frame;

void CreateFrame (int Fault) { 

  for (int i = 0; i < 8; i++) {
    frame.flag[i] = '0';      // Preenchendo com '0'  
  }  

  for (int i = 0; i < 8; i++) {
    frame.address[i] = '0';     // Preenchendo com '0'     
  } 

  for (int i = 0; i < 16; i++) {
    frame.control[i] = '0';     // Preenchendo com '0'    
  }  

  for (int i = 0; i < 10; i++) {
    frame.information[i] = '0';  // Preenchendo com '0'
  }  
  
  for (int i = 0; i < 32; i++) {
    frame.fcs[i] = '0';
    }
  
  if(Fault == 0){
    frame.fcs[20] = '0';	 
  }else{
    frame.fcs[20] = '1';    
  }    
  frame.fcs[32] = '\0';  // Adicionando o caractere nulo no final

  Mestre.write(frame.flag);
  Mestre.println();
}

int GenerateFault()
{
  int Fault = digitalRead(PinFault);
  return Fault;
}
  
void setup() {  
  Serial.begin(115200);
  Mestre.begin(115200, SERIAL_8N1, 16, 17); // RX, TX
  pinMode(PinFault, INPUT);
  pinMode(LED,OUTPUT);
}

void loop() { 

  int GenerateFault1 = GenerateFault();
  
  while (Mestre.available()){
    delay(1);  
    if(Mestre.available()>0){      
      readString = Mestre.readString();
    }
  }

  if(readString != " " & readString[8] == '1'){
    Serial.println();    
    Serial.print(readString);         
  }

  if(readString[3] == '1' & GenerateFault1 == 0 ){   

    if (readString[8] == '1') { 
      Serial.println();
      Serial.println("SERIAL - Comando Recebido do Mestre");
      digitalWrite(LED, HIGH);

      if(temp == 0){
        CreateFrame(GenerateFault1);
        delay(1);
        temp = 1;     
      }

      if (readString[41] == '0') {
        Serial.println("Desliga LED ");
        digitalWrite(LED, LOW);
        temp = 0;
      }else{
        digitalWrite(LED, HIGH);
        Serial.println("Liga LED "); 
      }
    } else
    {
      digitalWrite(LED, LOW);
    }
  }
  if (readString[3] == '1' & readString[8] == '1' & GenerateFault1 == 1){
    Serial.println("#Retorno Falha#");
    CreateFrame(GenerateFault1);    
    digitalWrite(LED, LOW);
    delay(1);
  }

  readString = " ";
  delay(1);

}
