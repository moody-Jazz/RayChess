#include "helper.hpp"

namespace Globals 
{
    size_t tileSize = 115;
    size_t totalPiece = 32;
    size_t totalCaptured = 0;
    size_t topPadding = 40;
    size_t leftPadding = 40;

    Color light = {218, 217, 233, 255};
    Color dark = {161, 123, 185, 255};
    Color bg = {49, 54, 63, 255};

    const std::string asciiPieces = "PNBRQKpnbrqk";
    const std::unordered_map<char, int> charPieces = 
    {
        {'P', P}, {'N', N}, {'B', B}, {'R', R}, {'Q', Q}, {'K', K},
        {'p', p}, {'n', n}, {'b', b}, {'r', r}, {'q', q}, {'k', k}
    };

    GameSound sound;
   
    PieceUI pieceOnBoard[32];
    PieceUI pieceCaptured[30];
}

void InitializePiece(PieceUI *piece, char type, Texture2D texture, int row, int col)
{
    piece->type = type;
    piece->texture = texture;
    piece->row = row;
    piece->col = col;
}

// Function to initialize all pieces
void InitializePieces(Texture2D texture[])
{
    // pieceTexture[12] contains the texture of pieces in this order: [br, bk, bb, bq, bking, bpawn, wpawn, wr, wk, wb, wq, wking]
    // Initialize black pieces
    InitializePiece(&Globals::pieceOnBoard[0], 'r', texture[0], 0,0); // black Rook
    InitializePiece(&Globals::pieceOnBoard[1], 'n', texture[1], 0,1); // black Knight
    InitializePiece(&Globals::pieceOnBoard[2], 'b', texture[2], 0,2); // black Bishop
    InitializePiece(&Globals::pieceOnBoard[3], 'q', texture[3], 0,3); // black Queen
    InitializePiece(&Globals::pieceOnBoard[4], 'k', texture[4], 0,4); // black King
    InitializePiece(&Globals::pieceOnBoard[5], 'b', texture[2], 0,5); // black Bishop
    InitializePiece(&Globals::pieceOnBoard[6], 'n', texture[1], 0,6); // black Knight
    InitializePiece(&Globals::pieceOnBoard[7], 'r', texture[0], 0,7); // black Rook
    for (int i = 0; i < 8; i++)
        InitializePiece(&Globals::pieceOnBoard[8 + i], 'p', texture[5], 1,i); // black Pawns

    // Initialize white pieces
    InitializePiece(&Globals::pieceOnBoard[16], 'R', texture[7], 7,0);  // whtie Rook
    InitializePiece(&Globals::pieceOnBoard[17], 'N', texture[8], 7,1);  // whtie Knight
    InitializePiece(&Globals::pieceOnBoard[18], 'B', texture[9], 7,2);  // whtie Bishop
    InitializePiece(&Globals::pieceOnBoard[19], 'Q', texture[10], 7,3); // whtie Queen
    InitializePiece(&Globals::pieceOnBoard[20], 'K', texture[11], 7,4); // whtie King
    InitializePiece(&Globals::pieceOnBoard[21], 'B', texture[9], 7,5);  // whtie Bishop
    InitializePiece(&Globals::pieceOnBoard[22], 'N', texture[8], 7,6);  // whtie Knight
    InitializePiece(&Globals::pieceOnBoard[23], 'R', texture[7], 7,7);  // whtie Rook
    for (int i = 0; i < 8; i++)
        InitializePiece(&Globals::pieceOnBoard[24 + i], 'P', texture[6], 6,i); // whtie Pawns
}

void deletePiece(PieceUI* piece)
{
    // resize the piece texture as it has been captured now and will be drawn on the side
    Image image = ImageCopy(LoadImageFromTexture(piece->texture));
    ImageResize(&image, Globals::tileSize/3, Globals::tileSize/3);
    piece->texture = LoadTextureFromImage(image);
    UnloadImage(image);

    // put the captured piece in piececaputured array
    Globals::pieceCaptured[Globals::totalCaptured].texture = piece->texture;
    Globals::pieceCaptured[Globals::totalCaptured].type = piece->type;
    Globals::pieceCaptured[Globals::totalCaptured].row = piece->row;
    Globals::pieceCaptured[Globals::totalCaptured].col = piece->col;

    // to remove the captured piece from pieceonboard array simply swap it with the last element of array
    
    size_t tempRow = piece->row;
    size_t tempCol = piece->col;
    char tempType = piece->type; 
    Texture tempTexture = piece->texture;

    piece->row = Globals::pieceOnBoard[Globals::totalPiece-1].row;
    piece->col = Globals::pieceOnBoard[Globals::totalPiece-1].col;
    piece->type = Globals::pieceOnBoard[Globals::totalPiece-1].type;
    piece->texture = Globals::pieceOnBoard[Globals::totalPiece-1].texture;

    Globals::pieceOnBoard[Globals::totalPiece-1].row = tempRow;
    Globals::pieceOnBoard[Globals::totalPiece-1].col = tempCol;
    Globals::pieceOnBoard[Globals::totalPiece-1].type = tempType;
    Globals::pieceOnBoard[Globals::totalPiece-1].texture = tempTexture;

    Globals::totalPiece--;
    Globals::totalCaptured++;
}

PieceUI* isThereAPieceTexture(size_t x, size_t y)
{
    for (size_t i{}; i < Globals::totalPiece; i++)
            if ((Globals::pieceOnBoard[i].row == x) && (Globals::pieceOnBoard[i].col == y)) return &Globals::pieceOnBoard[i]; 
    return nullptr;
}