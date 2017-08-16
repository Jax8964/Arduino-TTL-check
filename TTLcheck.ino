// Created by ZJUI ece120 team: Jax Sun, Steve Wang, Thomas Tian, and Zachary Wu.
// This is an arduino program that checks the gates in 14-foot TTL, available models are: 7400,7402,7404,7408,7410,7411,7420,7421,7427, and 7474. 
// We use arduino feet 2 to 7 for TTL feet 1 to 6, and arduino feet 8 to 13 for TTL feet 8 to 13, we connect VCC to 5V on arduino, and GND to one of the ground feet on arduino.
// Please connect the circuit and input the simplified model (four digits number) of the TTL into the serial monitor, then the test information will be shown on the computer screen.
#include <avr/pgmspace.h>   
int gates[6][6];  
int a,b,gatesNumber;  //a is the number of inputs of the gate, b is the number of output of the gate, gate number is the number of the gates in a TTL
int in[32][5];  //input
int out[32][5]; //correct output
int flag=1; //if it does not work, we substract one

void check(int pins[6]) //check the gate. check() only operates on the variable that has been defined and change the values if necessary.
{
  float volt,error;
  for(int m=0;m<a;m++)
  {
    pinMode(pins[m],OUTPUT);
  }
  for (int n=a;n<a+b;n++)
  {
    pinMode(pins[n],INPUT);
  }
  for (int i=0;i<power2(a);i++)
  {
    for (int m=0;m<a;m++)
    {
      digitalWrite(pins[m],in[i][m]); Serial.print(in[i][m]);Serial.print(" ");
    }
    delay(100); Serial.print(" ---- ");
    for (int n=a;n<a+b;n++)
    {
      volt = digitalRead(pins[n]); Serial.print(volt); Serial.print(" "); //we use digital read and compare the read values with the desired values.
      error=volt-float(out[i][n-a]);  
      if(error>0.1||error<-0.1) {flag--;} 
    }   
    Serial.println();
  }
}
int power2(int a) //2 to the ath power
{
  int b =1;
  for (int i=1;i<=a;i++)
  {
    b = b*2;
  }
  return b;
}
void gray(int a)  //generate the gray code
{
  for (int i=1;i<=a;i++)
  {
    int j = power2(i-1);
    for (int x = 0;x<j;x++)
    {
      in[x][i-1]=0;
    }
    for (int y = j;y<2*j;y++)
    {
      in[y][i-1]=1;
      for (int k=0;k<i-1;k++)
      {
        in[y][k]=in[2*j-1-y][k];
      }
    }    
  }
}

void setup()
{  
  Serial.begin(9600);
  while(Serial.read()>= 0){}  // clear the momory because the Serial.read() returns the ASCII code and delete it from the memory
  Serial.println(F("Enter a TCL mode listed to check whether the gate works well:"));
  Serial.println(F("7400 5400"));Serial.println(F("7402 5402")); Serial.println(F("7404 5404")); Serial.println(F("7408 5408"));Serial.println(F("7410 5410")); 
  Serial.println(F("7411 5411"));Serial.println(F("7421 5421"));Serial.println(F("7420 5420")); Serial.println(F("7427 5427"));Serial.println(F("7474 5474"));//string in F()
  
}  

