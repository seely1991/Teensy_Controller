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

char makeDirectory(char folder, char file){
  char directory = folder;
    strcat(directory, "/");
    strcat(directory, file);
    strcat(directory, ".wav");
  return directory;
}

    class Preset {
  public:
  String folder;
  String a;
  String b;
  String x;
  String y;
  String up;
  String down;
  String left;
  String right;
  String l;
  String r;
  String startButton;
  String selectButton;
  String bgr[8];
  void setBgrArr(String arr[8]){
    for (int i = 0; i < 8; i++){
        bgr[i] = arr[i];
      }
    }
  void playFile(String input) {
    String directory = folder + "/" + input + ".wav";
    playSdWav1.play(directory.c_str());
    Serial.println("input: " + input);
    Serial.println("directory: " + directory);
  }
  void playRandomBgr() {
    //not working...
    srand((unsigned)millis());
    int arrSize = sizeof(bgr)/sizeof(bgr[0]);
    int randIndex = rand() % arrSize;
    String directory = (folder + "/" + bgr[randIndex] + ".wav");
    playSdWav2.play(directory.c_str());
    Serial.println("trying to play a bgr");
    Serial.println(directory);
  }
  Preset() {
    }
};


String konamiCode[] = {"Up", "Up", "Down", "Down", "Left", "Right", "Left", "Right", "B", "A", "Start"};
int konamiCodePlace = 0;
bool konamiCodeCheck (String input) {
  if (konamiCode[konamiCodePlace] == input){
    konamiCodePlace++;
  }else{
    konamiCodePlace = 0;
  }
  return true;
}

float vol = 0.9;

void changeVolume(float num) {
  vol = vol + num;
  mixer1.gain(0, vol);
  mixer1.gain(1, vol);
  mixer1.gain(2, vol);
  mixer1.gain(3, vol);
  Serial.println(vol);
  }

int selectPin = 21;

bool loopOn = false;

bool gameState = true;

int tempo = 200;

//int lengthOfDrums = 6;
//int lengthOfGames = 4;

//Vector<Preset> drumPresets;
//Vector<Preset> gamePresets;

Preset drumPresets[4];
Preset gamePresets[4];

int currentIndex = 0;

Preset* currentPreset;


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
  
  Preset kit606("606");
  //Preset kit707("707");
  Preset kit808("808");
  //Preset kit909("909");
  Preset vinylKit("vinyl");
  
  Preset goingPostal("postal");
  strcpy(goingPostal.y, "clap");
  strcpy(goingPostal.left, "revclap");
  strcpy(goingPostal.right, "revsnare");
  strcpy(goingPostal.l, "guio");
  strcpy(goingPostal.r, "rim");

  Preset SMB1("smb");
<<<<<<< HEAD
  SMB1.a = "coin";
  SMB1.b = "jump";
  SMB1.x = "firework";
  SMB1.y = "fireball";
  SMB1.up = "blockbr";
  SMB1.down = "pipe";
  SMB1.left = "powerup";
  SMB1.right = "stomp";
  SMB1.l = "powerup";
  SMB1.r = "1-up";
  String smbArr[8] = {"theme", "theme", "water", "castle", "undrwrld", "undrwrld", "castle", "theme"};
  SMB1.setBgrArr(smbArr);
  
  Preset megaMan("megaman");
  megaMan.a = "shoot";
  megaMan.b = "jump";
  megaMan.x = "hit";
  megaMan.y = "blowup";
  megaMan.up = "exit";
  megaMan.down = "appear";
  megaMan.left = "energy";
  megaMan.right = "sheen";
  megaMan.l = "death";
  megaMan.r = "life";
  String mmArr[8] = {"airman", "bombman", "elecman", "cutman", "skullman", "snakeman", "sparkman", "wily"};
  megaMan.setBgrArr(mmArr);  
  
  Preset LOZ("loz");
  LOZ.a = "swrdslsh";
  LOZ.b = "shield";
  LOZ.x = "arrow";
  LOZ.y = "bombdrop";
  LOZ.up = "magicrod";
  LOZ.down = "enmyhit";
  LOZ.left = "bombblow";
  LOZ.right = "enmydie";
  LOZ.l = "secret";
  LOZ.r = "recorder";
  String lozArr[8] = {"dthmnt", "dungeon", "theme", "theme", "dungeon", "dthmnt", "theme", "dungeon"};
  LOZ.setBgrArr(lozArr);
  
  Preset sonic("sonic");  
  sonic.a = "charge";
  sonic.b = "jump";
  sonic.x = "slash";
  sonic.y = "itembrk";
  sonic.up = "spring";
  sonic.down = "stall";
  sonic.left = "screech";
  sonic.right = "ring";
  sonic.l = "ringlose";
  sonic.r = "wrdjump";
  String sonicArr[8] = {"anglisl1", "anglisl2", "emrldhll", "hydro1", "hydro2", "hydro1", "emrldhll", "anglisl1"};
  sonic.setBgrArr(sonicArr);
  
  //Preset EJW2("606");

  drumPresets[0] = goingPostal;
  drumPresets[1] = kit606;
  drumPresets[2] = kit808;
  drumPresets[3] = vinylKit;
  //drumPresets[5] = kit909;
  //drumPresets[4] = kit707;

  gamePresets[0] = megaMan;
  gamePresets[1] = SMB1;
  gamePresets[2] = LOZ;
  gamePresets[3] = sonic;
  //gamePresets[4] = EJW2;
  
  currentPreset = &gamePresets[currentIndex];

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
  }
  delay(1000);    
}

