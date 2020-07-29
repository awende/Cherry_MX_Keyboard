#include <Encoder.h>       //https://github.com/PaulStoffregen/Encoder
#include <BleKeyboard.h>  // https://github.com/T-vK/ESP32-BLE-Keyboard

// Define Pins used
#define V_BATT A2

#define RED_LED 27
#define GRN_LED 12

#define KEYS_0 A4
#define KEYS_1 A3

#define ENCODER_L_SW  32
#define ENCODER_R_SW  23
#define ENCODER_R0    14
#define ENCODER_R1    22
#define ENCODER_L0    33
#define ENCODER_L1    15

// LED timing
#define POWER_BLINK_TIME             10000  //10sec
#define LOW_BATT_BLINK_TIME           5000  // 5sec
#define SLEEP_AFTER_TIMEOUT_VALUE  1200000  //20Min

BleKeyboard bleKeyboard("Eagle Keyboard","SparkFun",100);
Encoder knobLeft(ENCODER_L0,ENCODER_L1);
Encoder knobRight(ENCODER_R1,ENCODER_R0);

bool grid_units=0;
char newRightValue=0;

float imperial_grid[][2] = {
  {0.050,0.0050},
  {0.025,0.0025},
  {0.0125,0.00125},
  {0.01,0.001}  
};

float metric_grid[][2] = {
  {1.27,0.005},
  {1,0.005},
  {0.5,0.0025},
  {0.25,0.0025}
};

void setup() {
  Serial.begin(115200);

  //Initialize GPIO Pins
  pinMode(ENCODER_L_SW,INPUT_PULLUP);
  pinMode(ENCODER_R_SW,INPUT_PULLUP);
  pinMode(RED_LED,OUTPUT);
  pinMode(GRN_LED,OUTPUT);

  //Initialize Deep Sleep Wakeup Interrupt (Left Encoder)
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_15,0);

  //Turn off LEDs
  digitalWrite(RED_LED,HIGH);
  digitalWrite(GRN_LED,HIGH);

  //Initialize BLE Keyboard
  bleKeyboard.begin();

  //Alternate Red/Green Leds waiting for connection
  bool state = 0;
  while(!bleKeyboard.isConnected())
  {
    digitalWrite(RED_LED,state);
    digitalWrite(GRN_LED,!state);
    state = !state;
    delay(100);
  }

  //Turn off LEDs
  digitalWrite(RED_LED,HIGH);
  digitalWrite(GRN_LED,HIGH);

  delay(250);
  
  //Double Blink Green LED to show connection
  digitalWrite(GRN_LED,LOW);
  delay(100);
  digitalWrite(GRN_LED,HIGH);
  delay(100);
  digitalWrite(GRN_LED,LOW);
  delay(100);
  digitalWrite(GRN_LED,HIGH);
}