void loop() 
{
  Serial.print(F("Mode: "));
  while(Serial.available()<=0) delay(100); delay(100);  
  int z=0,mode = 0;
  while(Serial.available()>0)
  {
    z=Serial.read()-48;
    Serial.print(z);
    mode=mode*10+z;
    delay(10);
  }
  
//datasheet------------------------------------------------------------------------------------
  if (mode==0000)     // for tesing
  { 
    a=2;b=1;gatesNumber=4;gray(a);
    int gates_0[4][3]={{1,2,3},{4,5,6},{10,9,8},{13,12,11}};
    int out_0[4][1]={0,0,0,0};
    for (int m =0;m<gatesNumber;m++)
   {
      for (int n=0;n<a+b;n++){ if(gates_0[m][n]<7) gates[m][n]=gates_0[m][n]+1; else gates[m][n]=gates_0[m][n]; }                     //load data 
    }
    for (int m=0;m<power2(a);m++)
    {
      for (int n=0;n<b;n++){out[m][n]=out_0[m][n];}  // insert data
    }
  }
  else if (mode==7400||mode==5400)
  {
    a=2;b=1;gatesNumber=4;gray(a);
    int gates_00[4][3]={{1,2,3},{4,5,6},{10,9,8},{13,12,11}};
    int out_00[4][1]={1,1,0,1};
    for (int m =0;m<gatesNumber;m++)
    {
      for (int n=0;n<a+b;n++){ if(gates_00[m][n]<7) gates[m][n]=gates_00[m][n]+1;else gates[m][n]=gates_00[m][n]; }                     //load data 
    }
    for (int m=0;m<power2(a);m++)
    {
      for (int n=0;n<b;n++){out[m][n]=out_00[m][n];}  //load data 
    }
  }
  else if (mode==7402||mode==5402) 
  {
    a=2;b=1;gatesNumber=4;gray(a);  // generate the truth table
    int gates_02[4][3]={{3,2,1},{6,5,4},{8,9,10},{11,12,13}};//store the gate foot number, one group for one gate
    int out_02[4][1]={1,0,0,0};                              //store the output
    for (int m =0;m<gatesNumber;m++)
   {
      for (int n=0;n<a+b;n++){ if(gates_02[m][n]<7) gates[m][n]=gates_02[m][n]+1;else gates[m][n]=gates_02[m][n]; }                     //load data 
    }
    for (int m=0;m<power2(a);m++)
    {
      for (int n=0;n<b;n++){out[m][n]=out_02[m][n];}  //load data 
    }
  }
  
  else if (mode==7404||mode==5404)
  {
    a=1;b=1;gatesNumber=6;gray(a);     
    int gates_04[6][2]={{1,2},{3,4},{5,6},{9,8},{11,10},{13,12}};
    int out_04[2][1]={1,0};                              
    for (int m =0;m<gatesNumber;m++)
   {
      for (int n=0;n<a+b;n++){ if(gates_04[m][n]<7) gates[m][n]=gates_04[m][n]+1;else gates[m][n]=gates_04[m][n]; }                     //load data 
    }
    for (int m=0;m<power2(a);m++)
    {
      for (int n=0;n<b;n++){out[m][n]=out_04[m][n];}  //load data 
    }
  }
  else if (mode==7408||mode==5408)
  {
    a=2;b=1;gatesNumber=4;gray(a);
    int gates_08[4][3]={{1,2,3},{4,5,6},{10,9,8},{13,12,11}};
    int out_08[4][1]={0,0,1,0};
    for (int m =0;m<gatesNumber;m++)
    {
      for (int n=0;n<a+b;n++){ if(gates_08[m][n]<7) gates[m][n]=gates_08[m][n]+1;else gates[m][n]=gates_08[m][n]; }                     //load data 
    }
    for (int m=0;m<power2(a);m++)
    {
      for (int n=0;n<b;n++){out[m][n]=out_08[m][n];}  //load data 
    }
  }
  else if (mode==7410||mode==5410)
  {
    a=3;b=1;gatesNumber=3;gray(a);     
    int gates_10[3][4]={{1,2,13,12},{3,4,5,6},{11,10,9,8}};
    int out_10[8][1]={1,1,1,1,1,0,1,1};                            
    for (int m =0;m<gatesNumber;m++)
   {
      for (int n=0;n<a+b;n++){ if(gates_10[m][n]<7) gates[m][n]=gates_10[m][n]+1;else  gates[m][n]=gates_10[m][n]; }                     //load data 
    }
    for (int m=0;m<power2(a);m++)
    {
      for (int n=0;n<b;n++){out[m][n]=out_10[m][n];}  //load data 
    }
  }
  else if (mode==7411||mode==5411)
  {
    a=3;b=1;gatesNumber=3;gray(a);
    int gates_11[3][4]={{1,2,13,12},{3,4,5,6},{11,10,9,8}};
    int out_11[8][1]={0,0,0,0,0,1,0,0};
    for (int m =0;m<gatesNumber;m++)
    {
      for (int n=0;n<a+b;n++){ if(gates_11[m][n]<7) gates[m][n]=gates_11[m][n]+1;else gates[m][n]=gates_11[m][n]; }                     //load data 
    for (int m=0;m<power2(a);m++)
    {
      for (int n=0;n<b;n++){out[m][n]=out_11[m][n];}  //load data 
    }
  }
  } 
  else if (mode==7420||mode==5420)
  {
    a=4;b=1;gatesNumber=2;gray(a);     
    int gates_20[2][5]={{1,2,4,5,6},{13,12,10,9,8}};
    int out_20[16][1]={1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1};                             
    for (int m =0;m<gatesNumber;m++)
   {
      for (int n=0;n<a+b;n++){ if(gates_20[m][n]<7) gates[m][n]=gates_20[m][n]+1;else gates[m][n]=gates_20[m][n]; }                     //load data 
    }
    for (int m=0;m<power2(a);m++)
    {
      for (int n=0;n<b;n++){out[m][n]=out_20[m][n];}  //load data 
    }
  }
  else if (mode==7421||mode==5421)
  {
    a=4;b=1;gatesNumber=2;gray(a);     
    int gates_21[2][5]={{1,2,4,5,6},{13,12,10,9,8}};
    int out_21[16][1]={0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0};                            
    for (int m =0;m<gatesNumber;m++)
   {
      for (int n=0;n<a+b;n++){ if(gates_21[m][n]<7) gates[m][n]=gates_21[m][n]+1;else gates[m][n]=gates_21[m][n]; }                     //load data 
    }
    for (int m=0;m<power2(a);m++)
    {
      for (int n=0;n<b;n++){out[m][n]=out_21[m][n];}  //load data 
    }
  }
  else if (mode==7427||mode==5427)
  {
    a=3;b=1;gatesNumber=3;gray(a);     
    int gates_27[3][4]={{1,2,13,12},{3,4,5,6},{11,10,9,8}};
    int out_27[8][1]={1,0,0,0,0,0,0,0};                              
    for (int m =0;m<gatesNumber;m++)
   {
      for (int n=0;n<a+b;n++){ if(gates_27[m][n]<7) gates[m][n]=gates_27[m][n]+1;else gates[m][n]=gates_27[m][n]; }                     //load data 
    }
    for (int m=0;m<power2(a);m++)
    {
      for (int n=0;n<b;n++){out[m][n]=out_27[m][n];}  //load data 
    }
  }
  else if (mode==7474||mode==5474)
  {
    a=4;b=2;gatesNumber=2;
    int gates_74[2][6]={{1,2,3,4,5,6},{13,12,11,10,9,8}};
    int out_74[16][2]={{0,1},{0,1},{1,0},{1,0},{1,0},{0,1},{0,1},{0,1},{1,0},{1,0},{1,0},{0,1},{0,1},{0,1},{0,1},{0,1}};
    int in_74[16][4]={{0,1,0,1},{0,1,1,1},{1,0,0,0},{1,0,1,0},{1,0,0,1},{1,0,1,1},{1,1,1,1},{1,1,0,1},
                      {1,1,1,1},{1,0,1,1},{1,0,0,1},{0,1,0,1},{0,1,0,1},{0,1,0,1},{0,1,0,1},{0,1,0,1}};
    for (int m =0;m<gatesNumber;m++)
    {
      for (int n=0;n<a+b;n++){ if(gates_74[m][n]<7) gates[m][n]=gates_74[m][n]+1;else gates[m][n]=gates_74[m][n]; }                     //load data 
    }
    for (int m=0;m<16;m++)
    {
      for (int n=0;n<b;n++){out[m][n]=out_74[m][n];} 
    }
    for (int m=0;m<16;m++)
    {
      for (int n=0;n<a;n++){in[m][n]=in_74[m][n];} 
    }
  }

  //datasheet-------------------------------------------------------------------------------
  else 
  {
    Serial.println(F("\ncannot find this mode!\n")); 
    return; // return the whole loop
  }
  Serial.println(F("\n**************************************************"));
  for (int i=0;i<gatesNumber;i++)
  {
    flag = 1;
    check(gates[i]);
    Serial.print("PinNumber  ");
    for (int k=0;k<a+b;k++)
    {
      if (gates[i][k]<8) {Serial.print(gates[i][k]-1);}
      else {Serial.print(gates[i][k]); }
      Serial.print("\t");
    }
    if (flag < 1) { Serial.println(F("wrong\n----------------------------------")); }
    else { Serial.println(F("right\n----------------------------------"));}
  }
    Serial.println(F("**************************************************\n"));
}

