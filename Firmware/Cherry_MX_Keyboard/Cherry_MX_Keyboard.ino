/******************************************************************************
Cherry_MX_Keyboard.ino
A custom Eagle shortcut keyboard
Alex Wende @ SparkFun Electronics
June 14th 2016
~

This sketch turns the SparkFun Pro Micro (https://www.sparkfun.com/products/12587)
into a USB keyboard. Each of the 16 buttons and the right encoder sets a specific 
Eagle command that can be changed to whatever you'd like! The left encoder controls
the volume, and mute.

Resources:
HID-Project.h (https://github.com/NicoHood/HID)
Encoder.h (https://www.pjrc.com/teensy/td_libs_Encoder.html)

Development environment specifics:
Arduino 1.0+

This code is beerware; if you see me (or any other SparkFun employee) at
the local, and you've found our code helpful, please buy us a round!

Distributed as-is; no warranty is given.   
******************************************************************************/

#include <Encoder.h>
#include "HID-Project.h"

#define KEY_ROW0    A0 
#define KEY_ROW1    A2
#define KEY_ROW2    A1
#define KEY_ROW3    A3

#define ENCODER_L_SW  9
#define ENCODER_R_SW  4
#define ENCODER_R0  2
#define ENCODER_R1  3
#define ENCODER_L0  7
#define ENCODER_L1  8

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
  Serial.begin(9600);
  pinMode(ENCODER_L_SW,INPUT_PULLUP);
  pinMode(ENCODER_R_SW,INPUT_PULLUP);
  Consumer.begin();
  Keyboard.begin();
}

void loop() {
  // Check for key press
  bool key[][4] = {
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0}
  };
  int row[] = {0,0,0,0};
  
  row[0] = analogRead(KEY_ROW0);
  row[1] = analogRead(KEY_ROW1);
  row[2] = analogRead(KEY_ROW2);
  row[3] = analogRead(KEY_ROW3);
  
  for(byte i=0;i<4;i++)
  {
    if( row[i] < 40 ) // Column 3
    {
      key[i][3] = 1;
    }
    else if( (row[i] - 510) < 40 ) // Column 2
    {
      key[i][2] = 1;
    }
    else if( (row[i] - 680) < 40 ) // Column 1
    {
      key[i][1] = 1;
    }
    else if( (row[i] - 770) < 40 ) // Column 0
    {
      key[i][0] = 1;
    }
  }

  // KEY ACTIONS GO HERE
  if(key[0][0] == 1)
  {
    Serial.println("Key 00 Pressed");
    Keyboard.print("move\n");
    delay(100);
  }
  if(key[0][1] == 1)
  {
    Serial.println("Key 01 Pressed");
    Keyboard.print("copy\n");
    delay(100);
  }
  if(key[0][2] == 1)
  {
    Serial.println("Key 02 Pressed");
    Keyboard.print("paste\n");
    delay(100);
  }
  if(key[0][3] == 1)
  {
    Serial.println("Key 03 Pressed");
    Keyboard.print("group\n");
    delay(100);
  }
  if(key[1][0] == 1)
  {
    Serial.println("Key 10 Pressed");
    Keyboard.print("name\n");
    delay(100);
  }
  if(key[1][1] == 1)
  {
    Serial.println("Key 11 Pressed");
    Keyboard.print("value\n");
    delay(100);
  }
  if(key[1][2] == 1)
  {
    Serial.println("Key 12 Pressed");
    Keyboard.print("smash\n");
    delay(100);
  }
  if(key[1][3] == 1)
  {
    Serial.println("Key 13 Pressed");
    Keyboard.print("miter\n");
    delay(100);
  }
  if(key[2][0] == 1)
  {
    Serial.println("Key 20 Pressed");
    Keyboard.print("split\n");
    delay(100);
  }
  if(key[2][1] == 1)
  {
    Serial.println("Key 21 Pressed");
    Keyboard.print("route\n");
    delay(100);
  }
  if(key[2][2] == 1)
  {
    Serial.println("Key 22 Pressed");
    Keyboard.print("ripup\n");
    delay(100);
  }
  if(key[2][3] == 1)
  {
    Serial.println("Key 23 Pressed");
    Keyboard.print("wire\n");
    delay(100);
  }
  if(key[3][0] == 1)
  {
    Serial.println("Key 30 Pressed");
    Keyboard.print("text\n");
    delay(100);
  }
  if(key[3][1] == 1)
  {
    Serial.println("Key 31 Pressed");
    Keyboard.print("via\n");
    delay(100);
  }
  if(key[3][2] == 1)
  {
    Serial.println("Key 32 Pressed");
    Keyboard.print("drc\n");
    delay(100);
  }
  if(key[3][3] == 1)
  {
    Serial.println("Key 33 Pressed");
    Keyboard.print("ratsnest\n");
    delay(100);
  }

  // Read Encoder Switches
  if(digitalRead(ENCODER_L_SW) == 0)
  {
    Consumer.write(MEDIA_VOLUME_MUTE);
    delay(10);
    while(digitalRead(ENCODER_L_SW) == 0)
    {
      delay(10);
    }
  }

  if(digitalRead(ENCODER_R_SW) == 0)
  {
    grid_units = !grid_units;
    if(grid_units == 1)
    {
      Keyboard.print("grid mm\n");
    }
    else
    {
      Keyboard.print("grid inch\n");
    }
    
    delay(10);
    while(digitalRead(ENCODER_R_SW) == 0)
    {
      delay(10);
    }
  }

  // Read Encoders
  char leftValue=0,rightValue=0;
  leftValue = knobLeft.read();
  rightValue = knobRight.read()/4;

  // Left Encoder
  if(leftValue > 3)
  {
    Serial.println("Volume Up");
    knobLeft.write(0);
    Consumer.write(MEDIA_VOL_UP);
  }
  else if(leftValue < -3)
  {
    Serial.println("Volume Down");
    knobLeft.write(0);
    Consumer.write(MEDIA_VOL_DOWN);
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
        Keyboard.print("grid ");
        Keyboard.println(imperial_grid[0][0],5);
        Keyboard.print("grid alt ");
        Keyboard.println(imperial_grid[0][1],5);
      }
      else  // Metric
      {
        Keyboard.print("grid ");
        Keyboard.println(metric_grid[0][0],5);
        Keyboard.print("grid alt ");
        Keyboard.println(metric_grid[0][1],5);
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
        Keyboard.print("grid ");
        Keyboard.println(imperial_grid[rightValue][0],5);
        Keyboard.print("grid alt ");
        Keyboard.println(imperial_grid[rightValue][1],5);
      }
      else  // Metric
      {
        Keyboard.print("grid ");
        Keyboard.println(metric_grid[rightValue][0],5);
        Keyboard.print("grid alt ");
        Keyboard.println(metric_grid[rightValue][1],5);
      }
      
    }
    newRightValue = rightValue;
  }
}

