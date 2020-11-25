#include <Bounce.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Vector.h>

//documentation for these libraries are found here: https://www.pjrc.com/teensy/gui/?info=AudioPlaySdWav

AudioPlaySdWav           playSdWav2;     //xy=167,305
AudioPlaySdWav           playSdWav1;     //xy=172,176
AudioMixer4              mixer1;         //xy=438,219
AudioOutputI2S           i2s1;           //xy=735,258
AudioConnection          patchCord1(playSdWav2, 0, mixer1, 2);
AudioConnection          patchCord2(playSdWav2, 1, mixer1, 3);
AudioConnection          patchCord3(playSdWav1, 0, mixer1, 0);
AudioConnection          patchCord4(playSdWav1, 1, mixer1, 1);
AudioConnection          patchCord5(mixer1, 0, i2s1, 0);
AudioConnection          patchCord6(mixer1, 0, i2s1, 1);

AudioControlSGTL5000     sgtl5000_1;

// Use these with the Teensy Audio Shield
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

//int buttonPins[] = {0,1,2,3,4,5,8,15,16,17,20,21};

Bounce buttonA = Bounce(0, 10);
Bounce buttonB = Bounce(1, 10);
Bounce buttonX = Bounce(2, 10);
Bounce buttonY = Bounce(3, 10);
Bounce buttonL = Bounce(4, 10);
Bounce buttonR = Bounce(5, 10);
Bounce buttonDown = Bounce(15, 10);
Bounce buttonRight = Bounce(16, 10);
Bounce buttonUp = Bounce(17, 10);
Bounce buttonLeft = Bounce(20, 10);
Bounce buttonStart = Bounce(8, 10);
Bounce buttonSelect = Bounce(21, 10);




int selectPin = 21;

bool loopOn = false;

bool gameState = true;

float vol = 0.8;

int tempo = 200;

const char *drumPresets[4];
const char *gamePresets[4];

int numberOfDrums = sizeof(drumPresets)/sizeof(drumPresets[0]);
int numberOfGames = sizeof(gamePresets)/sizeof(gamePresets[0]);

int currentIndex = 0;

const char *currentPreset;

void playFile(const char *input) {
    char combined[32] = {0};
    
    strcpy(combined, currentPreset);
    strcat(combined, "/");
    strcat(combined, input);
    File folder = SD.open(combined);
    File sound = folder.openNextFile();
    strcat(combined, sound.name());
    playSdWav1.play(combined);

}

