// Control an 8x8 LED display with 2 x 74HC595 shift registers

#include <Wire.h>

const byte MY_ADDRESS = 42;
const byte OTHER_ADDRESS = 25;

long T0=0;  // Variable global para tiempo

//estado de la interrupcion
boolean m;

//Pin connected to Pin 12 of 74HC595 (Latch)
int latchPin = 12;

//Pin connected to Pin 11 of 74HC595 (Clock)
int clockPin = 13;

// bandera para los displays
int p;

uint8_t led[8];
uint8_t letters[87];
uint8_t currentdisplay[8];

//dir del multiplexor
int dir;

//para el timbre que no esste siempre encendido
int reloj=0;

volatile int t=20;
volatile int h=1; 

// Serial read Byte
int incomingByte = 100;
int counter = 0;

//Serial write Byte
volatile int info=0;


void setup() {
    Wire.begin (MY_ADDRESS);
    Wire.onReceive (receiveEvent);  
    //set pins to output
    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(11, OUTPUT);
    pinMode(10,OUTPUT);
    pinMode(9,OUTPUT);
    pinMode(8,OUTPUT);
    pinMode(7,OUTPUT);
    pinMode(6,OUTPUT);
    pinMode(5,OUTPUT);
    pinMode(4,OUTPUT);
    pinMode(3,OUTPUT);
    pinMode(2,INPUT);
    
    attachInterrupt( 0, Llamado, CHANGE);
    
    // Symbol [ ] space 
    letters[0] =  B00000000;
    letters[1] =  B00000000;
    letters[2] =  B00000000;
    letters[3] =  B00000000;
    letters[4] =  B00000000;
    letters[5] =  B00000000;
    letters[6] =  B00000000;
    letters[7] =  B00000000;

    // Symbol H
    letters[8]  =  B00000000;
    letters[9]  =  B11111111;
    letters[10] =  B11111111;
    letters[11] =  B00010000;
    letters[12] =  B00010000;
    letters[13] =  B00010000;
    letters[14] =  B11111111;
    letters[15] =  B11111111;
  

    // Symbol A
    letters[16] =  B00000000;
    letters[17] =  B00111111;
    letters[18] =  B01111111;
    letters[19] =  B10001000;
    letters[20] =  B10001000;
    letters[21] =  B10001000;
    letters[22] =  B01111111;
    letters[23] =  B00111111;

    // Symbol B
    letters[24] =  B00000000;
    letters[25] =  B01110110;
    letters[26] =  B10001001;
    letters[27] =  B10001001;
    letters[28] =  B10001001;
    letters[29] =  B10001001;
    letters[30] =  B11111111;
    letters[31] =  B11111111;

    // Symbol I
    letters[32] =  B00000000;
    letters[33] =  B10000001;
    letters[34] =  B10000001;
    letters[35] =  B11111111;
    letters[36] =  B11111111;
    letters[37] =  B10000001;
    letters[38] =  B10000001;
    letters[39] =  B00000000;

    // Symbol T
    letters[40] =  B10000001;
    letters[41] =  B10000000;
    letters[42] =  B10000000;
    letters[43] =  B11111111;
    letters[44] =  B11111111;
    letters[45] =  B10000000;
    letters[46] =  B10000000;
    letters[47] =  B10000000;

    // Symbol B
    letters[48] =  B10010001;
    letters[49] =  B10010001;
    letters[50] =  B10010001;
    letters[51] =  B11111111;
    letters[52] =  B11111111;
    letters[53] =  B00000000;
    letters[54] =  B00000000;
    letters[55] =  B00000000;
     
    // Symbol BA
    letters[56] =  B10001000;
    letters[57] =  B10001000;
    letters[58] =  B01111111;
    letters[59] =  B00111111;
    letters[60] =  B00000000;
    letters[61] =  B00000000;
    letters[62] =  B01101110;
    letters[63] =  B10010001;
    
    // Symbol AÑ
    letters[64] =  B10010000;
    letters[65] =  B10100000;
    letters[66] =  B01111111;
    letters[67] =  B01111111;
    letters[68] =  B00000000;
    letters[69] =  B00111111;
    letters[70] =  B01111111;
    letters[71] =  B10001000;
  
    // Symbol ÑO
    letters[72] =  B01111110;
    letters[73] =  B00111100;
    letters[74] =  B00000000;
    letters[75] =  B01111111;
    letters[76] =  B01111111;
    letters[77] =  B10000010;
    letters[78] =  B10000100;
    letters[79] =  B10001000;

    // Symbol O
    letters[80] =  B00000000;
    letters[81] =  B00000000;
    letters[82] =  B00000000;
    letters[83] =  B00111100;
    letters[84] =  B01111110;
    letters[85] =  B10000001;
    letters[86] =  B10000001;
    letters[87] =  B10000001;
    letters[87] =  B10000001;


    for (int i = 0; i < 8; i++){
        led[i] = letters[i];
        currentdisplay[i] = letters[i];
      }
}

