#include "../include/chessboard.hpp"
#include <iostream>

Board::Board():
    turn(white),
    legalMoves(0ULL),
    enPassant(0ULL),
    emptyTurns_(no_sq),
    totalTurns_(0),
    FENString_("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
{
    castle[white][kingside] = castle[white][queenside] = true;
    castle[black][kingside] = castle[black][queenside] = true;

/*  The following bitmask values are set for castling rights for both white and black sides.
    Refer to the getPseudoLegalMoves() function to understand why these specific numbers are assigned. */
    castleBitmasks[white][kingside] = 6ULL;
    castleBitmasks[white][queenside] = 48ULL;
    castleBitmasks[white][both] = 54ULL; 
    castleBitmasks[black][kingside] = 432345564227567616ULL;
    castleBitmasks[black][queenside] = 3458764513820540928ULL;
    castleBitmasks[black][both] = 3891110078048108544ULL;

    bitboards[white] = bitboards[black] = bitboards[both] = 0ULL;
    
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
            tileColor = ((row + col) % 2 == 0) ? Globals::light : Globals::dark; // white tiles will always be on (row + col == even) position
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
    DrawRectangle(0, Globals::topPadding, Globals::leftPadding, Globals::tileSize*8 ,BROWN);
    DrawRectangle(Globals::leftPadding+Globals::tileSize*8, Globals::topPadding, Globals::leftPadding, Globals::tileSize*8 ,BROWN);
    DrawRectangle(0, 0, Globals::tileSize*8+Globals::leftPadding*2, Globals::topPadding,BROWN);
    DrawRectangle(0, Globals::topPadding+Globals::tileSize*8, Globals::tileSize*8+Globals::leftPadding*2, Globals::topPadding,BROWN);
    
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
        bool occupied_tile = isThereAPieceTexture(col, row);
        row *= Globals::tileSize;
        col *= Globals::tileSize;
        row += Globals::leftPadding;
        col += Globals::topPadding;
        row += Globals::tileSize/2;
        col += Globals::tileSize/2;
        Color temp = {70, 70, 70, 100};
        
        if(occupied_tile) 
            DrawRing({static_cast<float>(row), static_cast<float>(col)}, Globals::tileSize/2-7, Globals::tileSize/2, 0, 360, 1000, temp);
        else 
            DrawCircle(row, col, Globals::tileSize/2-35, temp);

        tiles.popBit(x);
    }
}

void Board::drawUpdatedBoard() const
{
    drawTiles();
    drawStrips();
    // draw all the pieces on board
    for (size_t i{}; i < Globals::totalPiece; i++)
        DrawTexture(
            Globals::pieceOnBoard[i].texture, 
            (Globals::pieceOnBoard[i].col * Globals::tileSize)+Globals::leftPadding, 
            (Globals::pieceOnBoard[i].row * Globals::tileSize)+Globals::topPadding, WHITE
        );

    // highlight legal moves if there are any
    highlightTiles(legalMoves);

    // draw all the captured pieces 
    int posX = ((Globals::tileSize*8) + Globals::leftPadding*3);
    size_t itr1{}, itr2{};
    for(size_t i{}; i<Globals::totalCaptured; i++)
    {
        if(Globals::pieceCaptured[i].type < 'Z')
        {
            DrawTexture(Globals::pieceCaptured[i].texture, posX+(Globals::tileSize/3)*itr1, Globals::tileSize*2, WHITE );
            itr1++;
        } 
        else
        { 
            DrawTexture(Globals::pieceCaptured[i].texture, posX+(Globals::tileSize/3)*itr2, Globals::tileSize*8, WHITE );
            itr2++;
        }
    }
    std::string str1 = FENString_.substr(0, FENString_.length()/2);
    std::string str2 = FENString_.substr(FENString_.length()/2);
    DrawText(str1.c_str(), posX, Globals::tileSize*3, 20, RAYWHITE);
    DrawText(str2.c_str(), posX, Globals::tileSize*3+50, 20, RAYWHITE);
}

void Board::print() const
{
    for(size_t i{}; i<8; i++)
    {
        for(size_t j{}; j<8; j++)
        {
            std::cout<<matrixBoard_[i][j];
        }
        std::cout<<"\n";
    }
    std::cout<<FENString_<<"\n";
}

void Board::flipTurn()
{
    (turn == white)?turn = black: turn = white;
}

