int one = 3;   
int dir = 8;
void setup()

{

pinMode(one, OUTPUT);
pinMode(dir, OUTPUT);  
digitalWrite(dir, LOW);
}


void loop()

{
digitalWrite(one,HIGH);

delayMicroseconds(50);

digitalWrite(one,LOW);
delayMicroseconds(1100);

}