void loop() {
//going to loop hihat in the delay of the tempo
  if (!gameState){
    if (loopOn == true) {
      Serial.println(loopOn);
      playSdWav2.play((currentPreset->folder + "/" + currentPreset->y + ".wav").c_str());
      delay(tempo);
    }
  }

  if (buttonA.update()){
    if (buttonA.fallingEdge()){
      currentPreset->playFile(currentPreset->a);
      konamiCodeCheck("A");
    }
  }

  if (buttonB.update()){
    if (buttonB.fallingEdge()){
      currentPreset->playFile(currentPreset->b);
      konamiCodeCheck("B");
    }
  }
  if (buttonX.update()){
    if (buttonX.fallingEdge()){
      currentPreset->playFile(currentPreset->x);
      konamiCodePlace = 0;
    }
  }

  if (buttonY.update()){
    if (buttonY.fallingEdge()){
      currentPreset->playFile(currentPreset->y);
      konamiCodePlace = 0;
    }
  }

  if (buttonUp.update()){
    if (buttonUp.fallingEdge()){
      if (digitalRead(selectPin) == LOW && vol < 1){
        changeVolume(0.1);
      }
      currentPreset->playFile(currentPreset->up);
      konamiCodeCheck("Up");
    }
  }

  if (buttonDown.update()){
    if (buttonDown.fallingEdge()){
      currentPreset->playFile(currentPreset->down);
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
          Serial.print(currentIndex);
        //if it is zero, cycle back around to the last index
        }else{
          //what to do if in gameState
          if (gameState){
            //currentIndex = last index of gamePresets
            currentIndex = (sizeof(gamePresets)/sizeof(gamePresets[0])) - 1;
            Serial.print(currentIndex);
          }else{
            //currentIndex = last index of drumPresets
            currentIndex = (sizeof(drumPresets)/sizeof(drumPresets[0])) - 1;
            Serial.print(currentIndex);
          }
        }
        //sets the current preset with the previously set index
        if (gameState) {
          currentPreset = &gamePresets[currentIndex];
        }else{
          currentPreset = &drumPresets[currentIndex];
        }
      }else{
        //do this only if select isn't pressed
         konamiCodeCheck("Left");
      }
      //do this if select is pressed or not (put this with konamiCodeCheck if it should only be done without select
      currentPreset->playFile(currentPreset->left);
    }
  }

  if (buttonRight.update()){
    if (buttonRight.fallingEdge()){
      //if select is pressed
      if (digitalRead(selectPin) == LOW){
        //if not in game state
        playSdWav2.stop();
        if (!gameState) {
          //notworking here?
          //if not at the end of the list, index goes up
          if ((unsigned)currentIndex < (sizeof(drumPresets)/sizeof(drumPresets[0])) - 1){
            Serial.println(currentIndex);
            currentIndex++;
            Serial.println(currentIndex);
          }else{
            //if at the end of the list, index goes back to zero
            currentIndex = 0;
          }
          //sets the drum preset
          currentPreset = &drumPresets[currentIndex];
          Serial.println(currentPreset->folder);
          //what to do if in gamestate
        }else{
          //if not at the end of the list, index goes up
          if ((unsigned)currentIndex < (sizeof(gamePresets)/sizeof(gamePresets[0])) - 1){
            currentIndex++;
            Serial.print(currentIndex);
          }else{
            //if at the end of the list, index goes back to zero
            currentIndex = 0;
            Serial.print(currentIndex);
          }
          currentPreset = &gamePresets[currentIndex];
        }
      }else{
        //check for konami code if select is not being pressed
        konamiCodeCheck("Right");
      }
      //plays no matter what
      currentPreset->playFile(currentPreset->right);
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
          currentPreset->playRandomBgr();
        }
      if (konamiCode[konamiCodePlace] == "Start"){
        Serial.println("***You did the Konami code!***");
        playSdWav2.stop();
        gameState = !gameState;
        //change so that you can get out of drumPresets if done a second time
        currentPreset = &drumPresets[0];
        Serial.print(currentPreset->folder);
      }
      //always happens whether successfully inputting code or not
      konamiCodePlace = 0;
        }else{
          if (digitalRead(selectPin) == LOW){
          Serial.println("changing loopOn");
          //turns on loop if select is held
          loopOn = !loopOn;
        }
      }
    }
  }

  if (buttonSelect.update()){
    if (buttonSelect.fallingEdge()){
      Serial.println("select");
      konamiCodePlace = 0;
    }
  }

  if (buttonL.update()){
    if (buttonL.fallingEdge()){
      if (digitalRead(selectPin) == LOW && tempo < 1000) {
        tempo = tempo -5;
      }
      currentPreset->playFile(currentPreset->l);
      konamiCodePlace = 0;
    }
  }

  if (buttonR.update()){
    if (buttonR.fallingEdge()){
      if (digitalRead(selectPin) == LOW && tempo > 100) {
         tempo = tempo +5;
      }
      currentPreset->playFile(currentPreset->r);
      konamiCodePlace = 0;
    }
  }
}
