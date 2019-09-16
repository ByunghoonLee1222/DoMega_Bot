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
#include <Ultrasonic.h>

#define ID_NUM_1  61
#define ID_NUM_2  62
#define ID_NUM_3  63
#define ID_NUM_4  64
#define ID_NUM_5  31
#define ID_NUM_6  32
#define ID_NUM_7  33
#define ID_NUM_8  34
#define ID_NUM_9  82
#define ID_Left_Wheel 85
#define ID_Right_Wheel 84

#define sam  150
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
volatile double w_ex31_r,w_mx32_r,w_ex33_r,w_rx34_r,w_ex61_l,w_mx62_l,w_ex63_l,w_rx64_l,w_ex82_f;

int Id[MOTORS] = {61,  62,  63,  64,  31,  32,  33,  34, 82};
int Offset1[MOTORS] = {1366,  1593,  2675,  512,  2048,  1593,  2675,  512, 4095};
int Offset2[MOTORS] = {1366,  1593,  2675,  512,  2048,  1593,  2675,  512, 4095};

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
  UpdateMotorValue (Offset1, 100);
}

void motor_Position()
{
  UpdateMotorValue (Position, 70);
}

void motor_Position2()
{
  UpdateMotorValue (Position, 50);
}



void motion_walk(double ex61, double mx62, double ex63, double rx64, double ex31, double mx32, double ex33, double rx34, double ex82)
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
     w_ex82_f = (ex82*cos(PI * j/sam2 + PI ) + ex82)/2;

     Position[0] = Offset2[1-1] + w_ex61_l ;
     Position[1] = Offset2[2-1] + w_mx62_l ;  // 1638 + 500 = 2138 
     Position[2] = Offset2[3-1] + w_ex63_l ;
     Position[3] = Offset2[4-1] + w_rx64_l ;
     Position[4] = Offset2[5-1] + w_ex31_r ;
     Position[5] = Offset2[6-1] + w_mx32_r ;
     Position[6] = Offset2[7-1] + w_ex33_r ;
     Position[7] = Offset2[8-1] + w_rx34_r ;
     Position[8] = Offset2[9-1] + w_ex82_f ;
    

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
void motion_walk2(double ex61, double mx62, double ex63, double rx64, double ex31, double mx32, double ex33, double rx34, double ex82)
{
  for(int j=0; j<= sam2; j++)                         //  change 101 to 200  
  {
     w_ex61_l = (ex61*cos(PI * j/sam2 + PI ) + ex61)/2;
     w_mx62_l = (mx62*cos(PI * j/sam2 + PI ) + mx62)/2;     // change
     w_ex63_l = (ex63*cos(PI * j/sam2 + PI ) + ex63)/2;
     w_rx64_l = (rx64*cos(PI * j/sam2 + PI ) + rx64)/2;
     w_ex31_r = (ex31*cos(PI * j/sam2 + PI ) + ex31)/2;
     w_mx32_r = (mx32*cos(PI * j/sam2 + PI ) + mx32)/2;
     w_ex33_r = (ex33*cos(PI * j/sam2 + PI ) + ex33)/2;
     w_rx34_r = (rx34*cos(PI * j/sam2 + PI ) + rx34)/2;
     w_ex82_f = (ex82*cos(PI * j/sam2 + PI ) + ex82)/2;

     Position[0] = Offset2[1-1] + w_ex61_l ;
     Position[1] = Offset2[2-1] + w_mx62_l ;  // 1638 + 500 = 2138 
     Position[2] = Offset2[3-1] + w_ex63_l ;
     Position[3] = Offset2[4-1] + w_rx64_l ;
     Position[4] = Offset2[5-1] + w_ex31_r ;
     Position[5] = Offset2[6-1] + w_mx32_r ;
     Position[6] = Offset2[7-1] + w_ex33_r ;
     Position[7] = Offset2[8-1] + w_rx34_r ;
     Position[8] = Offset2[9-1] + w_ex82_f ;
    

    motor_Position2();
    
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
  //int Id[MOTORS] = {61,  62,  63,  64,  31,  32,  33,  34};
        
         motion_walk(  0,  0,  0,  290,  0,  0,  0,  -300,  0); 
         delay(300);
  
         
}
void wheel_offset()  // wheel offset set
{
    //int Id[MOTORS] = {61,  62,  63,  64,  31,  32,  33,  34, 82};
         motion_walk(  0,  0,  0,  0,  0,  0,  0,  0,  -1408); //1474 - 90do , 1408 - 86do
         delay(100);
         motion_walk2(  0,  961,  -1376,  0,  0,  961,  -1376,  0,  0); 
         delay(300);
}
void wheel_mode()  //leg up
{
                  //Left             //Right  
  //int Id[MOTORS] = {61,  62,  63,  64,  31,  32,  33,  34,82};
         
         motion_walk(  -1365,  2485,  -749,  -300,  1365,  2485,  -749,  290,  0); 
          delay(300);
  
  
}
void wheel_exit() // leg down
{
    //int Id[MOTORS] = {61,  62,  63,  64,  31,  32,  33,  34,82};
         motion_walk(  1365,  -2485,  749,  300,  -1365,  -2485,  749,  -290,  0); 
          delay(300);
}
void obstacle()
{
      //int Id[MOTORS] = {61,  62,  63,  64,  31,  32,  33,  34, 82};
         motion_walk(  0,  0,  0,  0,  0,  0,  0,  0,  -1408); //1474 - 90do , 1408 - 86do
         delay(100);
         motion_walk2(  0,  961,  -1376,  0,  0,  961,  -1376,  0,  0); 
         delay(100);
         
}
void obs_test()
{
   //int Id[MOTORS] = {61ex,  62mx,  63ex,  64,  31ex,  32mx,  33ex,  34, 82};
      motion_walk( 1550,  58,  1356,  -300,  -1550,  58,  1356,  300,  0);
      delay(2000);
      motion_walk( 0,  397,  0,  0,  0,  397,  0,  0,  0);
      delay(100);
}
void obs_test2()
{
   //int Id[MOTORS] = {61,  62,  63,  64,  31,  32,  33,  34, 82};
      motion_walk( 0,  -397,  0,  0,  0,  -397,  0,  0,  0);
      delay(500);
      motion_walk( -1550,  227,  0,  300,  1550,  227,  0,  -300,  0);
      delay(1000);
      motion_walk( 0,  -285,  -1356,  0,  0,  -285,  -1356,  0,  0);
      delay(100);
}
int ult = 0;
char a ='p';
int flag = 0;
Ultrasonic leftSensor(17,16); //(Trig,Echo)
void setup(){
// Dynamixel 2.0 Protocol -> 0: 9600, 1: 57600, 2: 115200, 3: 1Mbps 
  Dxl.begin(3);
  Dxl.jointMode(3);
  Dxl.writeWord(1, CCW_Angle_Limit, 0); 
  Dxl.writeByte(ID_Left_Wheel, CCW_Angle_Limit, 0);  //Dxl.jointMode(ID) can be used
  Dxl.writeByte(ID_Right_Wheel, CCW_Angle_Limit, 0);
 // Serial2.begin(57600);
  SerialUSB.begin();
  SerialUSB.attachInterrupt(usbInterrupt);
  pinMode(0, OUTPUT); // OUT3,4
  pinMode(1, OUTPUT); // OUT3,4
  pinMode(8, OUTPUT); // OUT3,4 ENA
  pinMode(3, OUTPUT); // OUT1,2
  pinMode(2, OUTPUT); // OUT1,2
  pinMode(9, OUTPUT); // OUT1,2 ENA
  //Set all dynamixels as same condition.
}
void usbInterrupt(byte* buffer, byte nCount) 
{
  a = (char)buffer[0];
  flag = 18;

}
char temp[1];


void ultra_moving()
{
  //left
  while(1)
  {
     digitalWrite(0,HIGH);  
     digitalWrite(1,LOW);
     digitalWrite(8,HIGH);//OUT 1,2
   
     digitalWrite(3,HIGH);
     digitalWrite(2,LOW);
     digitalWrite(9,HIGH); // OUT 3,4
     delay(10);
     ult = 1;
      SerialUSB.println("pppp");
     delay(20);
     if(ult ==1 && leftSensor.readDistance() > 60) 
     {
     //back  
       ult =0;
       break;
     }
      }

   
   }
void loop(){

/*
 * byte syncWrite(byte start_addr, byte num_of_data, int *param, int array_length);
 */
// if ( Serial2.available() )
//  {
//    temp[0] = Serial2.read();
//    flag = 18;
//   // SerialUSB.print(temp[0]);
//    //SerialUSB.println();
//  }
 if( flag == 18)                                                                              // two circle 
 {
   if( a == 'O') // walk_offset
 { 
   flag = 0;
    offset();
    for(int i=0; i<9; i++)
    {
      Offset2[i]=Offset1[i];
    }
    a = 'p';
 }
//  else if( a == 'c') //obstacle'c'
//  {
//    flag =0;
//    obstacle();
//    a = 'p';
//  }
   else if( a == '2')
 {
      flag = 0;
      obstacle();
      delay(3000);
      obs_test();
      delay(2500);
      Dxl.writeWord(ID_Left_Wheel, Moving_Speed,  50);// go ahead with velocity 300
      Dxl.writeWord(ID_Right_Wheel, Moving_Speed,  50);
      delay(1600);
      Dxl.writeWord(ID_Left_Wheel, Moving_Speed, 0);  //Dxl.jointMode(ID) can be used
      Dxl.writeWord(ID_Right_Wheel, Moving_Speed, 0);
      delay(1000);
      Dxl.writeWord(ID_Right_Wheel, Moving_Speed, 0x400 | 100);// go ahead with velocity 300
      Dxl.writeWord(ID_Left_Wheel, Moving_Speed,  100);
      delay(2100);
      Dxl.writeWord(ID_Left_Wheel, Moving_Speed, 0);  //Dxl.jointMode(ID) can be used
      Dxl.writeWord(ID_Right_Wheel, Moving_Speed, 0);
      delay(1000);
      Dxl.writeWord(ID_Left_Wheel, Moving_Speed,  50);// go ahead with velocity 300
      Dxl.writeWord(ID_Right_Wheel, Moving_Speed,  50);
      delay(1250);
      Dxl.writeWord(ID_Left_Wheel, Moving_Speed, 0);  //Dxl.jointMode(ID) can be used
      Dxl.writeWord(ID_Right_Wheel, Moving_Speed, 0);
      delay(1000);
      Dxl.writeWord(ID_Right_Wheel, Moving_Speed, 0x400 | 150);// go ahead with velocity 300
      Dxl.writeWord(ID_Left_Wheel, Moving_Speed,  150);
      delay(6500);
      Dxl.writeWord(ID_Left_Wheel, Moving_Speed, 0);  //Dxl.jointMode(ID) can be used
      Dxl.writeWord(ID_Right_Wheel, Moving_Speed, 0);
      delay(1500);
      obs_test2();
      delay(3000);
      offset();
        for(int i=0; i<9; i++)
     {
      Offset2[i]=Offset1[i];
     }
      delay(10);
      a = 'p';
 }
   else if( a == 'u') //obstacle'c'
  {
    flag =0;
    obstacle();
    delay(3000);
    obs_test();
    a = 'p';
  }
     else if( a == 'y') //obstacle'c'
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
    a = 'p';
  }
   else if(a =='1') //ultra start
   {
     
    while(1)
    {
        SerialUSB.print("LDistance ");
        SerialUSB.println(leftSensor.readDistance());    //Read and Display the Distance
        delay(20);
       //back
       digitalWrite(0,HIGH);
       digitalWrite(1,LOW);
       digitalWrite(8,HIGH);//OUT 1,2
   
       digitalWrite(3,LOW);
       digitalWrite(2,HIGH);
       digitalWrite(9,HIGH); // OUT 3,4
       delay(10);
    
       ult = 1;
     if(ult ==1 && leftSensor.readDistance() <= 25)   //left
     {
       digitalWrite(0,HIGH);
       digitalWrite(1,LOW);
       digitalWrite(8,HIGH);//OUT 1,2
   
       digitalWrite(3,HIGH);
       digitalWrite(2,LOW);
       digitalWrite(9,HIGH); // OUT 3,4
       delay(10);
       ult = 2;
       SerialUSB.println("second");
       delay(20);
     }  
     if(ult ==2 && leftSensor.readDistance() > 60) 
     {
           SerialUSB.println("last");
           delay(20);
           ult =3;
         break;
     }
     
    }
    while(1)
    {
    if(ult ==3)
    {
            //back
       digitalWrite(0,HIGH);
       digitalWrite(1,LOW);
       digitalWrite(8,HIGH);//OUT 1,2
   
       digitalWrite(3,LOW);
       digitalWrite(2,HIGH);
       digitalWrite(9,HIGH); // OUT 3,4
       ult = 4;
    }
     //back
     if(ult == 4 && leftSensor.readDistance()<= 25)
     {
       digitalWrite(0,HIGH);
       digitalWrite(1,LOW);
       digitalWrite(8,HIGH);//OUT 1,2
   
       digitalWrite(3,HIGH);
       digitalWrite(2,LOW);
       digitalWrite(9,HIGH); // OUT 3,4
       delay(10);
  
       SerialUSB.println("gogo");
         ult = 5;
     }
    
    if(ult == 5 && leftSensor.readDistance() > 60 )
    {
      break;
    }
    }
                //back
       digitalWrite(0,HIGH);
       digitalWrite(1,LOW);
       digitalWrite(8,HIGH);//OUT 1,2
   
       digitalWrite(3,LOW);
       digitalWrite(2,HIGH);
       digitalWrite(9,HIGH); // OUT 3,4   
       a = 'p';  
       ult = 0;
       flag = 0;
   
   }
   
   
  else if( a == 't') //wheel offset
 {
   flag=0;
   wheel_offset();
   delay(3800);
   wheel_mode();
   a = 'p';
 } 
//   else if ( temp[0] == 'w') //wheel mode
//   {
//     flag = 0;
//     wheel_mode();
//     temp[0] = 'p';
//   }
   else if ( a == 'z')
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
     a = 'p'; 
   }
   else if ( a == 'g') //go
   {
     flag = 0;
     digitalWrite(0,LOW);
     digitalWrite(1,HIGH);
     digitalWrite(8,HIGH);//OUT 1,2
    
     digitalWrite(3,HIGH);
     digitalWrite(2,LOW);
     digitalWrite(9,HIGH); // OUT 3,4
     delay(10);
     a = 'p';
     
   }
   else if (a == 'b') //back
   {
     flag = 0;
     digitalWrite(0,HIGH);
     digitalWrite(1,LOW);
     digitalWrite(8,HIGH);//OUT 1,2
   
     digitalWrite(3,LOW);
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
   else if (a == 'r') //right turn
   {
     flag = 0;
     digitalWrite(0,LOW);
     digitalWrite(1,HIGH);
     digitalWrite(8,HIGH);//OUT 1,2
   
     digitalWrite(3,LOW);
     digitalWrite(2,HIGH);
     digitalWrite(9,HIGH); // OUT 3,4
     delay(10);
     a = 'p';
   }
      else if (a == 'l')  //left turn
   {
     flag = 0;
     digitalWrite(0,HIGH);
     digitalWrite(1,LOW);
     digitalWrite(8,HIGH);//OUT 1,2
   
     digitalWrite(3,HIGH);
     digitalWrite(2,LOW);
     digitalWrite(9,HIGH); // OUT 3,4
     delay(10);
     a = 'p';
   }
   else if (a == 'i') //wheel backward
   {
      flag = 0;
      Dxl.writeWord(ID_Right_Wheel, Moving_Speed, 0x400 | 100);// go ahead with velocity 300
      Dxl.writeWord(ID_Left_Wheel, Moving_Speed,  100);
      delay(10);
      a = 'p';
   }
      else if (a == 'j') // wheel foward
   {
      flag = 0;
      Dxl.writeWord(ID_Left_Wheel, Moving_Speed, 0x400 | 100);// go ahead with velocity 300
      Dxl.writeWord(ID_Right_Wheel, Moving_Speed,  100);
      delay(10);
      a = 'p';
   }
       else if (a == 'q') // wheel left turn
   {
      flag = 0;
      Dxl.writeWord(ID_Left_Wheel, Moving_Speed, 0x400 | 50);// go ahead with velocity 300
      Dxl.writeWord(ID_Right_Wheel, Moving_Speed,0x400 | 50);
      delay(10);
      a = 'p';
   }
         else if (a == 'e') // wheel right turn
   {
      flag = 0;
      Dxl.writeWord(ID_Left_Wheel, Moving_Speed,  50);// go ahead with velocity 300
      Dxl.writeWord(ID_Right_Wheel, Moving_Speed,  50);
      delay(10);
      a = 'p';
   }
      else if (a == 'p') // wheel stop
   {
      flag = 0;
      Dxl.writeWord(ID_Left_Wheel, Moving_Speed, 0);  //Dxl.jointMode(ID) can be used
      Dxl.writeWord(ID_Right_Wheel, Moving_Speed, 0);  
      delay(10);
     a = 'p';
   }
   
  // SerialUSB.println("LOOP");
   delay(1000);
  
 
  flag =0;
}

}




