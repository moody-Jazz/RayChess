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

Board::Board(const Board& obj) :
    turn(obj.turn),
    piece(obj.piece),
    capturedPieceString(obj.capturedPieceString),     
    emptyTurns(obj.emptyTurns),
    totalTurns(obj.totalTurns)
{
    for(uint16_t i{}; i<8; i++)
        for(uint16_t j{}; j<8; j++) matrixBoard[i][j] = obj.matrixBoard[i][j];
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
       
        if(matrixBoard[row][col] != '.')
        {
            Texture2D currTexture =  Globals::pieceTextures[charPieces.at(matrixBoard[row][col])];
            
            if(Globals::player)
            { 
                row = 7 - row; 
                col = 7-col;
            }
        
            DrawTexture
            (
                currTexture, 
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

    size_t yPosForWhite, yPosForBlack;

    if(Globals::player)
    {
        yPosForBlack = Globals::tileSize*1.7;
        yPosForWhite = Globals::tileSize * 7.33 + Globals::topPadding;
    }
    else 
    {
        yPosForWhite = Globals::tileSize*1.7;
        yPosForBlack = Globals::tileSize * 7.33 + Globals::topPadding;
    }

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
        for(size_t j{}; j<8; j++) std::cout<<matrixBoard[i][j];
        std::cout<<"\n";
    }
    printf("\nCurrent Turn: %s",(this->turn)? "Black": "White");
    printf("\nEnPassant available on: %s",( piece.enPassant != noSq)? coordinate[piece.enPassant].c_str(): "none");
    std::cout<<"\nCastle Available for White on:";
    if(piece.castle[white][kingside]) std::cout<<" kingside ";
    if(piece.castle[white][queenside]) std::cout<<" queenside ";
    std::cout<<"\nCastle Available for black on:";
    if(piece.castle[black][kingside]) std::cout<<" kingside ";
    if(piece.castle[black][queenside]) std::cout<<" queenside ";
    std::cout<<"\n\n";
    std::cout<<Globals::FENString<<"\n";
}

void Board::highlightTiles(BitBoard tiles) const
{
    while(tiles.getVal())
    {
        int x = tiles.getLSBIndex();

        if(Globals::player) x = 63 - x;

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

        if(Globals::player) x = 63 - x;
        
        tiles.popBit(x);
    }
}

void Board::getMoveList(uint16_t* res, uint16_t&curr, bool side) const
{
    int offSet = side? 6 : 0;
    for(int i = P+offSet; i<=K+offSet; i++)
    {
        BitBoard currPiece(piece.pieceBitboards[i]);

        uint16_t size = currPiece.getSetBitCount();
        uint16_t setBitList[size];
        currPiece.getSetBitIndices(setBitList, size);

        for(uint16_t itr{}; itr<size; itr++)
        {
            BitBoard legalMove(const_cast<Piece&>(piece).getLegalMoves(asciiPieces[i], setBitList[itr]));
            uint16_t legalMoveListSize = legalMove.getSetBitCount();
            uint16_t legalMoveList[legalMoveListSize];
            legalMove.getSetBitIndices(legalMoveList, legalMoveListSize);
            for(uint16_t j{}; j<legalMoveListSize; j++)
            {
                // if this move is pawn promotion add three moves in the list
                if((i == p || i== P) && (legalMoveList[j] < 8 || legalMoveList[j] > 55))
                {
                    uint16_t arr[] = {knightProm, bishopProm, rookProm, queenProm};
                    for(uint16_t i{}; i < 4; i++)
                        res[curr++] = moveEncoder(setBitList[itr], legalMoveList[j], arr[i]);
                }
                else 
                    res[curr++] = moveEncoder(setBitList[itr], legalMoveList[j], 0);
            }
        }
    }
}

uint16_t Board::printMoveList(bool side) const
{
    uint16_t size{};
    uint16_t moveList[218];
    getMoveList(moveList, size, side);
    for(uint16_t i{}; i<size; i++)
    {
        std::cout<<getAlgebricNotation(moveList[i], side);
        std::cout<<" ";
    }
    std::cout<<"\n";
    return size;
}

void Board::setUIparams()
{
    padding[left]   = Globals::leftPadding;
    padding[right]  = Globals::tileSize * 8 + Globals::leftPadding;
    padding[top]    = Globals::topPadding;
    padding[bottom] = Globals::tileSize * 8 + Globals::topPadding;
}

void Board::setupInitialBoardState()
{
    emptyTurns     = 0;
    totalTurns     = 0;
    legalMoves.setVal(0ULL);
    capturedPieceString = "";
    piece.setupInitialFlagsAndPositions();
    piece.initPieceBitboards();
    piece.initFunctionPointers();
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
        if(pieceType != '0') matrixBoard[i][j] = pieceType;
        else matrixBoard[i][j] = '.';
    }
}

