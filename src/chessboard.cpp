#include "chessboard.hpp"
#include <iostream>

Board::Board():
    turn(white),
    legalMoves(0ULL),
    emptyTurns_(no_sq),
    totalTurns_(0)
{    
    padding[left] = Globals::leftPadding;
    padding[right] = Globals::tileSize * 8 + Globals::leftPadding;
    padding[top] = Globals::topPadding;
    padding[bottom] = Globals::tileSize * 8 + Globals::topPadding;

    updateMatrixBoard();
}

// Draw the Checkers pattern
void Board::drawTiles() const
{
    Color tileColor;
    for (size_t row{}; row < 8; row++)
    {
        for (size_t col{}; col < 8; col++)
        {
            tileColor = ((row + col) % 2 == 0) ? Colors::lightTile : Colors::darkTile; // white tiles will always be on (row + col == even) position
            DrawRectangle(
                ( Globals::tileSize * col)+Globals::leftPadding,
                ( Globals::tileSize * row)+Globals::topPadding,
                Globals::tileSize, Globals::tileSize, tileColor
            );
        }
    }
}

// Draw the file and rank strips
void Board::drawStrips() const
{
    DrawRectangle(0, Globals::topPadding, Globals::leftPadding, Globals::tileSize*8 ,Colors::boardOutline);
    DrawRectangle(Globals::leftPadding+Globals::tileSize*8, Globals::topPadding, Globals::leftPadding, Globals::tileSize*8 ,Colors::boardOutline);
    DrawRectangle(0, 0, Globals::tileSize*8+Globals::leftPadding*2, Globals::topPadding,Colors::boardOutline);
    DrawRectangle(0, Globals::topPadding+Globals::tileSize*8, Globals::tileSize*8+Globals::leftPadding*2, Globals::topPadding,Colors::boardOutline);
    
    size_t itr{};
    int rank, flag, offset;
    char file;

    if(black)
    {
        rank = -8;
        flag = 0;
        offset = 1;
        file = 'a';
    }
    else
    {
        rank = 1;
        flag = 9;
        offset = -1;
        file = 'h';
    }
    
    int posX = Globals::leftPadding/2;
    int posY = Globals::topPadding + Globals::tileSize/2;
    
    // Draw the rank number strip
    for(; rank != flag; rank++)
    {
        std::string str = std::to_string(abs(rank));
        DrawText(str.c_str(), posX, posY, 20, WHITE);
        posY += Globals::tileSize;
    }

    // Draw the file number strip like a, b, c etc
    posY = Globals::tileSize * 8 + Globals::topPadding + 8;
    posX = Globals::leftPadding + Globals::tileSize/2;
    for(itr = 0; itr<8; itr++)
    {
        std::string str = std::string(1, file);
        DrawText(str.c_str(), posX, posY, 20, WHITE);
        DrawText(str.c_str(), posX, 10, 20, WHITE);
        posX += Globals::tileSize;
        file += offset;
    }
}

void Board::highlightTiles(BitBoard tiles) const
{
    while(tiles.getVal())
    {
        int x = tiles.getLSBIndex();
        int row = (63 - x) % 8;
        int col = (63 - x) / 8;
        bool isTileOccupied = (piece.isTherePiece(tiles.getLSBIndex()) != '0')? true: false;
        row *= Globals::tileSize;
        col *= Globals::tileSize;
        row += Globals::leftPadding;
        col += Globals::topPadding;
        row += Globals::tileSize/2;
        col += Globals::tileSize/2;
        
        if(isTileOccupied) 
            DrawRing({static_cast<float>(row), static_cast<float>(col)}, Globals::tileSize/2-7, Globals::tileSize/2, 0, 360, 1000, Colors::tileHighlight);
        else 
            DrawCircle(row, col, Globals::tileSize/2-35, Colors::tileHighlight);

        tiles.popBit(x);
    }
}

void Board::drawBoardAndPieces() const
{
    drawTiles();
    drawStrips();

    size_t row{}, col{};
    // draw all the pieces on board using matrix board 
    for (size_t i{}; i < 64; i++)
    {
        row = i / 8;
        col = i % 8;
        if(matrixBoard_[row][col] != '.')
        {
            DrawTexture
            (
                Globals::pieceTextures[charPieces.at(matrixBoard_[row][col])], 
                col * Globals::tileSize + Globals::leftPadding,
                row * Globals::tileSize + Globals::topPadding, WHITE
            );
        }
    }
    // highlight legal moves if there are any
    highlightTiles(legalMoves);
}

void Board::print() const
{
    for(size_t i{}; i<8; i++)
    {
        for(size_t j{}; j<8; j++) std::cout<<matrixBoard_[i][j];

        std::cout<<"\n";
    }
    std::cout<<Globals::FENString<<"\n";
}

void Board::flipTurn()
{
    (turn == white)?turn = black: turn = white;
}

void Board::updateMatrixBoard() 
{
    size_t i{}, j{};
    for(size_t itr{}; itr<64; itr++)
    {
        i = itr / 8;
        j = itr % 8;
        char pieceType = piece.isTherePiece(63-(i * 8 + j));
        if(pieceType != '0') matrixBoard_[i][j] = pieceType;
        else matrixBoard_[i][j] = '.';
    }
}

