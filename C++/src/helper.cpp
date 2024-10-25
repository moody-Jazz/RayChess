#include "../include/helper.hpp"
#include <string>

int tileSize = 115;
int leftPadding = 40;
int topPadding = 40;
int totalPiece = 32;
int totalCaptured = 0;
Color bg = {49, 54, 63, 255};

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

PieceUI pieceOnBoard[32];
PieceUI PieceCaptured[30];

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
    InitializePiece(&pieceOnBoard[0], 'r', texture[0], 0,0); // Rook
    InitializePiece(&pieceOnBoard[1], 'n', texture[1], 0,1); // Knight
    InitializePiece(&pieceOnBoard[2], 'b', texture[2], 0,2); // Bishop
    InitializePiece(&pieceOnBoard[3], 'q', texture[3], 0,3); // Queen
    InitializePiece(&pieceOnBoard[4], 'k', texture[4], 0,4); // King
    InitializePiece(&pieceOnBoard[5], 'b', texture[2], 0,5); // Bishop
    InitializePiece(&pieceOnBoard[6], 'n', texture[1], 0,6); // Knight
    InitializePiece(&pieceOnBoard[7], 'r', texture[0], 0,7); // Rook
    for (int i = 0; i < 8; i++)
        InitializePiece(&pieceOnBoard[8 + i], 'p', texture[5], 1,i); // Pawns

    // Initialize white pieces
    InitializePiece(&pieceOnBoard[16], 'R', texture[7], 7,0);  // Rook
    InitializePiece(&pieceOnBoard[17], 'N', texture[8], 7,1);  // Knight
    InitializePiece(&pieceOnBoard[18], 'B', texture[9], 7,2);  // Bishop
    InitializePiece(&pieceOnBoard[19], 'Q', texture[10], 7,3); // Queen
    InitializePiece(&pieceOnBoard[20], 'K', texture[11], 7,4); // King
    InitializePiece(&pieceOnBoard[21], 'B', texture[9], 7,5);  // Bishop
    InitializePiece(&pieceOnBoard[22], 'N', texture[8], 7,6);  // Knight
    InitializePiece(&pieceOnBoard[23], 'R', texture[7], 7,7);  // Rook
    for (int i = 0; i < 8; i++)
        InitializePiece(&pieceOnBoard[24 + i], 'P', texture[6], 6,i); // Pawns
}

void deletePiece(PieceUI* piece){

    // resize the piece texture as it has been captured now and will be drawn on the side
    Image image = ImageCopy(LoadImageFromTexture(piece->texture));
    ImageResize(&image, tileSize/3, tileSize/3);
    piece->texture = LoadTextureFromImage(image);
    UnloadImage(image);

    // put the captured piece in piececaputured array
    PieceCaptured[totalCaptured].texture = piece->texture;
    PieceCaptured[totalCaptured].type = piece->type;
    PieceCaptured[totalCaptured].row = piece->row;
    PieceCaptured[totalCaptured].col = piece->col;

    // to remove the captured piece from pieceonboard array simply put that piece at the end
    piece->row = pieceOnBoard[totalPiece-1].row;
    piece->col = pieceOnBoard[totalPiece-1].col;
    piece->type = pieceOnBoard[totalPiece-1].type;
    piece->texture = pieceOnBoard[totalPiece-1].texture;
    totalPiece--;
    totalCaptured++;
}

PieceUI* isThereA_PieceUI(int x, int y){
     for (int i{}; i < totalPiece; i++)
            if ((pieceOnBoard[i].row == x) && (pieceOnBoard[i].col == y)) return &pieceOnBoard[i]; 
    return nullptr;
}