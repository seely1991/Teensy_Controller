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
Bounce buttonX = Bounce(3, 10);
Bounce buttonY = Bounce(2, 10);
Bounce buttonR = Bounce(4, 10);
Bounce buttonL = Bounce(5, 10);
Bounce buttonDown = Bounce(17, 10);
Bounce buttonRight = Bounce(16, 10);
Bounce buttonUp = Bounce(15, 10);
Bounce buttonLeft = Bounce(20, 10);
Bounce buttonStart = Bounce(8, 10);
Bounce buttonSelect = Bounce(21, 10);




int selectPin = 21;

bool loopOn = false;

bool gameState = true;

float vol = 0.9;

void changeVolume(float num) {
  vol = vol + num;
  mixer1.gain(0, vol);
  mixer1.gain(1, vol);
  mixer1.gain(2, vol);
  mixer1.gain(3, vol);
  Serial.println(vol);
}

int tempo = 200;

char drumPresets[40][32];

char gamePresets[40][32];

int numberOfDrums = 0;
int numberOfGames = 0;
int numberOfBgr[40];

int currentIndex = 0;

const char *currentPreset;

void playFile(const char *input) {
    char combined[32] = {0};
    Serial.println(input);
    strcpy(combined, currentPreset);
    strcat(combined, "/");
    strcat(combined, input);
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
    
    playSdWav1.play(combined);

}

int getRandomNumber(int maximum) {
    srand((unsigned)millis());
    return rand() % maximum;
  }

void playRandomBgr() {
  Serial.println("\ntrying to play bgr");
  
  char combined[32] = {0};
  
  strcpy(combined, currentPreset);
  strcat(combined, "/bgr");
  File directory = SD.open(combined);
  if (!numberOfBgr[currentIndex]){return;}

  int bgrSize = numberOfBgr[currentIndex];
  
  srand((unsigned)millis());
  int randIndex = (rand() % bgrSize) + 1;
  Serial.println("random index = ");
  Serial.print(randIndex);
  Serial.print("\n\n");
  for (int i = 1; i <= randIndex; i++) {
    File entry = directory.openNextFile();
    if (i == randIndex) {
      strcat(combined, "/");
      strcat(combined, entry.name());
      if (!entry) {
          Serial.println("**no bgr to play**");
          return;
        }
        Serial.println("trying to play ");
        Serial.print(combined);
        Serial.print("\n\n");
        if (!SD.exists(combined)){
          return;
          }
      playSdWav2.play(combined);
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
          Serial.print(currentPreset);
      }else{
          konamiCodePlace++;
      }
    return true;
  }else{
    konamiCodePlace = 0;
    return false;
  }
}

void scanBgr(const char *folderName, int index) {
  char combined[32] = {0};
  strcpy(combined, folderName);
  strcat(combined, "/bgr");
    File bgrDir = SD.open(combined);
    Serial.print("\ntrying to scan ");
    Serial.print(combined);
    Serial.print(" for bgr and length is ");
    int bgrInDir = 0;
        while (true) {
            File bgr = bgrDir.openNextFile();
            if (bgr) {
              bgrInDir++;
            }else{
              bgrDir.rewindDirectory();
              numberOfBgr[index] = bgrInDir;
              Serial.println(numberOfBgr[index]);
              break;
            }
        }
  }
    
    
void scanFolder(const char *folderName, bool isGame) {
  Serial.println("scanning folders");
    File directory = SD.open(folderName);
    int index = 0;
    while (true) {
        File entry = directory.openNextFile();
        if (entry) {
            char combined[32] = {0};
            strcpy(combined, folderName);
            strcat(combined, "/");
            strcat(combined, entry.name());
            if (isGame) {

                scanBgr(combined, index);
              
                strcpy(gamePresets[index], combined);
                Serial.println(gamePresets[0]);
                Serial.println(gamePresets[1]);
                Serial.println(gamePresets[2]);
                Serial.println(gamePresets[3]);
                Serial.println("\n\n\n\n\n");
                numberOfGames++;
            }else{
                //Serial.println(index);
                strcpy(drumPresets[index], combined);
                Serial.println(drumPresets[index]);
                Serial.println("\n\n\n\n\n");
                numberOfDrums++;
            }
            index++;
        }else{
            Serial.println("done with");
            Serial.println(folderName);
            directory.rewindDirectory();
            break;
        }
        
    }
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
  scanFolder("_GAMES",true);
  scanFolder("_DRUMS",false);
  
  //drumPresets[0] = "_DRUMS/postal";
  //gamePresets[0] = "_GAMES/megaman";
  //gamePresets[1] = "_GAMES/smb";
  //gamePresets[2] = "_GAMES/loz";
  //gamePresets[3] = "_GAMES/sonic";

  Serial.println(gamePresets[0]);
  //currentIndex = getRandomNumber(numberOfGames);
  currentPreset = gamePresets[getRandomNumber(numberOfGames)];
  delay(1000);   
}

void loop() {
    //playSdWav1.play("_GAMES/_MEGAMAN/BGR/AIRMAN.wav");
  
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
      Serial.print("a");
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
      if (digitalRead(selectPin) == LOW && vol < 1){
        changeVolume(0.1);
      }
      playFile("Up");
      konamiCodeCheck("Up");
    }
  }

  if (buttonDown.update()){
    if (buttonDown.fallingEdge()){
      playFile("Down");
      if (digitalRead(selectPin) == LOW && vol > 0.1){
        changeVolume(-0.1);
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
        //if it is zero, cycle back around to the last index
        }else{
          //what to do if in gameState
          if (gameState){
            //currentIndex = last index of gamePresets
            currentIndex = numberOfGames - 1;
          }else{
            //currentIndex = last index of drumPresets
            currentIndex = numberOfDrums - 1;
          }
        }
        //sets the current preset with the previously set index
        if (gameState) {
          currentPreset = gamePresets[currentIndex];
          Serial.println(currentPreset);
        }else{
          currentPreset = drumPresets[currentIndex];
          Serial.println(currentPreset);
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
            currentIndex++;
          }else{
            //if at the end of the list, index goes back to zero
            currentIndex = 0;
          }
          //sets the drum preset
          currentPreset = drumPresets[currentIndex];
          Serial.println(currentPreset);
          //what to do if in gamestate
        }else{
          //if not at the end of the list, index goes up
          if (currentIndex < numberOfGames - 1){
            currentIndex++;
          }else{
            //if at the end of the list, index goes back to zero
            currentIndex = 0;
          }
          currentPreset = gamePresets[currentIndex];
          Serial.println(currentPreset);
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
