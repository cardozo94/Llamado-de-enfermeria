#include <Wire.h>

const byte MY_ADDRESS = 25;
const byte SLAVE_ADDRESS = 42;

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
uint8_t letters[375];
uint8_t currentdisplay[8];

//dir del multiplexor
int dir;

volatile int t=20;
volatile int h=1;

// Serial read Byte
volatile int incomingByte = 100;
volatile int counter= 0;
//Serial write Byte
volatile int info=0;

void setup(){
    Wire.begin (MY_ADDRESS);
    Wire.onReceive (receiveEvent);
    
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

    attachInterrupt(0, Llamado, CHANGE);
    
      
    // Symbol [ ] space 
    letters[0] =  B00000000;
    letters[1] =  B00000000;
    letters[2] =  B00000000;
    letters[3] =  B00000000;
    letters[4] =  B00000000;
    letters[5] =  B00000000;
    letters[6] =  B00000000;
    letters[7] =  B00000000;

    // Symbol T
    letters[8]  =  B11111111;
    letters[9]  =  B11111111;
    letters[10] =  B10000000;
    letters[11] =  B10000000;
    letters[12] =  B10000000;
    letters[13] =  B00000000;
    letters[14] =  B00000000;
    letters[15] =  B00000000;

    // Symbol TR
    letters[16] =  B10000100;
    letters[17] =  B10000100;
    letters[18] =  B11111111;
    letters[19] =  B11111111;
    letters[20] =  B00000000;
    letters[21] =  B10000000;
    letters[22] =  B10000000;
    letters[23] =  B10000000;

    // Symbol RE
    letters[24] =  B10010001;
    letters[25] =  B11111111;
    letters[26] =  B11111111;
    letters[27] =  B00000000;
    letters[28] =  B00000000;
    letters[29] =  B00110001;
    letters[30] =  B01001010;
    letters[31] =  B10000100;

    // Symbol ES
    letters[32] =  B11110001;
    letters[33] =  B01100010;
    letters[34] =  B00000000;
    letters[35] =  B00000000;
    letters[36] =  B10000001;
    letters[37] =  B10010001;
    letters[38] =  B10010001;
    letters[39] =  B10010001;

    // Symbol S
    letters[40] =  B00000000;
    letters[41] =  B00000000;
    letters[42] =  B00000000;
    letters[43] =  B01001110;
    letters[44] =  B10011111;
    letters[45] =  B10010001;
    letters[46] =  B10010001;
    letters[47] =  B10010001;

     // Symbol CU
    letters[48] =  B11111100;
    letters[49] =  B00000000;
    letters[50] =  B00100100;
    letters[51] =  B01000010;
    letters[52] =  B10000001;
    letters[53] =  B10000001;
    letters[54] =  B01111110;
    letters[55] =  B00111100;

     // Symbol UA
    letters[56] =  B01111111;
    letters[57] =  B00111111;
    letters[58] =  B00000000;
    letters[59] =  B11111100;
    letters[60] =  B11111110;
    letters[61] =  B00000001;
    letters[62] =  B00000001;
    letters[63] =  B11111110;
    
    // Symbol AT
    letters[64] =  B11111111;
    letters[65] =  B10000000;
    letters[66] =  B10000000;
    letters[67] =  B00000000;
    letters[68] =  B00111111;
    letters[69] =  B01111111;
    letters[70] =  B10001000;
    letters[71] =  B10001000;
    
    // Symbol TR
    letters[72] =  B01010100;
    letters[73] =  B10001000;
    letters[74] =  B11111111;
    letters[75] =  B11111111;
    letters[76] =  B00000000;
    letters[77] =  B10000000;
    letters[78] =  B10000000;
    letters[79] =  B11111111;
    
    // Symbol RO
    letters[80] =  B00111100;
    letters[81] =  B01111110;
    letters[82] =  B10000001;
    letters[83] =  B10000001;
    letters[84] =  B01111110;
    letters[85] =  B00111100;
    letters[86] =  B00000000;
    letters[87] =  B00100011;
    
    // Symbol S
    letters[88] =  B10010001;
    letters[89] =  B10010001;
    letters[90] =  B10010001;
    letters[91] =  B11110001;
    letters[92] =  B01100010;
    letters[93] =  B00000000;
    letters[94] =  B00000000;
    letters[95] =  B00000000;
  
    // Symbol SE
    letters[96] =  B10010001;
    letters[97] =  B10010001;
    letters[98] =  B11111111;
    letters[99] =  B11111111;
    letters[100] =  B00000000;
    letters[101] =  B00000000;
    letters[102] =  B01001110;
    letters[103] =  B10011111;
    
    // Symbol EI
    letters[104] =  B10000001;
    letters[105] =  B10000001;
    letters[106] =  B00000000;
    letters[107] =  B00000000;
    letters[108] =  B00000000;
    letters[109] =  B10000001;
    letters[110] =  B10010001;
    letters[111] =  B10010001;
  
    // Symbol IS
    letters[112] =  B11110001;
    letters[113] =  B01100010;
    letters[114] =  B00000000;
    letters[115] =  B00000000;
    letters[116] =  B10000001;
    letters[117] =  B10000001;
    letters[118] =  B11111111;
    letters[119] =  B11111111;
    
    // Symbol S
    letters[120] =  B00000000;
    letters[121] =  B00000000;
    letters[122] =  B00000000;
    letters[123] =  B01001110;
    letters[124] =  B10011111;
    letters[125] =  B10010001;
    letters[126] =  B10010001;
    letters[127] =  B10010001;

    // Symbol O
    letters[128] =  B10000001;
    letters[129] =  B10000001;
    letters[130] =  B10000001;
    letters[131] =  B01111110;
    letters[132] =  B00111100;
    letters[133] =  B00000000;
    letters[134] =  B00000000;
    letters[135] =  B00000000;
    
    // Symbol OC
    letters[136] =  B10000001;
    letters[137] =  B10000001;
    letters[138] =  B01111110;
    letters[139] =  B00111100;
    letters[140] =  B00000000;
    letters[141] =  B00000000;
    letters[142] =  B00111100;
    letters[143] =  B01111110;
    
    // Symbol CH
    letters[144] =  B00010000;
    letters[145] =  B11111111;
    letters[146] =  B11111111;
    letters[147] =  B00000000;
    letters[148] =  B00000000;
    letters[149] =  B00100100;
    letters[150] =  B01000010;
    letters[151] =  B10000001;
  
    // Symbol HO
    letters[152] =  B01111110;
    letters[153] =  B00111100;
    letters[154] =  B00000000;
    letters[155] =  B00000000;
    letters[156] =  B11111111;
    letters[157] =  B11111111;
    letters[158] =  B00010000;
    letters[159] =  B00010000;

    // Symbol O
    letters[160] =  B00000000;
    letters[161] =  B00000000;
    letters[162] =  B00000000;
    letters[163] =  B00111100;
    letters[164] =  B01111110;
    letters[165] =  B10000001;
    letters[166] =  B10000001;
    letters[167] =  B10000001;
    
    // Symbol D
    letters[168] =  B10000001;
    letters[169] =  B10000001;
    letters[170] =  B10000001;
    letters[171] =  B11111111;
    letters[172] =  B11111111;
    letters[173] =  B00000000;
    letters[174] =  B00000000;
    letters[175] =  B00000000;
       
    // Symbol DI
    letters[176] =  B11111111;
    letters[177] =  B10000001;
    letters[178] =  B10000001;
    letters[179] =  B10000001;
    letters[180] =  B00000000;
    letters[181] =  B00000000;
    letters[182] =  B00111100;
    letters[183] =  B01000010;
    
    // Letter IE
    letters[184] = B10010001;
    letters[185] = B11111111;
    letters[186] = B11111111;
    letters[187] = B00000000;
    letters[188] = B10000001;
    letters[189] = B10000001;
    letters[190] = B10000001;
    letters[191] = B11111111;

    // Letter EZ
    letters[192] = B10000101;
    letters[193] = B10000011;
    letters[194] = B00000000;
    letters[195] = B00000000;
    letters[196] = B10000001;
    letters[197] = B10010001;
    letters[198] = B10010001;
    letters[199] = B10010001;

    // Letter Z
    letters[200] = B00000000;
    letters[201] = B00000000;
    letters[202] = B00000000;
    letters[203] = B10000001;
    letters[204] = B11000001;
    letters[205] = B10100001;
    letters[206] = B10010001;
    letters[207] = B10001001;
   
    // Symbol O
    letters[208] =  B10000001;
    letters[209] =  B10000001;
    letters[210] =  B10000001;
    letters[211] =  B01111110;
    letters[212] =  B00111100;
    letters[213] =  B00000000;
    letters[214] =  B00000000;
    letters[215] =  B00000000;
    
    // Symbol ON
    letters[216] = B00011000;
    letters[217] = B00100000;
    letters[218] = B01000000;
    letters[219] = B11111111;
    letters[220] = B11111111;
    letters[221] = B00000000;
    letters[222] = B00111100;
    letters[223] = B01111110;
  
    // Symbol NC
    letters[224] = B10000001;
    letters[225] = B01111110;
    letters[226] = B00111100;
    letters[227] = B00000000;
    letters[228] = B11111111;
    letters[229] = B11111111;
    letters[230] = B00000010;
    letters[231] = B00000100;

    // Symbol CE
    letters[232] = B11111111;
    letters[233] = B11111111;
    letters[234] = B00000000;
    letters[235] = B00000000;
    letters[236] = B00100100;
    letters[237] = B01000010;
    letters[238] = B10000001;
    letters[239] = B10000001;
  
    // Symbol E
    letters[240] = B00000000;
    letters[241] = B00000000;
    letters[242] = B00000000;
    letters[243] = B10000001;
    letters[244] = B10010001;
    letters[245] = B10010001;
    letters[246] = B10010001;
    letters[247] = B10010001;
    
    // Symbol D
    letters[248] =  B10000001;
    letters[249] =  B10000001;
    letters[250] =  B10000001;
    letters[251] =  B11111111;
    letters[252] =  B11111111;
    letters[253] =  B00000000;
    letters[254] =  B00000000;
    letters[255] =  B00000000;
  
    // Symbol DO
    letters[256] = B10000001;
    letters[257] = B10000001;
    letters[258] = B01111110;
    letters[259] = B00111100;
    letters[260] = B00000000;
    letters[261] = B00000000;
    letters[262] = B00111100;
    letters[263] = B01000010;
    
    // Symbol OC
    letters[264] = B10000001;
    letters[265] = B01111110;
    letters[266] = B00111100;
    letters[267] = B00000000;
    letters[268] = B00000000;
    letters[269] = B00111100;
    letters[270] = B01111110;
    letters[271] = B10000001;
  
    // Symbol CE
    letters[272] = B11111111;
    letters[273] = B11111111;
    letters[274] = B00000000;
    letters[275] = B00000000;
    letters[276] = B00100100;
    letters[277] = B01000010;
    letters[278] = B10000001;
    letters[279] = B10000001;
  
    // Symbol E
    letters[280] = B00000000;
    letters[281] = B00000000;
    letters[282] = B00000000;
    letters[283] = B10000001;
    letters[284] = B10010001;
    letters[285] = B10010001;
    letters[286] = B10010001;
    letters[287] = B10010001;
 
    // Letter C|
    letters[288] = B00000000;
    letters[289] = B00100100;
    letters[290] = B01000010;
    letters[291] = B10000001;
    letters[292] = B10000001;
    letters[293] = B10000001;
    letters[294] = B01111110;
    letters[295] = B00111100;
    
    // Letter D|
    letters[296] = B00000000;
    letters[297] = B00111100;
    letters[298] = B01000010;
    letters[299] = B10000001;
    letters[300] = B10000001;
    letters[301] = B10000001;
    letters[302] = B11111111;
    letters[303] = B11111111;

    // Letter E|
    letters[304] = B00000000;
    letters[305] = B10000001;
    letters[306] = B10000001;
    letters[307] = B10010001;
    letters[308] = B10010001;
    letters[309] = B10010001;
    letters[310] = B11111111;
    letters[311] = B11111111;

    // Letter I|
    letters[312] = B00000000;
    letters[313] = B10000001;
    letters[314] = B10000001;
    letters[315] = B11111111;
    letters[316] = B11111111;
    letters[317] = B10000001;
    letters[318] = B10000001;
    letters[319] = B00000000;

    // Letter N|
    letters[320] =  B11111111;
    letters[321] =  B11111110;
    letters[322] =  B00000100;
    letters[323] =  B00001000;
    letters[324] =  B00010000;
    letters[325] =  B00100000;
    letters[326] =  B01111111;
    letters[327] =  B11111111;

    // Letter O|
    letters[328] =  B00000000;
    letters[329] =  B00111100;
    letters[330] =  B01111110;
    letters[331] =  B10000001;
    letters[332] =  B10000001;
    letters[333] =  B10000001;
    letters[334] =  B01111110;
    letters[335] =  B00111100;
     
    // Letter R|
    letters[336] =  B00000000;
    letters[337] =  B00110001;
    letters[338] =  B01001010;
    letters[339] =  B10000100;
    letters[340] =  B10000100;
    letters[341] =  B10000100;
    letters[342] =  B11111111;
    letters[343] =  B11111111;

    // Letter S|
    letters[344] =  B00000000;
    letters[345] =  B01001110;
    letters[346] =  B10011111;
    letters[347] =  B10010001;
    letters[348] =  B10010001;
    letters[349] =  B10010001;
    letters[350] =  B11110001;
    letters[351] =  B01100110;
  
    // Letter T|
    letters[352] =  B10000000;
    letters[353] =  B10000000;
    letters[354] =  B10000000;
    letters[355] =  B11111111;
    letters[356] =  B11111111;
    letters[357] =  B10000000;
    letters[358] =  B10000000;
    letters[359] =  B10000000;
    
    // Letter U|
    letters[360] =  B00000000;
    letters[361] =  B11111100;
    letters[362] =  B11111110;
    letters[363] =  B00000001;
    letters[364] =  B00000001;
    letters[365] =  B00000001;
    letters[366] =  B11111110;
    letters[367] =  B11111100;
  
    // Letter V|
    letters[368] =  B00000000;
    letters[369] =  B11111000;
    letters[370] =  B11111100;
    letters[371] =  B00000010;
    letters[372] =  B00000001;
    letters[373] =  B00000010;
    letters[374] =  B11111100;
    letters[375] =  B11111000;

    for (int i = 0; i < 8; i++){
        led[i] = letters[i];
        currentdisplay[i] = letters[i];
      }
   } 
