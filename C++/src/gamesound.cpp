#include "../include/gamesound.hpp"

GameSound::GameSound()
{
    InitAudioDevice();
    defaultMove_ =   LoadSound("../../Assets/sounds/default.wav");
    capture_     =   LoadSound("../../Assets/sounds/capture.wav");
    castle_      =   LoadSound("../../Assets/sounds/castle.wav");
    check_       =   LoadSound("../../Assets/sounds/check.wav");
    checkMate_   =   LoadSound("../../Assets/sounds/gameEnd.wav");
}

GameSound::~GameSound()
{
    // Unload the music streams when they're no longer needed
    UnloadSound(defaultMove_);
    UnloadSound(capture_);
    UnloadSound(castle_);
    UnloadSound(check_);
    UnloadSound(checkMate_);

    CloseAudioDevice(); 
}

void GameSound::playDefault() const
{
    PlaySound(defaultMove_);
}
void GameSound::playCapture() const
{
    PlaySound(capture_);
}
void GameSound::playCastle() const
{
    PlaySound(castle_);
}
void GameSound::playCheck() const
{
    PlaySound(check_);
}
void GameSound::playCheckMate() const
{
    PlaySound(checkMate_);
}
