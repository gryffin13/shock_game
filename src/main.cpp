/*
  Rui Santos
  Complete project details
   - Arduino IDE: https://RandomNerdTutorials.com/esp32-ota-over-the-air-arduino/
   - VS Code: https://RandomNerdTutorials.com/esp32-ota-over-the-air-vs-code/
  
  This sketch shows a Basic example from the AsyncElegantOTA library: ESP32_Async_Demo
  https://github.com/ayushsharma82/AsyncElegantOTA
*/

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <Adafruit_NeoPixel.h>
#include <FastLED.h>
const char* ssid = "303c";
const char* password = "avengers1";

AsyncWebServer server(80);
long randNumber;
// the setup function runs once when you press reset or power the board

//define input pins (buttons) and output pins (relays)
// int shock_pin1 = D0;
// int shock_pin2 = D1;
int game_mode=0;
int num_players=4;
int temp_time=0;

int wait_for_player_input=2;
//different modes to consider:
//two player
//mutliplayer only one winner
//multiplayer only one loser
int temp_player1=0;
int temp_player2=0;
int temp_player3=0;
int temp_player4=0;
int temp_players[]={0,0,0,0};
int active_players[]={0,0,0,0};
int shock_pin1 = 25;
int shock_pin2 = 26;

int shock_pin3 = 27;
int shock_pin4 = 32;

// int button_pin1 = D5;
// int button_pin1 = D6;
int button_pins[]={34,35,36,39};
int shock_pins[]={25,26,27,32};
int button_pin1 = 34;
int button_pin2 = 35;
int button_pin3 = 36;
int button_pin4 = 39;
//define LED pins


int player1_time=0;
int player2_time=0;
int player3_time=0;
int player4_time=0;

int ready_to_start=0;
int start_time=0;

//LED setup
// int LED_pin1 = D7;
// int LED_pin2 = D8;
int LED_pin1 = 4;
int LED_pin2 = 2;
#define NUM_LEDS 1
#define DATA_PIN 4
CRGB leds[NUM_LEDS];
//
void setup(void) {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("beginning setup");

randomSeed(analogRead(0));
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hi! I am the shockgame ESP32.");
  });

  AsyncElegantOTA.begin(&server);    // Start ElegantOTA
  server.begin();
  Serial.println("HTTP server started");

// for (int i = 0; i <= 4; i++) {
//     pinMode(shock_pins[i], OUTPUT);
//     pinMode(button_pins[i], INPUT);
// }
  pinMode(shock_pin1, OUTPUT);
  pinMode(shock_pin2, OUTPUT);
  pinMode(shock_pin3, OUTPUT);
  pinMode(shock_pin4, OUTPUT);
  

  pinMode(button_pin1, INPUT);
  pinMode(button_pin2, INPUT);
  pinMode(button_pin3, INPUT);
  pinMode(button_pin4, INPUT);

  pinMode(LED_pin1, OUTPUT);
  pinMode(LED_pin2, OUTPUT);
}

