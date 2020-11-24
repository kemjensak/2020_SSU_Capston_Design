#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid= "control";
const char* password= "12341234";
const char* mqtt_server = "192.168.43.254";


int Pulse = 5; //D1		
int Dir = 4; //D2
int Ena = 14;	//D5

int Echo = 12;	//D6
int Trigger = 13;	//D7

int IR = 16;	//D0
int IR2 = 15;	//D8


char* topic_sub = "motor";
char* topic_pub = "order";

WiFiClient espClient;
PubSubClient client(espClient);
 
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    for(int i = 0; i<500; i++){
      delay(1);
    }
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
 
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ClientID")) { //change to ClientID
      Serial.println("connected");
       
      // ... and resubscribe
      client.subscribe(topic_sub);
 
      // Once connected, publish an announcement...
      
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
    }
  }
}
 
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String msg = "";
  for (int i = 0; i < length; i++) {
    msg +=(char)payload[i];
  }
  Serial.print(msg);
  Serial.println();

  int order=(char)payload[0]-48;
  
  
  

  motor(order);

}
 
void setup() {

  pinMode(Pulse, OUTPUT);
  pinMode(Dir, OUTPUT);  
  pinMode(Ena, OUTPUT);  
  pinMode(Trigger, OUTPUT);
  pinMode(Echo, INPUT);
  pinMode(IR, INPUT);
  pinMode(IR2, INPUT);

  digitalWrite(Ena, LOW);
  
  Serial.begin(115200);   //ESP8266
  setup_wifi();                   // Connect to wifi
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
   
}

void motor(int order){

  digitalWrite(Dir, order);

  Serial.println(IR_state());
  int safe = dist();
  Serial.println(safe);
  if (order == 1 && safe != 0)
     return;
  
  digitalWrite(Ena, HIGH);
  
  if(IR_state()!=order){
    if(order==1)
      client.publish(topic_pub,"1");
    if(order==0)
      client.publish(topic_pub,"0");
  }
  delay(2000);
    
  while(1){
   if(IR_state()==order)
    break;
   for(int i=0;i<50;i++){
    digitalWrite(Pulse,HIGH);
    delayMicroseconds(100);
    digitalWrite(Pulse,LOW);
    delayMicroseconds(4000);
    delay(0);
    
   }
   
  }
  
  delay(1000);
  if(0==order)
  digitalWrite(Ena, LOW);
}

int dist(){

  float duration, distance;
  int safe_dist = 150, loop = 5, result = 0;

  for(int j=0; j<loop; j++){
    digitalWrite(Trigger, HIGH);
    delay(20);
    digitalWrite(Trigger, LOW);
    // echoPin 이 HIGH를 유지한 시간을 저장 한다.
    duration = pulseIn(Echo, HIGH);
    // HIGH 였을 때 시간(초음파가 보냈다가 다시 들어온 시간)을 가지고 거리를 계산 한다.
    // 340은 초당 초음파(소리)의 속도, 10000은 밀리세컨드를 세컨드로, 왕복거리이므로 2로 나눠준다.
    distance = ((float)(340 * duration) / 10000) / 2;
    //loop횟수만큼 안전거리 검증
    if(distance<safe_dist)
      result++; //0은 안전
      delay(100);
    }
    Serial.println(distance);
    if(result != 0)
      client.publish(topic_pub,"2");
    delay(2000);
  
  return result;
  
}

int IR_state(){
  int state = digitalRead(IR);
  int state2 = digitalRead(IR2);
  
  if (state==0 && state2==1)
  return(1);
  if (state==1 && state2==0)
  return(0);
  else
  return(2);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
 
  client.loop();
  //int a=dist(5);
  
  
}
