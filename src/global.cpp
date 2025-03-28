#include "global.hpp"
#include "iostream"


namespace Globals 
{
    bool player             = white;
    bool engineToggleOn     = true;
    size_t depth            = 3;
    size_t tileSize         = 0;
    float capturedSize      = 0;
    size_t totalPiece       = 0;
    size_t totalCaptured    = 0;
    size_t topPadding       = 0;
    size_t leftPadding      = 0;
    size_t btnHeight        = 0;
    size_t btnWidth         = 0;
    size_t boardSize        = 0;
    size_t windowWidth      = 0;
    size_t windowHeight     = 0;
    Image icon              = LoadImage("images/icon.png");

    std::string FENString   = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    std::ofstream outFile("fen_log.txt", std::ios::app);

    std::string testPosition[] = 
    {
        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 1 2",
        "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 1 3",
        "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",
        "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8",
        "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 "
    };

    GameSound sound;
   
    std::string pieceDir[12] =
    {
        "images/whitePawn.png", "images/whiteKnight.png", "images/whiteBishop.png",
        "images/whiteRook.png", "images/whiteQueen.png", "images/whiteKing.png",
        "images/blackPawn.png", "images/blackKnight.png", "images/blackBishop.png",
        "images/blackRook.png", "images/blackQueen.png", "images/blackKing.png", 
    };
    Texture2D pieceTextures[12];
    Texture2D captureTextures[12];

    /*  
        notAFile            notHFile            notGHFile           notABFile 
        0 1 1 1 1 1 1 1     1 1 1 1 1 1 1 0     1 1 1 1 1 1 0 0     0 0 1 1 1 1 1 1
        0 1 1 1 1 1 1 1     1 1 1 1 1 1 1 0     1 1 1 1 1 1 0 0     0 0 1 1 1 1 1 1
        0 1 1 1 1 1 1 1     1 1 1 1 1 1 1 0     1 1 1 1 1 1 0 0     0 0 1 1 1 1 1 1
        0 1 1 1 1 1 1 1     1 1 1 1 1 1 1 0     1 1 1 1 1 1 0 0     0 0 1 1 1 1 1 1
        0 1 1 1 1 1 1 1     1 1 1 1 1 1 1 0     1 1 1 1 1 1 0 0     0 0 1 1 1 1 1 1
        0 1 1 1 1 1 1 1     1 1 1 1 1 1 1 0     1 1 1 1 1 1 0 0     0 0 1 1 1 1 1 1
        0 1 1 1 1 1 1 1     1 1 1 1 1 1 1 0     1 1 1 1 1 1 0 0     0 0 1 1 1 1 1 1
        0 1 1 1 1 1 1 1     1 1 1 1 1 1 1 0     1 1 1 1 1 1 0 0     0 0 1 1 1 1 1 1
        a b c d e f g h     a b c d e f g h     a b c d e f g h     a b c d e f g h  
    */
    // Below are the Decimal representation of above binary
    const uint64_t rankZero  = 0x00000000000000FF;
    const uint64_t rankEight = 0xFF00000000000000;
    const uint64_t notAFile  = 9187201950435737471ULL;   
    const uint64_t notHFile  = 18374403900871474942ULL;
    const uint64_t notGHFile = 4557430888798830399ULL;
    const uint64_t notABFile = 18229723555195321596ULL;

    size_t kingsInitPos[2]   = { 3, 59 };
    size_t queenInitPos[2]   = { 4, 60 };
    
    size_t rookInitPos[2][2] = 
    {
        { 0, 7 },
        { 56 ,63 }
    };

    size_t castleRookTargetTiles[2][2] = 
    {
        { 2, 4 },
        { 58, 60 }
    };
    size_t castleKingTargetTile[2][2] = 
    {
        { 1, 5 },
        { 57, 61 }
    };

   /*  The following bitmask values are set for castling rights for both white and black sides.
    Refer to the getPseudoLegalMoves() function to understand why these specific numbers are assigned. */
    uint64_t castleBitmasks[2][3] =
    { 
        { 6uLL, 48ULL, 54ULL }, 
        { 432345564227567616ULL, 3458764513820540928ULL, 3891110078048108544ULL }
    };
    uint64_t occupancyBitmask[2] = {112ULL, 8070450532247928832ULL};

    void setUIparams()
    {
        if(windowWidth == 0) windowWidth = GetMonitorWidth(0) * 0.8;
        if(windowHeight == 0) windowHeight = GetMonitorHeight(0) * 0.8;
        tileSize         = (windowHeight <= windowWidth)? windowHeight * 0.116 : windowWidth * 0.11;
        capturedSize     = tileSize/2.5;
        topPadding       = tileSize/3;
        leftPadding      = tileSize/3;
        btnHeight        = tileSize/2;
        btnWidth         = tileSize * 2 + leftPadding;
        boardSize        = tileSize * 8;
    }
}

namespace Colors
{
    Color lightTile     = {218, 217, 233, 255};
    Color darkTile      = {161, 123, 185, 255};
    Color boardOutline  = {142, 80, 48, 255};
    Color tileHighlight = {70, 70, 70, 100};
    Color background    = {49, 54, 63, 255};
    Color danger        = RED;

    Color btnBase       = {242, 46, 73, 255};
    Color btnBorder     = background;
    Color labelColor    = WHITE;
    Color btnHighlight  = btnBorder;
}

