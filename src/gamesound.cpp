#include "gamesound.hpp"

GameSound::GameSound()
{
    InitAudioDevice();
    defaultMove_   =   LoadSound("sounds/default.wav");
    capture_       =   LoadSound("sounds/capture.wav");
    castle         =   LoadSound("sounds/castle.wav");
    check          =   LoadSound("sounds/check.wav");
    checkMate_     =   LoadSound("sounds/gameEnd.wav");
    buttonClicked_ =   LoadSound("sounds/mouseClick.wav");
}

GameSound::~GameSound()
{
    // Unload the music streams when they're no longer needed
    UnloadSound(defaultMove_);
    UnloadSound(capture_);
    UnloadSound(castle);
    UnloadSound(check);
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
    PlaySound(castle);
}
void GameSound::playCheck() const
{
    PlaySound(check);
}
void GameSound::playCheckMate() const
{
    PlaySound(checkMate_);
}
void GameSound::playButtonClicked() const
{
    PlaySound(buttonClicked_);
}