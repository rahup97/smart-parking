#include <SPI.h>

#include <mcp_can.h>

#include <SoftwareSerial.h>

#define MAX_DISTANCE 200
#define ledpin4 14

SoftwareSerial serial(0, 1);
int ledpin[3][2] = {6, 9, 17, 18};
int trigPin[3] = {8, 4};
int echoPin[3] = {7, 3};
int i, j, k = 0, ctr = 0, flag[3], pos, arr[10];
long duration, distance, sensor[3];
char data;

unsigned int rxId;

unsigned long rcvTime;

unsigned char len = 0;
unsigned char buf[8];

const int SPI_CS_PIN = 10;

long unsigned int txID = 0x1881ABBA; // This format is typical of a 29 bit identifier.. the most significant digit is never greater than one.
unsigned char stmp[8] = {0x0E, 0x00, 0xFF, 0x22, 0xE9, 0xFA, 0xDD, 0x51};


//Construct a MCP_CAN Object and set Chip Select to 10.

MCP_CAN CAN(SPI_CS_PIN);


void setup()
{
    serial.begin(9600);
    Serial.begin(115200);
  
    for (i = 0; i < 3; i++)
    {
      for (j = 0; j < 2; j++)
        pinMode(ledpin[i][j], OUTPUT);
      pinMode(trigPin[i], OUTPUT);
      pinMode(echoPin[i], INPUT);
    }
    pinMode(ledpin4, OUTPUT);
    digitalWrite(ledpin4, LOW);
    for (i = 0; i < 2; i++)
      digitalWrite(ledpin[i][0], LOW);
    for (i = 0; i < 2; i++)
      digitalWrite(ledpin[i][1], HIGH);

    while (CAN_OK != CAN.begin(CAN_250KBPS))              // init can bus : baudrate = 250K
    {
         Serial.println("CAN BUS Module Failed to Initialized");
        Serial.println("Retrying....");
        delay(200);
        
    }
    Serial.println("CAN BUS Shield init ok!");
    Serial.println("Time\t\tPGN\tByte0\tByte1\tByte2\tByte3\tByte4\tByte5\tByte6\tByte7"); 
}

void loop()
{
  //Serial.println("Before sending");
  //CAN.sendMsgBuf(txID,1, 8, stmp); 
  //Serial.println("After sending");
  for (i = 0; i < 2; i++)
  {
    sensor[i] = Sensor(trigPin[i], echoPin[i]);
    if (sensor[i] < 10)
    {
      digitalWrite(ledpin[i][1], LOW);
      digitalWrite(ledpin[i][0], HIGH);
      flag[i] = 0;
    }
    else if (serial.available() || flag[i])
    {
      if (serial.available())
      {
        data = serial.read();
        pos = (int)data - 49;
        arr[k++] = pos;
        flag[pos] = 1;
        Serial.println(pos);
        if(pos==2)
        {
            CAN.sendMsgBuf(txID,1, 8, stmp);  
            Serial.println("CAN SENT"); 
        }
      }
      if (present(i))
      {
        digitalWrite(ledpin[i][1], LOW);
        digitalWrite(ledpin[i][0], HIGH);
        delay(300);
        digitalWrite(ledpin[i][0], LOW);
        delay(300);
      }
    }
    else
    {
      digitalWrite(ledpin[i][0], LOW);
      digitalWrite(ledpin[i][1], HIGH);
    }
  }

  if(CAN_MSGAVAIL == CAN.checkReceive())            // check if data coming
    {
        rcvTime = millis();
        CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
        Serial.println("CAN RECEIVED");
        rxId = CAN.getCanId();
//        digitalWrite(7, HIGH);
//        delay(500);
//        digitalWrite(7, LOW);
//        delay(500);
        Serial.print(rcvTime);
        Serial.print("\t\t");
        //Serial.print("0x");
        Serial.print(rxId, HEX);
        Serial.print("\t");

        for(int i = 0; i<len; i++)    // print the data
        {
            //if(buf[i] > 15){
              //Serial.print("0x");
              Serial.print(buf[i]);    
           
            //Serial.print("0x");
            //Serial.print(buf[i], HEX);
            
            Serial.print("\t");            
        }

        Serial.println();
    }
  
  for (i = 0; i < 2; i++)
    if (sensor[i] < 10);
}

int present(int i)
{
  for (int l = 0; l < k; l++)
    if (arr[l] == i)
      return 1;
  return 0;
}

long Sensor(int trigPin, int echoPin)
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;
  return distance;
}