void loop(void) {
  int n=0;
  while (n<10)
  {
    leds[0] = CRGB::Red;
  FastLED.show();
  delay(500);
  leds[0] = CRGB::Blue;
  FastLED.show();
  delay(500);
  leds[0] = CRGB::Green;
  FastLED.show();

  digitalWrite(shock_pin1,HIGH);
  digitalWrite(shock_pin2,HIGH);
  digitalWrite(shock_pin3,HIGH);
  digitalWrite(shock_pin4,HIGH);
  n++;
  delay(1000);
  digitalWrite(shock_pin1,LOW);
  digitalWrite(shock_pin2,LOW);
  digitalWrite(shock_pin3,LOW);
  digitalWrite(shock_pin4,LOW);


  }
  

  // digitalWrite(LED_pin1, LOW);
  // digitalWrite(LED_pin2, LOW);

/*
Serial.println("beginning main loop");
      //flash lights to indicate main loop starting
    digitalWrite(LED_pin1, LOW);
    // digitalWrite(LED_pin2, HIGH);
    delay(100);
    digitalWrite(LED_pin1, HIGH);
    delay(100);
    // digitalWrite(LED_pin2, LOW);
    delay(100);
    digitalWrite(LED_pin1, LOW);
    delay(100);
    // digitalWrite(LED_pin2, HIGH);
    delay(100);
    digitalWrite(LED_pin1, HIGH);
    delay(100);
    // digitalWrite(LED_pin2, HIGH);
    delay(100);

    digitalWrite(LED_pin1, LOW);
    digitalWrite(LED_pin2, LOW);
*/

  game_mode =0; //two player
  game_mode =1; // multiplayer 1 winner
  game_mode =2; // multiplayer 1 loser

 


  randNumber = random(500,6000);
  //   digitalWrite(LED_pin1, LOW);
  // digitalWrite(shock_pin1, LOW);
  //   digitalWrite(LED_pin2, LOW);
  // digitalWrite(shock_pin2, LOW);


  // if (digitalRead(button_pin1))
  // {
  //   digitalWrite(LED_pin1, HIGH);
  // digitalWrite(shock_pin1, HIGH);
  // }

  // if (digitalRead(button_pin2))
  // {
  //   digitalWrite(LED_pin2, HIGH);
  // digitalWrite(shock_pin2, HIGH);
  // }




  while (ready_to_start==0){ //loop until players are ready to start


    // run some condition to determine players are ready to start:
    player1_time=0;
    player2_time=0;
    player3_time=0;
    player4_time=0;
    // Serial.println("not ready");
    if((digitalRead(button_pin1)+digitalRead(button_pin2)+digitalRead(button_pin3)+digitalRead(button_pin4))>0){
    // if any button is pressed, check for num of players
    // digitalWrite(LED_pin1, HIGH);//turn lights on to indicate waiting for player inputs
    // digitalWrite(LED_pin2, HIGH);
    temp_time=millis();
    while (((millis()-temp_time)<wait_for_player_input)){ //wait a certain amount of time to let each player who is playing press their button
      Serial.println("waiting for players");
      if (digitalRead(button_pin1)==1){//if during the waiting period a button is pressed at all
        temp_player1=1; //set the player to active
        temp_players[0]=1;
      }
        if (digitalRead(button_pin2)==1){
        temp_player2=1;
        temp_players[1]=1;
      }
        if (digitalRead(button_pin3)==1){
        temp_player3=1;
        temp_players[2]=1;
      }
        if (digitalRead(button_pin4)==1){
        temp_player4=1;
        temp_players[3]=1;
      }
    } 
    //create some code to determine how many players:
    num_players=temp_player1+temp_player2+temp_player3+temp_player4;

    Serial.println("number of players:");
    Serial.println(num_players);
    // temp_players={0,0,0,0};
    active_players[0]=temp_player1;
    active_players[1]=temp_player2;
    active_players[2]=temp_player3;
    active_players[3]=temp_player4;

    temp_player1=0;
    temp_player2=0;
    temp_player3=0;
    temp_player4=0;

    if (num_players==2){
      game_mode=0;
    }
    if (num_players>2){
      game_mode=1;
    }
    Serial.println("game mode:");
    Serial.println(game_mode);
    }
    
    digitalWrite(LED_pin1, LOW);//turn lights off to indicate waiting for player inputs has concluded
    digitalWrite(LED_pin2, LOW);
  

 

  // if (num_players==2){
  // if (digitalRead(button_pin1)==HIGH && digitalRead(button_pin2)==HIGH){// both buttons currently pressed
  // ready_to_start=1; 
  // }
  // }

  // if (num_players==3){
  // if (digitalRead(button_pin1)==HIGH && digitalRead(button_pin2)==HIGH && digitalRead(button_pin3)==HIGH && digitalRead(button_pin4)==HIGH){// both buttons currently pressed
  // ready_to_start=1; 
  // }
  // }

  // if (num_players==4){
  // if (digitalRead(button_pin1)==HIGH && digitalRead(button_pin2)==HIGH && digitalRead(button_pin3)==HIGH && digitalRead(button_pin4)==HIGH){// both buttons currently pressed
  // ready_to_start=1; 
  // }
  // }
    if (num_players>1){//only start if players are more than 1, otherwise should restart
      if(digitalRead(button_pins[0])+digitalRead(button_pins[1])+digitalRead(button_pins[2])+digitalRead(button_pins[3])){//if exactly one button per player is pressed, then all are ready
        ready_to_start=1;
        digitalWrite(LED_pin2, HIGH);//turn green light on to indicate game is ready to start
      }
    }

  }


  if (game_mode==0){
    Serial.println("entering game mode zero loop:");
    
    //run LED sequence:
  //randomize this eventually
  digitalWrite(LED_pin1, LOW);
  // digitalWrite(LED_pin2, LOW);


  digitalWrite(LED_pin1, HIGH);
  delay(400);
  digitalWrite(LED_pin1, LOW);
  delay(300);
  digitalWrite(LED_pin1, HIGH);
  delay(250);
  digitalWrite(LED_pin1, LOW);
  delay(200);
  digitalWrite(LED_pin1, HIGH);
  delay(150);
  digitalWrite(LED_pin1, LOW);
  delay(100);
  digitalWrite(LED_pin1, HIGH);
  delay(75);
  digitalWrite(LED_pin1, LOW);
  delay(50);
  digitalWrite(LED_pin1, HIGH);
  delay(40);
  digitalWrite(LED_pin1, LOW);
  delay(30);
  digitalWrite(LED_pin1, HIGH);
  delay(20);
  digitalWrite(LED_pin1, LOW);

  delay(randNumber);
  digitalWrite(LED_pin1, HIGH);
  digitalWrite(LED_pin2, HIGH);

  //check that no buttons are pressed before turning "green"
    //reset time right before green light to accurately record time
    start_time=millis(); //need to look into max time for millis before "ticks over to zero"

  //check which button  presses first:
  //while (digitalRead(button_pin1)==LOW || digitalRead(button_pin2==LOW)){
    while (player1_time*player2_time==LOW){
      Serial.println("player1 time:");
      Serial.println(player1_time); 
    //keep looping until all buttons have been pressed:
  if (digitalRead(button_pin1)==HIGH && player1_time==0){ //this means player1 pressed the button and it had not been recorded before
    player1_time=millis()-start_time; //set registered time for player 1 (elapsed time since start of game)
  }


    if (digitalRead(button_pin2)==HIGH && player2_time==0){ //this means player2 pressed the button and it had not been recorded before
    player2_time=millis()-start_time; //set registered time for player 2 (elapsed time since start of game)
  }
  }
  Serial.println("both players pressed buttons");
  delay(500);
  //once all players have registered button press, while loop will end, now evaluate end game criteria:

  if (player2_time>player1_time){ //this means it took player2 longer to press button
    digitalWrite(shock_pin2,HIGH);//shock player2
    Serial.println("player1 wins!!");
    delay(1500);
    player1_time=0;
  player2_time=0;

  }
  if (player1_time>player2_time){
    //this means it took player1 longer to press button, player 2 wins
    digitalWrite(shock_pin1,HIGH);//shock player1
    Serial.println("player2 wins!!");
    delay(1500);
    player1_time=0;
  player2_time=0;

  }
  Serial.println("button1");
  Serial.println(digitalRead(button_pin1));
  Serial.println("btton2");
  Serial.println(digitalRead(button_pin2));

  Serial.println("button1*button2");
  Serial.println(digitalRead(button_pin2)*digitalRead(button_pin1));

  Serial.println("player 1 time");
  Serial.println(player1_time);

  Serial.println("player 2 time");
  Serial.println(player2_time);
  delay(1100);
      ready_to_start=0; // reset for next game
  digitalWrite(LED_pin1, LOW);
  digitalWrite(LED_pin2, LOW);
  digitalWrite(shock_pin1,LOW);
  digitalWrite(shock_pin2,LOW);
  player1_time=0;
  player2_time=0;
  }



  if (game_mode==1){//only one winner
  Serial.println("entering game mode 1 loop:");
    //run LED sequence:
  //randomize this eventually
  digitalWrite(LED_pin1, LOW);
  digitalWrite(LED_pin2, LOW);


  digitalWrite(LED_pin1, HIGH);
  delay(400);
  digitalWrite(LED_pin1, LOW);
  delay(300);
  digitalWrite(LED_pin1, HIGH);
  delay(250);
  digitalWrite(LED_pin1, LOW);
  delay(200);
  digitalWrite(LED_pin1, HIGH);
  delay(150);
  digitalWrite(LED_pin1, LOW);
  delay(100);
  digitalWrite(LED_pin1, HIGH);
  delay(75);
  digitalWrite(LED_pin1, LOW);
  delay(50);
  digitalWrite(LED_pin1, HIGH);
  delay(40);
  digitalWrite(LED_pin1, LOW);
  delay(30);
  digitalWrite(LED_pin1, HIGH);
  delay(20);
  digitalWrite(LED_pin1, LOW);

  delay(randNumber);
  digitalWrite(LED_pin1, HIGH);
  digitalWrite(LED_pin2, HIGH);

  //check that no buttons are pressed before turning "green"
    //reset time right before green light to accurately record time
    start_time=millis(); //need to look into max time for millis before "ticks over to zero"

  //check which button  presses first:
  //while (digitalRead(button_pin1)==LOW || digitalRead(button_pin2==LOW)){
    while (player1_time*player2_time*player3_time*player4_time==LOW){
      // Serial.println("player1 time:");
      // Serial.println(player1_time); 
    //keep looping until all buttons have been pressed:
  if (digitalRead(button_pin1)==HIGH && player1_time==0){ //this means player1 pressed the button and it had not been recorded before
    player1_time=millis()-start_time; //set registered time for player 1 (elapsed time since start of game)
  }


    if (digitalRead(button_pin2)==HIGH && player2_time==0){ //this means player2 pressed the button and it had not been recorded before
    player2_time=millis()-start_time; //set registered time for player 2 (elapsed time since start of game)
  }
    if (digitalRead(button_pin3)==HIGH && player3_time==0){ //this means player1 pressed the button and it had not been recorded before
    player3_time=millis()-start_time; //set registered time for player 1 (elapsed time since start of game)
  }


    if (digitalRead(button_pin4)==HIGH && player4_time==0){ //this means player2 pressed the button and it had not been recorded before
    player4_time=millis()-start_time; //set registered time for player 2 (elapsed time since start of game)
  }
  }
  Serial.println("all players pressed buttons");
  delay(500);
  //once all players have registered button press, while loop will end, now evaluate end game criteria:

  // if (player2_time==(max(max(player1_time,player2_time),max(player3_time,player4_time)))){ //this means it took player2 longer to press button
  if (player1_time==(min(min(player1_time,player2_time),min(player3_time,player4_time)))){ //this means that player1 time is equal to the smallest time and therefore is the winner
    digitalWrite(shock_pin2,HIGH);//shock player2
    digitalWrite(shock_pin3, HIGH);//shock player3
    digitalWrite(shock_pin4,HIGH);//shock player4
  

    Serial.println("player1 wins!!");
    delay(1500);
    player1_time=0;
    player2_time=0;
    player3_time=0;
    player4_time=0;

  }
   else if (player2_time==(min(min(player1_time,player2_time),min(player3_time,player4_time)))){ //this means that player2 time is equal to the smallest time and therefore is the winner
    digitalWrite(shock_pin1,HIGH);//shock player1
    digitalWrite(shock_pin3, HIGH);//shock player3
    digitalWrite(shock_pin4,HIGH);//shock player4
  

    Serial.println("player1 wins!!");
    delay(1500);
    player1_time=0;
    player2_time=0;
    player3_time=0;
    player4_time=0;

  }
     else if (player2_time==(min(min(player1_time,player2_time),min(player3_time,player4_time)))){ //this means that player3 time is equal to the smallest time and therefore is the winner
    digitalWrite(shock_pin2,HIGH);//shock player2
    digitalWrite(shock_pin1, HIGH);//shock player1
    digitalWrite(shock_pin4,HIGH);//shock player4
  

    Serial.println("player1 wins!!");
    delay(1500);
    player1_time=0;
    player2_time=0;
    player3_time=0;
    player4_time=0;

  }
     else if (player2_time==(min(min(player1_time,player2_time),min(player3_time,player4_time)))){ //this means that player4 time is equal to the smallest time and therefore is the winner
    digitalWrite(shock_pin2,HIGH);//shock player2
    digitalWrite(shock_pin3, HIGH);//shock player3
    digitalWrite(shock_pin1,HIGH);//shock player1
  

    Serial.println("player1 wins!!");
    delay(1500);
    player1_time=0;
    player2_time=0;
    player3_time=0;
    player4_time=0;

  }
  // if (player1_time>player2_time){
  //   //this means it took player1 longer to press button, player 2 wins
  //   digitalWrite(shock_pin1,HIGH);//shock player1
  //   Serial.println("player2 wins!!");
  //   delay(1500);
  //   player1_time=0;
  // player2_time=0;

  // }

  delay(1100);
      ready_to_start=0; // reset for next game
  digitalWrite(LED_pin1, LOW);
  digitalWrite(LED_pin2, LOW);
  digitalWrite(shock_pin1,LOW);
  digitalWrite(shock_pin2,LOW);
  digitalWrite(shock_pin3,LOW);
  digitalWrite(shock_pin4,LOW);
  player1_time=0;
  player2_time=0;
  player3_time=0;
  player4_time=0;
  }

}