#include "../include/chessboard.hpp"
#include "../include/bitboard.hpp"
#include "../include/helper.hpp"
#include <iostream>

Board::Board(){
    turn = white;
    legal_moves.set_val(0ULL);
    castle[white][kingside] = castle[white][queenside] = true;
    castle[black][kingside] = castle[black][queenside] = true;

/*  The following bitmask values are set for castling rights for both white and black sides.
    Refer to the get_pseudo_legal_move() function to understand why these specific numbers are assigned. */
    castle_bitmasks[white][kingside] = 6ULL;
    castle_bitmasks[white][queenside] = 48ULL;
    castle_bitmasks[white][both] = 54ULL; 
    castle_bitmasks[black][kingside] = 432345564227567616ULL;
    castle_bitmasks[black][queenside] = 3458764513820540928ULL;
    castle_bitmasks[black][both] = 3891110078048108544ULL;
    
    bitboards[white] = bitboards[black] = bitboards[both] = 0ULL;
    FEN_string = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    this->update_matrix_board();
    en_passant = -1;
    empty_turns = 0;
    total_turns = 0;
    bounds[left] = leftPadding;
    bounds[right] = tileSize * 8 + leftPadding;
    bounds[top] = topPadding;
    bounds[bottom] = tileSize * 8 + topPadding;
}

/*
=======================================================================================================================================================================================================
                    Below are all the function which are used to draw board related stuff
=======================================================================================================================================================================================================
*/
void Board::draw_tiles(){
    Color tileColor;
    for (int row{}; row < 8; row++)
    {
        for (int col{}; col < 8; col++)
        {
            tileColor = ((row + col) % 2 == 0) ? light : dark; // white tiles will always be on (row + col == even) position
            DrawRectangle((tileSize * col)+leftPadding,( tileSize * row)+topPadding, tileSize, tileSize, tileColor);
        }
    }
}

void Board::draw_strips(){
    DrawRectangle(0, topPadding, leftPadding, tileSize*8 ,BROWN);
    DrawRectangle(leftPadding+tileSize*8, topPadding, leftPadding, tileSize*8 ,BROWN);
    DrawRectangle(0, 0, tileSize*8+leftPadding*2, topPadding,BROWN);
    DrawRectangle(0, topPadding+tileSize*8, tileSize*8+leftPadding*2, topPadding,BROWN);
    
    int itr{};
    int rank, flag, offset;
    char file;

    if(black){
        rank = -8;
        flag = 0;
        offset = 1;
        file = 'a';
    }
    else{
        rank = 1;
        flag = 9;
        offset = -1;
        file = 'h';
    }
    
    int posX = leftPadding/2;
    int posY = topPadding + tileSize/2;
    // draw the rank number strip
    for(; rank != flag; rank++){
        std::string str = std::to_string(abs(rank));
        DrawText(str.c_str(), posX, posY,20, WHITE);
        posY += tileSize;
    }

    // draw the file number strip like a, b, c etc
    posY = tileSize * 8 + topPadding + 8;
    posX = leftPadding + tileSize/2;
    for(itr = 0; itr<8; itr++){
        std::string str = std::string(1, file);
        DrawText(str.c_str(), posX, posY,20, WHITE);
        DrawText(str.c_str(), posX, 10, 20, WHITE);
        posX += tileSize;
        file += offset;
    }
}

void Board::highlight_tiles(BitBoard tiles){
    while(tiles.val){
        int x = tiles.get_lsb_index();
        int row = (63 - x) % 8;
        int col = (63 - x) / 8;
        bool occupied_tile = isThereA_PieceUI(col, row);
        row *= tileSize;
        col *= tileSize;
        row += leftPadding;
        col += topPadding;
        row += tileSize/2;
        col += tileSize/2;
        Color temp = {70, 70, 70, 100};
        
        if(occupied_tile) DrawRing({(float)row, (float)col}, 
                tileSize/2-7, tileSize/2, 0, 360, 1000, temp);

        else DrawCircle(row, col, tileSize/2-35, temp);
        tiles.pop_bit(x);
    }
}

void Board::draw_updated_board(){

    draw_tiles();
    draw_strips();
    // draw all the pieces on board
    for (int i{}; i < totalPiece; i++)
        DrawTexture(pieceOnBoard[i].texture, (pieceOnBoard[i].col * tileSize)+leftPadding, (pieceOnBoard[i].row * tileSize)+topPadding, WHITE);

    // highlight legal moves if there are any
    highlight_tiles(legal_moves);

    // draw all the captured pieces 
    int posX = ((tileSize*8) + leftPadding*3);
    int itr1{}, itr2{};
    for(int i{}; i<totalCaptured; i++){
        if(PieceCaptured[i].type < 'Z'){
            DrawTexture(PieceCaptured[i].texture, posX+(tileSize/3)*itr1, tileSize*2, WHITE );
            itr1++;
        } 
        else{ 
            DrawTexture(PieceCaptured[i].texture, posX+(tileSize/3)*itr2, tileSize*8, WHITE );
            itr2++;
        }
    }
    std::string str1 = FEN_string.substr(0, FEN_string.length()/2);
    std::string str2 = FEN_string.substr(FEN_string.length()/2);
    DrawText(str1.c_str(), posX, tileSize*3, 20, RAYWHITE);
    DrawText(str2.c_str(), posX, tileSize*3+50, 20, RAYWHITE);
}

