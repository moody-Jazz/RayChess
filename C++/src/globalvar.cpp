#include "../include/globalvar.hpp"

int tileSize = 110;
int totalPiece = 32;

Piece piece[32];

void InitializePiece(Piece *piece, char type, Texture2D texture, int row, int col)
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
    InitializePiece(&piece[0], 'r', texture[0], 0,0); // Rook
    InitializePiece(&piece[1], 'n', texture[1], 0,1); // Knight
    InitializePiece(&piece[2], 'b', texture[2], 0,2); // Bishop
    InitializePiece(&piece[3], 'q', texture[3], 0,3); // Queen
    InitializePiece(&piece[4], 'k', texture[4], 0,4); // King
    InitializePiece(&piece[5], 'b', texture[2], 0,5); // Bishop
    InitializePiece(&piece[6], 'n', texture[1], 0,6); // Knight
    InitializePiece(&piece[7], 'r', texture[0], 0,7); // Rook
    for (int i = 0; i < 8; i++)
        InitializePiece(&piece[8 + i], 'p', texture[5], 1,i); // Pawns

    // Initialize white pieces
    InitializePiece(&piece[16], 'R', texture[7], 7,0);  // Rook
    InitializePiece(&piece[17], 'N', texture[8], 7,1);  // Knight
    InitializePiece(&piece[18], 'B', texture[9], 7,2);  // Bishop
    InitializePiece(&piece[19], 'Q', texture[10], 7,3); // Queen
    InitializePiece(&piece[20], 'K', texture[11], 7,4); // King
    InitializePiece(&piece[21], 'B', texture[9], 7,5);  // Bishop
    InitializePiece(&piece[22], 'N', texture[8], 7,6);  // Knight
    InitializePiece(&piece[23], 'R', texture[7], 7,7);  // Rook
    for (int i = 0; i < 8; i++)
        InitializePiece(&piece[24 + i], 'P', texture[6], 6,i); // Pawns
}

Piece* isThereA_Piece(int x, int y){
     for (int i{}; i < totalPiece; i++)
            if ((piece[i].row == x) && (piece[i].col == y)) return &piece[i]; 
    return nullptr;
}