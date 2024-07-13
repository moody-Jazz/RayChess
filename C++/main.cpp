#include "include/raylib.h"
#include <string>
#include <iostream>
const unsigned int tileSize = 100;
class GameSound
{
    private:
        Sound defaultMove,
            capture,
            castle,
            check,
            gameEnd;
          
    public:
        GameSound(){
            InitAudioDevice();
            defaultMove = LoadSound("../Assets/sounds/default.wav");
            capture = LoadSound("../Assets/sounds/capture.wav");
            castle = LoadSound("../Assets/sounds/castle.wav");
            check = LoadSound("../Assets/sounds/check.wav");
            gameEnd = LoadSound("../Assets/sounds/gameEnd.wav");
        }
        ~GameSound(){
            // Unload the music streams when they're no longer needed
            UnloadSound(defaultMove);
            UnloadSound(capture);
            UnloadSound(castle);
            UnloadSound(check);
            UnloadSound(gameEnd);

            CloseAudioDevice(); // Close the audio device
        }
        void playCapture(){
            PlaySound(capture);
        }
        void playDefault(){
            PlaySound(defaultMove);
        }
        void playCastle(){
            PlaySound(castle);
        }
        void playCheck(){
            PlaySound(check);
        }
    
};
    GameSound sound;
typedef struct Piece
{
    char type;
    Texture2D texture;
    int row;
    int col;
} Piece;

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

void loadTextures()
{
    std::string pieceDir[] = {"../Assets/images/blackRook.png", "../Assets/images/blackKnight.png", "../Assets/images/blackBishop.png", "../Assets/images/blackQueen.png", "../Assets/images/blackKing.png", "../Assets/images/blackPawn.png",
                              "../Assets/images/whitePawn.png", "../Assets/images/whiteRook.png", "../Assets/images/whiteKnight.png", "../Assets/images/whiteBishop.png", "../Assets/images/whiteQueen.png", "../Assets/images/whiteKing.png"};
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
    for (int i{}; i < totalPiece; i++)
    {
        DrawTexture(piece[i].texture, piece[i].col * tileSize, piece[i].row * tileSize, WHITE);
    }
} 

int clickedOnRow = -1, clickedOnCol = -1;
int releasedOnTileRow = -1, releasedOnTileCol = -1;
bool pieceSelected = false;
Piece* currPiece = nullptr;

Piece* isThereA_Piece(int x, int y){
     for (int i{}; i < totalPiece; i++)
            if ((piece[i].row == x) && (piece[i].col == y)) return &piece[i]; 
    return nullptr;
}

void mouseInputHandler()
{
    // if a piece is bieng clicked copy it into the currPiece and change the cursor  
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        clickedOnRow = GetMousePosition().y/tileSize; // this will give the row of the tile clicked
        clickedOnCol = GetMousePosition().x/tileSize; // this will give the column of the tile clicked
        // std::cout<<pressedMousePosX<<" "<<pressedMousePosY<<std::endl;

        //if clicked on a tile with a piece; 
        currPiece = isThereA_Piece(clickedOnRow, clickedOnCol);
        if(currPiece != nullptr){
            pieceSelected = true;
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            // draw an outline and highlight the square bieng clicked
            Color temp = {255, 150, 84, 100};
            DrawRectangle((float)clickedOnCol * tileSize, (float)clickedOnRow * tileSize, tileSize, tileSize, temp);
            //std::cout<<currPiece->type<<std::endl;
        }
    }
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && pieceSelected)
    {
        clickedOnRow = (int)GetMousePosition().y / tileSize;
        clickedOnCol = (int)GetMousePosition().x / tileSize;
        Rectangle rec = {
            static_cast<float>(clickedOnCol) * tileSize,
            static_cast<float>(clickedOnRow) * tileSize,
            tileSize,
            tileSize
        };
        DrawRectangleLinesEx(rec, 4, WHITE);
        // std::cout <<currPiece->type<<" "<< clickedOnRow << " " << clickedOnCol << std::endl;
        // if piece is dragged redraw its textures
        DrawTexture(currPiece->texture, GetMousePosition().x - 45, GetMousePosition().y - 45, WHITE);

    }
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && pieceSelected)
    {
        SetMouseCursor(MOUSE_CURSOR_ARROW);
        releasedOnTileCol = GetMousePosition().x/tileSize;
        releasedOnTileRow = GetMousePosition().y/tileSize;
        Piece *releasedOnPiece = isThereA_Piece(releasedOnTileRow,releasedOnTileCol); 

        bool isMouseInsideVertically = GetMousePosition().y >= 0 && GetMousePosition().y <= GetScreenWidth(),
             isMouseInsideHorizontally = GetMousePosition().x >= 0 && GetMousePosition().x <= GetScreenHeight(),
             isPieceReleasedOnEmptyTile = (releasedOnPiece)? false : true,
             isPieceReleasedOnEnemyTile = (!isPieceReleasedOnEmptyTile && ((releasedOnPiece->type >= 'a' && currPiece->type <= 'Z') || (releasedOnPiece->type <= 'Z' && currPiece->type >= 'a')));

        // if mouse is holding a piece and released inside the board update the position of the piece 
        if(isMouseInsideHorizontally && isMouseInsideVertically && pieceSelected){

            if(isPieceReleasedOnEmptyTile){    
                sound.playDefault();
                currPiece->row = releasedOnTileRow;
                currPiece->col = releasedOnTileCol;
            }

            // if enemy piece is captured then put the captured piece at the end of the array and decrease the size till which updateBoard have acess 
            else if(isPieceReleasedOnEnemyTile){
                sound.playCapture();
                currPiece->row = releasedOnTileRow;
                currPiece->col = releasedOnTileCol;
                releasedOnPiece->row = piece[totalPiece-1].row;
                releasedOnPiece->col = piece[totalPiece-1].col;
                releasedOnPiece->type = piece[totalPiece-1].type;
                releasedOnPiece->texture = piece[totalPiece-1].texture;
                totalPiece--;
            }
        }
        releasedOnPiece = nullptr;     
        currPiece = nullptr;
        pieceSelected = false;
    }
}

int main()
{
    InitWindow(tileSize * 8, tileSize * 8, "Chess");
    SetTargetFPS(60);
    loadTextures();

    //print the intial position of every piece
    // for (int i{}; i < 32; i++) 
    // {
    //     std::cout << piece[i].type << " " << piece[i].row << " " << piece[i].col << std::endl;
    // }
    
    while (!WindowShouldClose())
    {      
        BeginDrawing();
        ClearBackground(RAYWHITE);
        updateBoard();;
        mouseInputHandler();
        EndDrawing();
    }
    CloseWindow();
}