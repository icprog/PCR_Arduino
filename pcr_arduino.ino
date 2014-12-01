#include <PID_v1.h>
#include <Messenger.h>

Messenger message;
double Setpoint, Input, Output; 
int KP;
int KI;

PID myPid(&Input, &Output, &Setpoint, 1,1,0,DIRECT);

int set_tuningsKP()
{
 int check_tunings = 0;
 while (check_tunings == 0 )
 {
   if(Serial.available())
   {
     message.process(Serial.read());
   }
  
   KP = message.readInt();
   
   if(KP>0)
   {
     Serial.println(KP);
     myPid.SetTunings(KP,KI,0);
   }
   if(KP != 0)
   {
     check_tunings = 1;
   }
   else 
     check_tunings = 0;
 }
}

int set_tuningsKI()
{
  int check_tunings = 0;
  while (check_tunings == 0 )
  {
   if(Serial.available())
   {
     message.process(Serial.read());
   }
  
   KI = message.readInt();
   if (KI> 0)
   Serial.println(KI);
   if(KI != 0)
     {
       myPid.SetTunings(KP,KI,0);
       check_tunings = 1;
     }
   else 
     check_tunings = 0;
  }
}

//do set temp należałoby dodać jeszcze sprawdzanie czy liczba jest z zakresu 0-1023.
int set_temp()
{
 int check_tunings = 0;
 int T;
 
 while (check_tunings == 0 )
 {
   digitalWrite(8,HIGH);
   if(Serial.available())
   {
     message.process(Serial.read());
   }
  
   T = message.readInt();
   if(T>0)
     Serial.println(T);
   if(T != 0)
   {
     Setpoint = T;
     check_tunings = 1;
   }
   else 
     check_tunings = 0;
 }
}

void send_temp()
{
  
  int analog_read0,analog_read1,analog_read2,analog_read3;
  
  analog_read0 = analogRead(A0);
  analog_read1 = analogRead(A1);
  analog_read2 = analogRead(A2);
  analog_read3 = analogRead(A3);
  
  String temp0 ,temp1,temp2,temp3;
  String str0 = "<temp0>";
  String str1 = "<temp1>";
  String str2 = "<temp2>";
  String str3 = "<temp3>";
  
  temp0 = str0 + analog_read0 + str0;
  temp1 = str1 + analog_read1 + str1;
  temp2 = str2 + analog_read2 + str2;
  temp3 = str3 + analog_read3 + str3;
  
  Serial.println("<Sens>Sensors</Sens>");
  Serial.println(temp0);
  Serial.println(temp1);
  Serial.println(temp2);
  Serial.println(temp3); 

  
  Serial.flush();
}
void MessageCompleted()
{
    if (message.checkString("b"))
      {
          send_temp();
      }
    if(message.checkString("a"))
      {
          set_tuningsKP();
      }
    if(message.checkString("c"))
      {
          set_tuningsKI();
      }
    if(message.checkString("t"))
      {
          set_temp();
      }
}
void setup()
{
  Serial.begin(115200);
  message.attach(MessageCompleted);
  myPid.SetMode(AUTOMATIC);
  pinMode(8,OUTPUT);
}

void loop()
{
  while ( Serial.available( ) ) message.process(Serial.read( ) );
  Input = analogRead(0);
  //Serial.println(Output);
  //Serial.println(Input);
  myPid.Compute();
  analogWrite(3,Output);
  digitalWrite(8,LOW);
}
