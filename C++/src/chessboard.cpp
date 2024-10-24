#include "../include/chessboard.hpp"
#include "../include/helper.hpp"
#include <iostream>
#include <vector>

Board::Board(){
    turn = white;
    castle[wk] = castle[wq] = castle[bk] = castle[bq] = true;
    bitboards[white].val = bitboards[black].val = bitboards[both].val = 0ULL;
    FEN_string = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    this->update_matrix_board();
    en_passant = -1;
    empty_moves = 0;
    total_moves = 0;
    bounds[left] = leftPadding;
    bounds[right] = tileSize * 8 + leftPadding;
    bounds[top] = topPadding;
    bounds[bottom] = tileSize * 8 + topPadding;
}

void Board::draw()
{
    DrawRectangle(0, topPadding, leftPadding, tileSize*8 ,BROWN);
    DrawRectangle(leftPadding+tileSize*8, topPadding, leftPadding, tileSize*8 ,BROWN);
    DrawRectangle(0, 0, tileSize*8+leftPadding*2, topPadding,BROWN);
    DrawRectangle(0, topPadding+tileSize*8, tileSize*8+leftPadding*2, topPadding,BROWN);

    Color tileColor;
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
        DrawText(str.c_str(), posX, 0 + topPadding/2,20, WHITE);
        posX += tileSize;
        file += offset;
    }

    // draw all the tiles
    for (int row{}; row < 8; row++)
    {
        for (int col{}; col < 8; col++)
        {
            tileColor = ((row + col) % 2 == 0) ? light : dark; // white tiles will always be on (row + col == even) position
            DrawRectangle((tileSize * col)+leftPadding,( tileSize * row)+topPadding, tileSize, tileSize, tileColor);
        }
    }
    for (int i{}; i < totalPiece; i++)
        DrawTexture(pieceTextures[i].texture, (pieceTextures[i].col * tileSize)+leftPadding, (pieceTextures[i].row * tileSize)+topPadding, WHITE);
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
    /*
    bool empty;
    for(int tile = 63; tile>=0; tile--){ 
        empty = true;  
        for(int i = P; i<=k; i++){
            if(piece_set[i].get_bit(tile)){
                std::cout<<ascii_pieces[i]<<" ";
                empty = false;
            }
        }
        if(empty) std::cout<<". ";
        if(tile % 8 == 0) std::cout<<"\n";
    }
    */
}

void Board::sync_bitboards(BitBoard *piece_set){
    bitboards[white].val = bitboards[black].val = 0ULL;
    for(int i{}; i<12; i++){
        if(i<6) bitboards[white].val |= piece_set[i].val;
        else  bitboards[black].val |= piece_set[i].val;
    }
    bitboards[both].val =  bitboards[white].val |  bitboards[black].val;
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
    if(castle[wk] || castle[wq] || castle[bk] || castle[bq]){
        if(castle[wk]) str += "K";
        if(castle[wq]) str += "Q";
        if(castle[bk]) str += "k";
        if(castle[bq]) str += "q";
    }
    else str += "-";
    str+=" ";
    if(en_passant != -1) str += coordinate[en_passant];
    else str += "-";
    str+=" ";
    str+= std::to_string(empty_moves);
    str += " ";
    str += std::to_string(total_moves);
    FEN_string = str;
}

/*
=======================================================================================================================================================================================================
                    This function takes array of tiles and highlights them 
=======================================================================================================================================================================================================
*/
void Board::highlight_tiles(const std::vector<int> &arr){
    for(auto &x :arr){
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
    }
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

            if(castle[wk] || castle[wq]){
                castle[wk] = false;
                castle[wq] = false;
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

            if(castle[bk] || castle[bq]){
                castle[bk] = false;
                castle[bq] = false;
                std::cout<<"Black king can't castle now\n";
            }
        }
        if((castle[wk] || castle[wq]) && currPiece->type == 'R'){
            if(currPiece->row == 7 && currPiece->col == 7){
                castle[wk] = false;
                std::cout<<"White king can't castle king side now\n";
            }
            else if (currPiece->row == 7 && currPiece->col == 0){
                castle[wq] = false;
                std::cout<<"White king can't castle queen side now\n";
            }
        }
        else if((castle[bk] || castle[bq]) && currPiece->type == 'r'){
            if(currPiece->row == 0 && currPiece->col == 7){
                castle[bk] = false;
                std::cout<<"black king can't castle king side now\n";
            }
            else if (currPiece->row == 0 && currPiece->col == 0){
                castle[bq] = false;
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
            if((castle[wk] || castle[wq]) && releasedOnPiece->type == 'R'){
                if(releasedOnPiece->row == 7 && releasedOnPiece->col == 7){
                    castle[wk] = false;
                    std::cout<<"White king can't castle king side now\n";
                }
                else if (releasedOnPiece->row == 7 && releasedOnPiece->col == 0){  
                    castle[wq] = false;
                    std::cout<<"White king can't castle queen side now\n";
                }
            }
            else if((castle[bk] || castle[bq]) && releasedOnPiece->type == 'r'){
                if(releasedOnPiece->row == 0 && releasedOnPiece->col == 7)  {
                    castle[bk] = false;
                    std::cout<<"black king can't castle king side now\n";
                }
                else if (releasedOnPiece->row == 0 && releasedOnPiece->col == 0)  {
                    castle[bq] = false;
                    std::cout<<"black king can't castle queen side now\n";
                }
            }

            deletePiece(releasedOnPiece);
            empty_moves = 0;
        }
        else if(isPieceReleasedOnEmptyTile){
            sound.playDefault();
            if(std::toupper(currPiece->type) != 'P')empty_moves++;
        }
        total_moves++;
        if(std::toupper(currPiece->type) == 'P' || !isPieceReleasedOnEmptyTile) empty_moves = 0; // if pawn is moved or piece is captured reset empty moves

        sync_bitboards(piece_set);
        update_matrix_board();
        matrix_to_FEN();
        std::cout<<FEN_string<<"\n";
}