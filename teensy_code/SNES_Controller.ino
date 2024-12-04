#include <Bounce.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// Audio setup
AudioPlaySdWav           playSdWav2;
AudioPlaySdWav           playSdWav1;
AudioMixer4              mixer1;
AudioOutputI2S           i2s1;
AudioConnection          patchCord1(playSdWav2, 0, mixer1, 1);
AudioConnection          patchCord2(playSdWav1, 0, mixer1, 0);
AudioConnection          patchCord3(mixer1, 0, i2s1, 0);
AudioConnection          patchCord4(mixer1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;

// SD Card Pins
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

// Button Debouncing
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

// Miscellaneous Variables
int selectPin = 21;
bool loopOn = false;
bool gameState = true;
float vol = 0.9;
int tempo = 200;
int savedTime = millis();
char drumPresets[40][128];
char gamePresets[40][128];
int numberOfDrums = 0;
int numberOfGames = 0;
int numberOfBgr[40] = {0};
int currentIndex = 0;
const char *currentPreset;

// Konami Code Variables
char konamiCode[11][9] = {"Up", "Up", "Down", "Down", "Left", "Right", "Left", "Right", "B", "A", "Start"};
int konamiCodePlace = 0;

// Function to change volume
void changeVolume(float num) {
    vol = constrain(vol + num, 0.0, 1.0);
    for (int i = 0; i < 4; i++) {
        mixer1.gain(i, vol);
    }
    Serial.println(vol);
}

// Function to increment time and play file
void incrementTime() {
    if (millis() - savedTime >= tempo) {
        savedTime = millis();
        playFile("X");
    }
}

// Function to play audio files
void playFile(const char *input) {
    char combined[128] = {0};
    strcpy(combined, currentPreset);
    strcat(combined, "/");
    strcat(combined, input);
    strcat(combined, ".WAV");

    if (SD.exists(combined)) {
        Serial.println("Playing file:");
        Serial.println(combined);
        playSdWav1.play(combined);
    } else {
        Serial.println("File not found:");
        Serial.println(combined);
    }
}

// Function to generate a random number
int getRandomNumber(int maximum) {
    return random(maximum);
}

// Function to play a random background music file
void playRandomBgr() {
    if (numberOfBgr[currentIndex] <= 0) {
        Serial.println("No BGR files available.");
        return;
    }

    char combined[128] = {0};
    strcpy(combined, currentPreset);
    strcat(combined, "/bgr");

    File directory = SD.open(combined);
    if (!directory) {
        Serial.println("Failed to open BGR directory.");
        return;
    }

    int randIndex = getRandomNumber(numberOfBgr[currentIndex]);
    File entry;

    for (int i = 0; i <= randIndex; i++) {
        entry = directory.openNextFile();
        if (!entry) {
            Serial.println("No file found in BGR directory.");
            directory.close();
            return;
        }
    }

    strcat(combined, "/");
    strcat(combined, entry.name());
    entry.close();
    directory.close();

    if (SD.exists(combined)) {
        playSdWav2.play(combined);
        Serial.println("Playing BGR:");
        Serial.println(combined);
    } else {
        Serial.println("Failed to play BGR:");
        Serial.println(combined);
    }
}

// Function to check Konami code input
bool konamiCodeCheck(const char *input) {
    if (!strcmp(konamiCode[konamiCodePlace], input)) {
        if (konamiCodePlace == 10) {
            konamiCodePlace = 0;
            playSdWav2.stop();
            Serial.println("Konami code activated!");
            gameState = !gameState;
            currentPreset = gameState ? gamePresets[0] : drumPresets[0];
        } else {
            konamiCodePlace++;
        }
        return true;
    } else {
        konamiCodePlace = 0;
        return false;
    }
}

// Functions to scan SD card and populate preset arrays
void scanBgr(const char *folderName, int index) {
    char combined[128] = {0};
    strcpy(combined, folderName);
    strcat(combined, "/bgr");

    File bgrDir = SD.open(combined);
    if (!bgrDir) {
        numberOfBgr[index] = 0;
        return;
    }

    int bgrInDir = 0;
    while (true) {
        File bgr = bgrDir.openNextFile();
        if (bgr) {
            bgr.close();
            bgrInDir++;
        } else {
            bgrDir.close();
            break;
        }
    }
    numberOfBgr[index] = bgrInDir;
}

void scanFolder(const char *folderName, bool isGame) {
    File directory = SD.open(folderName);
    int index = 0;

    while (true) {
        File entry = directory.openNextFile();
        if (entry) {
            char combined[128] = {0};
            strcpy(combined, folderName);
            strcat(combined, "/");
            strcat(combined, entry.name());

            if (isGame) {
                scanBgr(combined, index);
                strcpy(gamePresets[index], combined);
                numberOfGames++;
            } else {
                strcpy(drumPresets[index], combined);
                numberOfDrums++;
            }
            entry.close();
            index++;
        } else {
            directory.close();
            break;
        }
    }
}

// Setup function
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
    AudioMemory(16);
    sgtl5000_1.enable();
    sgtl5000_1.volume(0.5);
    SPI.setMOSI(SDCARD_MOSI_PIN);
    SPI.setSCK(SDCARD_SCK_PIN);

    if (!SD.begin(SDCARD_CS_PIN)) {
        while (1) {
            Serial.println("Unable to access the SD card");
            delay(500);
        }
    }

    scanFolder("_GAMES", true);
    scanFolder("_DRUMS", false);
    currentPreset = drumPresets[0];
}

// Main loop
void loop() {
    if (!gameState && loopOn) {
        incrementTime();
    }

    // Button handling
    if (buttonA.update() && buttonA.fallingEdge()) { playFile("A"); konamiCodeCheck("A"); }
    if (buttonB.update() && buttonB.fallingEdge()) { playFile("B"); konamiCodeCheck("B"); }
    if (buttonX.update() && buttonX.fallingEdge()) { playFile("X"); konamiCodePlace = 0; }
    if (buttonY.update() && buttonY.fallingEdge()) { playFile("Y"); konamiCodePlace = 0; }
    if (buttonUp.update() && buttonUp.fallingEdge()) { 
        if (digitalRead(selectPin) == LOW && vol < 1) { changeVolume(0.1); } 
        playFile("Up"); konamiCodeCheck("Up"); 
    }
    if (buttonDown.update() && buttonDown.fallingEdge()) { 
        if (digitalRead(selectPin) == LOW && vol > 0.1) { changeVolume(-0.1); } 
        playFile("Down"); konamiCodeCheck("Down"); 
    }
    // Add remaining button handlers similarly...
}
