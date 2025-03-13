#include "interface.hpp"
#include <chrono>
#include <thread>
#include <iostream>

Interface::Interface(Board& board, Engine& engine):
    board(board),
    engine(engine),
    nodesSearched(0),
    bestMove("Null"),
    sidePanelX_(Globals::boardSize + Globals::leftPadding*2),
    leftPadding_(Globals::leftPadding),
    topPadding_(Globals::topPadding),
    logo_("Ray Chess"),
    btnFlipBoard_(),
    btnResetBoard_(),
    currPieceType_('0'),
    clickedOnRow_(-1),
    clickedOnCol_(-1),
    pieceSelected_(false)
{
    Rectangle copyFENBase
    {
        static_cast<float>(sidePanelX_ + leftPadding_), 
        static_cast<float>((Globals::tileSize*6 + topPadding_) - Globals::btnHeight),
        static_cast<float>(Globals::btnWidth), 
        static_cast<float>(Globals::btnHeight)
    };
    btnCopyFEN_ = Button(copyFENBase,  Colors::btnBase, Colors::btnBorder, "Copy FEN", Globals::tileSize/4, Colors::labelColor, [&](){});
    Rectangle startBase 
    {
        static_cast<float>(sidePanelX_ + leftPadding_*3 + Globals::btnWidth),
        static_cast<float>((Globals::tileSize*6 + topPadding_) - Globals::btnHeight),
        static_cast<float>(Globals::btnWidth), 
        static_cast<float>(Globals::btnHeight)
    };
    btnStart_ = Button(startBase,  Colors::btnBase, Colors::btnBorder, "Start Game", Globals::tileSize/4, Colors::labelColor, [&](){});

    // Create the play white button
    Rectangle flipBoard
    {
        static_cast<float>(sidePanelX_ + leftPadding_), 
        static_cast<float>((Globals::tileSize*7 + topPadding_) - Globals::btnHeight),
        static_cast<float>(Globals::btnWidth), 
        static_cast<float>(Globals::btnHeight)
    };
    btnFlipBoard_ = Button(flipBoard, WHITE, BLACK, "Flip Board", Globals::tileSize/4, BLACK, [&]()
        {
            Globals::player = !Globals::player;
        }
    );

    // Create the play black button
    Rectangle resetBoard
    {
        static_cast<float>(sidePanelX_ + leftPadding_*3 + Globals::btnWidth),
        static_cast<float>((Globals::tileSize*7 + topPadding_) - Globals::btnHeight),
        static_cast<float>(Globals::btnWidth), 
        static_cast<float>(Globals::btnHeight)
    };
    btnResetBoard_ = Button(resetBoard, BLACK, WHITE, "Reset Board", Globals::tileSize/4, WHITE, [&]()
        {
            board.setupInitialBoardState();
            Globals::outFile.open("fen_log.txt", std::ofstream::trunc);
            Globals::outFile<<Globals::FENString<<std::endl;
            Globals::outFile.close();
        }
    );
}

void Interface::drawSidePanel() const
{
    size_t posX = sidePanelX_ + leftPadding_;

    // Draw the logo
    DrawText(logo_.c_str(), posX, Globals::topPadding, Globals::tileSize, RAYWHITE);

    // Draw the FEN string
    std::string str1 = Globals::FENString.substr(0, Globals::FENString.length()/2);
    std::string str2 = Globals::FENString.substr(Globals::FENString.length()/2);
    DrawText(str1.c_str(), posX, Globals::tileSize*2.5, Globals::tileSize/5, RAYWHITE);
    DrawText(str2.c_str(), posX, Globals::tileSize*2.5+Globals::tileSize/4, Globals::tileSize/5, RAYWHITE);

    
    std::string totalMoves = "Total moves available for ";
    totalMoves += (board.turn)? "black : ": "white : ";
    totalMoves += std::to_string(board.findTotalLegalMoves(board.turn));

    std::string depthMessage = "Current Global depth : " + std::to_string(Globals::depth);
    std::string bestMoveStr = "Current best move : ";
    bestMoveStr += bestMove;
    std::string totalNodes = "Total positions Searched : " + std::to_string(nodesSearched);
    
    DrawText(depthMessage.c_str(), sidePanelX_ + leftPadding_, Globals::tileSize*3.5, Globals::tileSize/5, RAYWHITE);
    DrawText(totalMoves.c_str(), sidePanelX_ + leftPadding_, Globals::tileSize*5, Globals::tileSize/5, RAYWHITE);
  
    DrawText(bestMoveStr.c_str(), posX, Globals::tileSize*4, Globals::tileSize/5, RAYWHITE);
    DrawText(totalNodes.c_str(), posX, Globals::tileSize*4.5, Globals::tileSize/5, RAYWHITE);

    // Draw all the button
    btnCopyFEN_.draw();
    btnStart_.draw();
    btnFlipBoard_.draw();
    btnResetBoard_.draw();
}

