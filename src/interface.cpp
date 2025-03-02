#include "interface.hpp"
#include <iostream>

Interface::Interface(Board& board, Engine& engine):
    board(board),
    engine(engine),
    sidePanelX_(Globals::boardSize + Globals::leftPadding*2),
    leftPadding_(Globals::leftPadding),
    topPadding_(Globals::topPadding),
    logo_("Ray Chess"),
    btnPlayWhite_(),
    btnPlayBlack_(),
    currPieceType_('0'),
    clickedOnRow_(-1),
    clickedOnCol_(-1),
    pieceSelected_(false)
{
    Rectangle copyFENBase
    {
        static_cast<float>(sidePanelX_ + leftPadding_), 
        static_cast<float>((Globals::tileSize*5 + topPadding_) - Globals::btnHeight),
        static_cast<float>(Globals::btnWidth), 
        static_cast<float>(Globals::btnHeight)
    };
    btnCopyFEN_ = Button(copyFENBase,  Colors::btnBase, Colors::btnBorder, "Copy FEN", Globals::tileSize/4, Colors::labelColor, [&](){});
    Rectangle startBase 
    {
        static_cast<float>(sidePanelX_ + leftPadding_*3 + Globals::btnWidth),
        static_cast<float>((Globals::tileSize*5 + topPadding_) - Globals::btnHeight),
        static_cast<float>(Globals::btnWidth), 
        static_cast<float>(Globals::btnHeight)
    };
    btnStart_ = Button(startBase,  Colors::btnBase, Colors::btnBorder, "Start Game", Globals::tileSize/4, Colors::labelColor, [&](){});

    // Create the play white button
    Rectangle playWhiteBase
    {
        static_cast<float>(sidePanelX_ + leftPadding_), 
        static_cast<float>((Globals::tileSize*6 + topPadding_) - Globals::btnHeight),
        static_cast<float>(Globals::btnWidth), 
        static_cast<float>(Globals::btnHeight)
    };
    btnPlayWhite_ = Button(playWhiteBase, WHITE, BLACK, "Play White", Globals::tileSize/4, BLACK, [&]()
        {
            if(Globals::player != white) invertPieceTextures();
            Globals::player = white;

            Globals::kingsInitPos[white]                        = 3;
            Globals::kingsInitPos[black]                        = 59;
            Globals::queenInitPos[white]                        = 4;
            Globals::queenInitPos[black]                        = 60;

            Globals::rookInitPos[white][kingside]               = 0;
            Globals::rookInitPos[white][queenside]              = 7;
            Globals::rookInitPos[black][kingside]               = 56;
            Globals::rookInitPos[black][queenside]              = 63;

            Globals::castleRookTargetTiles[white][kingside]     = 2;
            Globals::castleRookTargetTiles[white][queenside]    = 4;
            Globals::castleRookTargetTiles[black][kingside]     = 58;
            Globals::castleRookTargetTiles[black][queenside]    = 60;

            Globals::castleKingTargetTile[white][kingside]      = 1;
            Globals::castleKingTargetTile[white][queenside]     = 5;
            Globals::castleKingTargetTile[black][kingside]      = 57;
            Globals::castleKingTargetTile[black][queenside]     = 61;

            Globals::castleBitmasks[white][kingside]            = 6ULL;
            Globals::castleBitmasks[white][queenside]           = 48UL;
            Globals::castleBitmasks[black][kingside]            = 432345564227567616ULL;
            Globals::castleBitmasks[black][queenside]           = 3458764513820540928ULL;
            Globals::castleBitmasks[white][both]                = 54ULL;
            Globals::castleBitmasks[black][both]                = 3891110078048108544ULL;

            board.setupInitialBoardState();
        }
    );

    // Create the play black button
    Rectangle playBlackBase
    {
        static_cast<float>(sidePanelX_ + leftPadding_*3 + Globals::btnWidth),
        static_cast<float>((Globals::tileSize*6 + topPadding_) - Globals::btnHeight),
        static_cast<float>(Globals::btnWidth), 
        static_cast<float>(Globals::btnHeight)
    };
    btnPlayBlack_ = Button(playBlackBase, BLACK, WHITE, "Play Black", Globals::tileSize/4, WHITE, [&]()
        {
            if(Globals::player != black) invertPieceTextures();
            Globals::player = black;

            Globals::kingsInitPos[white]                        = 4;
            Globals::kingsInitPos[black]                        = 60;
            Globals::queenInitPos[white]                        = 3;
            Globals::queenInitPos[black]                        = 59;

            Globals::rookInitPos[white][kingside]               = 7;
            Globals::rookInitPos[white][queenside]              = 0;
            Globals::rookInitPos[black][kingside]               = 63;
            Globals::rookInitPos[black][queenside]              = 56;

            Globals::castleRookTargetTiles[white][kingside]     = 5;
            Globals::castleRookTargetTiles[white][queenside]    = 3;
            Globals::castleRookTargetTiles[black][kingside]     = 61;
            Globals::castleRookTargetTiles[black][queenside]    = 59;

            Globals::castleKingTargetTile[white][kingside]      = 6;
            Globals::castleKingTargetTile[white][queenside]     = 2;
            Globals::castleKingTargetTile[black][kingside]      = 62;
            Globals::castleKingTargetTile[black][queenside]     = 58;

            Globals::castleBitmasks[white][kingside]            = 96ULL;
            Globals::castleBitmasks[white][queenside]           = 12UL;
            Globals::castleBitmasks[black][kingside]            = 6917529027641081856ULL;
            Globals::castleBitmasks[black][queenside]           = 864691128455135232ULL;
            Globals::castleBitmasks[white][both]                = 108ULL;
            Globals::castleBitmasks[black][both]                = 7782220156096217088ULL;

            board.setupInitialBoardState();
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
    DrawText(str1.c_str(), posX, Globals::tileSize*3, Globals::tileSize/5, RAYWHITE);
    DrawText(str2.c_str(), posX, Globals::tileSize*3+Globals::tileSize/4, Globals::tileSize/5, RAYWHITE);

    // Draw all the button
    btnCopyFEN_.draw();
    btnStart_.draw();
    btnPlayWhite_.draw();
    btnPlayBlack_.draw();
}

void Interface::boardInteractionHandler()
{
    // check if mouse is clicked inside the board
    bool isMouseInsideBoard =
        GetMousePosition().x > board.padding[left] && GetMousePosition().x < board.padding[right] &&
        GetMousePosition().y > board.padding[top] && GetMousePosition().y < board.padding[bottom];

    if(isMouseInsideBoard && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        clickedOnRow_ = ((size_t)GetMousePosition().y-Globals::topPadding)/Globals::tileSize; // this will give the row of the tile clicked
        clickedOnCol_ = ((size_t)GetMousePosition().x-Globals::leftPadding)/Globals::tileSize; // this will give the column of the tile clicked

        size_t srcTile = 63-(clickedOnRow_ * 8 + clickedOnCol_);
        currPieceType_ = board.piece.getPieceType(srcTile);

        char currPieceCase(currPieceType_);
        if(Globals::player) currPieceCase = flipCase(currPieceCase);

        // if a piece is bieng clicked
        if(currPieceType_ != '0')
        {
            pieceSelected_ = true;

            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);

            size_t currTurn = (currPieceType_>= 'a')?black : white; 

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
            DrawRectangleLinesEx(rec, 4, WHITE);
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

        bool isMoveLegal = (board.legalMoves.getVal() & (1ULL << destTile));
        
        if(isMoveLegal)
        {
            std::string move = moveEncoder(srcTile, destTile, '0');
            size_t moveType = board.makeMove(move);
            
            playSound(moveType);
        
            board.updateMatrixBoard();
            board.updateFENViamatrixBoard();
            std::cout<<Globals::FENString<<"\n";
        };
        board.legalMoves.setVal(0ULL);
        currPieceType_ = '0';
        pieceSelected_ = false;
        clickedOnRow_ = -1;
        clickedOnCol_ = -1;
        board.printMoveList(board.turn);
        std::pair<int, std::string> bestMove = engine.minimax(board, Globals::depth, board.turn, "");
        std::cout<<bestMove.first<<" "<<bestMove.second<<std::endl;
    }
}

void Interface::sidePanelInteractionHandler()
{
    btnCopyFEN_.interactionHandler();
    btnStart_.interactionHandler();
    btnPlayWhite_.interactionHandler();
    btnPlayBlack_.interactionHandler();
}

void Interface::interactionHandler()
{
    boardInteractionHandler();
    sidePanelInteractionHandler();
}