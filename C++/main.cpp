#include "include/raylib.h"
#include <string>
#include <iostream>
const unsigned int tileSize = 100;

typedef struct Piece
{
    char type;
    Texture2D texture;
    Vector2 position;
} Piece;

Piece piece[32];

void InitializePiece(Piece *piece, char type, Texture2D texture, Vector2 position)
{
    piece->type = type;
    piece->texture = texture;
    piece->position = position;
}
// Function to initialize all pieces
void InitializePieces(Texture2D texture[])
{
    // pieceTexture[12] contains the texture of pieces in this order: [br, bk, bb, bq, bking, bpawn, wpawn, wr, wk, wb, wq, wking]
    // Initialize black pieces
    InitializePiece(&piece[0], 'r', texture[0], (Vector2){0, 0}); // Rook
    InitializePiece(&piece[1], 'n', texture[1], (Vector2){1, 0}); // Knight
    InitializePiece(&piece[2], 'b', texture[2], (Vector2){2, 0}); // Bishop
    InitializePiece(&piece[3], 'q', texture[3], (Vector2){3, 0}); // Queen
    InitializePiece(&piece[4], 'k', texture[4], (Vector2){4, 0}); // King
    InitializePiece(&piece[5], 'b', texture[2], (Vector2){5, 0}); // Bishop
    InitializePiece(&piece[6], 'n', texture[1], (Vector2){6, 0}); // Knight
    InitializePiece(&piece[7], 'r', texture[0], (Vector2){7, 0}); // Rook
    for (int i = 0; i < 8; i++)
        InitializePiece(&piece[8 + i], 'p', texture[5], (Vector2){i, 1}); // Pawns

    // Initialize white pieces
    InitializePiece(&piece[16], 'R', texture[7], (Vector2){0, 7});  // Rook
    InitializePiece(&piece[17], 'N', texture[8], (Vector2){1, 7});  // Knight
    InitializePiece(&piece[18], 'B', texture[9], (Vector2){2, 7});  // Bishop
    InitializePiece(&piece[19], 'Q', texture[10], (Vector2){3, 7}); // Queen
    InitializePiece(&piece[20], 'K', texture[11], (Vector2){4, 7}); // King
    InitializePiece(&piece[21], 'B', texture[9], (Vector2){5, 7});  // Bishop
    InitializePiece(&piece[22], 'N', texture[8], (Vector2){6, 7});  // Knight
    InitializePiece(&piece[23], 'R', texture[7], (Vector2){7, 7});  // Rook
    for (int i = 0; i < 8; i++)
        InitializePiece(&piece[24 + i], 'P', texture[6], (Vector2){i, 6}); // Pawns
}

void loadTextures()
{
    std::string pieceDir[] = {"../Assets/blackRook.png", "../Assets/blackKnight.png", "../Assets/blackBishop.png", "../Assets/blackQueen.png", "../Assets/blackKing.png", "../Assets/blackPawn.png",
                              "../Assets/whitePawn.png", "../Assets/whiteRook.png", "../Assets/whiteKnight.png", "../Assets/whiteBishop.png", "../Assets/whiteQueen.png", "../Assets/whiteKing.png"};
    Texture2D pieceTexture[12]; //[br, bk, bb, bq, bking, bpawn, wpawn, wr, wk, wb, wq, wking]

    for (int i{}; i < 12; i++)
    {
        Image image = LoadImage(pieceDir[i].c_str());
        ImageResize(&image, tileSize, tileSize);
        pieceTexture[i] = LoadTextureFromImage(image); // Load texture in graphics VRAM
        UnloadImage(image);                            // unload image from RAM
    }
    InitializePieces(pieceTexture);
}
void updateBoard()
{
    Color light{218, 217, 233, 255},
        dark{161, 123, 185, 255},
        tileColor;
    // coloring the board
    for (int row{}; row < 8; row++)
    {
        for (int col{}; col < 8; col++)
        {
            tileColor = ((row + col) % 2 == 0) ? light : dark; // white tiles will always be on (row + col == even) position
            DrawRectangle(tileSize * col, tileSize * row, tileSize, tileSize, tileColor);
        }
    }
    for (int i{}; i < 32; i++)
    {
        DrawTexture(piece[i].texture, piece[i].position.x * tileSize, piece[i].position.y * tileSize, WHITE);
    }
}
void mouseDragHandler()
{
    int pressedMousePosX = -1, pressedMousePosY = -1;
    int releasedMousePosX = -1, releasedMousePosY = -1;
    bool pieceSelected;
    Piece currPiece;
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        pressedMousePosX = GetMousePosition().x;
        pressedMousePosY = GetMousePosition().y;
        pressedMousePosX /= tileSize;
        pressedMousePosY /= tileSize;

        // draw an outline and highlight the square bieng clicked
        Color temp = {255, 150, 84, 100};
        DrawRectangle(pressedMousePosX * tileSize, pressedMousePosY * tileSize, tileSize, tileSize, temp);
        Rectangle rec = {pressedMousePosX * tileSize, pressedMousePosY * tileSize, tileSize, tileSize};
        DrawRectangleLinesEx(rec, 4, WHITE);

        // if clicked on a piece a creat a copy of that piece so that we can update its position and all in future
        for (int i{}; i < 32; i++)
        {
            if ((piece[i].position.x == pressedMousePosX) && (piece[i].position.y == pressedMousePosY))
            {
                InitializePiece(&currPiece, piece[i].type, piece[i].texture, piece[i].position);
                pieceSelected = true;
                std::cout<<currPiece.type<<std::endl;
                break;
            }
        }
        std::cout << pressedMousePosX << " " << pressedMousePosY << std::endl;
        // if piece is dragged redraw its textures
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && pieceSelected)
        {
            DrawTexture(currPiece.texture, GetMousePosition().x - 45, GetMousePosition().y - 45, WHITE);
        }
    }
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
    {
        pieceSelected = false;
        releasedMousePosX = GetMousePosition().x;
        releasedMousePosY = GetMousePosition().y;
    }
}

int main()
{
    InitWindow(tileSize * 8, tileSize * 8, "Chess");
    SetTargetFPS(30);
    loadTextures();
    // for (int i{}; i < 32; i++) print the intial position of every piece
    // {
    //     std::cout << piece[i].type << " " << piece[i].position.x << " " << piece[i].position.y << std::endl;
    // }
    while (!WindowShouldClose())
    {
        BeginDrawing();
        updateBoard();
        mouseDragHandler();
        EndDrawing();
    }
    CloseWindow();
}