void loop() {
      borrar();
      reloj=0;
      if(h==0){
         dir=0;
         for(int i=0;i<7;i++){
          if(dir==0){
                      digitalWrite(4,LOW);
                      digitalWrite(5,LOW);
                      digitalWrite(6,LOW);  
              }else if(dir==1){
                      digitalWrite(4,HIGH);
                      digitalWrite(5,LOW);
                      digitalWrite(6,LOW);
              }else if(dir==2){    
                      digitalWrite(4,LOW);
                      digitalWrite(5,HIGH);
                      digitalWrite(6,LOW);
              }else if(dir==3){
                      digitalWrite(4,HIGH);
                      digitalWrite(5,HIGH);
                      digitalWrite(6,LOW);
              }else if(dir==4){
                      digitalWrite(4,LOW);
                      digitalWrite(5,LOW);
                      digitalWrite(6,HIGH);
                      delay(100);
              }else if(dir==5){
                      digitalWrite(4,HIGH);
                      digitalWrite(5,LOW);
                      digitalWrite(6,HIGH);
              }else if(dir==6){
                      digitalWrite(4,LOW);
                      digitalWrite(5,HIGH);
                      digitalWrite(6,HIGH);
                      delay(100);
              }//else{}
              if(h==1){//poner un else          
                  i=50;
                  dir=dir-1;            
              }//else{}
              dir=dir+1;
          }
     }//else{}
     counter=0;
     while(h==1){
         if(t==0){//CUARTO 9
                  mensaje1(8);
            }
            else if (t==1){//CUARTO 13
                  mensaje1(8);
            }else if (t==2){//BAÑO 3
                  mensaje1(48);
            }else if (t==3){//BAÑO 4
                  mensaje1(48);
            }else if (t==4){//BAÑO No quieres servir
                  //mensaje1(48);
            }else if (t==5){//BAÑO 5 
                  mensaje1(48);
            }else if (t==6){//BAÑO 6
                  mensaje1(48);
            }else if (t==20){//Respuesta
                  borrar();        
                  h=0;
            }//else{} 
           if(counter==0){
                  Wire.beginTransmission (OTHER_ADDRESS);
                  Wire.write (info);
                  Wire.endTransmission ();
                  counter=1;
             }// else{}
           if(reloj<15){
              digitalWrite(3,HIGH);
              reloj=reloj+1;
          }else{
              reloj=reloj+1;
              digitalWrite(3,LOW);
              if(reloj>40){
                  reloj=0;
              }//else{}
          }      
     }
   digitalWrite(3,LOW);  
   reloj=0; 
   borrar();
  }

void borrar(){
  for(p=11;p>6;p--){
      for (int i=0; i <8; i++){
        led[i] = ~letters[i];
        currentdisplay[i] = ~letters[i];;
      } 
      screenUpdate();
  }
}   
void mensaje1(int inicio){
    for(p=11;p>6;p--){
        for (int i=inicio; i <(inicio+8); i++){
          led[i-inicio] = ~letters[i];
          currentdisplay[i-inicio] = ~letters[i];
        } 
        screenUpdate();  
        inicio=inicio+8;
    }
}

void screenUpdate() {
  uint8_t row = B00000001; //fila
  for (byte k = 0; k < 8; k++) {
      // Open up the latch ready to receive data
      digitalWrite(latchPin, LOW);
      shiftIt(row,p );                  
      shiftIt(led[k],p); // LED array
      // Close the latch, sending the data in the registers out to the matrix
      digitalWrite(latchPin, HIGH);
      row = row << 1;      
      }
 }

void shiftIt(byte dataOut,int pin) {
  // Shift out 8 bits LSB first,
  // on rising edge of clock
  
  boolean pinState;
  //clear shift register read for sending data
  digitalWrite(pin, LOW);

  // for each bit in dataOut send out a bit
  for (int i=7; i>=0; i--) {

      //set clockPin to LOW prior to sending bit
      digitalWrite(clockPin, LOW);

      // if the value of DataOut and (logical AND) a bitmask
      // are true, set pinState to 1 (HIGH)
      if ( dataOut & (1<<i) ) {
          pinState = HIGH;
        }
      else {
          pinState = LOW;
        }

      //sets dataPin to HIGH or LOW depending on pinState
      digitalWrite(pin, pinState);
  
      //send bit out on rising edge of clock
      digitalWrite(clockPin, HIGH);
      digitalWrite(pin, LOW);
  }

  //stop shifting
  digitalWrite(clockPin, LOW);
  
}

  void Llamado(){
    if ( millis() > T0  + 250){
       m=digitalRead(2);
       if (m==HIGH){//LAMADO
          h=1;
          if(dir==0){//CUARTO 9
                t=dir;
                info=9;
            }
          else if (dir==1){//CUARTO 13
                  t=dir;
                  info=13;
            }
          else if (dir==2){//BAÑO 3 
                 t=dir;
                  info=3;
            }
          else if (dir==3){//BAÑO 4
                  t=dir;
                  info=4;
            }
          else if (dir==4){//BAÑO 
                  //t=dir;
            }
          else if (dir==5){//BAÑO 5
                  t=dir;
                  info=5;
            }
          else if (dir==6){//BAÑO 6
                  t=dir;
                  info=6;
           }//else{}       
       }
       if(m==LOW) {//Respuesta
            t=20;
            h=0;
            info=10; 
            counter=0;             
         }//else{}
       T0 = millis();
    }//else{}
   }

   
void receiveEvent (int incomingByte){
      incomingByte=Wire.read();
      if(h==0){
            if(incomingByte==9){
                  h=1;
                  t=0;
            }
            else if (incomingByte==13){
                  h=1;
                  t=2;
            }else{
                  borrar();
            }
      }
      else if (h==1){
             if(incomingByte==9){
            }
            else if (incomingByte==13){
            }else{
                  borrar();
                  t=20;
            }
      }//else{}
} // end of receiveEvent
