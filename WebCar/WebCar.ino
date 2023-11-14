#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
int interval=25;
//Creating Motor Class
class Motor{
  uint8_t pwm;
  uint8_t positive;
  uint8_t negetive;
  int maxSpeed;
  int Speed=0;
  int minSpeed=343;
  public:Motor(uint8_t pin1,uint8_t pin2,uint8_t pin3,int value){
    pwm=pin1;
    positive=pin2;
    negetive=pin3;
    digitalWrite(positive,LOW);
    digitalWrite(negetive,LOW);
    analogWrite(pwm,value);
    
    maxSpeed=value;
  };
  void rotate(){
    digitalWrite(positive,HIGH);
    digitalWrite(negetive,LOW);
  }
  void rotateAnti(){
    digitalWrite(positive,LOW);
    digitalWrite(negetive,HIGH);
  }
  
  void speedUP(){
    this->rotate();
    while(Speed<maxSpeed){
      analogWrite(pwm,Speed);
      delay(80);
      Speed=Speed+100;
    }
  }
  void reverseUP(){
    this->rotateAnti();
    while(Speed<maxSpeed){
      analogWrite(pwm,Speed);
      delay(80);
      Speed=Speed+80;
    }
  }
  void setMax(int value){
   maxSpeed=value;
  }
  void setSpd(int value){
    analogWrite(pwm,value);
  }
  void Stop(){
    Speed=minSpeed;
    digitalWrite(positive,LOW);
    digitalWrite(negetive,LOW);
  }
};
ESP8266WebServer server(80);
//Motor Class Object
Motor backMotor(D0,D1,D2,512);
Motor frontMotor(D3,D4,D5,600);

void handleSpeed(){
  if(server.argName(0)=="I")
    interval=server.arg(0).toInt();
 else if(server.argName(0)=="type")
    frontMotor.setSpd(server.arg(0).toInt());
  else
  backMotor.setMax(server.arg(0).toInt());
  server.send(200);
}
void handleDirection(){
  Serial.println(server.arg(0));
  if(server.arg(0)=="F"){
    backMotor.Stop();
    backMotor.speedUP();
  }
  if(server.arg(0)=="B"){
      backMotor.Stop();
      backMotor.reverseUP();
  }
  if(server.arg(0)=="S"){
    backMotor.Stop();
    frontMotor.Stop();
    
  }
  
  if(server.arg(0)=="L"){
    frontMotor.rotate();
    delay(interval);
    frontMotor.Stop();    
  }
  if(server.arg(0)=="R"){
    frontMotor.rotateAnti();
    delay(interval);
    frontMotor.Stop();
  } 
  server.send(200);  
}

