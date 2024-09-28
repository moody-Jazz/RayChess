#include "../include/globalvar.hpp"
#include <string>

int tileSize = 110;

int totalPiece = 32;

GameSound sound;

Color light = {218, 217, 233, 255};
Color dark = {161, 123, 185, 255};

// ASCII pieces
const std::string ascii_pieces = "PNBRQKpnbrqk";

const std::unordered_map<char, int>char_pieces = {
    {'P', P},
    {'N', N},
    {'B', B},
    {'R', R},
    {'Q', Q},
    {'K', K},
    {'p', p},
    {'n', n},
    {'b', b},
    {'r', r},
    {'q', q},
    {'k', k}
};
PieceUI pieceTextures[32];

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
    InitializePiece(&pieceTextures[0], 'r', texture[0], 0,0); // Rook
    InitializePiece(&pieceTextures[1], 'n', texture[1], 0,1); // Knight
    InitializePiece(&pieceTextures[2], 'b', texture[2], 0,2); // Bishop
    InitializePiece(&pieceTextures[3], 'q', texture[3], 0,3); // Queen
    InitializePiece(&pieceTextures[4], 'k', texture[4], 0,4); // King
    InitializePiece(&pieceTextures[5], 'b', texture[2], 0,5); // Bishop
    InitializePiece(&pieceTextures[6], 'n', texture[1], 0,6); // Knight
    InitializePiece(&pieceTextures[7], 'r', texture[0], 0,7); // Rook
    for (int i = 0; i < 8; i++)
        InitializePiece(&pieceTextures[8 + i], 'p', texture[5], 1,i); // Pawns

    // Initialize white pieces
    InitializePiece(&pieceTextures[16], 'R', texture[7], 7,0);  // Rook
    InitializePiece(&pieceTextures[17], 'N', texture[8], 7,1);  // Knight
    InitializePiece(&pieceTextures[18], 'B', texture[9], 7,2);  // Bishop
    InitializePiece(&pieceTextures[19], 'Q', texture[10], 7,3); // Queen
    InitializePiece(&pieceTextures[20], 'K', texture[11], 7,4); // King
    InitializePiece(&pieceTextures[21], 'B', texture[9], 7,5);  // Bishop
    InitializePiece(&pieceTextures[22], 'N', texture[8], 7,6);  // Knight
    InitializePiece(&pieceTextures[23], 'R', texture[7], 7,7);  // Rook
    for (int i = 0; i < 8; i++)
        InitializePiece(&pieceTextures[24 + i], 'P', texture[6], 6,i); // Pawns
}

void deletePiece(PieceUI* piece){
    piece->row = pieceTextures[totalPiece-1].row;
    piece->col = pieceTextures[totalPiece-1].col;
    piece->type = pieceTextures[totalPiece-1].type;
    piece->texture = pieceTextures[totalPiece-1].texture;
    totalPiece--;
}

PieceUI* isThereA_Piece(int x, int y){
     for (int i{}; i < totalPiece; i++)
            if ((pieceTextures[i].row == x) && (pieceTextures[i].col == y)) return &pieceTextures[i]; 
    return nullptr;
}