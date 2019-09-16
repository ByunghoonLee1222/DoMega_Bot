/*  Dynamixel SyncWrite
 
 This example shows same movement as previous syncwrite
 
                 Compatibility
 CM900                  O
 OpenCM9.04             O
 
                  Dynamixel Compatibility
               AX    MX      RX    XL-320    Pro
 CM900          O      O      O        O      X
 OpenCM9.04     O      O      O        O      X
 **** OpenCM 485 EXP board is needed to use 4 pin Dynamixel and Pro Series ****  
 
 created 16 Nov 2012
 by ROBOTIS CO,.LTD.
 */
/* Dynamixel ID defines */
#define ID_NUM_1  61
#define ID_NUM_2  62
#define ID_NUM_3  63
#define ID_NUM_4  64
#define ID_NUM_5  31
#define ID_NUM_6  32
#define ID_NUM_7  33
#define ID_NUM_8  34
#define sam 150
#define MOTORS 8

#define GOAL_SPEED 32
#define CCW_Angle_Limit 8

/* Control table defines */
#define P_GOAL_POSITION    30
#define P_GOAL_SPEED    32

/********* Sync write data  **************
 * ID1, DATA1, DATA2..., ID2, DATA1, DATA2,...
 ******************************************
 */
 /* Serial device defines for dxl bus */
#define DXL_BUS_SERIAL1 1  //Dynamixel on Serial1(USART1)  <-OpenCM9.04
#define DXL_BUS_SERIAL2 2  //Dynamixel on Serial2(USART2)  <-LN101,BT210
#define DXL_BUS_SERIAL3 3  //Dynamixel on Serial3(USART3)  <-OpenCM 485EXP

Dynamixel Dxl(DXL_BUS_SERIAL3);
int Speed[8]={0,};
int Position[8]={0,};
volatile double nPosition[8]={0,};
volatile double w_ex31_r,w_mx32_r,w_ex33_r,w_rx34_r,w_ex61_l,w_mx62_l,w_ex63_l,w_rx64_l;

int Id[MOTORS] = {61,  62,  63,  64,  31,  32,  33,  34};
int Offset1[MOTORS] = {1366,  1366,  3052,  512,  2048,  1366,  3052,  512};
int Offset2[MOTORS] = {1366,  1366,  3052,  512,  2048,  1366,  3052,  512};

word SyncPage1[24]=
{ 
  Id[0],0,100,  // 3 Dynamixels are move to position 0
  Id[1],0,100,  // with velocity 100
  Id[2],0,100,
  Id[3],0,100,
  Id[4],0,100,
  Id[5],0,100,
  Id[6],0,100,
  Id[7],0,100,
 
}; 

void UpdateMotorValue(int *frameData, int Speed)
{
  int cnt=1;
  for(int i=0;i<9;i++) //position
  {
    SyncPage1[cnt]=frameData[i];
    cnt=cnt+3;
  }
  cnt=2;
  for(int i=0;i<9;i++) //speed
  {
    SyncPage1[cnt]=Speed;
    cnt=cnt+3;
  }
   Dxl.syncWrite(30,2, SyncPage1,24);
}


void offset()
{
  UpdateMotorValue (Offset1, 100);
}

void motor_Position()
{
  UpdateMotorValue (Position, 70);
}



void motion_walk(double ex61, double mx62, double ex63, double rx64, double ex31, double mx32, double ex33, double rx34)
{
  for(int j=0; j<= sam; j++)                         //  change 101 to 200  
  {
     w_ex61_l = (ex61*cos(PI * j/sam + PI ) + ex61)/2;
     w_mx62_l = (mx62*cos(PI * j/sam + PI ) + mx62)/2;     // change
     w_ex63_l = (ex63*cos(PI * j/sam + PI ) + ex63)/2;
     w_rx64_l = (rx64*cos(PI * j/sam + PI ) + rx64)/2;
     w_ex31_r = (ex31*cos(PI * j/sam + PI ) + ex31)/2;
     w_mx32_r = (mx32*cos(PI * j/sam + PI ) + mx32)/2;
     w_ex33_r = (ex33*cos(PI * j/sam + PI ) + ex33)/2;
     w_rx34_r = (rx34*cos(PI * j/sam + PI ) + rx34)/2;

     Position[0] = Offset2[1-1] + w_ex61_l ;
     Position[1] = Offset2[2-1] + w_mx62_l ;  // 1638 + 500 = 2138 
     Position[2] = Offset2[3-1] + w_ex63_l ;
     Position[3] = Offset2[4-1] + w_rx64_l ;
     Position[4] = Offset2[5-1] + w_ex31_r ;
     Position[5] = Offset2[6-1] + w_mx32_r ;
     Position[6] = Offset2[7-1] + w_ex33_r ;
     Position[7] = Offset2[8-1] + w_rx34_r ;
    

    motor_Position();
    
    delay_us(8000) ;
  }
   Offset2[0] = Position[0];
   Offset2[1] = Position[1];   
   Offset2[2] = Position[2];
   Offset2[3] = Position[3];
   Offset2[4] = Position[4];
   Offset2[5] = Position[5];   
   Offset2[6] = Position[6];
   Offset2[7] = Position[7];
   
}