const std::string asciiPieces = "PNBRQKpnbrqk";

const std::unordered_map<char, int> charPieces = 

{
    {'P', P}, {'N', N}, {'B', B}, {'R', R}, {'Q', Q}, {'K', K},
    {'p', p}, {'n', n}, {'b', b}, {'r', r}, {'q', q}, {'k', k}
};

const std::string coordinate[64] = 
{
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"
};

const std::unordered_map<std::string, int> coordsToAbsolute = 
{
    {"a8", 63}, {"b8", 62}, {"c8", 61}, {"d8", 60}, {"e8", 59}, {"f8", 58}, {"g8", 57}, {"h8", 56},
    {"a7", 55}, {"b7", 54}, {"c7", 53}, {"d7", 52}, {"e7", 51}, {"f7", 50}, {"g7", 49}, {"h7", 48},
    {"a6", 47}, {"b6", 46}, {"c6", 45}, {"d6", 44}, {"e6", 43}, {"f6", 42}, {"g6", 41}, {"h6", 40},
    {"a5", 39}, {"b5", 38}, {"c5", 37}, {"d5", 36}, {"e5", 35}, {"f5", 34}, {"g5", 33}, {"h5", 32},
    {"a4", 31}, {"b4", 30}, {"c4", 29}, {"d4", 28}, {"e4", 27}, {"f4", 26}, {"g4", 25}, {"h4", 24},
    {"a3", 23}, {"b3", 22}, {"c3", 21}, {"d3", 20}, {"e3", 19}, {"f3", 18}, {"g3", 17}, {"h3", 16},
    {"a2", 15}, {"b2", 14}, {"c2", 13}, {"d2", 12}, {"e2", 11}, {"f2", 10}, {"g2", 9},  {"h2", 8},
    {"a1", 7},  {"b1", 6},  {"c1", 5},  {"d1", 4},  {"e1", 3},  {"f1", 2},  {"g1", 1},  {"h1", 0}
};

void loadPieceTextures()
{
    for(int i{}; i<12; i++)
    {
        Image pieceImage = LoadImage(Globals::pieceDir[i].c_str());
        ImageResize(&pieceImage, Globals::tileSize, Globals::tileSize);
        Texture2D pieceTexture = LoadTextureFromImage(pieceImage);
        Globals::pieceTextures[i] = pieceTexture;
        ImageResize(&pieceImage, Globals::capturedSize, Globals::capturedSize);
        Texture2D captureTexture = LoadTextureFromImage(pieceImage);
        Globals::captureTextures[i] = captureTexture;
        UnloadImage(pieceImage);
    }
}

void unloadPieceTextures()
{
    for(int i{}; i < 12; i++)
    {
        UnloadTexture(Globals::pieceTextures[i]);
        UnloadTexture(Globals::captureTextures[i]);
    }
}

bool getType(char type)
{
    return (isupper(type))? white: black;
}

bool flipType(char type)
{
    if(type < 'Z') return white;
    return black;   
}

char flipCase(char ch)
{
    return isupper(ch)? tolower(ch): toupper(ch);
}

void playSound(size_t moveType)
{
    switch (moveType)
    {
        case regular:
            Globals::sound.playDefault();
            break;

        case capture:
            Globals::sound.playCapture();
            break;

        case castle:
            Globals::sound.playCastle();
            break;

        case check:
            Globals::sound.playCheck();
            break;
        
        case checkmate:
            Globals::sound.playCheckmate();
            break;

        case stalemate:
            Globals::sound.playStalemate();

        default:
            break;
    }
}

uint16_t moveEncoder(uint16_t srcTile, uint16_t destTile, uint16_t promo)
{
    uint16_t res(promo);
    res |= (srcTile << 6);
    res |= destTile;
    return res;
}

void moveDecoder(uint16_t& srcTile, uint16_t& destTile, uint16_t& promo, uint16_t move)
{
    destTile = move & 63;
    srcTile = (move & 4032) >> 6;
    promo = (move & 28672);
}

std::string getAlgebricNotation(uint16_t& move, bool side)
{
    uint16_t src{}, dest{}, promo{};
    std::string res = "";
    moveDecoder(src, dest, promo, move);
    res += coordinate[63-src];
    res +=  coordinate[63-dest];
    if(promo) res += asciiPieces[promo/knightProm + (side * 6)];
    return res;
}

void writeInFile(std::string FEN)
{
    try
    {
        Globals::outFile.open("fen_log.txt", std::ios::app);
        Globals::outFile<<Globals::FENString<<std::endl;
        Globals::outFile.close();
    }
    catch(const std::exception& e)
    {
        std::cerr << "Couldn't open the file\n";
        std::cerr <<"Exception Occured: "<< e.what() << '\n';
    }
}

void centerWindow()
{
    int posX = GetMonitorWidth(0) - GetScreenWidth();
    int posY = GetMonitorHeight(0) - GetScreenHeight();
    posX /= 2;
    posY /= 2;
    SetWindowPosition(posX, posY);
}

void resizeWindow()
{
    Globals::windowHeight = GetScreenHeight();
    Globals::windowWidth  = GetScreenWidth();
    Globals::setUIparams();
    unloadPieceTextures();
    loadPieceTextures();
}