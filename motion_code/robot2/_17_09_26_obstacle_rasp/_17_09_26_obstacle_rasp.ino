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
 */////////////////////////////////////////////////////male////////////////////////////////////////////////////////
/* Dynamixel ID defines */
#define ID_NUM_1  11
#define ID_NUM_2  12
#define ID_NUM_3  13
#define ID_NUM_4  14
#define ID_NUM_5  21
#define ID_NUM_6  22
#define ID_NUM_7  23
#define ID_NUM_8  24
#define ID_NUM_9  72
#define ID_Left_Wheel 75
#define ID_Right_Wheel 74


#define sam 150
#define sam2 150
#define MOTORS 9

#define GOAL_SPEED 32
#define CCW_Angle_Limit 8
#define Moving_Speed 32


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
int Speed[9]={0,};
int Position[9]={0,};
volatile double nPosition[9]={0,};
volatile double m_ex11_l,m_mx12_l,m_ex13_l,m_rx14_l,m_ex21_r,m_mx22_r,m_ex23_r,m_rx24_r,m_mx72_f;

int Id[MOTORS] = {11,  12,  13,  14,  21,  22,  23,  24,  72};
int Offset1[MOTORS] = {2048,  1593,  2675,  512,  2388,  1593,  2675,  512,20};
int Offset2[MOTORS] = {2048,  1593,  2675,  512,  2388,  1593,  2675,  512,20};

word SyncPage1[27]=
{ 
  Id[0],0,100,  // 3 Dynamixels are move to position 0
  Id[1],0,100,  // with velocity 100
  Id[2],0,100,
  Id[3],0,100,
  Id[4],0,100,
  Id[5],0,100,
  Id[6],0,100,
  Id[7],0,100,
  Id[8],0,100
 
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
   Dxl.syncWrite(30,2, SyncPage1,27);
}


void offset()
{
  UpdateMotorValue (Offset1, 70);
}

void motor_Position()
{
  UpdateMotorValue (Position, 70);
}
void motor_Position2()
{
  UpdateMotorValue (Position, 50);
}



