//IMPORTS
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
//GLOBAL DECLARATION
#ifndef CAR_SSID
#define CAR_SSID "car"
#define Password "Running"
#endif

const char *ssid = CAR_SSID;
const char *password = Password;
const int default_pwm = 1024;
int m1[] = {1,2,3};
int m2[] = {4,5,6};
  
//MOTOR CLASS
class Motor{
  private:
    int pin1 = 2,pin2 = 3, pwm = 1;
   public:
  Motor(int p[3]):pin1(p[1]),pin2(p[2]),pwm(p[0]){
    pinMode(pin1,OUTPUT);
    pinMode(pin2,OUTPUT);
    pinMode(pwm,OUTPUT);
    this->stop();
    set_pwm(default_pwm);
  }
  void set_pwm(int value){
    analogWrite(pwm,value);
  }
  void spin(){
    digitalWrite(pin1,HIGH);
    digitalWrite(pin2,LOW);
  }
  void spin_anti(){
    digitalWrite(pin2,HIGH);
    digitalWrite(pin1,LOW);
  }
  void reverse(){
    std::swap(pin1,pin2);
  }
  void stop(){
    digitalWrite(pin1,LOW);
    digitalWrite(pin2,LOW);
  }
};
//SETTING-UP SERVER
ESP8266WebServer server(80);
//Motor steer(m2);
//Motor bull(m1);
void control_page(){
  server.send(200,"text/html","<h1>SERVER STARTED</h1>");
}
void setup() {
  // put your setup code here, to run once:
  delay(1000);
  Serial.begin(9600);
  Serial.println();
  Serial.print("Configuring access point...");
  WiFi.softAP(ssid,password);
  
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  
  server.on("/",control_page);
//  server.on("/up",[]()->void {bull.spin();});
//  server.on("/back",[]()->void {bull.spin_anti();});
//  server.on("/left",[]()->void{steer.spin_anti();});
//  server.on("/right",[]()->void{steer.spin();});
//  server.on("/stop",[]()->void{bull.stop(); steer.stop();});
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();

}
