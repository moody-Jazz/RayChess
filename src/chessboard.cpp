#include "chessboard.hpp"
#include <iostream>

Board::Board()
{    
    turn            = Globals::player;
    padding[left]   = Globals::leftPadding;
    padding[right]  = Globals::tileSize * 8 + Globals::leftPadding;
    padding[top]    = Globals::topPadding;
    padding[bottom] = Globals::tileSize * 8 + Globals::topPadding;
    setupInitialBoardState();
}

// Draw the Checkers pattern
void Board::drawTiles() const
{
    Color tileColor;
    for (size_t row{}; row < 8; row++)
    {
        for (size_t col{}; col < 8; col++)
        {
            // white tiles will always be on (row + col == even) position
            tileColor = ((row + col) % 2 == 0) ? Colors::lightTile : Colors::darkTile; 
            DrawRectangle
            (
                ( Globals::tileSize * col) + Globals::leftPadding,
                ( Globals::tileSize * row) + Globals::topPadding,
                Globals::tileSize, Globals::tileSize, tileColor
            );
        }
    }
}

// Draw the file and rank strips
void Board::drawStrips() const
{
    DrawRectangle
    (
        0, Globals::topPadding, Globals::leftPadding,
        Globals::tileSize*8 ,Colors::boardOutline
    );
    DrawRectangle
    (
        Globals::leftPadding + Globals::tileSize*8, 
        Globals::topPadding, Globals::leftPadding,
        Globals::tileSize*8 ,Colors::boardOutline
    );
    DrawRectangle
    (
        0, 0, Globals::tileSize*8 + Globals::leftPadding*2,
        Globals::topPadding,Colors::boardOutline
    );
    DrawRectangle
    (
        0, Globals::topPadding + Globals::tileSize*8, 
        Globals::tileSize*8+Globals::leftPadding*2, 
        Globals::topPadding,Colors::boardOutline
    );
    
    size_t itr{};
    int rank, flag, offset;
    char file;

    if(!Globals::player) // if player has chose white
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
        DrawText(str.c_str(), posX, posY, Globals::leftPadding/1.85, WHITE);
        posY += Globals::tileSize;
    }

    // Draw the file number strip like a, b, c etc
    posY = Globals::boardSize + Globals::topPadding + Globals::topPadding/5;
    posX = Globals::leftPadding + Globals::tileSize/2;
    for(itr = 0; itr<8; itr++)
    {
        std::string str = std::string(1, file);
        DrawText(str.c_str(), posX, posY, Globals::topPadding/1.85, WHITE);
        DrawText(str.c_str(), posX, Globals::leftPadding/5, Globals::topPadding/1.85, WHITE);
        posX += Globals::tileSize;
        file += offset;
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
    // Draw all the captured pieces
    drawCapturedPieces();
    // highlight legal moves if there are any
    highlightTiles(legalMoves);
}