void Board::updateMatrixBoard() 
{
    for(size_t i{}; i<8; i++){
        for(size_t j{}; j<8; j++){
            PieceUI *currPiece = isThereAPieceTexture(i, j);
            if(currPiece)
                matrixBoard_[i][j] = currPiece->type;
            else matrixBoard_[i][j] = '.';
        }
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
    if(castle[white][kingside] || castle[white][queenside] || castle[black][kingside] || castle[black][queenside])
    {
        if(castle[white][kingside]) str += "K";
        if(castle[white][queenside]) str += "Q";
        if(castle[black][kingside]) str += "k";
        if(castle[black][queenside]) str += "q";
    }
    else str += "-";
    str+=" ";
    if(enPassant != no_sq) str += coordinate[enPassant];
    else str += "-";
    str+=" ";
    str+= std::to_string(emptyTurns_);
    str += " ";
    str += std::to_string(totalTurns_);
    FENString_ = str;
}

void Board::syncBitboards(BitBoard *pieceBitboards)
{
    bitboards[white] = bitboards[black] = 0ULL;
    for(size_t i{}; i<12; i++)
    {
        if(i<6) bitboards[white] |= pieceBitboards[i].getVal();
        else  bitboards[black] |= pieceBitboards[i].getVal();
    }
    bitboards[both] =  bitboards[white] |  bitboards[black];
}

/*
=======================================================================================================================================================================================================
                    Function to Make move: this function updates all the logical bitboards and also the visual board
=======================================================================================================================================================================================================
*/
void Board::makeMove(PieceUI *currPiece, int releasedOnTileRow, int releasedOnTileCol, BitBoard *pieceBitboards, size_t *kingPosition){
    unsigned int sourceTile = 63-(currPiece->row * 8 + currPiece->col),
                  destTile = 63-(releasedOnTileRow * 8 + releasedOnTileCol);

    PieceUI *releasedOnPiece = isThereAPieceTexture(releasedOnTileRow,releasedOnTileCol); 
    
    bool isPieceReleasedOnEmptyTile = (!releasedOnPiece && (currPiece->row != releasedOnTileRow || currPiece->col != releasedOnTileCol))? true : false;

    // If condtion to handle en passant related conditions
    if(currPiece->type == 'P' || currPiece->type == 'p'){

        // check if current move activates any enpassant on the board
        if(abs(sourceTile - destTile) == 16)
            (currPiece->type == 'P')? enPassant = 63 - (destTile-8): enPassant = 63 - (destTile + 8);

        // check if this is an enpassant capture
        else if((abs(sourceTile - destTile) == 7 || abs(sourceTile - destTile) == 9) && isPieceReleasedOnEmptyTile){
            PieceUI *captured_pawn = nullptr;

            if(currPiece->type == 'P') {
                captured_pawn = isThereAPieceTexture(((63-destTile)+8)/8, ((63-destTile)+8)%8);
                pieceBitboards[Globals::charPieces.at(captured_pawn->type)].popBit(destTile-8);
            }
            else {
                captured_pawn = isThereAPieceTexture(((63-destTile)-8)/8, ((63-destTile)-8)%8);
                pieceBitboards[Globals::charPieces.at(captured_pawn->type)].popBit(destTile+8);
            }

            Globals::sound.playCapture();
            
            // remove the pawn capture via enpassant
            deletePiece(captured_pawn);
        }

        //conditions to handle pawn promotion
        if(destTile <= 7 || destTile >= 56){
            PieceUI *promoted_pawn = isThereAPieceTexture((63-sourceTile)/8, (63-sourceTile)%8);
            pieceBitboards[Globals::charPieces.at(promoted_pawn->type)].popBit(sourceTile);
            if(promoted_pawn->type >= 'a'){
                pieceBitboards['q'].setBit(destTile);
                promoted_pawn->type = 'q';
                Image image = LoadImage("../../Assets/images/blackQueen.png");
                ImageResize(&image, Globals::tileSize, Globals::tileSize);
                promoted_pawn->texture = LoadTextureFromImage(image); // Load texture in graphics VRAM
                UnloadImage(image);
            } 
            else{
                pieceBitboards['Q'].setBit(destTile);
                promoted_pawn->type = 'Q';
                Image image = LoadImage("../../Assets/images/whiteQueen.png");
                ImageResize(&image, Globals::tileSize, Globals::tileSize);
                promoted_pawn->texture = LoadTextureFromImage(image); // Load texture in graphics VRAM
                UnloadImage(image);
            }   
        }
    }
    

    // below if else conditions are for checking if the king or rooks have moved to update the casteling state ont he board
    // if white king is moved
    if(currPiece->type == 'K'){
    
            kingPosition[white] = destTile; //update the global white king position
            if(sourceTile == 3 && destTile == 1){
                PieceUI* rook = isThereAPieceTexture(7,7);
                pieceBitboards[Globals::charPieces.at(rook->type)].popBit(63-(rook->row*8+rook->col)); // eraset the old position of this rook in bitboards
                rook->col = 5;
                pieceBitboards[Globals::charPieces.at(rook->type)].setBit(63-(rook->row*8+rook->col)); // update the new position of this rook in bitboards
                Globals::sound.playCastle();
            }
            else if(sourceTile == 3 && destTile == 5){
                PieceUI* rook = isThereAPieceTexture(7,0);
                pieceBitboards[Globals::charPieces.at(rook->type)].popBit(63-(rook->row*8+rook->col)); // eraset the old position of this rook in bitboards
                rook->col = 3;
                pieceBitboards[Globals::charPieces.at(rook->type)].setBit(63-(rook->row*8+rook->col)); // update the new position of this rook in bitboards
                Globals::sound.playCastle();
            }

            if(castle[white][kingside] || castle[white][queenside]){
                castle[white][kingside] = false;
                castle[white][queenside] = false;
                std::cout<<"White king can't castle now\n";
            }
        }

        // if black king is moved
        else if(currPiece->type == 'k'){

            kingPosition[black] =  destTile; //update the global black king positions

            if(sourceTile == 59 && destTile == 57){
                PieceUI* rook = isThereAPieceTexture(0,7);
                pieceBitboards[Globals::charPieces.at(rook->type)].popBit(63-(rook->row*8+rook->col)); // eraset the old position of this rook in bitboards
                rook->col = 5;
                pieceBitboards[Globals::charPieces.at(rook->type)].setBit(63-(rook->row*8+rook->col)); // update the new position of this rook in bitboards
                Globals::sound.playCastle();
            }
            else if(sourceTile == 59 && destTile == 61){
                PieceUI* rook = isThereAPieceTexture(0,0);
                pieceBitboards[Globals::charPieces.at(rook->type)].popBit(63-(rook->row*8+rook->col)); // eraset the old position of this rook in bitboards
                rook->col = 3;
                pieceBitboards[Globals::charPieces.at(rook->type)].setBit(63-(rook->row*8+rook->col)); // update the new position of this rook in bitboards
                Globals::sound.playCastle();
            }

            if(castle[black][kingside] || castle[black][queenside]){
                castle[black][kingside] = false;
                castle[black][queenside] = false;
                std::cout<<"Black king can't castle now\n";
            }
        }
        if((castle[white][kingside] || castle[white][queenside]) && currPiece->type == 'R'){
            if(currPiece->row == 7 && currPiece->col == 7){
                castle[white][kingside] = false;
                std::cout<<"White king can't castle king side now\n";
            }
            else if (currPiece->row == 7 && currPiece->col == 0){
                castle[white][queenside] = false;
                std::cout<<"White king can't castle queen side now\n";
            }
        }
        else if((castle[black][kingside] || castle[black][queenside]) && currPiece->type == 'r'){
            if(currPiece->row == 0 && currPiece->col == 7){
                castle[black][kingside] = false;
                std::cout<<"black king can't castle king side now\n";
            }
            else if (currPiece->row == 0 && currPiece->col == 0){
                castle[black][queenside] = false;
                std::cout<<"black king can't castle queen side now\n";
            }
        }

        pieceBitboards[Globals::charPieces.at(currPiece->type)].popBit(sourceTile);
        pieceBitboards[Globals::charPieces.at(currPiece->type)].setBit(destTile);

        currPiece->row = releasedOnTileRow;
        currPiece->col = releasedOnTileCol;


        // if enemy piece is captured then put the captured piece at the end of the array and decrease the size till which updateBoard have acess 
        if(!isPieceReleasedOnEmptyTile){
            Globals::sound.playCapture();

            // if piece is release on enemy tile pop that enemy piece from the bitboards
            pieceBitboards[Globals::charPieces.at(releasedOnPiece->type)].popBit(destTile);

            //std::cout<<"captured piece: "<<releasedOnPiece->type<<std::endl;

            // swap the captured piece from the last piece in the pieceui array and reduce the size basically 
            // like deleting the captured piece textures updating the casteling state

            // below if else conditions are for checking if the rooks have been captured to update the casteling state ont he board
            if((castle[white][kingside] || castle[white][queenside]) && releasedOnPiece->type == 'R'){
                if(releasedOnPiece->row == 7 && releasedOnPiece->col == 7){
                    castle[white][kingside] = false;
                    std::cout<<"White king can't castle king side now\n";
                }
                else if (releasedOnPiece->row == 7 && releasedOnPiece->col == 0){  
                    castle[white][queenside] = false;
                    std::cout<<"White king can't castle queen side now\n";
                }
            }
            else if((castle[black][kingside] || castle[black][queenside]) && releasedOnPiece->type == 'r'){
                if(releasedOnPiece->row == 0 && releasedOnPiece->col == 7)  {
                    castle[black][kingside] = false;
                    std::cout<<"black king can't castle king side now\n";
                }
                else if (releasedOnPiece->row == 0 && releasedOnPiece->col == 0)  {
                    castle[black][queenside] = false;
                    std::cout<<"black king can't castle queen side now\n";
                }
            }

            deletePiece(releasedOnPiece);
            emptyTurns_ = 0;
        }
        else if(isPieceReleasedOnEmptyTile){
            Globals::sound.playDefault();
            if(std::toupper(currPiece->type) != 'P')emptyTurns_++;
        }
        totalTurns_++;
        if(std::toupper(currPiece->type) == 'P' || !isPieceReleasedOnEmptyTile) emptyTurns_ = 0; // if pawn is moved or piece is captured reset empty moves

        syncBitboards(pieceBitboards);
        updateMatrixBoard();
        updateFENViamatrixBoard();
        std::cout<<FENString_<<"\n";
}