void loop() {
  
  // Blink power led
  static double prevTime,prevTime1,wakeTime;

  // Go to deep sleep after specified time of inactivity
  if( (millis()-wakeTime) > SLEEP_AFTER_TIMEOUT_VALUE)
  {
    Serial.println("Going to sleep now");

    //Double Blink Red LED
    digitalWrite(RED_LED,LOW);
    delay(100);
    digitalWrite(RED_LED,HIGH);
    delay(100);
    digitalWrite(RED_LED,LOW);
    delay(500);
    digitalWrite(RED_LED,HIGH);
    
    esp_deep_sleep_start();
  }
  
  // Blink Active LED
  if( (millis()-prevTime) > POWER_BLINK_TIME )
  {
    prevTime = millis();
    
    //Double Blink Green LED
    digitalWrite(GRN_LED,LOW);
    delay(100);
    digitalWrite(GRN_LED,HIGH);
    delay(100);
    digitalWrite(GRN_LED,LOW);
    delay(100);
    digitalWrite(GRN_LED,HIGH);
  }

  // Convert ADC battery level to voltage
  // Battery voltage is from a voltage divider using a 47k and 100k resistor (0.68 = 100k / (100k = 47k) ) 
  float battery = (analogRead(V_BATT)/4095.0)*3.3/0.68;

  // Convert battery voltage to *rough* battery voltage. Full battery: 4.2V, Flat Battery: 3.0V
  float battPercent = ((battery-3.0)/1.2)*100;
  bleKeyboard.setBatteryLevel((uint8_t)battPercent);

  // Blink Low Battery LED
  if( ((millis()-prevTime) > LOW_BATT_BLINK_TIME) && (battery < 3.0) )
  {
    prevTime1 = millis();
    
    //Double Blink Red LED
    digitalWrite(RED_LED,LOW);
    delay(100);
    digitalWrite(RED_LED,HIGH);
    delay(100);
    digitalWrite(RED_LED,LOW);
    delay(100);
    digitalWrite(RED_LED,HIGH);
  }
  
  // Check for key press
  bool key[][4] = {
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0}
  };
  uint8_t row[] = {0,0};

  //Read keys (row0 0-7, row1 8-15)
  row[0] = (analogRead(KEYS_0)>>4);
  row[1] = (analogRead(KEYS_1)>>4);

  if((row[0] < 255) || (row[1] < 255)) wakeTime = millis(); //reset sleep timer if key is pressed

  if( (row[0]>166) && (row[0]<183) )      key[0][0] = 1;
  else if( (row[0]>134) && (row[0]<166) ) key[0][1] = 1;
  else if( (row[0]>86) && (row[0]<134) )  key[0][2] = 1;
  else if( row[0] < 86 )                 key[0][3] = 1;
  else if( (row[0]>183) && (row[0]<195) ) key[1][0] = 1;
  else if( (row[0]>195) && (row[0]<205) ) key[1][1] = 1;
  else if( (row[0]>205) && (row[0]<214) ) key[1][2] = 1;
  else if( (row[0]>214) && (row[0]<230) ) key[1][3] = 1;

  if( (row[1]>166) && (row[1]<183) )      key[2][0] = 1;
  else if( (row[1]>134) && (row[1]<166) ) key[2][1] = 1;
  else if( (row[1]>86) && (row[1]<134) )  key[2][2] = 1;
  else if( row[1] < 86 )                 key[2][3] = 1;
  else if( (row[1]>183) && (row[1]<195) ) key[3][0] = 1;
  else if( (row[1]>195) && (row[1]<205) ) key[3][1] = 1;
  else if( (row[1]>205) && (row[1]<214) ) key[3][2] = 1;
  else if( (row[1]>214) && (row[1]<230) ) key[3][3] = 1;

  // KEY ACTIONS GO HERE
  if(key[0][0] == 1)
  {
    Serial.println("Key 00 Pressed");
    bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
    //bleKeyboard.print("move\n");
  }
  if(key[0][1] == 1)
  {
    Serial.println("Key 01 Pressed");
    bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
    //bleKeyboard.print("copy\n");
  }
  if(key[0][2] == 1)
  {
    Serial.println("Key 02 Pressed");
    bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);
    //bleKeyboard.print("paste\n");
  }
  if(key[0][3] == 1)
  {
    Serial.println("Key 03 Pressed");
    bleKeyboard.write(KEY_MEDIA_CALCULATOR);
    //bleKeyboard.print("group\n");
  }
  if(key[1][0] == 1)
  {
    Serial.println("Key 10 Pressed");
    bleKeyboard.println("line");
    //bleKeyboard.write(KEY_F13);
  }
  if(key[1][1] == 1)
  {
    Serial.println("Key 11 Pressed");
    bleKeyboard.println("net");
    //bleKeyboard.write(KEY_F14);
  }
  if(key[1][2] == 1)
  {
    Serial.println("Key 12 Pressed");
    bleKeyboard.println("label");
    //bleKeyboard.write(KEY_F15);
  }
  if(key[1][3] == 1)
  {
    Serial.println("Key 13 Pressed");
    bleKeyboard.println("text");
    //bleKeyboard.write(KEY_F16);
  }
  if(key[2][0] == 1)
  {
    Serial.println("Key 20 Pressed");
    bleKeyboard.println("move");
    //bleKeyboard.write(KEY_F17);
  }
  if(key[2][1] == 1)
  {
    Serial.println("Key 21 Pressed");
    bleKeyboard.println("route");
    //bleKeyboard.write(KEY_F18);
  }
  if(key[2][2] == 1)
  {
    Serial.println("Key 22 Pressed");
    bleKeyboard.println("ripup");
    //bleKeyboard.write(KEY_F19);
  }
  if(key[2][3] == 1)
  {
    Serial.println("Key 23 Pressed");
    bleKeyboard.println("via");
    //bleKeyboard.write(KEY_F20);
  }
  if(key[3][0] == 1)
  {
    Serial.println("Key 30 Pressed");
    bleKeyboard.println("ratsnest");
    //bleKeyboard.write(KEY_F21);
  }
  if(key[3][1] == 1)
  {
    Serial.println("Key 31 Pressed");
    bleKeyboard.println("drc");
    //bleKeyboard.write(KEY_F22);
  }
  if(key[3][2] == 1)
  {
    Serial.println("Key 32 Pressed");
    bleKeyboard.println("name");
    //bleKeyboard.write(KEY_F23);
  }
  if(key[3][3] == 1)
  {
    Serial.println("Key 33 Pressed");
    bleKeyboard.println("value");
    //bleKeyboard.write(KEY_F24);
  }

  while(true)
  {
    row[0] = (analogRead(KEYS_0)>>4);
    row[1] = (analogRead(KEYS_1)>>4);
    delay(10);
    if( (row[0]>230) && (row[1]>230) ) break;
  }
  
  // Read Encoder Switches
  if(digitalRead(ENCODER_L_SW) == 0)
  {
    wakeTime = millis(); //Reset sleep timer
    bleKeyboard.write(KEY_MEDIA_MUTE);
    delay(10);
    while(digitalRead(ENCODER_L_SW) == 0)
    {
      delay(10);
    }
  }

  if(digitalRead(ENCODER_R_SW) == 0)
  {
    wakeTime = millis(); //Reset sleep timer
    grid_units = !grid_units;
    if(grid_units == 1)
    {
      bleKeyboard.print("grid mm\n");
    }
    else
    {
      bleKeyboard.print("grid inch\n");
    }
    
    delay(10);
    while(digitalRead(ENCODER_R_SW) == 0)
    {
      delay(10);
    }
  }

  // Read Encoders
  int8_t leftValue=0,rightValue=0;
  leftValue = knobLeft.read();
  rightValue = knobRight.read()/4;

  if((leftValue!=0) || (rightValue!=0)) wakeTime = millis(); //Reset sleep timer

  // Left Encoder
  if(leftValue > 3)
  {
    Serial.println("Volume Up");
    knobLeft.write(0);
    bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
  }
  else if(leftValue < -3)
  {
    Serial.println("Volume Down");
    knobLeft.write(0);
    bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
  }

  // Right Encoder
  if(newRightValue != rightValue)
  {
    if(rightValue < 0)
    {
      rightValue = 0;
      knobRight.write(0);
      if(grid_units == 0) // Imperial
      {
        bleKeyboard.print("grid ");
        bleKeyboard.println(imperial_grid[0][0],5);
        bleKeyboard.print("grid alt ");
        bleKeyboard.println(imperial_grid[0][1],5);
      }
      else  // Metric
      {
        bleKeyboard.print("grid ");
        bleKeyboard.println(metric_grid[0][0],5);
        bleKeyboard.print("grid alt ");
        bleKeyboard.println(metric_grid[0][1],5);
      }
    }
    else
    {
      if(rightValue > 3)
      {
        rightValue = 3;
        knobRight.write(12);
      }
      if(grid_units == 0) // Imperial
      {
        bleKeyboard.print("grid ");
        bleKeyboard.println(imperial_grid[rightValue][0],5);
        bleKeyboard.print("grid alt ");
        bleKeyboard.println(imperial_grid[rightValue][1],5);
      }
      else  // Metric
      {
        bleKeyboard.print("grid ");
        bleKeyboard.println(metric_grid[rightValue][0],5);
        bleKeyboard.print("grid alt ");
        bleKeyboard.println(metric_grid[rightValue][1],5);
      }
    }
    newRightValue = rightValue;
  }
}
