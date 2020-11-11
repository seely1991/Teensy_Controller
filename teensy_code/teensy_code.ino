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
  Vector<String> bgr;
  void setBgrVector(String arr[], int arrSize) {
    for (int i = 0; i < arrSize; i++) {
        bgr.push_back(arr[i]);
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
    playSdWav2.play((folder + "/" + bgr[randIndex] + ".wav").c_str());
  }
  Preset() {
    }
  Preset(String folderDir){
      folder = folderDir;
      a = "snare";
      b = "kick";
      x = "hhclosed";
      y = "hhopen";
      up = "hhclosed";
      down = "kick";
      left = "hhopen";
      right = "snare";
      l = "clap";
      r = "rim";
      startButton = "";
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

int selectPin = 21;

bool loopOn = false;

bool gameState = true;

float vol = 0.5;

int tempo = 200;

int numberOfDrums = 6;
int numberOfGames = 4;

Preset drumPresets[numberOfDrums];
Preset gamePresets[numberOfGames];

int currentIndex = 0;

Preset currentPreset;


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
  
  Preset kit606("606");
  Preset kit707("707");
  Preset kit808("808");
  Preset kit909("909");
  Preset vinylKit("vinyl");
  
  Preset goingPostal("postal");
  goingPostal.y = "clap";
  goingPostal.left = "revclap";
  goingPostal.right = "revsnare";
  goingPostal.l = "guio";
  goingPostal.r = "rim";

  Preset SMB1("smb");
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
  String smbArr[] = {"theme", "theme", "water", "castle", "undrwrld"};
  SMB1.setBgrVector(smbArr, sizeof(smbArr)/sizeof(smbArr[0]));
  
  Preset megaMan("megaman");
  megaMan.a = "shoor";
  megaMan.b = "jump";
  megaMan.x = "hit";
  megaMan.y = "blowup";
  megaMan.up = "exit";
  megaMan.down = "appear";
  megaMan.left = "energy";
  megaMan.right = "sheen";
  megaMan.l = "death";
  megaMan.r = "life";
  String mmArr[] = {"airman", "bombman", "elecman", "cutman", "skullman", "snakeman", "sparkman", "wily"};
  megaMan.setBgrVector(mmArr, sizeof(mmArr)/sizeof(mmArr[0]));  
  
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
  String lozArr[] = {"dthmnt", "dungeon", "theme"};
  LOZ.setBgrVector(lozArr, sizeof(lozArr)/sizeof(lozArr[0]));
  
  Preset sonic("sonic");  
  sonic.a = "charge";
  sonic.b = "jump";
  sonic.x = "slash";
  sonic.y = "ring";
  sonic.up = "spring";
  sonic.down = "itembrk";
  sonic.left = "screech";
  sonic.right = "stall";
  sonic.l = "ringlose";
  sonic.r = "wrjump";
  String sonicArr[] = {"anglisl1", "anglisl2", "emrldhill", "hydro1", "hydro2"};
  sonic.setBgrVector(sonicArr, sizeof(sonicArr)/sizeof(sonicArr[0]));
  
  //Preset EJW2("606");

  drumPresets[0] = kit606;
  drumPresets[1] = kit707;
  drumPresets[2] = kit808;
  drumPresets[3] = kit909;
  drumPresets[4] = goingPostal;
  drumPresets[5] = vinylKit;

  gamePresets[0] = SMB1;
  gamePresets[1] = megaMan;
  gamePresets[2] = LOZ;
  gamePresets[3] = sonic;
  //gamePresets[4] = EJW2;
  
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
      playSdWav2.play((currentPreset.folder + "/" + currentPreset.y + ".wav").c_str());
      delay(tempo);
    }
  }

  if (buttonA.update()){
    if (buttonA.fallingEdge()){
      currentPreset.playFile(currentPreset.a);
      konamiCodeCheck("A");
    }
  }

  if (buttonB.update()){
    if (buttonB.fallingEdge()){
      currentPreset.playFile(currentPreset.b);
      konamiCodeCheck("B");
    }
  }
  if (buttonX.update()){
    if (buttonX.fallingEdge()){
      currentPreset.playFile(currentPreset.x);
      konamiCodePlace = 0;
    }
  }

  if (buttonY.update()){
    if (buttonY.fallingEdge()){
      currentPreset.playFile(currentPreset.y);
      konamiCodePlace = 0;
    }
  }

  if (buttonUp.update()){
    if (buttonUp.fallingEdge()){
      if (digitalRead(selectPin) == LOW && vol < 0.8){
        vol = vol + 0.1;
        sgtl5000_1.volume(vol);
      }
      currentPreset.playFile(currentPreset.up);
      konamiCodeCheck("Up");
    }
  }

  if (buttonDown.update()){
    if (buttonDown.fallingEdge()){
      currentPreset.playFile(currentPreset.down);
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
      currentPreset.playFile(currentPreset.left);
    }
  }

  if (buttonRight.update()){
    if (buttonRight.fallingEdge()){
      //if select is pressed
      if (digitalRead(selectPin) == LOW){
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
      currentPreset.playFile(currentPreset.right);
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
          currentPreset.playRandomBgr();
        }
      if (konamiCode[konamiCodePlace] == "Start"){
        Serial.println("***You did the Konami code!***");
        gameState = !gameState;
        //change so that you can get out of drumPresets if done a second time
        currentPreset = drumPresets[0];
        Serial.print(currentPreset.folder);
      }
      //always happens whether successfully inputting code or not
      konamiCodePlace = 0;
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
      currentPreset.playFile(currentPreset.l);
      konamiCodePlace = 0;
    }
  }

  if (buttonR.update()){
    if (buttonR.fallingEdge()){
      if (digitalRead(selectPin) == LOW && tempo < 1000) {
         tempo = tempo -5;
      }
      currentPreset.playFile(currentPreset.r);
      konamiCodePlace = 0;
    }
  }
}