void Board::copyPosition(std::string& FEN)
{
    uint16_t row{}, col{};
    char pieceType;

    this->piece.enPassant = noSq;
    this->piece.castle[white][kingside]   = false;
    this->piece.castle[white][queenside]  = false;
    this->piece.castle[black][kingside]   = false;
    this->piece.castle[black][queenside]  = false;
    
    for(auto& piece: piece.pieceBitboards) piece.setVal(0ULL);
    size_t i{};

    for (; i<FEN.size(); i++) 
    {
        if (FEN[i] == ' ') break;
        if (FEN[i] == '/')
        {
            row++;
            col = 0;
        }
        else if (isdigit(FEN[i]))
            col += FEN[i] - '0';
        
        else 
        {
            uint16_t indx = 63 - (row * 8 + col);
            pieceType = FEN[i];

            if(pieceType == 'k') piece.kingPosition[black] = indx;
            else if(pieceType == 'K') piece.kingPosition[white] = indx;

            piece.pieceBitboards[charPieces.at(pieceType)].setVal(piece.pieceBitboards[charPieces.at(pieceType)].getVal() | (1ULL << (int)indx));
            col++;
        }
    }
    this->turn = FEN[++i] == 'w'?  white: black;
    i += 2;
    while(FEN[i] != ' '){
        if(FEN[i] == 'K') this->piece.castle[white][kingside]   = true;
        if(FEN[i] == 'Q') this->piece.castle[white][queenside]  = true;
        if(FEN[i] == 'k') this->piece.castle[black][kingside]   = true;
        if(FEN[i] == 'q') this->piece.castle[black][queenside]  = true;
        i++;
    }
    if(FEN[i+1] != '-')
    {
        std::string str = "";
        str += FEN[++i];
        str += FEN[i+1];
        this->piece.enPassant = 63 - coordsToAbsolute.at(str);
    }
    i += 2;
    this->piece.updateCombinedBitboards();
    this->piece.updateUnsafeTiles();
    updateMatrixBoard();
    updateFENViamatrixBoard();
}

void Board::updateFENViamatrixBoard()
{
    std::string str;
    for(size_t i{}; i<8; i++)
    {
        int emptyCol{};
        for(size_t j{}; j<8; j++)
        {
            if(matrixBoard[i][j] != '.')
            {
                if(emptyCol)
                {
                    str += std::to_string(emptyCol);
                    emptyCol = 0;
                }
                str += matrixBoard[i][j];
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

    if(piece.castle[white][kingside] || piece.castle[white][queenside] 
        || piece.castle[black][kingside] || piece.castle[black][queenside]){
        if(piece.castle[white][kingside])  str += "K";
        if(piece.castle[white][queenside]) str += "Q";
        if(piece.castle[black][kingside])  str += "k";
        if(piece.castle[black][queenside]) str += "q";
    }
    else str += "-";
    str += " ";
    if(Globals::player == white && piece.enPassant != noSq)
        str += coordinate[piece.enPassant];
    else if (Globals::player == black && piece.enPassant != noSq) 
        str += coordinate[63 - piece.enPassant];
    else str += "-";
    str += " ";
    str += std::to_string(emptyTurns);
    str += " ";
    if(!totalTurns) str += std::to_string(1);
    else str += std::to_string(totalTurns);
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
            uint16_t setBitIndx(tempPiece.getLSBIndex());
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

size_t Board::makeMove(uint16_t move)
{
    size_t moveType = regular;
    uint16_t srcTile{};
    uint16_t destTile{};
    uint16_t promo{};

    moveDecoder(srcTile, destTile, promo, move);
    char movedType = piece.getPieceType(srcTile);
    char capturedType = piece.getPieceType(destTile);
    piece.enPassant = noSq;
    bool side = getType(movedType); 

    bool isPieceReleasedOnEmptyTile = (capturedType == '0')? true : false;

    // If condtion to handle enpassant related conditions
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
    }
    
    if (toupper(movedType) == 'K') {
        
        int16_t movedSide = -1; // Start with an invalid value
        
        // If king has castled, update the rook position too
        if (destTile < srcTile && abs(srcTile - destTile) == 2)
            movedSide = kingside;  // Kingside castling
        else if (destTile > srcTile && abs(srcTile - destTile) == 2)
            movedSide = queenside; // Queenside castling
        
        if (movedSide != -1) { // Only update if castling happened
            piece.updatePieceBitboards(
                (side)? 'r': 'R',
                Globals::rookInitPos[side][movedSide], 
                Globals::castleRookTargetTiles[side][movedSide]
            );
            moveType = castle;
        }

        piece.kingPosition[side] = destTile; // Update global king position
        // Disable castling rights
        piece.castle[side][kingside] = false;
        piece.castle[side][queenside] = false;
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
    // handle pawn promotion
    if(promo) piece.promotePawn(srcTile, destTile, promo);
   
    piece.updateCombinedBitboards();
    piece.updateUnsafeTiles();
    flipTurn();
    if(!isPieceReleasedOnEmptyTile) moveType = capture;
    if(!piece.isKingSafe(!side)) moveType = check;

    if(isPieceReleasedOnEmptyTile && std::toupper(movedType) != 'P') emptyTurns++;
    totalTurns++;
    // if pawn is moved or piece is captured reset empty moves
    if(std::toupper(movedType) == 'P' || !isPieceReleasedOnEmptyTile) emptyTurns = 0; 
    if(getGameEndState(!side)) moveType = getGameEndState(!side);
    return moveType;
}
