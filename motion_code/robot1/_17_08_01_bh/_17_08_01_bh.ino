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

#define MOTORS 8

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
int Offset[MOTORS] = {1320,  1638,  2620,  512,  2048,  1638,  2620,  512};

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
  UpdateMotorValue (Offset, 100);
}

void motor_Position()
{
  UpdateMotorValue (Position, 70);
}



void motion_walk(double ex61, double mx62, double ex63, double rx64, double ex31, double mx32, double ex33, double rx34)
{
  for(int j=0; j<101; j++)                         //  change 101 to 200  
  {
     w_ex61_l = (ex61*cos(PI * j/100 + PI ) + ex61)/2;
     w_mx62_l = (mx62*cos(PI * j/100 + PI ) + mx62)/2;     // change
     w_ex63_l = (ex63*cos(PI * j/100 + PI ) + ex63)/2;
     w_rx64_l = (rx64*cos(PI * j/100 + PI ) + rx64)/2;
     w_ex31_r = (ex31*cos(PI * j/100 + PI ) + ex31)/2;
     w_mx32_r = (mx32*cos(PI * j/100 + PI ) + mx32)/2;
     w_ex33_r = (ex33*cos(PI * j/100 + PI ) + ex33)/2;
     w_rx34_r = (rx34*cos(PI * j/100 + PI ) + rx34)/2;

     Position[0] = Offset[1-1] + w_ex61_l ;
     Position[1] = Offset[2-1] + w_mx62_l ;  // 1638 + 500 = 2138 
     Position[2] = Offset[3-1] + w_ex63_l ;
     Position[3] = Offset[4-1] + w_rx64_l ;
     Position[4] = Offset[5-1] + w_ex31_r ;
     Position[5] = Offset[6-1] + w_mx32_r ;
     Position[6] = Offset[7-1] + w_ex33_r ;
     Position[7] = Offset[8-1] + w_rx34_r ;
    

    motor_Position();
    
    delay_us(8000) ;
  }
   Offset[0] = Position[0];
   Offset[1] = Position[1];   
   Offset[2] = Position[2];
   Offset[3] = Position[3];
   Offset[4] = Position[4];
   Offset[5] = Position[5];   
   Offset[6] = Position[6];
   Offset[7] = Position[7];
   
}

void w_test()
{
  //int Id[MOTORS] = {61,  62,  63,  64,  31,  32,  33,  34};
        
         motion_walk(  0,  500,  0,  0,  0,  500,  0,  0); 
         delay(300);
         motion_walk(  0,  -500,  0,  0,  0,  -500,  0,  0); 
         
}
char a ='p';
int flag = 0;




void setup(){
// Dynamixel 2.0 Protocol -> 0: 9600, 1: 57600, 2: 115200, 3: 1Mbps 
  Dxl.begin(3);
  Dxl.jointMode(3);
  SerialUSB.begin();
  SerialUSB.attachInterrupt(usbInterrupt);
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
   if( a == 'O')
 { 
   flag = 0;
    offset();
    a = 'p';
 }
   else if( a == 't')
   {
     flag = 0;
     w_test();
     SerialUSB.println(" end of tttttttttttttttttttttttttttttt");
     
   }
   SerialUSB.println("LOOP");
   delay(1000);
  
 
  flag =0;
}

}




