#include <Bounce.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

//documentation for these libraries are found here: https://www.pjrc.com/teensy/gui/?info=AudioPlaySdWav

AudioPlaySdWav           playSdWav2;     //xy=291,430
AudioPlaySdWav           playSdWav1;     //xy=296,301
AudioMixer4              mixer1;         //xy=563,363
AudioOutputI2S           i2s1;           //xy=780,362
AudioConnection          patchCord1(playSdWav2, 0, mixer1, 1);
AudioConnection          patchCord2(playSdWav1, 0, mixer1, 0);
AudioConnection          patchCord3(mixer1, 0, i2s1, 0);
AudioConnection          patchCord4(mixer1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=64.5,50

// Use these with the Teensy Audio Shield
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

//int buttonPins[] = {0,1,2,3,4,5,8,15,16,17,20,21};

Bounce buttonA = Bounce(0, 10);
Bounce buttonB = Bounce(1, 10);
Bounce buttonY = Bounce(2, 10);
Bounce buttonX = Bounce(3, 10);
Bounce buttonR = Bounce(4, 10);
Bounce buttonL = Bounce(5, 10);
Bounce buttonStart = Bounce(8, 10);
Bounce buttonUp = Bounce(15, 10);
Bounce buttonRight = Bounce(16, 10);
Bounce buttonDown = Bounce(17, 10);
Bounce buttonLeft = Bounce(20, 10);
Bounce buttonSelect = Bounce(21, 10);



const char *currentPreset;
void playFile(const char *input) {
    char combined[32] = {0};
    Serial.println(input);
    strcpy(combined, currentPreset);
    strcat(combined, "/");
    strcat(combined, input);
  //combined now looks like "_GAMES/MARIO/A" if input is "A." 
  //SD.open will then find the first file in the "A" folder and the name of this file will be concatenated onto combined variable

    File folder = SD.open(combined);
    File sound = folder.openNextFile();
    if (!sound) {
      Serial.println("no file to play");
      return;
      }
    strcat(combined, "/");
    strcat(combined, sound.name());


  
    Serial.println("combined: ");
    Serial.println(combined);
    //play the file
    playSdWav1.play(combined);
}

    


void setup() {

  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);
  pinMode(17, INPUT_PULLUP);
  pinMode(20, INPUT_PULLUP);
  pinMode(21, INPUT_PULLUP);
    
  currentPreset = "_MEGAMAN";
    

  Serial.begin(9600);
  AudioMemory(8);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  if (!(SD.begin(SDCARD_CS_PIN))) {
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  delay(1000);
  }
  delay(1000);   
}

void loop() {
//going to loop hihat in the delay of the tempo
  if (!playSdWav2.isPlaying()) {
    playSdWav2.play("BOSS.WAV");
  }
  //what to do for each button press
  if (buttonA.update()){
    if (buttonA.fallingEdge()){
      playFile("A");
      //playSdWav1.play("JUMP.WAV");
    }
  }
}