void Board::updateFENViamatrixBoard()
{
    std::string str;
    for(size_t i{}; i<8; i++)
    {
        int emptyCol{};
        for(size_t j{}; j<8; j++)
        {
            if(matrixBoard_[i][j] != '.')
            {
                if(emptyCol)
                {
                    str += std::to_string(emptyCol);
                    emptyCol = 0;
                }
                str += matrixBoard_[i][j];
            }
            else emptyCol++;
        }
        if(emptyCol)
        {
            str += std::to_string(emptyCol);
            emptyCol = 0;
        }
        if(i<7)str+='/';
    }
    str += (turn)?" w":" b"; // Add turn
    str+=" ";

    if(piece.castle[white][kingside]) str += "K";
    if(piece.castle[white][queenside]) str += "Q";
    if(piece.castle[black][kingside]) str += "k";
    if(piece.castle[black][queenside]) str += "q";

    else str += "-";
    str += " ";
    if(piece.enPassant != no_sq) str += coordinate[piece.enPassant];
    else str += "-";
    str += " ";
    str += std::to_string(emptyTurns_);
    str += " ";
    str += std::to_string(totalTurns_);
    Globals::FENString = str;
}

size_t Board::makeMove(std::string move)
{
    size_t moveType = Sounds::regular;
    size_t srcTile{};
    size_t destTile{};
    moveDecoder(srcTile, destTile, move);
    char movedType = piece.isTherePiece(srcTile);
    char capturedType = piece.isTherePiece(destTile);
    
    bool side = findTurn(movedType); 

    bool isPieceReleasedOnEmptyTile = (capturedType == '0')? true : false;

    // If condtion to handle en passant related conditions
    if(toupper(movedType) == 'P')
    {
        // check if current move activates any enpassant on the board
        if(abs(srcTile - destTile) == 16)
            (movedType== 'P')? piece.enPassant = 63 - (destTile-8): piece.enPassant = 63 - (destTile + 8);

        // check if this is an enpassant capture
        else if((abs(srcTile - destTile) == 7 || abs(srcTile - destTile) == 9) && isPieceReleasedOnEmptyTile)
        {
            if(movedType == 'P') 
                piece.updatePieceBitboards('p', destTile-8, destTile-8);
            
            else 
                piece.updatePieceBitboards('P', destTile+8, destTile+8);
            moveType = Sounds::capture;
        }
       // handle pawn promotion later
    }

    // logic to update all the required variables if king has moved
    if(toupper(movedType) == 'K')
    {
        piece.kingPosition[side] = destTile; //update the global king position
        size_t movedSide = 2;

        // if king has castled update the rook position too
        if(srcTile == Globals::kingsInitPos[side] && destTile == Globals::castleKingTargetTile[side][kingside])
            movedSide = kingside;
        else if(srcTile == Globals::kingsInitPos[side] && destTile == Globals::castleKingTargetTile[side][queenside])
            movedSide = queenside;

        if(movedSide < 2) // if any of the above condition is true then it means king has moved two tiles/castled
        {
            piece.updatePieceBitboards(Globals::rookInitPos[side][movedSide], Globals::castleRookTargetTiles[side][movedSide]);
            moveType = Sounds::castle;
        }

        if(piece.castle[side][kingside] || piece.castle[side][queenside])
        {
            side?std::cout<<"black ": std::cout<<"white ";
            std::cout<<"king can't castle now\n";
            piece.castle[side][kingside] = false;
            piece.castle[side][queenside] = false;
        }
    }

    // logic to update the casteling states if any rook is moved or captured
    if(toupper(movedType) == 'R' || toupper(capturedType) == 'R')
    {
        size_t rookPos;
        if(toupper(movedType) == 'R') rookPos = srcTile;
        else if(toupper(capturedType) == 'R') rookPos = destTile;

        // if rookPos == source tile then it means rook is moved elseif rookpos == destination tile then rook is captured
        int movedSide = -1;
        if(rookPos == destTile) side = !side; // if rook is captured then we have to change state of the opposing side

        if((piece.castle[side][kingside] || piece.castle[side][queenside])) 
        {
            if(rookPos == Globals::rookInitPos[side][kingside]) movedSide = kingside;
            else if(rookPos == Globals::rookInitPos[side][queenside]) movedSide = queenside;

            piece.castle[side][movedSide] = false;
            side?std::cout<<"black ": std::cout<<"white ";
            std::cout<<"king can't castle";
            movedSide? std::cout<<" queenside now": std::cout<<" kingside now";
            std::cout<<std::endl;
        }
    }

    piece.updatePieceBitboards(movedType, srcTile, destTile);
    piece.updateCombinedBitboards();
    piece.updateUnsafeTiles();

    if(!isPieceReleasedOnEmptyTile) moveType = Sounds::capture;
    if(!piece.isKingSafe(!side)) moveType = Sounds::check;
    if(isPieceReleasedOnEmptyTile && std::toupper(movedType) != 'P') emptyTurns_++;
    totalTurns_++;
    if(std::toupper(movedType) == 'P' || !isPieceReleasedOnEmptyTile) emptyTurns_ = 0; // if pawn is moved or piece is captured reset empty moves
    return moveType;
}
