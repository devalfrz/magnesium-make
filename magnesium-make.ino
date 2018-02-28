/*
*  Magnesium Make
*  This project was developed as an idea of how to implement
*  automatized games during a Paint Ball session. The code was
*  built for the Arduino/Genuino Uno.
*  
*  This code includes two games:
*    - Conquest:
*        The team that keeps control of the base for the most
*        time will win.
*    - King Of The Hill:
*        The last team to control the base will win.
*        
*  Alfredo Rius 2018
*  alfredo.rius@gmail.com
*/

// Version
#define VERSION "Magnesium   v1.0"

// Buttons
#define TEAM_R 3
#define TEAM_G 2
#define LED_R 10
#define LED_G 11
#define LED_B 9

#include <LiquidCrystal.h>

// Initialize LCD
LiquidCrystal lcd(4, 5, 7, 8, 12, 13);

// Games
#define NO_GAME          0
#define CONQUEST         1
#define CAPTURE_THE_FLAG 2
#define RUSH             3

byte game = NO_GAME;
byte last_game = NO_GAME;

/*
000-009 mode
010-019 variables
250-254 set
255 end game
*/
#define END_GAME 255

byte state = 0;

unsigned long game_time = 0;
unsigned long game_countdown = 0;


byte team = 0;
unsigned long team_red = 0;
unsigned long team_green = 0;
unsigned long time_left;
unsigned long time_elapsed;
unsigned long score = 0;
boolean first_run = true;


void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  pinMode(TEAM_R, INPUT_PULLUP);
  pinMode(TEAM_G, INPUT_PULLUP);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(TEAM_R), set, FALLING);
  attachInterrupt(digitalPinToInterrupt(TEAM_G), select, FALLING);
  lcd.setCursor(0,0);
  lcd.print(VERSION);
  lcd.setCursor(0,1);
  lcd.print("                ");
  delay(2000);
  lcd.setCursor(0,0);
  lcd.print("Select mode:    ");
  lcd.setCursor(0,1);
  lcd.print("                ");
  
}

void loop() {
  if(game == CONQUEST){
    if(state == 250 || state == 251){
      delay(100);
      display_time();
    }
  }else if(game == CAPTURE_THE_FLAG){
    if(state == 250 || state == 251){
      delay(100);
      display_time();
    }
  }
  if(state == END_GAME && game == NO_GAME){
    delay(500);
    if(team == TEAM_G){
      RGB_green();
    }else if(team == TEAM_R){
      RGB_red();
    }else{
      RGB_white();
    }
    delay(500);
    RGB_off();
  }
}

void set(){
  if(game){
    if(game == CONQUEST){ // Conquest
      state ++;
      if(state == 1){
        lcd.setCursor(0,0);
        lcd.print("Set play time:  ");
        lcd.setCursor(0,1);
        lcd.print("                ");
      }else if(state == 2){
        lcd.setCursor(0,0);
        lcd.print("Set countdown:  ");
        lcd.setCursor(0,1);
        lcd.print("                ");
      }else if(state == 3){
        lcd.setCursor(0,0);
        lcd.print("Press \"Set\" To");
        lcd.setCursor(0,1);
        lcd.print("Start Countdown");
      }else if(state == 4){
        state = 250;
        detachInterrupt(digitalPinToInterrupt(TEAM_R));
        detachInterrupt(digitalPinToInterrupt(TEAM_G));
        lcd.setCursor(0,0);
        lcd.print("Game begins in: ");
        lcd.setCursor(0,1);
        lcd.print("                ");
        reset_timer(game_countdown);
      }
    }
    else if(game == CAPTURE_THE_FLAG){ // Capture The Flag
      state ++;
      if(state == 1){
        lcd.setCursor(0,0);
        lcd.print("Set play time:  ");
        lcd.setCursor(0,1);
        lcd.print("                ");
      }else if(state == 2){
        lcd.setCursor(0,0);
        lcd.print("Set countdown:  ");
        lcd.setCursor(0,1);
        lcd.print("                ");
      }else if(state == 3){
        lcd.setCursor(0,0);
        lcd.print("Press \"Set\" To");
        lcd.setCursor(0,1);
        lcd.print("Start Countdown");
      }else if(state == 4){
        state = 250;
        detachInterrupt(digitalPinToInterrupt(TEAM_R));
        detachInterrupt(digitalPinToInterrupt(TEAM_G));
        lcd.setCursor(0,0);
        lcd.print("Game begins in: ");
        lcd.setCursor(0,1);
        lcd.print("                ");
        reset_timer(game_countdown);
      }
    }
  }
}