void loop(){
    borrar();
     if(h==0){
        dir=0;
         for(int i=0;i<15;i++){
              if(dir==0){
                      digitalWrite(4,LOW);
                      digitalWrite(5,LOW);
                      digitalWrite(6,LOW);
                      digitalWrite(3,LOW); 
                  }else if(dir==1){
                      digitalWrite(4,HIGH);
                      digitalWrite(5,LOW);
                      digitalWrite(6,LOW);
                      digitalWrite(3,LOW);
                  }else if(dir==2){
                      digitalWrite(4,LOW);
                      digitalWrite(5,HIGH);
                      digitalWrite(6,LOW);
                      digitalWrite(3,LOW);
                  }else if(dir==3){
                      digitalWrite(4,HIGH);
                      digitalWrite(5,HIGH);
                      digitalWrite(6,LOW);
                      digitalWrite(3,LOW);
                  }else if(dir==4){
                      digitalWrite(4,LOW);
                      digitalWrite(5,LOW);
                      digitalWrite(6,HIGH);
                      digitalWrite(3,LOW);
                  }else if(dir==5){
                      digitalWrite(4,HIGH);
                      digitalWrite(5,LOW);
                      digitalWrite(6,HIGH);
                      digitalWrite(3,LOW);
                  }else if(dir==6){
                      digitalWrite(4,LOW);
                      digitalWrite(5,HIGH);
                      digitalWrite(6,HIGH);
                      digitalWrite(3,LOW);
                  }else if(dir==7){
                      digitalWrite(4,HIGH);
                      digitalWrite(5,HIGH);
                      digitalWrite(6,HIGH);
                      digitalWrite(3,LOW);
                  }else if(dir==8){
                      digitalWrite(4,LOW);
                      digitalWrite(5,LOW);
                      digitalWrite(6,LOW);
                      digitalWrite(3,HIGH);
                      delay(100);
                  }else if(dir==9){
                      digitalWrite(4,HIGH);
                      digitalWrite(5,LOW);
                      digitalWrite(6,LOW);
                      digitalWrite(3,HIGH);
                  }else if(dir==10){
                      digitalWrite(4,LOW);
                      digitalWrite(5,HIGH);
                      digitalWrite(6,LOW);
                      digitalWrite(3,HIGH);
                  }else if(dir==11){
                      digitalWrite(4,HIGH);
                      digitalWrite(5,HIGH);
                      digitalWrite(6,LOW);
                      digitalWrite(3,HIGH);
                  }else if(dir==12){
                      digitalWrite(4,LOW);
                      digitalWrite(5,LOW);
                      digitalWrite(6,HIGH);
                      digitalWrite(3,HIGH);
                  }else if(dir==13){
                      digitalWrite(4,HIGH);
                      digitalWrite(5,LOW);
                      digitalWrite(6,HIGH);
                      digitalWrite(3,HIGH);
                  }else if(dir==14){
                      digitalWrite(4,LOW);
                      digitalWrite(5,HIGH);
                      digitalWrite(6,HIGH);
                      digitalWrite(3,HIGH);
                      delay(100);
                  }else if(dir==15){
                      digitalWrite(4,HIGH);
                      digitalWrite(5,HIGH);
                      digitalWrite(6,HIGH);
                      digitalWrite(3,HIGH);
                  }//else {}
              if(h==1){                
                  i=16;
                  dir=dir-1;            
              }//else{}
                  dir=dir+1; 
          }
     }
     counter=0;
     while(h==1){
          if(t==0){//CUARTO UNO
                  mensaje3(1);
           }else if (t==1){//CUARTO DOS
                  mensaje3(2);
            }else if (t==2){//CUARTO TRES
                  mensaje1(8);
            }else if (t==3){//CUARTO 4
                  mensaje1(48);
            }else if (t==4){//CUARTO 5
                  mensaje3(5);
            }else if (t==5){//CUARTO 6
                  mensaje1(88);
            }else if (t==6){//CUARTO 7
                  mensaje3(7);
            }else if (t==7){//CUARTO 8
                  mensaje1(128);
            }else if (t==8){//CUARTO 9 //no responde la interrupcion lo manda al 1 
                  mensaje3(9);
            }else if (t==9){//CUARTO 10
                  mensaje1(168);
            }else if (t==10){//CUARTO 11
                  mensaje1(208);
            }else if (t==11){//CUARTO 12
                  mensaje1(248);
            }else if (t==12){//CUARTO 13// manda la interrupcion al 1
                 mensaje3(13);
            }else if (t==13){//BAÑO 1
                  mensaje3(1);  
            }else if (t==14){//BAÑO 2
                  mensaje3(2);
            }else if (t==15){//BAÑO 3//La interrupcion no responde
                  //mensaje1(8);
            }else if (t==20){//Respuesta
                  borrar();        
                  h=0;
            }//else{}
            if(counter==0){
                Wire.beginTransmission (SLAVE_ADDRESS);
                Wire.write (info);
                Wire.endTransmission ();
                counter=1;
            }//else{}
    }
    //Espera 0.5 seg. para que la enfermera sepa que si lo apago. Y si hay un evento esperando suene despues.  
    if (counter == 0 && t == 20 && info == 10 && h == 0)
    {
        borrar();
        delay(500);
    }
  }

