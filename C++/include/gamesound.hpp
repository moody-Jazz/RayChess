#pragma once

#include<raylib.h>


class GameSound
{    
    public:

        GameSound();

        ~GameSound();

        void playCapture();

        void playDefault();

        void playCastle();

        void playCheck();

    private:

    Sound defaultMove;

    Sound capture;

    Sound castle;

    Sound check;

    Sound gameEnd;

};