void Interface::runSelf(bool mode)
{
    std::this_thread::sleep_for(std::chrono::nanoseconds(0));
    std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(0));

    uint16_t temp[218];
    uint16_t size{};
    board.getMoveList(temp, size, board.turn);
    Globals::outFile.open("fen_log.txt", std::ios::app);
    Globals::outFile<<Globals::FENString<<std::endl;
    Globals::outFile<<(int)size<<std::endl;
    Globals::outFile.close();
    
    uint64_t total{};
    char moveType;
    if(mode){
        uint16_t bestMove = engine.minimax(board, Globals::depth, INT_MIN, INT_MAX, board.turn, 0, total).second;
        uint16_t src, dest;
        uint16_t promo{};
        moveDecoder(src, dest, promo, bestMove);
        this->bestMove = coordinate[src] + coordinate[dest];
        moveType = board.makeMove(bestMove);
        nodesSearched = total;
    }
    else  moveType = board.makeMove(temp[rand() % size]);
    playSound(moveType);

    board.updateMatrixBoard();
    board.updateFENViamatrixBoard();

}

void Interface::promotionHandler(uint16_t& promo)
{

    // queen is default promotion if player click outside of promotion panel queen promotion will be considered
    size_t x = Globals::tileSize * 2 + Globals::leftPadding;
    float y = Globals::tileSize * 3.5 + Globals::topPadding;
    promo = queenProm;
    Vector2 mousePos = GetMousePosition();
    bool isMouseInsideHorizontally = false, isMouseInsideVertically = false;

    while(!IsMouseButtonPressed(MouseButton::MOUSE_BUTTON_LEFT))
    {
        mousePos = GetMousePosition();
        isMouseInsideHorizontally   = (size_t)mousePos.x >= x && (size_t)mousePos.x < Globals::tileSize * 6 + Globals::leftPadding;
        isMouseInsideVertically     = (size_t)mousePos.y >= y && (size_t)mousePos.y < Globals::tileSize * 4.5 + Globals::topPadding;
        
        BeginDrawing();
        ClearBackground(Colors::background);
        board.drawBoardAndPieces();
        board.drawCapturedPieces();

        // draw all the texture for all four options
        for (uint16_t i = 0; i < 4; i++) 
        {
            uint16_t piece = (N + i) + (board.turn * 6);
            DrawRectangle(Globals::tileSize * i + x, y, Globals::tileSize, Globals::tileSize, GRAY);
            DrawTexture(Globals::pieceTextures[piece], Globals::tileSize * i + x, y, RAYWHITE);
        }
        Rectangle rec = 
        {   
            static_cast<float>(((size_t)mousePos.x - Globals::leftPadding)/Globals::tileSize * Globals::tileSize + Globals::leftPadding), 
            static_cast<float>(Globals::tileSize * 3.5 + Globals::topPadding), 
            static_cast<float>(Globals::tileSize),
            static_cast<float>(Globals::tileSize)
        };
        if(isMouseInsideHorizontally && isMouseInsideVertically) 
            DrawRectangleLinesEx (rec, Globals::tileSize/20, RAYWHITE);
        
        drawSidePanel();
        EndDrawing();
    }
    // if player made a choice by clicking inside one of options then make it promotion choice
    if(isMouseInsideHorizontally && isMouseInsideVertically) 
        promo= (((size_t)mousePos.x - Globals::leftPadding)/Globals::tileSize - 1) * knightProm;
}