void Board::drawCapturedPieces() const
{
    if(!capturedPieceString.size()) return;
    size_t yPosForWhite(Globals::tileSize*2);
    size_t yPosForBlack(Globals::tileSize * 7 + Globals::topPadding);
    size_t xPosition(Globals::boardSize + Globals::leftPadding*3);

    size_t itrBlack{}, itrWhite{};
    for(auto& ch: capturedPieceString)
    {
        Vector2 positionVector;
        if(isupper(ch)) 
        {
            positionVector.x = xPosition + (Globals::capturedSize)*itrWhite;
            positionVector.y = yPosForWhite;
            itrWhite++;
        }
        else 
        {
            positionVector.x = xPosition + (Globals::capturedSize)*itrBlack;
            positionVector.y = yPosForBlack;
            itrBlack++;
        } 
        
        DrawTexture
        (
            Globals::captureTextures[charPieces.at(ch)], 
            positionVector.x,positionVector.y,RAYWHITE
        );
    }
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

void Board::highlightTiles(BitBoard tiles) const
{
    while(tiles.getVal())
    {
        int x = tiles.getLSBIndex();
        int row = (63 - x) % 8;
        int col = (63 - x) / 8;
        bool isTileOccupied = (piece.getPieceType(tiles.getLSBIndex()) != '0')? true: false;
        row *= Globals::tileSize;
        col *= Globals::tileSize;
        row += Globals::leftPadding;
        col += Globals::topPadding;
        row += Globals::tileSize/2;
        col += Globals::tileSize/2;
        
        if(isTileOccupied) 
            DrawRing
            (
                {static_cast<float>(row), static_cast<float>(col)}, 
                Globals::tileSize/2-Globals::tileSize/12, Globals::tileSize/2, 0, 360, 
                1000, Colors::tileHighlight
            );
        else 
            DrawCircle(row, col, Globals::tileSize/5, Colors::tileHighlight);

        tiles.popBit(x);
    }
}

std::vector<std::string> Board::getMoveList(bool side) const
{
    std::vector<std::string> res;
    int offSet = side? 6 : 0;

    for(int i = P+offSet; i<=K+offSet; i++)
    {
        BitBoard currPiece(piece.pieceBitboards[i]);
        std::vector<size_t> setBit = currPiece.getSetBitIndices();
        for(const auto& pos: setBit)
        { 
            BitBoard legalMove(piece.getLegalMoves(asciiPieces[i], pos));
            std::vector<size_t> legalMoveList = legalMove.getSetBitIndices();
            for(const auto& x: legalMoveList)
            {
                std::string move = moveEncoder(pos, x, '0');
                res.push_back(move);
            }
        }
    }
    return res;
}

void Board::printMoveList(bool side) const
{
    std::vector<std::string> moveList = getMoveList(side);
    for(const auto& str: moveList) std::cout<<str<<" ";
    std::cout<<"\n";
}

void Board::setupInitialBoardState()
{
    turn            = white;
    emptyTurns_     = 0;
    totalTurns_     = 0;
    legalMoves.setVal(0ULL);
    capturedPieceString = "";
    piece.setupInitialFlagsAndPositions();
    piece.initPieceBitboards();
    updateMatrixBoard();
    updateFENViamatrixBoard();
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
        char pieceType = piece.getPieceType(63-(i * 8 + j));
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
            if(!Globals::player && matrixBoard_[i][j] != '.')
            {
                if(emptyCol)
                {
                    str += std::to_string(emptyCol);
                    emptyCol = 0;
                }
                str += matrixBoard_[i][j];
            }
            else if (Globals::player && matrixBoard_[7-i][7-j] != '.')
            {
                if(emptyCol)
                {
                    str += std::to_string(emptyCol);
                    emptyCol = 0;
                }
                str += flipCase(matrixBoard_[7-i][7-j]);
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
    str += (turn)?" b":" w"; // Add turn
    str+=" ";

    if(piece.castle[white][kingside]) str += "K";
    if(piece.castle[white][queenside]) str += "Q";
    if(piece.castle[black][kingside]) str += "k";
    if(piece.castle[black][queenside]) str += "q";

    else str += "-";
    str += " ";
    if(Globals::player == white && piece.enPassant != noSq)
        str += coordinate[piece.enPassant];
    else if (Globals::player == black && piece.enPassant != noSq) 
        str += coordinate[63 - piece.enPassant];
    else str += "-";
    str += " ";
    str += std::to_string(emptyTurns_);
    str += " ";
    if(!totalTurns_) str += std::to_string(1);
    else str += std::to_string(totalTurns_);
    Globals::FENString = str;
}

size_t Board::findTotalLegalMoves(bool side)
{
    size_t peiceType = (side)? p : P;
    size_t totalMovesCount{};
    int itr = 6;
    while(itr--)
    {
        BitBoard tempPiece(piece.pieceBitboards[peiceType].getVal());
        while(tempPiece.getVal())
        {
            size_t setBitIndx(tempPiece.getLSBIndex());
            uint64_t movesBitboard = piece.getLegalMoves(asciiPieces[peiceType], setBitIndx);
            totalMovesCount += BitBoard(movesBitboard).getSetBitCount();
            tempPiece.popBit(setBitIndx);
        }
        peiceType++;
    }
    return totalMovesCount;
}

size_t Board::getGameEndState(bool side)
{
    size_t totalMovesCount(findTotalLegalMoves(side));
    bool isKingSafe(piece.isKingSafe(side));

    if(!isKingSafe && !totalMovesCount) return checkmate;
    else if(isKingSafe && !totalMovesCount) return stalemate;
    return 0;
}

size_t Board::makeMove(std::string move)
{
    size_t moveType = regular;
    size_t srcTile{};
    size_t destTile{};
    moveDecoder(srcTile, destTile, move);
    char movedType = piece.getPieceType(srcTile);
    char capturedType = piece.getPieceType(destTile);
    piece.enPassant = noSq;
    bool side = getType(movedType); 

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
            {
                piece.updatePieceBitboards('p', destTile-8, destTile-8);
                capturedPieceString += 'p';
            }
            else 
            {
                piece.updatePieceBitboards('P', destTile+8, destTile+8);
                capturedPieceString += 'P';
            }
            moveType = capture;
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
            piece.updatePieceBitboards
            (
                Globals::rookInitPos[side][movedSide], 
                Globals::castleRookTargetTiles[side][movedSide]
            );
            moveType = castle;
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

        // if rookPos == source tile then it means rook is moved else rook is captured
        int movedSide = -1;
        // if rook is captured then we have to change state of the opposing side
        if(rookPos == destTile) side = !side; 

        if((piece.castle[side][kingside] || piece.castle[side][queenside])) 
        {
            if(rookPos == Globals::rookInitPos[side][kingside]) movedSide = kingside;
            else if(rookPos == Globals::rookInitPos[side][queenside]) movedSide = queenside;

            piece.castle[side][movedSide] = false;
        }
        if(rookPos == destTile) side = !side; // reset the side
    }
    if(capturedType != '0') capturedPieceString += capturedType;
    piece.updatePieceBitboards(movedType, srcTile, destTile);
    piece.updateCombinedBitboards();
    piece.updateUnsafeTiles();
    flipTurn();
    if(!isPieceReleasedOnEmptyTile) moveType = capture;
    if(!piece.isKingSafe(!side)) moveType = check;

    if(isPieceReleasedOnEmptyTile && std::toupper(movedType) != 'P') emptyTurns_++;
    totalTurns_++;
    // if pawn is moved or piece is captured reset empty moves
    if(std::toupper(movedType) == 'P' || !isPieceReleasedOnEmptyTile) emptyTurns_ = 0; 
    if(getGameEndState(!side)) moveType = getGameEndState(!side);
    return moveType;
}
