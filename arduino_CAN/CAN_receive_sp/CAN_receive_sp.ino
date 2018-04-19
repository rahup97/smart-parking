#include <mcp_can.h>

#include <SPI.h>

unsigned int rxId;

unsigned long rcvTime;

unsigned char len = 0;
unsigned char buf[8];

int i, flag;
long duration, distance, sensor;


long unsigned int txID = 0x1881ABBA; // This format is typical of a 29 bit identifier.. the most significant digit is never greater than one.
unsigned char stmp[8] = {0x0E, 0x00, 0xFF, 0x22, 0xE9, 0xFA, 0xDD, 0x51};

const int SPI_CS_PIN = 10;


MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin

void setup()
{
    Serial.begin(115200);
    pinMode(6, OUTPUT);
    digitalWrite(6, LOW);
    pinMode(7, OUTPUT);
    digitalWrite(7, LOW);
    pinMode(9 ,OUTPUT); 
    pinMode(8 ,INPUT);
    

    while (CAN_OK != CAN.begin(CAN_250KBPS))              // init can bus : baudrate = 500k
    {
        Serial.println("CAN BUS Module Failed to Initialized");
        Serial.println("Retrying....");
        delay(200);
    }    
    Serial.println("CAN BUS Module Initialized!");
    Serial.println("Time\t\tPGN\tByte0\tByte1\tByte2\tByte3\tByte4\tByte5\tByte6\tByte7");    
}


void loop()
{
    sensor = Sensor(9, 8);
    //Serial.print(sensor);
    if (sensor < 10)
    {
      digitalWrite(7, LOW);
      digitalWrite(6, HIGH);
      flag = 0;
    }
    
    else if(CAN_MSGAVAIL == CAN.checkReceive())            // check if data coming
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

        if((int)buf[0]==14)
        {
          if(flag==1)
          {
            for(i=0;i<10;i++)
            {
              digitalWrite(7, LOW);
              digitalWrite(6, HIGH);
              delay(300);
              digitalWrite(6, LOW);
              delay(300); 
            }
          }
        }
        
    }
    else
    {
      digitalWrite(6, LOW);
      digitalWrite(7, HIGH);
      flag = 1;
    }

    CAN.sendMsgBuf(txID,1, 8, stmp); 
    delay(100);
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
