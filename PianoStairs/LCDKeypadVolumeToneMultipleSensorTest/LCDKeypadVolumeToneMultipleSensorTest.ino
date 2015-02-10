#include <SharpIR.h>
#include <LCD4Bit_mod.h> 

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

#define NUMSENSORS 8
#define MAXVOLUME 4
#define NUMKEYS 5

/*KEYPAD
0 - Right Key
1 - Up Key
2 - Down Key
3 - Left Key
4 - Select Key
*/
 
//number of lines in display=2
LCD4Bit_mod lcd = LCD4Bit_mod(2); 
int  adc_key_val[5] ={30, 150, 360, 535, 760 };
int adc_key_in;
int key=-1;
int oldkey=-1;
int volume = 3;
int pitch = 0;
int frequencies[] = { NOTE_C3, NOTE_D3, NOTE_E3, NOTE_F3, NOTE_G3+1, NOTE_A3+2, NOTE_B3+3, NOTE_C4+4 };
int pins_digital[] = { 42, 38, 34, 30 };
int pins_analog[] = { 8, 9, 10, 11, 12, 13, 14, 15 };
int init_distances[NUMSENSORS];
SharpIR sensors[NUMSENSORS];

void setup() { 
  Serial.begin(9600);
  
  pinMode(13, OUTPUT);  //we'll use the debug LED to output a heartbeat
  
  lcd.init();
  //optionally, now set up our application-specific display settings, overriding whatever the lcd did in lcd.init()
  //lcd.commandWrite(0x0F);//cursor on, display on, blink on.  (nasty!)
  lcd.clear();
  lcd.printIn("Piano Stairs ON");
  lcd.cursorTo(2, 0); //line=2, x=0
  lcd.printIn("Volume: ");
  lcd.cursorTo(2, 8);
  lcd.print(volume + 48);
  //tone(pins_digital[volume], frequencies[0]);
  
  for (int i = 0; i < NUMSENSORS; i++) {
    SharpIR temp_sensor(pins_analog[i], 25, 93, 20150);
    sensors[i] = temp_sensor;
    
    int init_dist = temp_sensor.distance();
    if (init_dist <= 150 && init_dist >= 20) {
      //This is the effective range
      init_distances[i] = init_dist;
    }
    else {
      //error if outside of effective range. Set to max range
      init_distances[i] = 150;
    }

    Serial.print("Initial Actual Distance: ");
    Serial.println(init_dist);
    Serial.print("  Set Distance: ");
    Serial.println(init_distances[i]);
  }
}

void loop() 
{
  adc_key_in = analogRead(0);    // read the value from the sensor  
  digitalWrite(13, HIGH);  
  key = get_key(adc_key_in);    // convert into key press
  if (key != oldkey)	// if keypress is detected
  {
    delay(50);		// wait for debounce time
    adc_key_in = analogRead(0);    // read the value from the sensor  
    key = get_key(adc_key_in);    // convert into key press
    if (key != oldkey)				
    {			
      oldkey = key;
      if (key == 1 || key == 2) {
        processUpDown(key);
      }
      else if (key == 4) {
        processSelect();
      }
    }
  }
  digitalWrite(13, LOW);
  //Check each sensor to monitor movement. 
  //Turn sound on at specified volume if movement is detected.
  //Otherwise turn off for that sensor.
  for (int i = 0; i < NUMSENSORS; i++) {
    int temp_dist = sensors[i].distance();
    if (temp_dist <= (init_distances[i] - 2) && temp_dist >= 20) {
      tone(pins_digital[volume], frequencies[i]);
      delay(100);
    }
    else {
      noTone(pins_digital[volume]);
    }
  }
}

// Convert ADC value to key number
int get_key(unsigned int input)
{	
  int k;
  for (k = 0; k < NUMKEYS; k++)
  {
    if (input < adc_key_val[k])
    {
      return k;
    }
  }
  if (k >= NUMKEYS)
    k = -1;     // No valid key pressed
  return k; 
}

void processUpDown(int k) {
  noTone(pins_digital[volume]);
  if (k == 2) {
    //volume down
    if (volume > 0) {
      volume--;
    }
  }
  else { //k == 1
    //volume up
    if (volume < MAXVOLUME - 1) {
      volume++;
    }
  }
  lcd.cursorTo(2, 8);
  lcd.print(volume + 48);
  tone(pins_digital[volume], frequencies[pitch]);
}

void processSelect() {
  
  noTone(pins_digital[volume]);
  pitch++;
  if (pitch == NUMSENSORS) {
    pitch = 0;
  }
  tone(pins_digital[volume], frequencies[pitch]);
}