void controlPage(){
  String Page="<!DOCTYPE html>\n"
"<html lang=\"en\">\n"
"<head>\n"
"    <meta charset=\"UTF-8\">\n"
"    <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n"
"    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
"    <title>Document</title>\n"
"</head>\n"
"<style>\n"
"    body {\n"
"        background-image: linear-gradient(45deg, rgb(8, 63, 85), skyblue);\n"
"    }\n"
"\n"
"    .container {\n"
"        display: flex;\n"
"        justify-content: center;\n"
"        align-items: center;\n"
"        flex-direction: column;\n"
"        height: 99vh;\n"
"        width: 97vw;\n"
"        background-image: linear-gradient(45deg, rgb(8, 63, 85), skyblue);\n"
"    }\n"
"\n"
"    .control {\n"
"        display: flex;\n"
"        justify-content: center;\n"
"        align-items: center;\n"
"        height: 60vh;\n"
"        width: 400px;\n"
"        background-color: lightgreen;\n"
"        opacity: 40%;\n"
"        box-shadow: 5px 2px 2px black;\n"
"    }\n"
"\n"
"    .vertical {\n"
"        display: flex;\n"
"        justify-content: center;\n"
"        align-items: center;\n"
"        flex-direction: column;\n"
"        height: 100%;\n"
"        width: 500px;\n"
"    }\n"
"\n"
"    button {\n"
"        color: white;\n"
"        background-color: red;\n"
"        width: 100px;\n"
"        height: 40px;\n"
"        border-radius: 20%;\n"
"        margin: 10px;\n"
"    }\n"
"</style>\n"
"<body>\n"
"    \n"
"    <div class=\"container\">\n"
"        <div>\n"
"            <h1>Speed<input type=\"range\" min=\"0\" max=\"1024\" name=\"\" step=\"51.2\" value=\"0\" onchange=\"speed(this);\"\n"
"                    id='speed'>\n"
"                <span id='speedText'>Rpm 0</span></h1>\n"
"            <h1>interval<input type=\"range\" name=\"angle\" min=\"0\" max=\"150\" value='30\n"
" onchange=\"deg(this);\" step=\"5\"\n"
"                    id=\'angle\'>\n"
"                 <span id=\"angleText\">0</span>MILLISEC</h1>\n"
"                 \n"
"        </div>\n"
"        <div class=\"control\">\n"
"            <div class=\"vertical\">\n"
"                <button onclick=\"move(this);\" value='F'>\n"
"                    UP\n"
"                </button>\n"
"                <div>\n"
"                    <button  onclick=\"move(this);\" value='L'>\n"
"                        Left\n"
"                    </button>\n"
"                    <button onclick=\"move(this);\" value='S'>\n"
"                       STOP\n"
"                    </button>\n"
"                    <button  onclick=\"move(this);\" value='R'>\n"
"                        Right\n"
"                    </button>\n"
"                </div>\n"
"                <button onclick=\"move(this);\" value='B'>Down</button>\n"
"            </div>\n"
"        </div>\n"
"    </div>\n"
"    <script src=\"https://cdnjs.cloudflare.com/ajax/libs/axios/0.19.0/axios.min.js\"></script>\n"
"    <script>\n"
"        var speedRange = document.getElementById('speed');\n"
"        var displayAngle = document.getElementById('angleText');\n"
"        var displaySpeed = document.getElementById('speedText');\n"
"        var rpm=300;\n"
"        var angle = 0;\n"
"        var Direction = '';\n"
"        var Speed = 0;\n"
"        var Stop=0;\n"
"\n"
"        function send(parameters){\n"
"            let url=`http://192.168.4.1:80/${parameters}`;\n"
"            fetch(url)\n"
"            .then(res=>console.log(res.status));\n"
"        }\n"
"\n"
"        function speed(e) {\n"
"            Speed=e.value;\n"
"            let Rpm=Math.ceil(0.292968*Speed);\n"
"            displaySpeed.innerHTML=\"Rpm \"+Rpm;\n"
"            let mes=`action?speed=${Speed}`;\n"
"            send(mes);\n"
"        }\n"
"        function deg(e){\n"
"            displayAngle.innerTEXT=e.value;\n"
"            let mes=`action?type=${e.value}`;\n"
"            send(mes);\n"
"        }\n"
"        function move(e) {\n"
"            Direction=e.value;\n"
"            console.log(Direction);\n"
"            Stop=0;\n"
"            let mes=`move?Direction=${Direction}`;\n"
"            send(mes);\n"
"        }\n"
"\n"
"    </script>\n"
"</body>\n"
"</html>";
  server.send(200,"text/html",Page);
}
void setup() {
  // put your setup code here, to run once:
  pinMode(D0,OUTPUT);
  pinMode(D1,OUTPUT);
  pinMode(D2,OUTPUT);
  pinMode(D3,OUTPUT);
  pinMode(D4,OUTPUT);
  pinMode(D5,OUTPUT);
  Serial.begin(9600);
  WiFi.mode(WIFI_AP);
  WiFi.disconnect();
  Serial.println(WiFi.softAP("CAR","12345678"));
  Serial.println(" Server IP:- "+WiFi.softAPIP());
  server.on("/",controlPage);
  server.on("/action",handleSpeed);
  server.on("/move",handleDirection);
  
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
}