void playRandomBgr() {
  
  char combined[32] = {0};
  
  strcpy(combined, currentPreset);
  strcat(combined, "/bgr");
  File directory = SD.open(combined);
  
  int bgrSize = 0;
  
  while (true) {
    File entry = directory.openNextFile();
    if (entry) {
      bgrSize++
    }else{
      directory.rewindDirectory();
      srand((unsigned)millis());
      int randIndex = rand() % bgrSize;
      for (int i = 1; i <= randIndex; i++) {
        File entry = directory.openNextFile();
        if (i == randIndex) {
          strcat(combined, entry.name());
          playSdWav2.play(combined);
        }
      break;
    }
  }  
}

char konamiCode[11][9] = {"Up", "Up", "Down", "Down", "Left", "Right", "Left", "Right", "B", "A", "Start"};
    int konamiCodePlace = 0;
    bool konamiCodeCheck(const char *input) {
      if (!strcmp(konamiCode[konamiCodePlace], input)){
          if (konamiCodePlace == 10) {
              konamiCodePlace = 0;
              playSdWav2.stop();
              Serial.println("***You did the konami code!***");
              gameState = !gameState;
              //change so that you can get out of drumPresets if done a second time
              currentPreset = drumPresets[0];
              Serial.print(currentPreset.folder);
          }else{
              konamiCodePlace++;
          }
        return true;
      }else{
        konamiCodePlace = 0;
        return false;
      }
    }


void setup() {

  pinMode(0, INPUT);
  pinMode(1, INPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(8, INPUT);
  pinMode(15, INPUT);
  pinMode(16, INPUT);
  pinMode(17, INPUT);
  pinMode(20, INPUT);
  pinMode(21, INPUT);
  
  drumPresets[0] = "postal";
  drumPresets[1] = "808"
  drumPresets[2] = "909";
  drumPresets[3] = "vinyl";
  
  gamePresets[0] = "megaman";
  gamePresets[1] = "smb";
  gamePresets[2] = "zelda";
  gamePresets[3] = "sonic";

    
  currentPreset = gamePresets[currentIndex];

  Serial.begin(9600);
  AudioMemory(8);
  sgtl5000_1.enable();
  sgtl5000_1.volume(vol);
  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  if (!(SD.begin(SDCARD_CS_PIN))) {
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
  delay(1000);    
}

void loop() {
//going to loop hihat in the delay of the tempo
  if (!gameState){
    if (loopOn == true) {
      //too loud
      playFile("Y");
      delay(tempo);
    }
  }

  if (buttonA.update()){
    if (buttonA.fallingEdge()){
      playFile("A");
      konamiCodeCheck("A");
    }
  }

  if (buttonB.update()){
    if (buttonB.fallingEdge()){
      playFile("B");
      konamiCodeCheck("B");
    }
  }
  if (buttonX.update()){
    if (buttonX.fallingEdge()){
      playFile("X");
      konamiCodePlace = 0;
    }
  }

  if (buttonY.update()){
    if (buttonY.fallingEdge()){
      playFile("Y");
      konamiCodePlace = 0;
    }
  }

  if (buttonUp.update()){
    if (buttonUp.fallingEdge()){
      if (digitalRead(selectPin) == LOW && vol < 0.8){
        vol = vol + 0.1;
        sgtl5000_1.volume(vol);
      }
      playFile("Up");
      konamiCodeCheck("Up");
    }
  }

  if (buttonDown.update()){
    if (buttonDown.fallingEdge()){
      playFile("Down");
      if (digitalRead(selectPin) == LOW && vol > 0){
        vol = vol - 0.1;
        sgtl5000_1.volume(vol);
      }
      konamiCodeCheck("Down");
    }
  }

  if (buttonLeft.update()){
    if (buttonLeft.fallingEdge()){
      //if select is pressed also
      if (digitalRead(selectPin) == LOW){
        playSdWav2.stop();
        //if it's greater than zero, increment up
        if (currentIndex > 0){
          currentIndex--;
          Serial.print(currentIndex);
        //if it is zero, cycle back around to the last index
        }else{
          //what to do if in gameState
          if (gameState){
            //currentIndex = last index of gamePresets
            currentIndex = numberOfGames - 1;
            Serial.print(currentIndex);
          }else{
            //currentIndex = last index of drumPresets
            currentIndex = numberOfDrums - 1;
            Serial.print(currentIndex);
          }
        }
        //sets the current preset with the previously set index
        if (gameState) {
          currentPreset = gamePresets[currentIndex];
        }else{
          currentPreset = drumPresets[currentIndex];
        }
      }else{
        //do this only if select isn't pressed
         konamiCodeCheck("Left");
      }
      //do this if select is pressed or not (put this with konamiCodeCheck if it should only be done without select
      playFile("Left");
    }
  }

  if (buttonRight.update()){
    if (buttonRight.fallingEdge()){
      //if select is pressed
      if (digitalRead(selectPin) == LOW){
        playSdWav2.stop();
        //if not in game state
        if (!gameState) {
          //notworking here?
          //if not at the end of the list, index goes up
          if (currentIndex < numberOfDrums - 1){
            Serial.println(currentIndex);
            currentIndex++;
            Serial.println(currentIndex);
          }else{
            //if at the end of the list, index goes back to zero
            currentIndex = 0;
          }
          //sets the drum preset
          currentPreset = drumPresets[currentIndex];
          Serial.println(currentPreset.folder);
          //what to do if in gamestate
        }else{
          //if not at the end of the list, index goes up
          if (currentIndex < numberOfGames - 1){
            currentIndex++;
            Serial.print(currentIndex);
          }else{
            //if at the end of the list, index goes back to zero
            currentIndex = 0;
            Serial.print(currentIndex);
          }
          currentPreset = gamePresets[currentIndex];
        }
      }else{
        //check for konami code if select is not being pressed
        konamiCodeCheck("Right");
      }
      //plays no matter what
      playFile("Right");
    }
  }

   if (buttonStart.update()){
    if (buttonStart.fallingEdge()){
      Serial.print("Start");
      if (gameState) {
        if (playSdWav2.isPlaying()){
          //if in gamestate and something is already playing, stop playing
          playSdWav2.stop();
        }else{
          //if not playing, start playing a random bgr
          playRandomBgr();
        }
        konamiCodeCheck("Start");
      //always happens whether successfully inputting code or not
      if (digitalRead(selectPin) == LOW){
        //turns on loop if select is held
        loopOn = !loopOn;
        }
      }
    }
  }

  if (buttonSelect.update()){
    if (buttonSelect.fallingEdge()){
      konamiCodePlace = 0;
    }
  }

  if (buttonL.update()){
    if (buttonL.fallingEdge()){
      if (digitalRead(selectPin) == LOW && tempo > 100) {
        tempo = tempo +5;
      }
      playFile("L");
      konamiCodePlace = 0;
    }
  }

  if (buttonR.update()){
    if (buttonR.fallingEdge()){
      if (digitalRead(selectPin) == LOW && tempo < 1000) {
         tempo = tempo -5;
      }
      playFile("R");
      konamiCodePlace = 0;
    }
  }
}