void select(){
  if(state == NO_GAME){
    lcd.setCursor(0,0);
    lcd.print("Select mode:     ");
    lcd.setCursor(0,1);
    game = (game >= 2)? 1 : game+1;
    switch(game){
      case CAPTURE_THE_FLAG:
          lcd.print("Conquest        ");
          break;
      case CONQUEST:
          lcd.print("King Of The Hill");
          break;
      //case 3:
      //    lcd.print("Rush            ");
      //    break;
    }
  }else{
    if(game == CONQUEST){ // Conquest
      if(state == 1){
        if(game_time < 5*60){
          game_time += 60;
        }else if(game_time < 60*60){
          game_time += 5*60;
        }else if(game_time < 2*60*60){
          game_time += 10*60;
        }else{
          game_time = 60;
        }
        lcd.setCursor(0,1);
        lcd.print("                ");
        lcd.setCursor(0,1);
        lcd.print(game_time/60);
        lcd.print(" min");
      }else if(state == 2){
        if(game_countdown < 30*60){
          game_countdown += 60;
        }else if(game_countdown < 60*60){
          game_countdown += 5*60;
        }else if(game_countdown < 2*60*60){
          game_countdown += 10*60;
        }else{
          game_countdown = 60;
        }
        lcd.setCursor(0,1);
        lcd.print("                ");
        lcd.setCursor(0,1);
        lcd.print(game_countdown/60);
        lcd.print(" min");
      }
    }
    else if(game == CAPTURE_THE_FLAG){ // Capture The Flag
      if(state == 1){
        if(game_time < 5*60){
          game_time += 60;
        }else if(game_time < 60*60){
          game_time += 5*60;
        }else if(game_time < 3*60*60){
          game_time += 10*60;
        }else{
          game_time = 60;
        }
        lcd.setCursor(0,1);
        lcd.print("                ");
        lcd.setCursor(0,1);
        lcd.print(game_time/60);
        lcd.print(" min");
      }else if(state == 2){
        if(game_countdown < 30*60){
          game_countdown += 60;
        }else if(game_countdown < 60*60){
          game_countdown += 5*60;
        }else if(game_countdown < 2*60*60){
          game_countdown += 10*60;
        }else{
          game_countdown = 60;
        }
        lcd.setCursor(0,1);
        lcd.print("                ");
        lcd.setCursor(0,1);
        lcd.print(game_countdown/60);
        lcd.print(" min");
      }
    }
  }
}



void count_red(){
  if(game == 1){
    if(!team){
      score = millis();
    }else if(team == TEAM_G){
      team_green += millis() - score;
      score = millis();
    }
    team = TEAM_R;
    print_score();
  }else if(game == 2){
    team = TEAM_R;
    print_king();
  }
}

void count_green(){
  if(game == 1){
    if(!team){
      score = millis();
    }else if(team == TEAM_R){
      team_red += millis() - score;
      score = millis();
    }
    team = TEAM_G;
    print_score();
  }else if(game == 2){
    team = TEAM_G;
    print_king();
  }
}


void RGB_green(){
  digitalWrite(LED_R,LOW);
  digitalWrite(LED_G,HIGH);
  digitalWrite(LED_B,LOW);
}
void RGB_red(){
  digitalWrite(LED_R,HIGH);
  digitalWrite(LED_G,LOW);
  digitalWrite(LED_B,LOW);
}
void RGB_off(){
  digitalWrite(LED_R,LOW);
  digitalWrite(LED_G,LOW);
  digitalWrite(LED_B,LOW);
}

