#include "../include/gamesound.hpp"

GameSound::GameSound(){
    InitAudioDevice();
    defaultMove = LoadSound("../Assets/sounds/default.wav");
    capture = LoadSound("../Assets/sounds/capture.wav");
    castle = LoadSound("../Assets/sounds/castle.wav");
    check = LoadSound("../Assets/sounds/check.wav");
    gameEnd = LoadSound("../Assets/sounds/gameEnd.wav");
}
GameSound::~GameSound(){
    // Unload the music streams when they're no longer needed
    UnloadSound(defaultMove);
    UnloadSound(capture);
    UnloadSound(castle);
    UnloadSound(check);
    UnloadSound(gameEnd);

    CloseAudioDevice(); // Close the audio device
}
void GameSound::playCapture(){
    PlaySound(capture);
}
void GameSound::playDefault(){
    PlaySound(defaultMove);
}
void GameSound::playCastle(){
    PlaySound(castle);
}
void GameSound::playCheck(){
    PlaySound(check);
}