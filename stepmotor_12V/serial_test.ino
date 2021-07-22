

// Update these with values suitable for your network.


int Pulse = 5; //D1		
int Dir = 4; //D2
int Ena = 14;	//D5

int Echo = 12;	//D6
int Trigger = 13;	//D7

int IR = 16;	//D0
int IR2 = 15;	//D8


void setup() {

  pinMode(Pulse, OUTPUT);
  pinMode(Dir, OUTPUT);  
  pinMode(Ena, OUTPUT);  
  pinMode(Trigger, OUTPUT);
  pinMode(Echo, INPUT);
  pinMode(IR, INPUT);
  pinMode(IR2, INPUT);

  digitalWrite(Ena, LOW);
  Serial.begin(115200);
  

   
}



void loop() {
  
  
  Serial.print(digitalRead(IR));
  Serial.println(digitalRead(IR2));
  
  delay(1);
  
  
}
