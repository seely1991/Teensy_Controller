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
  char folder[9];
  char a[9];
  char b[9];
  char x[9];
  char y[9];
  char up[9];
  char down[9];
  char left[9];
  char right[9];
  char l[9];
  char r[9];
  char startButton;
  char selectButton;
  char bgr[8][9];
  void setBgrArray(char arr[8][9]) {
    for (int i = 0; i < 8; i++) {
        strcpy(bgr[i], arr[i]);
    }
  }
  void playFile(char *input) {
    char directory = makeDirectory(folder, input);
    playSdWav1.play(directory);
    Serial.println("input: " + input);
    Serial.println("directory: " + directory);
  }
  void playRandomBgr() {
    //not working...
    srand((unsigned)millis());
    int randIndex = rand() % 8;
    char directory = makeDirectory(folder, bgr[randIndex]);
    playSdWav2.play(directory);
    Serial.println(directory);
  }
  Preset() {
    }
};

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

char konamiCode[11][9] = {"Up", "Up", "Down", "Down", "Left", "Right", "Left", "Right", "B", "A", "Start"};
    int konamiCodePlace = 0;
    bool konamiCodeCheck(const char *input) {
      if (!strcmp(konamiCode[konamiCodePlace], input)){
          if (konamiCodePlace == 10) {
              konamiCodePlace = 0;
              cout<<"***You did the konami code!***";
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
  
  Preset kit606("606");
  Preset kit707("707");
  Preset kit808("808");
  Preset kit909("909");
  Preset vinylKit("vinyl");
  
  Preset goingPostal("postal");
  strcpy(goingPostal.y, "clap");
  strcpy(goingPostal.left, "revclap");
  strcpy(goingPostal.right, "revsnare");
  strcpy(goingPostal.l, "guio");
  strcpy(goingPostal.r, "rim");

  Preset SMB1("smb");
  strcpy(SMB1.a, "coin");
  strcpy(SMB1.b, "jump");
  strcpy(SMB1.x, "firework");
  strcpy(SMB1.y, "fireball");
  strcpy(SMB1.up, "blockbr");
  strcpy(SMB1.down, "pipe");
  strcpy(SMB1.left, "powerup");
  strcpy(SMB1.right, "stomp");
  strcpy(SMB1.l, "powerup");
  strcpy(SMB1.r, "1-up");
  char smbArr[8][9] = {"theme", "theme", "water", "castle", "undrwrld", "theme", "castle", "undrwrld"};
  SMB1.setBgrArray(smbArr);
  
  Preset megaMan("megaman");
  strcpy(megaMan.a, "shoot");
  strcpy(megaMan.b, "jump");
  strcpy(megaMan.x, "hit");
  strcpy(megaMan.y, "blowup");
  strcpy(megaMan.up, "exit");
  strcpy(megaMan.down, "appear");
  strcpy(megaMan.left, "energy");
  strcpy(megaMan.right, "sheen");
  strcpy(megaMan.l, "death");
  strcpy(megaMan.r, "life");
  char mmArr[8][9] = {"airman", "bombman", "elecman", "cutman", "skullman", "snakeman", "sparkman", "wily"};
  megaMan.setBgrArray(mmArr);  
  
  Preset LOZ("loz");
  strcpy(LOZ.a, "swrdslsh");
  strcpy(LOZ.b, "shield");
  strcpy(LOZ.x, "arrow");
  strcpy(LOZ.y, "bombdrop");
  strcpy(LOZ.up, "magicrod");
  strcpy(LOZ.down, "enmyhit");
  strcpy(LOZ.left, "bombblow");
  strcpy(LOZ.right, "enmydie");
  strcpy(LOZ.l, "secret");
  strcpy(LOZ.r, "recorder");
  char lozArr[8][9] = {"dthmnt", "dungeon", "theme", "dthmnt", "dungeon", "theme", "dungeon", "theme"};
  LOZ.setBgrArray(lozArr);
  
  Preset sonic("sonic");  
  strcpy(sonic.a, "charge");
  strcpy(sonic.b, "jump");
  strcpy(sonic.x, "slash");
  strcpy(sonic.y, "ring");
  strcpy(sonic.up, "spring");
  strcpy(sonic.down, "itembrk");
  strcpy(sonic.left, "screech");
  strcpy(sonic.right, "stall");
  strcpy(sonic.l, "ringlose");
  strcpy(sonic.r, "wrjump");
  char sonicArr[8][9] = {"anglisl1", "anglisl2", "emrldhll", "hydro1", "hydro2", "anglisl2", "emrldhll", "hydro2"};
  sonic.setBgrArray(sonicArr);
  
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
      playSdWav2.play(makeDirectory(currentPreset.folder, currentPreset.y));
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