void motion_walk(double ex11, double mx12, double ex13, double rx14, double ex21, double mx22, double ex23, double rx24, double mx72)
{
  for(int j=0; j<= sam; j++)                         //  sam - sampling count
  {
     m_ex11_l = (ex11*cos(PI * j/sam + PI ) + ex11)/2;
     m_mx12_l = (mx12*cos(PI * j/sam + PI ) + mx12)/2;    
     m_ex13_l = (ex13*cos(PI * j/sam + PI ) + ex13)/2;
     m_rx14_l = (rx14*cos(PI * j/sam + PI ) + rx14)/2;
     m_ex21_r = (ex21*cos(PI * j/sam + PI ) + ex21)/2;
     m_mx22_r = (mx22*cos(PI * j/sam + PI ) + mx22)/2;
     m_ex23_r = (ex23*cos(PI * j/sam + PI ) + ex23)/2;
     m_rx24_r = (rx24*cos(PI * j/sam + PI ) + rx24)/2;
     m_mx72_f = (mx72*cos(PI * j/sam + PI ) + mx72)/2;

     Position[0] = Offset2[1-1] + m_ex11_l ;
     Position[1] = Offset2[2-1] + m_mx12_l ;  
     Position[2] = Offset2[3-1] + m_ex13_l ;
     Position[3] = Offset2[4-1] + m_rx14_l ;
     Position[4] = Offset2[5-1] + m_ex21_r ;
     Position[5] = Offset2[6-1] + m_mx22_r ;
     Position[6] = Offset2[7-1] + m_ex23_r ;
     Position[7] = Offset2[8-1] + m_rx24_r ;
     Position[8] = Offset2[9-1] + m_mx72_f ;
    

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
   Offset2[8] = Position[8];
   
}
void motion_walk2(double ex11, double mx12, double ex13, double rx14, double ex21, double mx22, double ex23, double rx24, double mx72)
{
  for(int j=0; j<= sam2; j++)                         //  change 101 to 200  
  {
     m_ex11_l = (ex11*cos(PI * j/sam2 + PI ) + ex11)/2;
     m_mx12_l = (mx12*cos(PI * j/sam2 + PI ) + mx12)/2;     // change
     m_ex13_l = (ex13*cos(PI * j/sam2 + PI ) + ex13)/2;
     m_rx14_l = (rx14*cos(PI * j/sam2 + PI ) + rx14)/2;
     m_ex21_r = (ex21*cos(PI * j/sam2 + PI ) + ex21)/2;
     m_mx22_r = (mx22*cos(PI * j/sam2 + PI ) + mx22)/2;
     m_ex23_r = (ex23*cos(PI * j/sam2 + PI ) + ex23)/2;
     m_rx24_r = (rx24*cos(PI * j/sam2 + PI ) + rx24)/2;
     m_mx72_f = (mx72*cos(PI * j/sam2 + PI ) + mx72)/2;

     Position[0] = Offset2[1-1] + m_ex11_l ;
     Position[1] = Offset2[2-1] + m_mx12_l ;  // 1638 + 500 = 2138 
     Position[2] = Offset2[3-1] + m_ex13_l ;
     Position[3] = Offset2[4-1] + m_rx14_l ;
     Position[4] = Offset2[5-1] + m_ex21_r ;
     Position[5] = Offset2[6-1] + m_mx22_r ;
     Position[6] = Offset2[7-1] + m_ex23_r ;
     Position[7] = Offset2[8-1] + m_rx24_r ;
     Position[8] = Offset2[9-1] + m_mx72_f ;
    

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
   Offset2[8] = Position[8];
   
}


void w_test()
{                      //Left             //Right  
  //int Id[MOTORS] = {11,  12,  13,  14,  21,  22,  23,  24};
        
         motion_walk(  0,  0,  0,  290,  0,  0,  0,  -300,  0); 
         delay(300);
  
         
}

void wheel_offset() // wheel offset set
{
                  //Left             //Right  
  //int Id[MOTORS] = {11,  12,  13,  14,  21,  22,  23,  24};
       
        motion_walk(  0,  0,  0,  0,  0,  0,  0,  0,  1023); 
         delay(100);
         motion_walk2(  0,  961,  -1376,  0,  0,  961,  -1376,  0,  0); 
         delay(100);
  
}
void wheel_mode()  //leg up
{
                  //Left             //Right  
  //int Id[MOTORS] = {11,  12,  13,  14,  21,  22,  23,  24};
         
         motion_walk(  -1365,  2485,  -749,  -300,  1365,  2485,  -749,  290,  0); 
          delay(300);
  
}
void wheel_exit() // leg down
{
   //int Id[MOTORS] = {11,  12,  13,  14,  21,  22,  23,  24};
         motion_walk(  1365,  -2485,  749,  300,  -1365,  -2485,  749,  -290,  0); 
          delay(300);
}
void obstacle() //obs offset'c'
{
    //int Id[MOTORS] = {11,  12,  13,  14,  21,  22,  23,  24,  72};
        motion_walk(  0,  0,  0,  0,  0,  0,  0,  0,  1023); 
         delay(100);
         motion_walk2(  0,  961,  -1376,  0,  0,  961,  -1376,  0,  0); 
         delay(300);
}

void obs_test() // obs up 'u'
{
  //int Id[MOTORS] = {11,  12,  13,   14,      21,   22,   23,   24,  72};
      motion_walk( 1550,  58,  1356,  -300,  -1550,  58,  1356,  300,  0);
      delay(2000);
      motion_walk( 0,  397,  0,  0,  0,  397,  0,  0,  0);
      delay(100);
}

void obs_test2() // obs down 'y'
{
   //int Id[MOTORS] = {61,  62,  63,  64,  31,  32,  33,  34, 82};
      motion_walk( 0,  -397,  0,  0,  0,  -397,  0,  0,  0);
      delay(500);
      motion_walk( -1550,  227,  0,  300,  1550,  227,  0,  -300,  0);
      delay(1000);
      motion_walk( 0,  -285,  -1356,  0,  0,  -285,  -1356,  0,  0);
      delay(100);
}

char a ='p';
int flag = 0;

void setup(){
// Dynamixel 2.0 Protocol -> 0: 9600, 1: 57600, 2: 115200, 3: 1Mbps 
  Dxl.begin(3);
  Dxl.jointMode(3);
  Dxl.writeWord(1, CCW_Angle_Limit, 0); 
  Dxl.writeWord(ID_Left_Wheel, CCW_Angle_Limit, 0);  //Dxl.jointMode(ID) can be used
  Dxl.writeWord(ID_Right_Wheel, CCW_Angle_Limit, 0);
  Serial2.begin(57600);
//  SerialUSB.begin();
//  SerialUSB.attachInterrupt(usbInterrupt);
  pinMode(0, OUTPUT); // OUT1,2
  pinMode(1, OUTPUT); // OUT1,2
  pinMode(8, OUTPUT); // OUT1,2 ENA
  pinMode(2, OUTPUT); // OUT3,4 
  pinMode(3, OUTPUT); // OUT3,4
  pinMode(9, OUTPUT); // OUT3,4 ENA
  //Set all dynamixels as same condition.
}

void usbInterrupt(byte* buffer, byte nCount) 
{
 a = (char)buffer[0];
  flag = 18;

}
char temp[1];
void loop(){
/*
 * byte syncWrite(byte start_addr, byte num_of_data, int *param, int array_length);
 */
  if ( Serial2.available() )
  {
    temp[0] = Serial2.read();
    flag = 18;
   // SerialUSB.print(temp[0]);
    //SerialUSB.println();
  }
 if( flag == 18)                                                                              // two circle 
 {
   if( temp[0] == 'O') // walk_offset
 { 
   flag = 0;
    offset();
    for(int i=0; i<9; i++)
    {
      Offset2[i]=Offset1[i];
    }
    temp[0] = 'p';
 }
//   else if( temp[0] == 'm') // move_offset
//   {
//     flag = 0;
//     w_test();
//     temp[0] = 'p';
//   }
   
   else if( temp[0] == 'u') //obstacle'u' up
  {
    flag =0;
    obstacle();
    delay(3000);
    obs_test();
    temp[0] = 'p';
  }
       else if( temp[0] == 'y') //obstacle'y'down
  {
    flag =0;
    obs_test2();
    delay(3000);
    offset();
        for(int i=0; i<9; i++)
    {
      Offset2[i]=Offset1[i];
    }
    delay(10);
    temp[0] = 'p';
  }
   else if ( temp[0] == 'g') //go
   {
     flag = 0;
     digitalWrite(0,HIGH);
     digitalWrite(2,LOW);
     digitalWrite(8,HIGH);//OUT 1,2
    
     digitalWrite(1,LOW);
     digitalWrite(3,HIGH);
     digitalWrite(9,HIGH); // OUT 3,4
     delay(10);
     temp[0] = 'p';
     
   }
   else if (temp[0] == 'b') //back
   {
     flag = 0;
     digitalWrite(0,LOW);
     digitalWrite(2,HIGH);
     digitalWrite(8,HIGH);//OUT 1,2
    
    digitalWrite(1,HIGH);
     digitalWrite(3,LOW);
   digitalWrite(9,HIGH); // OUT 3,4
     delay(10);
     temp[0] = 'p';
   }
   else if ( temp[0] == 's') //stop
   {
     flag = 0;
     digitalWrite(8,LOW);
    
     digitalWrite(9,LOW);
     delay(10);
     temp[0] = 'p';
   }
   else if (temp[0] == 'r')
   {
     flag = 0;
     digitalWrite(0,LOW);
     digitalWrite(2,HIGH);
     digitalWrite(8,HIGH);//OUT 1,2
   
     digitalWrite(1,LOW);
     digitalWrite(3,HIGH);
     digitalWrite(9,HIGH); // OUT 3,4
     delay(10);
     temp[0] = 'p';
   }
      else if (temp[0] == 'l')
   {
     flag = 0;
     digitalWrite(0,HIGH);
     digitalWrite(2,LOW);
     digitalWrite(8,HIGH);//OUT 1,2
   
     digitalWrite(1,HIGH);
     digitalWrite(3,LOW);
     digitalWrite(9,HIGH); // OUT 3,4
     delay(10);
     temp[0] = 'p';
   }
     else if( temp[0] == 't') //wheel offset
 {
   flag=0;
   wheel_offset();
   delay(3800);
   wheel_mode();
   temp[0] = 'p';
 } 
//   else if ( temp[0] == 'w') //wheel mode
//   {
//     flag = 0;
//     wheel_mode();
//     temp[0] = 'p';                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
//   }
   else if ( temp[0] == 'z')
   {
     flag = 0;
     wheel_exit();
     delay(3800);
     offset();                                                                                                                     
       for(int i=0; i<9; i++)
    {
      Offset2[i]=Offset1[i];
    }
    delay(10);
     temp[0]= 'p'; 
   }
  else if (temp[0] == 'x')
   {
      Dxl.writeWord(1, GOAL_SPEED, 400); 
      delay(10);
      temp[0] = 'p';
   
   }
   else if( temp[0] == 'n')
   {
      Dxl.writeWord(1, GOAL_SPEED, 0); 
      delay(10);
       temp[0] = 'p';
   }
     else if (temp[0] == 'h')
   {
       Dxl.writeWord(1, GOAL_SPEED, 400 | 0x400);
       delay(10);
        temp[0] = 'p';
//      delay(3000); 
//      //stop
//      Dxl.writeWord(1, GOAL_SPEED, 0); 
//      delay(2000);
   }
   else if (temp[0] == 'i')
   {
      flag = 0;
      Dxl.writeWord(ID_Right_Wheel, Moving_Speed, 0x400 | 100);// go ahead with velocity 300
      Dxl.writeWord(ID_Left_Wheel, Moving_Speed,  100);
      delay(10);
      temp[0] = 'p';
   }
      else if (temp[0] == 'j')
   {
      flag = 0;
      Dxl.writeWord(ID_Left_Wheel, Moving_Speed, 0x400 | 100);// go ahead with velocity 300
      Dxl.writeWord(ID_Right_Wheel, Moving_Speed,  100);
      delay(10);
      temp[0] = 'p';
   }
       else if (temp[0] == 'q')
   {
      flag = 0;
      Dxl.writeWord(ID_Left_Wheel, Moving_Speed, 0x400 | 50);// go ahead with velocity 300
      Dxl.writeWord(ID_Right_Wheel, Moving_Speed,0x400 | 50);
      delay(10);
      temp[0] = 'p';
   }
         else if (temp[0] == 'e')
   {
      flag = 0;
      Dxl.writeWord(ID_Left_Wheel, Moving_Speed,  50);// go ahead with velocity 300
      Dxl.writeWord(ID_Right_Wheel, Moving_Speed,  50);
      delay(10);
      temp[0] = 'p';
   }
      else if (temp[0] == 'p')
   {
      flag = 0;
      Dxl.writeWord(ID_Left_Wheel, Moving_Speed, 0);  //Dxl.jointMode(ID) can be used
      Dxl.writeWord(ID_Right_Wheel, Moving_Speed, 0);  
      delay(10);
      temp[0] = 'p';
   }
  // SerialUSB.println("LOOP");
   delay(1000);
  
 
  flag =0;
}

}




