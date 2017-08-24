#include <avr/pgmspace.h>   //用于把数据存在闪存
int gates[6][6];  //生成一个6*6数组，默认所有值为零。第一行存第一个gate输入输出管脚；第二行存第二个gate输入输出管脚......
int a,b,gatesNumber;  //a为单个gate输入个数，b为单个gate输出个数，gatesNumber为gate个数
int in[32][5];  //单个gate的truth-table输入，一般为gray code
int out[32][5]; //应该输出的值
int flag=1; //输出与预期不符就减一

void check(int pins[6]) //用于检查单一gate。void表示不返回任何值，仅对已有变量操作。所以上面先定义了所需变量，且为全局变量（函数外的变量）。所有输入变量需申明类型，数组申明大小，否则报错不匹配。
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
    delay(50); Serial.print(" ---- ");
    for (int n=a;n<a+b;n++)
    {
      volt = digitalRead(pins[n]); Serial.print(volt); Serial.print(" "); //读取gate输出并比较
      error=volt-float(out[i][n-a]);  
      if(error>0.1||error<-0.1) {flag--;} 
    }   
    Serial.println();
  }
}
int power2(int a) //以2为底幂函数
{
  int b =1;
  for (int i=1;i<=a;i++)
  {
    b = b*2;
  }
  return b;
}
void gray(int a)  //在in[32][5]中生成a位格雷码，最大5位。生成方法：已有N-1位格雷码，顺序写一遍末尾添0，逆序写一遍末尾添1，从而生成N位格雷码
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
  Serial.begin(9600);  //设置串口
  while(Serial.read()>= 0){}  //Serial.read()返回缓存中最早输入的一个字符的！ASCII码！，并将这个字符从缓存中删除。所以此语句可清空缓存
  Serial.println(F("Enter a TCL mode listed to check whether the gate works well:"));
  Serial.println(F("7400 5400"));Serial.println(F("7402 5402")); Serial.println(F("7404 5404")); Serial.println(F("7408 5408"));Serial.println(F("7410 5410")); 
  Serial.println(F("7411 5411"));Serial.println(F("7421 5421"));Serial.println(F("7420 5420")); Serial.println(F("7427 5427"));Serial.println(F("7474 5474"));Serial.println(F("7175 5175"));//字符串写在函数F()中可以使其存储在闪存里
  
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
  if (mode==0000)        //测试用
  { 
    a=2;b=1;gatesNumber=4;gray(a);
    int gates_0[4][3]={{1,2,3},{4,5,6},{10,9,8},{13,12,11}};
    int out_0[4][1]={0,0,0,0};
    for (int m =0;m<gatesNumber;m++)
   {
      for (int n=0;n<a+b;n++){ if(gates_0[m][n]<7) gates[m][n]=gates_0[m][n]+1; else gates[m][n]=gates_0[m][n]; }                     //导入 
    }
    for (int m=0;m<power2(a);m++)
    {
      for (int n=0;n<b;n++){out[m][n]=out_0[m][n];}  //导入
    }
  }
  else if (mode==7400||mode==5400)
  {
    a=2;b=1;gatesNumber=4;gray(a);
    int gates_00[4][3]={{1,2,3},{4,5,6},{10,9,8},{13,12,11}};
    int out_00[4][1]={1,1,0,1};
    for (int m =0;m<gatesNumber;m++)
    {
      for (int n=0;n<a+b;n++){ if(gates_00[m][n]<7) gates[m][n]=gates_00[m][n]+1;else gates[m][n]=gates_00[m][n]; }                     //导入 
    }
    for (int m=0;m<power2(a);m++)
    {
      for (int n=0;n<b;n++){out[m][n]=out_00[m][n];}  //导入
    }
  }
  else if (mode==7402||mode==5402) 
  {
    a=2;b=1;gatesNumber=4;gray(a);      //a为单个gate输入个数，b为单个gate输出个数，gate个数，生成真值表输入
    int gates_02[4][3]={{3,2,1},{6,5,4},{8,9,10},{11,12,13}};//储存引脚编号及功能
    int out_02[4][1]={1,0,0,0};                              //储存单个gate真值表输出
    for (int m =0;m<gatesNumber;m++)
   {
      for (int n=0;n<a+b;n++){ if(gates_02[m][n]<7) gates[m][n]=gates_02[m][n]+1;else gates[m][n]=gates_02[m][n]; }                     //导入 
    }
    for (int m=0;m<power2(a);m++)
    {
      for (int n=0;n<b;n++){out[m][n]=out_02[m][n];}  //导入
    }
  }
  
  else if (mode==7404||mode==5404)
  {
    a=1;b=1;gatesNumber=6;gray(a);      //a为单个gate输入个数，b为单个gate输出个数，gate个数，生成真值表输入
    int gates_04[6][2]={{1,2},{3,4},{5,6},{9,8},{11,10},{13,12}};//储存引脚编号及功能
    int out_04[2][1]={1,0};                              //储存单个gate真值表输出
    for (int m =0;m<gatesNumber;m++)
   {
      for (int n=0;n<a+b;n++){ if(gates_04[m][n]<7) gates[m][n]=gates_04[m][n]+1;else gates[m][n]=gates_04[m][n]; }                     //导入 
    }
    for (int m=0;m<power2(a);m++)
    {
      for (int n=0;n<b;n++){out[m][n]=out_04[m][n];}  //导入
    }
  }
  else if (mode==7408||mode==5408)
  {
    a=2;b=1;gatesNumber=4;gray(a);
    int gates_08[4][3]={{1,2,3},{4,5,6},{10,9,8},{13,12,11}};
    int out_08[4][1]={0,0,1,0};
    for (int m =0;m<gatesNumber;m++)
    {
      for (int n=0;n<a+b;n++){ if(gates_08[m][n]<7) gates[m][n]=gates_08[m][n]+1;else gates[m][n]=gates_08[m][n]; }                     //导入 
    }
    for (int m=0;m<power2(a);m++)
    {
      for (int n=0;n<b;n++){out[m][n]=out_08[m][n];}  //导入
    }
  }
  else if (mode==7410||mode==5410)
  {
    a=3;b=1;gatesNumber=3;gray(a);      //a为单个gate输入个数，b为单个gate输出个数，gate个数，生成真值表输入
    int gates_10[3][4]={{1,2,13,12},{3,4,5,6},{11,10,9,8}};//储存引脚编号及功能
    int out_10[8][1]={1,1,1,1,1,0,1,1};                              //储存单个gate真值表输出
    for (int m =0;m<gatesNumber;m++)
   {
      for (int n=0;n<a+b;n++){ if(gates_10[m][n]<7) gates[m][n]=gates_10[m][n]+1;else  gates[m][n]=gates_10[m][n]; }                     //导入  
    }
    for (int m=0;m<power2(a);m++)
    {
      for (int n=0;n<b;n++){out[m][n]=out_10[m][n];}  //导入
    }
  }
  else if (mode==7411||mode==5411)
  {
    a=3;b=1;gatesNumber=3;gray(a);
    int gates_11[3][4]={{1,2,13,12},{3,4,5,6},{11,10,9,8}};
    int out_11[8][1]={0,0,0,0,0,1,0,0};
    for (int m =0;m<gatesNumber;m++)
    {
      for (int n=0;n<a+b;n++){ if(gates_11[m][n]<7) gates[m][n]=gates_11[m][n]+1;else gates[m][n]=gates_11[m][n]; }                     //导入 
    }
    for (int m=0;m<power2(a);m++)
    {
      for (int n=0;n<b;n++){out[m][n]=out_11[m][n];}  //导入
    }
  }
  else if (mode==7420||mode==5420)
  {
    a=4;b=1;gatesNumber=2;gray(a);      //a为单个gate输入个数，b为单个gate输出个数，gate个数，生成真值表输入
    int gates_20[2][5]={{1,2,4,5,6},{13,12,10,9,8}};//储存引脚编号及功能
    int out_20[16][1]={1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1};                              //储存单个gate真值表输出
    for (int m =0;m<gatesNumber;m++)
   {
      for (int n=0;n<a+b;n++){ if(gates_20[m][n]<7) gates[m][n]=gates_20[m][n]+1;else gates[m][n]=gates_20[m][n]; }                     //导入 
    }
    for (int m=0;m<power2(a);m++)
    {
      for (int n=0;n<b;n++){out[m][n]=out_20[m][n];}  //导入
    }
  }
  else if (mode==7421||mode==5421)
  {
    a=4;b=1;gatesNumber=2;gray(a);      //a为单个gate输入个数，b为单个gate输出个数，gate个数，生成真值表输入
    int gates_21[2][5]={{1,2,4,5,6},{13,12,10,9,8}};//储存引脚编号及功能
    int out_21[16][1]={0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0};                              //储存单个gate真值表输出
    for (int m =0;m<gatesNumber;m++)
   {
      for (int n=0;n<a+b;n++){ if(gates_21[m][n]<7) gates[m][n]=gates_21[m][n]+1;else gates[m][n]=gates_21[m][n]; }                     //导入 
    }
    for (int m=0;m<power2(a);m++)
    {
      for (int n=0;n<b;n++){out[m][n]=out_21[m][n];}  //导入
    }
  }
  else if (mode==7427||mode==5427)
  {
    a=3;b=1;gatesNumber=3;gray(a);      //a为单个gate输入个数，b为单个gate输出个数，gate个数，生成真值表输入
    int gates_27[3][4]={{1,2,13,12},{3,4,5,6},{11,10,9,8}};//储存引脚编号及功能
    int out_27[8][1]={1,0,0,0,0,0,0,0};                              //储存单个gate真值表输出
    for (int m =0;m<gatesNumber;m++)
   {
      for (int n=0;n<a+b;n++){ if(gates_27[m][n]<7) gates[m][n]=gates_27[m][n]+1;else gates[m][n]=gates_27[m][n]; }                     //导入 
    }
    for (int m=0;m<power2(a);m++)
    {
      for (int n=0;n<b;n++){out[m][n]=out_27[m][n];}  //导入
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
      for (int n=0;n<a+b;n++){ if(gates_74[m][n]<7) gates[m][n]=gates_74[m][n]+1;else gates[m][n]=gates_74[m][n]; }                     //导入 
    }
    for (int m=0;m<11;m++)
    {
      for (int n=0;n<b;n++){out[m][n]=out_74[m][n];} 
    }
    for (int m=0;m<11;m++)
    {
      for (int n=0;n<a;n++){in[m][n]=in_74[m][n];} 
    }
  }
  else if (mode==7175||mode==5175)
  {
    a=3;b=2;gatesNumber=2;
    int gates_75[2][5]={{1,4,9,2,3},{1,5,9,7,6}};
    int out_75[8][2]={{0,1},{0,1},{1,0},{0,1},{0,1},{0,1},{1,0},{1,0}};
    int in_75[8][3]={{0,1,0},{0,1,1},{1,0,0},{1,0,1},
                     {1,1,1},{1,1,0},{1,1,1},{1,0,1}};
    for (int m =0;m<gatesNumber;m++)
    {
      for (int n=0;n<a+b;n++){ if(gates_75[m][n]<7) gates[m][n]=gates_75[m][n]+1;else gates[m][n]=gates_75[m][n]; }                     //导入 
    }
    for (int m=0;m<9;m++)
    {
      for (int n=0;n<b;n++){out[m][n]=out_75[m][n];} 
    }
    for (int m=0;m<9;m++)
    {
      for (int n=0;n<a;n++){in[m][n]=in_75[m][n];} 
    }
  }
  //datasheet-------------------------------------------------------------------------------
  else 
  {
    Serial.println(F("\ncannot find this mode!\n")); 
    return; //返回整个loop
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