void Interface::boardInteractionHandler()
{   
    if(board.getGameEndState(board.turn) == checkmate || board.getGameEndState(board.turn) == stalemate || board.emptyTurns >= 50)
        return;
    
    //runSelf(false);

    bool isMouseInsideBoard =
        GetMousePosition().x > board.padding[left] && GetMousePosition().x < board.padding[right] &&
        GetMousePosition().y > board.padding[top] && GetMousePosition().y < board.padding[bottom];

    if(isMouseInsideBoard && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        clickedOnRow_ = ((size_t)GetMousePosition().y-Globals::topPadding)/Globals::tileSize; // this will give the row of the tile clicked
        clickedOnCol_ = ((size_t)GetMousePosition().x-Globals::leftPadding)/Globals::tileSize; // this will give the column of the tile clicked

        size_t srcTile = 63-(clickedOnRow_ * 8 + clickedOnCol_);
        if(Globals::player) srcTile = 63 - srcTile;
        currPieceType_ = board.piece.getPieceType(srcTile);

        char currPieceCase(currPieceType_);

        // if a piece is bieng clicked
        if(currPieceType_ != '0')
        {
            pieceSelected_ = true;
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            // if a valid piece is clicked then insert its corrosponding legal moves into the bitboard
            if(getType(currPieceCase) == board.turn)
                board.legalMoves.setVal(board.piece.getLegalMoves(currPieceType_, srcTile));
        }
    }

    // If a piece is bieng dragged keep drawing it on the cursor
    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON) && pieceSelected_)
    {
        // is right button is clicked while dragging a piece cancel everything
        if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
        {
            SetMouseCursor(MOUSE_CURSOR_ARROW);
            pieceSelected_ = false;
            board.legalMoves.setVal(0ULL);
            return;
        }
        
        if(isMouseInsideBoard) // Outline the tiles on which piece is bieng dragged to
        {    
            Rectangle rec = 
            {
                float(((int)(GetMousePosition().x-Globals::leftPadding) / Globals::tileSize) * Globals::tileSize + Globals::leftPadding),
                float(((int)(GetMousePosition().y-Globals::topPadding) / Globals::tileSize) * Globals::tileSize + Globals::topPadding),
                (float)Globals::tileSize, (float)Globals::tileSize
            };
            // draw an outline and highlight the square bieng clicked
            DrawRectangleLinesEx(rec, Globals::tileSize/20, WHITE);
        }
        Color temp = {255, 150, 84, 100};
        DrawRectangle
        (
            ((float)clickedOnCol_ * Globals::tileSize)+Globals::topPadding, 
            ((float)clickedOnRow_ * Globals::tileSize)+Globals::leftPadding,
            Globals::tileSize, Globals::tileSize, temp
        );
        // if piece is dragged keep redrawing its textures
        DrawTexture
        (
            Globals::pieceTextures[charPieces.at(currPieceType_)], 
            GetMousePosition().x - Globals::tileSize/2, GetMousePosition().y - Globals::tileSize/2, WHITE
        );
    }

    if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && pieceSelected_)
    {
        SetMouseCursor(MOUSE_CURSOR_ARROW);
        size_t releasedOnTileCol = (GetMousePosition().x-Globals::leftPadding)/Globals::tileSize;
        size_t releasedOnTileRow = (GetMousePosition().y-Globals::topPadding)/Globals::tileSize;

        isMouseInsideBoard = 
            GetMousePosition().y > board.padding[top] && GetMousePosition().y <= board.padding[bottom] && 
            GetMousePosition().x > board.padding[left] && GetMousePosition().x < board.padding[right];

        /*  
        subtracting 63 to get source and destination tile because in and array we start from left to right
        but in binary we start from right to left hence ith position in array will be 63-i position in binary/bitboard
        */
        size_t srcTile = 63-(clickedOnRow_ * 8 + clickedOnCol_);
        size_t destTile = 63-(releasedOnTileRow * 8 + releasedOnTileCol);

        if(Globals::player)
        {
            srcTile = 63-srcTile;
            destTile = 63-destTile;
        }

        bool isMoveLegal = (board.legalMoves.getVal() & (1ULL <<  destTile));
        if(isMoveLegal)
        {
            uint16_t promo{};
            if(toupper(currPieceType_) == 'P' && (destTile < 8 || destTile > 55)) promotionHandler(promo);
            size_t moveType = board.makeMove(moveEncoder(srcTile, destTile, promo));
            playSound(moveType);
        
            board.updateMatrixBoard();
            board.updateFENViamatrixBoard();
            Globals::outFile.open("fen_log.txt", std::ios::app);
            Globals::outFile<<Globals::FENString<<std::endl;
            Globals::outFile.close();
        };
        board.legalMoves.setVal(0ULL);
        currPieceType_ = '0';
        pieceSelected_ = false;
        clickedOnRow_ = -1;
        clickedOnCol_ = -1;
        // testing for minimax
        // std::cout<<engine.perft(board, 5, board.turn)<<"\n";
        uint64_t total{};
        uint16_t src{}, dest{};
        //moveDecoder(src, dest, promo, engine.minimax(board, Globals::depth, INT_MIN, INT_MAX, board.turn, 0, total).second);
        bestMove = (coordinate[63-src] + coordinate[63-dest]);
        nodesSearched = total;
    }
}

void Interface::sidePanelInteractionHandler()
{
    btnCopyFEN_.interactionHandler();
    btnStart_.interactionHandler();
    btnFlipBoard_.interactionHandler();
    btnResetBoard_.interactionHandler();
}

void Interface::interactionHandler()
{
    boardInteractionHandler();
    sidePanelInteractionHandler();
}