void borrar(){
    for(p=11;p>6;p--){
        for (int i=0; i <8 ; i++){
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
          currentdisplay[i-inicio] = ~letters[i];;
        } 
        screenUpdate();  
        inicio=inicio+8;
    }
  }  

void mensaje3(int inicio){
    if(inicio==1){
        p=11;
        for (int i = 0; i < 8; i++){
            led[i] = ~letters[i];
            currentdisplay[i] = ~letters[i];
          }
        screenUpdate();        
        p=10;
        for (int i = 360; i < 368; i++){
            led[i-360] = ~letters[i];
            currentdisplay[i-360] = ~letters[i];
          }
        screenUpdate();   
        p=9;
        for (int i = 320; i < 328; i++){
            led[i-320] = ~letters[i];
            currentdisplay[i-320] = ~letters[i];
          }
        screenUpdate();
        p=8;
        for (int i = 328; i < 336; i++){
            led[i-328] = ~letters[i];
            currentdisplay[i-328] = ~letters[i];
          }
        screenUpdate();
        p=7;
        for (int i = 0; i < 8; i++){
            led[i] = ~letters[i];
            currentdisplay[i] = ~letters[i];
          }
        screenUpdate();
    }
    else if(inicio==2){
        p=11;
        for (int i = 0; i < 8; i++){
            led[i] = ~letters[i];
            currentdisplay[i] = ~letters[i];
          }
        screenUpdate();        
        p=10;
        for (int i = 296; i < 304; i++){
            led[i-296] = ~letters[i];
            currentdisplay[i-296] = ~letters[i];
          }
        screenUpdate();   
        p=9;
        for (int i = 328; i < 336; i++){
            led[i-328] = ~letters[i];
            currentdisplay[i-328] = ~letters[i];
          }
        screenUpdate();
        p=8;
        for (int i = 344; i < 352; i++){
            led[i-344] = ~letters[i];
            currentdisplay[i-344] = ~letters[i];
          }
        screenUpdate();
        p=7;
        for (int i = 0; i < 8; i++){
            led[i] = ~letters[i];
            currentdisplay[i] = ~letters[i];
          }
        screenUpdate();
    }else if(inicio==5){
        p=11;
        for (int i = 288; i < 296; i++){
            led[i-288] = ~letters[i];
            currentdisplay[i-288] = ~letters[i];
          }
        screenUpdate();        
        p=10;
        for (int i = 312; i < 320; i++){
            led[i-312] = ~letters[i];
            currentdisplay[i-312] = ~letters[i];
          }
        screenUpdate();   
        p=9;
        for (int i = 320; i < 328; i++){
            led[i-320] = ~letters[i];
            currentdisplay[i-320] = ~letters[i];
          }
        screenUpdate();
        p=8;
        for (int i = 288; i < 296; i++){
            led[i-288] = ~letters[i];
            currentdisplay[i-288] = ~letters[i];
          }
        screenUpdate();
        p=7;
        for (int i = 328; i < 336; i++){
            led[i-328] = ~letters[i];
            currentdisplay[i-328] = ~letters[i];
          }
        screenUpdate();
    }
    else if(inicio==7){
        p=11;
        for (int i = 344; i <352; i++){
            led[i-344] = ~letters[i];
            currentdisplay[i-344] = ~letters[i];
          }
        screenUpdate();        
        p=10;
        for (int i = 312; i < 320; i++){
            led[i-312] = ~letters[i];
            currentdisplay[i-312] = ~letters[i];
          }
        screenUpdate();   
        p=9;
        for (int i = 304; i < 312; i++){
            led[i-304] = ~letters[i];
            currentdisplay[i-304] = ~letters[i];
          }
        screenUpdate();
        p=8;
        for (int i = 352; i < 360; i++){
            led[i-352] = ~letters[i];
            currentdisplay[i-352] = ~letters[i];
          }
        screenUpdate();
        p=7;
        for (int i = 304; i < 312; i++){
            led[i-304] = ~letters[i];
            currentdisplay[i-304] = ~letters[i];
          }
        screenUpdate();
    }
    else if(inicio==9){
        p=11;
        for (int i = 320; i < 328; i++){
            led[i-320] = ~letters[i];
            currentdisplay[i-320] = ~letters[i];
          }
        screenUpdate();        
        p=10;
        for (int i = 360; i < 368; i++){
            led[i-360] = ~letters[i];
            currentdisplay[i-360] = ~letters[i];
          }
        screenUpdate();   
        p=9;
        for (int i = 304; i < 312; i++){
            led[i-304] = ~letters[i];
            currentdisplay[i-304] = ~letters[i];
          }
        screenUpdate();
        p=8;
        for (int i = 368; i < 376; i++){
            led[i-368] = ~letters[i];
            currentdisplay[i-368] = ~letters[i];
          }
        screenUpdate();
        p=7;
        for (int i = 304; i < 312; i++){
            led[i-304] = ~letters[i];
            currentdisplay[i-304] = ~letters[i];
          }
        screenUpdate();
    }
    else if(inicio==13){
        p=11;
        for (int i = 352; i < 360; i++){
            led[i-352] = ~letters[i];
            currentdisplay[i-352] = ~letters[i];
          }
        screenUpdate();        
        p=10;
        for (int i = 336; i < 344; i++){
            led[i-336] = ~letters[i];
            currentdisplay[i-336] = ~letters[i];
          }
        screenUpdate();   
        p=9;
        for (int i = 304; i < 312; i++){
            led[i-304] = ~letters[i];
            currentdisplay[i-304] = ~letters[i];
          }
        screenUpdate();
        p=8;
        for (int i = 288; i < 296; i++){
            led[i-288] = ~letters[i];
            currentdisplay[i-288] = ~letters[i];
          }
        screenUpdate();
        p=7;
        for (int i = 304; i < 312; i++){
            led[i-304] = ~letters[i];
            currentdisplay[i-304] = ~letters[i];
          }
        screenUpdate();
    }
    else{}
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
   
void Llamado()
   {
    if ( millis() > T0  + 250){
       m=digitalRead(2);
       if (m==HIGH){//LAMADO
          h=1;
          if(dir==0){//CUARTO UNO
              t=dir;
              info=9;
            }else if (dir==1){//CUARTO DOS
                 t=dir;
                 info=9;
            }else if (dir==2){//CUARTO TRES
                 t=dir;
                 info=9;
            }else if (dir==3){//CUARTO 4
                  t=dir;
                  info=9;
            }else if (dir==4){//CUARTO 5
                  t=dir;
                  info=9;
            }else if (dir==5){//CUARTO 6
                  t=dir;
                  info=9;
            }else if (dir==6){//CUARTO 7
                  t=dir;
                  info=9;
            }else if (dir==7){//CUARTO 8
                  t=dir;
                  info=9;
            }else if (dir==8){//CUARTO 9
                  //t=dir;
            }else if (dir==9){//CUARTO 10
                  t=dir;
                  info=9;
            }else if (dir==10){//CUARTO 11
                  t=dir;
                  info=9;
            }else if (dir==11){//CUARTO 12
                  t=dir;
                  info=9;
            }else if (dir==12){//CUARTO 13
                  //t=dir;
            }else if (dir==13){//BAÑO 1
                  t=dir;
                  info=13;
            }else if (dir==14){//BAÑO 2
                  t=dir;
                  info=13;
            }else if (dir==15){//BAÑO 3
                  //t=dir;
            }//else{}        
       }
       if (m==LOW){//Respuesta, la enfermera oprime por segunda vez el pulsador para desactivar la alarma
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
            if(incomingByte==9){//Cuarto 9
                  h=1;
                  t=8;
            }
            else if (incomingByte==13){//cuarto 13
                  h=1;
                  t=12;
            }
            else if (incomingByte==3){//Baño 3
                  h=1;
                  t=2;
            }
            else if (incomingByte==4){//Baño 4
                  h=1;
                  t=3;
            }
            else if (incomingByte==5){//Baño 5
                  h=1;
                  t=4;
            }
            else if (incomingByte==6){//Baño 6
                  h=1;
                  t=5;
            }else{
                  borrar();
                  t=20;
            }
      }
      else if (h==1){
             if(incomingByte==9){//Cuarto 9
            }
            else if (incomingByte==13){//cuarto 13
            }
            else if (incomingByte==3){//Baño 3
            }
            else if (incomingByte==4){//Baño 4
            }
            else if (incomingByte==5){//Baño 5
            }
            else if (incomingByte==6){//Baño 6
            }else{
                  borrar();
                  t=20;
            }
      }
      else{
            borrar();
            t=20;
      }
} // end of receiveEvent