void RGB_white(){
  digitalWrite(LED_R,HIGH);
  digitalWrite(LED_G,HIGH);
  digitalWrite(LED_B,HIGH);
}


void print_score(){
  if(team == TEAM_R){
    lcd.setCursor(11, 1);
    lcd.print("  RED");
    RGB_red();
  }else if(team == TEAM_G){
    lcd.setCursor(11, 1);
    lcd.print("GREEN");
    RGB_green();
  }
  lcd.setCursor(0, 0);
  lcd.write("                ");
  lcd.setCursor(0, 0);
  lcd.print("G: ");
  lcd.print(team_green/1000);
  lcd.setCursor(8, 0);
  lcd.print("R: ");
  lcd.print(team_red/1000);
}


void print_king(){
  lcd.setCursor(0, 0);
  if(team == TEAM_R){
    lcd.print("      RED       ");
    RGB_red();
  }else if(team == TEAM_G){
    lcd.print("     GREEN      ");
    RGB_green();
  }
}

void display_time(){
  //time_elapsed = millis() - time_left;
  time_elapsed = time_left - millis();
  if(time_elapsed > 10800000){// Max time is 3 hrs
    if(game == 1){
      if(state == 250){
        reset_timer(game_time);
        time_elapsed = time_left - millis();
        print_score();
        attachInterrupt(digitalPinToInterrupt(TEAM_R), count_red, FALLING);
        attachInterrupt(digitalPinToInterrupt(TEAM_G), count_green, FALLING);
        state = 251;
      }else if(state == 251){
        game = 0;
        state = 255;
        if(team == TEAM_G){
          team_green += millis() - score;
        }else if(team == TEAM_R){
          team_red += millis() - score;
        }
      }
    }else if(game == 2){
      if(state == 250){
        reset_timer(game_time);
        time_elapsed = time_left - millis();
        lcd.setCursor(0,0);
        lcd.print("King Of The Hill");
        lcd.setCursor(0,1);
        lcd.print("                ");
        attachInterrupt(digitalPinToInterrupt(TEAM_R), count_red, FALLING);
        attachInterrupt(digitalPinToInterrupt(TEAM_G), count_green, FALLING);
        state = 251;
      }else if(state == 251){
        game = 0;
        state = 255;
        if(team == TEAM_G){
          team_green = 1;
          team_red = 0;
        }else if(team == TEAM_R){
          team_green = 0;
          team_red = 1;
        }
      }
    }
  }
  
  if(game){
    last_game=game;
    lcd.setCursor(0, 1);
    if(time_elapsed>60000){
      lcd.print(time_elapsed/60000);
      if(time_elapsed<600000)
        lcd.print(" min ");
      else
        lcd.print(" min");
    }else{
      lcd.print(time_elapsed/1000);
      if(time_elapsed<10000)
        lcd.print(" sec ");
      else
        lcd.print(" sec");
    }
  }else if(team_red < team_green){

  lcd.setCursor(0, 0);
  lcd.write("                ");
  if(last_game==CONQUEST){
    lcd.setCursor(0, 0);
    lcd.print("G: ");
    lcd.print(team_green/1000);
    lcd.setCursor(8, 0);
    lcd.print("R: ");
    lcd.print(team_red/1000);
  }

    lcd.setCursor(0, 1);
    lcd.print("WINNER: GREEN T ");
    team = TEAM_G;
  }else if(team_red > team_green){



  lcd.setCursor(0, 0);
  lcd.write("                ");
  if(last_game==CONQUEST){
    lcd.setCursor(0, 0);
    lcd.print("G: ");
    lcd.print(team_green/1000);
    lcd.setCursor(8, 0);
    lcd.print("R: ");
    lcd.print(team_red/1000);
  }


    lcd.setCursor(0, 1);
    lcd.print("WINNER: RED TEAM");
    team = TEAM_R;
  }else{
    lcd.setCursor(0, 0);
    lcd.print("     TIED!!      ");
    lcd.setCursor(0, 1);
    lcd.print("                ");
    team = 0;
  }
}  
void reset_timer(long seconds){
  time_left = millis()+(seconds*1000);
  //time_left = millis();
}