void w_test()
{                      //Left             //Right  
  //int Id[MOTORS] = {61,  62,  63,  64,  31,  32,  33,  34};
        
         motion_walk(  0,  0,  0,  290,  0,  0,  0,  -300); 
         delay(300);
  
         
}

void wheel_mode()
{
                  //Left             //Right  
  //int Id[MOTORS] = {61,  62,  63,  64,  31,  32,  33,  34};
         motion_walk(  -1470,  3357,  -1882,  290,  1470,  3357,  -1882,  -300); 
          delay(300);
  
  
}
char a ='p';
int flag = 0;

void setup(){
// Dynamixel 2.0 Protocol -> 0: 9600, 1: 57600, 2: 115200, 3: 1Mbps 
  Dxl.begin(3);
  Dxl.jointMode(3);
  Dxl.writeWord(1, CCW_Angle_Limit, 0); 
  SerialUSB.begin();
  SerialUSB.attachInterrupt(usbInterrupt);
  pinMode(1, OUTPUT); // OUT1,2
  pinMode(3, OUTPUT); // OUT1,2
  pinMode(8, OUTPUT); // OUT1,2 ENA
  pinMode(0, OUTPUT); // OUT3,4 
  pinMode(2, OUTPUT); // OUT3,4
  pinMode(9, OUTPUT); // OUT3,4 ENA
  //Set all dynamixels as same condition.
}
void usbInterrupt(byte* buffer, byte nCount) 
{
  a = (char)buffer[0];
  flag = 18;

}

void loop(){
/*
 * byte syncWrite(byte start_addr, byte num_of_data, int *param, int array_length);
 */
 if( flag == 18)                                                                              // two circle 
 {
   if( a == 'O') // walk_offset
 { 
   flag = 0;
    offset();
    for(int i=0; i<8; i++)
    {
      Offset2[i]=Offset1[i];
    }
    a = 'p';
 }
   else if( a == 'm') // move_offset
   {
     flag = 0;
     w_test();
     a = 'p';
   }
   
   else if ( a == 'w') //wheel mode
   {
     flag = 0;
     wheel_mode();
     a = 'p';
   }
   else if ( a == 'g') //go
   {
     flag = 0;
     digitalWrite(1,LOW);
     digitalWrite(3,HIGH);
     digitalWrite(8,HIGH);//OUT 1,2
    
     digitalWrite(0,HIGH);
     digitalWrite(2,LOW);
     digitalWrite(9,HIGH); // OUT 3,4
     delay(10);
     a = 'p';
     
   }
   else if (a == 'b') //back
   {
     flag = 0;
     digitalWrite(1,HIGH);
     digitalWrite(3,LOW);
     digitalWrite(8,HIGH);//OUT 1,2
   
     digitalWrite(0,LOW);
     digitalWrite(2,HIGH);
     digitalWrite(9,HIGH); // OUT 3,4
     delay(10);
     a = 'p';
   }
   else if ( a == 's') //stop
   {
     flag = 0;
     digitalWrite(8,LOW);
    
     digitalWrite(9,LOW);
     delay(10);
     a = 'p';
   }
   else if (a == 'r')
   {
     flag = 0;
     digitalWrite(1,LOW);
     digitalWrite(3,HIGH);
     digitalWrite(8,HIGH);//OUT 1,2
   
     digitalWrite(0,LOW);
     digitalWrite(2,HIGH);
     digitalWrite(9,HIGH); // OUT 3,4
     delay(10);
     a = 'p';
   }
      else if (a == 'l')
   {
     flag = 0;
     digitalWrite(1,HIGH);
     digitalWrite(3,LOW);
     digitalWrite(8,HIGH);//OUT 1,2
   
     digitalWrite(0,HIGH);
     digitalWrite(2,LOW);
     digitalWrite(9,HIGH); // OUT 3,4
     delay(10);
     a = 'p';
   }
   else if (a == 'x')
   {
     
   }
   
   SerialUSB.println("LOOP");
   delay(1000);
  
 
  flag =0;
}

}