void Board::update_matrix_board(){
    for(int i{}; i<8; i++){
        for(int j{}; j<8; j++){
            PieceUI *currPiece = isThereA_PieceUI(i, j);
            if(currPiece)
                matrix_board[i][j] = currPiece->type;
            else matrix_board[i][j] = '.';
        }
    }
}
void Board::print(){
    for(int i{}; i<8; i++){
        for(int j{}; j<8; j++){
            std::cout<<matrix_board[i][j];
        }
        std::cout<<"\n";
    }
    std::cout<<FEN_string<<"\n";
}

void Board::sync_bitboards(BitBoard *piece_set){
    bitboards[white] = bitboards[black] = 0ULL;
    for(int i{}; i<12; i++){
        if(i<6) bitboards[white] |= piece_set[i].val;
        else  bitboards[black] |= piece_set[i].val;
    }
    bitboards[both] =  bitboards[white] |  bitboards[black];
}

void Board::flip_turn(){
    (turn == white)?turn = black: turn = white;
}

/*
=======================================================================================================================================================================================================
                    Function Uses the matrix board representation and convert it into a FEN notation string
=======================================================================================================================================================================================================
*/
void Board::matrix_to_FEN(){
    std::string str;
    for(int i{}; i<8; i++){
        int emptyCol{};
        for(int j{}; j<8; j++){
            if(matrix_board[i][j] != '.'){
                if(emptyCol){
                    str += std::to_string(emptyCol);
                    emptyCol = 0;
                }
                str += matrix_board[i][j];
            }
            else{
                emptyCol++;
            }
        }
        if(emptyCol){
            str += std::to_string(emptyCol);
            emptyCol = 0;
        }
        if(i<7)str+='/';
    }
    str += (turn)?" w":" b"; // Add turn
    str+=" ";
    if(castle[white][kingside] || castle[white][queenside] || castle[black][kingside] || castle[black][queenside]){
        if(castle[white][kingside]) str += "K";
        if(castle[white][queenside]) str += "Q";
        if(castle[black][kingside]) str += "k";
        if(castle[black][queenside]) str += "q";
    }
    else str += "-";
    str+=" ";
    if(en_passant != -1) str += coordinate[en_passant];
    else str += "-";
    str+=" ";
    str+= std::to_string(empty_turns);
    str += " ";
    str += std::to_string(total_turns);
    FEN_string = str;
}

