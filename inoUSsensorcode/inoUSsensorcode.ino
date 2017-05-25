#include<SoftwareSerial.h>

#define MAX_DISTANCE 200
#define ledpin4 14

SoftwareSerial serial(0,1);
int ledpin[3][2]={8,7,10,9,17,18};
int trigPin[3]={5,13,4};
int echoPin[3]={11,12,3};
int i,j,k=0,ctr=0,flag[3],pos,arr[10];
long duration,distance,sensor[3];
char data;

void setup() 
{
    serial.begin(9600); 
    
    for(i=0;i<3;i++)
    {
        for(j=0;j<2;j++)
            pinMode(ledpin[i][j],OUTPUT);
        pinMode(trigPin[i],OUTPUT);
        pinMode(echoPin[i],INPUT);
    }                       
    pinMode(ledpin4,OUTPUT);
    digitalWrite(ledpin4,LOW);
    for(i=0;i<3;i++)
        digitalWrite(ledpin[i][0],LOW);
    for(i=0;i<3;i++)
        digitalWrite(ledpin[i][1],HIGH);
}

void loop() 
{
    for(i=0;i<3;i++)
    {
        sensor[i] = Sensor(trigPin[i],echoPin[i]);
        if(sensor[i]<10)
        {
            digitalWrite(ledpin[i][1],LOW);
            digitalWrite(ledpin[i][0],HIGH);
            flag[i]=0;
        }
        else if(serial.available() || flag[i])
        {
            if(serial.available())
            {
                data = serial.read();
                pos = (int)data - 49;
                arr[k++]=pos;
                flag[pos]=1;
            }
            if(present(i))
            {
                digitalWrite(ledpin[i][1],LOW);
                digitalWrite(ledpin[i][0],HIGH);
                delay(300);
                digitalWrite(ledpin[i][0],LOW);
                delay(300);
            }
        }
        else
        {
            digitalWrite(ledpin[i][0],LOW);
            digitalWrite(ledpin[i][1],HIGH);
        }
    }
    for(i=0;i<3;i++)
      if(sensor[i]<10)
}

int present(int i)
{
    for(int l=0;l<k;l++)
      if(arr[l]==i)
        return 1;
    return 0;
}

long Sensor(int trigPin, int echoPin)
{
    digitalWrite(trigPin,LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin,LOW);
    duration = pulseIn(echoPin,HIGH);
    distance = (duration/2)/29.1;
    return distance;
}  

