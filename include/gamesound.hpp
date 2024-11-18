#pragma once

#include "raylib.h"

class GameSound
{    
public:
    GameSound();                    // constructor to extract and load sound from a file

    ~GameSound();                   // destructors to unload all the sounds

    // Utility Functions
    void playDefault() const;       // function to play defaultMove_ sound
    void playCapture() const;       // function to play capture_ sound  
    void playCastle() const;        // function to play castle sound
    void playCheck() const;         // function to play check sound
    void playCheckmate() const;     // function to play checkmate_ sound
    void playStalemate() const;     // function to play stalemate_ sound
    void playGameDraw() const;      // function to play gameDraw_ sound
    void playButtonClicked() const; // function to play a click sound

private:
    Sound defaultMove_;             // sound for move without any captures
    Sound capture_;                 // sound for captures
    Sound castle;                   // sound for casteling
    Sound check;                    // sound for when giving a check
    Sound checkmate_;               // sound for checkmate
    Sound stalemate_;               // sound for stalemate
    Sound gameDraw_;                // sound for when games end in a draw     
    Sound buttonClicked_;           // sound for button click feedback
};