/*
=======================================================================================================================================================================================================
                    Function to Make move: this function updates all the logical bitboards and also the visual board
=======================================================================================================================================================================================================
*/
void Board::make_move(PieceUI *currPiece, int releasedOnTileRow, int releasedOnTileCol, BitBoard *piece_set, int *kingPosition){
    unsigned int source_tile = 63-(currPiece->row * 8 + currPiece->col),
                  destination_tile = 63-(releasedOnTileRow * 8 + releasedOnTileCol);

    PieceUI *releasedOnPiece = isThereA_PieceUI(releasedOnTileRow,releasedOnTileCol); 
    
    bool isPieceReleasedOnEmptyTile = (!releasedOnPiece && (currPiece->row != releasedOnTileRow || currPiece->col != releasedOnTileCol))? true : false;

    // If condtion to handle en passant related conditions
    if(currPiece->type == 'P' || currPiece->type == 'p'){

        // check if current move activates any enpassant on the board
        if(abs(source_tile - destination_tile) == 16)
            (currPiece->type == 'P')? en_passant = 63 - (destination_tile-8): en_passant = 63 - (destination_tile + 8);

        // check if this is an enpassant capture
        else if((abs(source_tile - destination_tile) == 7 || abs(source_tile - destination_tile) == 9) && isPieceReleasedOnEmptyTile){
            PieceUI *captured_pawn = nullptr;

            if(currPiece->type == 'P') {
                captured_pawn = isThereA_PieceUI(((63-destination_tile)+8)/8, ((63-destination_tile)+8)%8);
                piece_set[char_pieces.at(captured_pawn->type)].pop_bit(destination_tile-8);
            }
            else {
                captured_pawn = isThereA_PieceUI(((63-destination_tile)-8)/8, ((63-destination_tile)-8)%8);
                piece_set[char_pieces.at(captured_pawn->type)].pop_bit(destination_tile+8);
            }

            sound.playCapture();
            
            // remove the pawn capture via enpassant
            deletePiece(captured_pawn);
        }

        //conditions to handle pawn promotion
        if(destination_tile <= 7 || destination_tile >= 56){
            PieceUI *promoted_pawn = isThereA_PieceUI((63-source_tile)/8, (63-source_tile)%8);
            piece_set[char_pieces.at(promoted_pawn->type)].pop_bit(source_tile);
            if(promoted_pawn->type >= 'a'){
                piece_set['q'].set_bit(destination_tile);
                promoted_pawn->type = 'q';
                Image image = LoadImage("../../Assets/images/blackQueen.png");
                ImageResize(&image, tileSize, tileSize);
                promoted_pawn->texture = LoadTextureFromImage(image); // Load texture in graphics VRAM
                UnloadImage(image);
            } 
            else{
                piece_set['Q'].set_bit(destination_tile);
                promoted_pawn->type = 'Q';
                Image image = LoadImage("../../Assets/images/whiteQueen.png");
                ImageResize(&image, tileSize, tileSize);
                promoted_pawn->texture = LoadTextureFromImage(image); // Load texture in graphics VRAM
                UnloadImage(image);
            }   
        }
    }
    

    // below if else conditions are for checking if the king or rooks have moved to update the casteling state ont he board
    // if white king is moved
    if(currPiece->type == 'K'){
    
            kingPosition[white] = destination_tile; //update the global white king position
            if(source_tile == 3 && destination_tile == 1){
                PieceUI* rook = isThereA_PieceUI(7,7);
                piece_set[char_pieces.at(rook->type)].pop_bit(63-(rook->row*8+rook->col)); // eraset the old position of this rook in bitboards
                rook->col = 5;
                piece_set[char_pieces.at(rook->type)].set_bit(63-(rook->row*8+rook->col)); // update the new position of this rook in bitboards
                sound.playCastle();
            }
            else if(source_tile == 3 && destination_tile == 5){
                PieceUI* rook = isThereA_PieceUI(7,0);
                piece_set[char_pieces.at(rook->type)].pop_bit(63-(rook->row*8+rook->col)); // eraset the old position of this rook in bitboards
                rook->col = 3;
                piece_set[char_pieces.at(rook->type)].set_bit(63-(rook->row*8+rook->col)); // update the new position of this rook in bitboards
                sound.playCastle();
            }

            if(castle[white][kingside] || castle[white][queenside]){
                castle[white][kingside] = false;
                castle[white][queenside] = false;
                std::cout<<"White king can't castle now\n";
            }
        }

        // if black king is moved
        else if(currPiece->type == 'k'){

            kingPosition[black] =  destination_tile; //update the global black king positions

            if(source_tile == 59 && destination_tile == 57){
                PieceUI* rook = isThereA_PieceUI(0,7);
                piece_set[char_pieces.at(rook->type)].pop_bit(63-(rook->row*8+rook->col)); // eraset the old position of this rook in bitboards
                rook->col = 5;
                piece_set[char_pieces.at(rook->type)].set_bit(63-(rook->row*8+rook->col)); // update the new position of this rook in bitboards
                sound.playCastle();
            }
            else if(source_tile == 59 && destination_tile == 61){
                PieceUI* rook = isThereA_PieceUI(0,0);
                piece_set[char_pieces.at(rook->type)].pop_bit(63-(rook->row*8+rook->col)); // eraset the old position of this rook in bitboards
                rook->col = 3;
                piece_set[char_pieces.at(rook->type)].set_bit(63-(rook->row*8+rook->col)); // update the new position of this rook in bitboards
                sound.playCastle();
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

        piece_set[char_pieces.at(currPiece->type)].pop_bit(source_tile);
        piece_set[char_pieces.at(currPiece->type)].set_bit(destination_tile);

        currPiece->row = releasedOnTileRow;
        currPiece->col = releasedOnTileCol;


        // if enemy piece is captured then put the captured piece at the end of the array and decrease the size till which updateBoard have acess 
        if(!isPieceReleasedOnEmptyTile){
            sound.playCapture();

            // if piece is release on enemy tile pop that enemy piece from the bitboards
            piece_set[char_pieces.at(releasedOnPiece->type)].pop_bit(destination_tile);

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
            empty_turns = 0;
        }
        else if(isPieceReleasedOnEmptyTile){
            sound.playDefault();
            if(std::toupper(currPiece->type) != 'P')empty_turns++;
        }
        total_turns++;
        if(std::toupper(currPiece->type) == 'P' || !isPieceReleasedOnEmptyTile) empty_turns = 0; // if pawn is moved or piece is captured reset empty moves

        sync_bitboards(piece_set);
        update_matrix_board();
        matrix_to_FEN();
        std::cout<<FEN_string<<"\n";
}