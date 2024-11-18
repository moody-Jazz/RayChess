#include "gamesound.hpp"

GameSound::GameSound()
{
    InitAudioDevice();
    defaultMove_   =   LoadSound("sounds/default.wav");
    capture_       =   LoadSound("sounds/capture.wav");
    castle         =   LoadSound("sounds/castle.wav");
    check          =   LoadSound("sounds/check.wav");
    checkmate_     =   LoadSound("sounds/checkmate.wav");
    stalemate_     =   LoadSound("sounds/stalemate.wav");
    buttonClicked_ =   LoadSound("sounds/mouseClick.wav");
}

GameSound::~GameSound()
{
    // Unload the sounds when they're no longer needed
    UnloadSound(defaultMove_);
    UnloadSound(capture_);
    UnloadSound(castle);
    UnloadSound(check);
    UnloadSound(checkmate_);
    UnloadSound(stalemate_);
    UnloadSound(buttonClicked_);

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
void GameSound::playCheckmate() const
{
    PlaySound(checkmate_);
}
void GameSound::playStalemate() const
{
    PlaySound(stalemate_);
}
void GameSound::playButtonClicked() const
{
    PlaySound(buttonClicked_);
}