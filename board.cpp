#include <iostream>
#include <string>
#include <cstdint>

#include "board.hpp"

using namespace std;

//Initialize the board in its starting position
void Board :: board_initialize(){
    //starting positions of white pieces
    piece[0] =  0x00FF000000000000; //pawn
    piece[1] = 0x4200000000000000;  //knight
    piece[2] = 0x2400000000000000;  //bishop
    piece[3] = 0x8100000000000000;  //rook
    piece[4] = 0x0800000000000000;  //queen
    piece[5] = 0x1000000000000000;  //king

    //starting positions of black pieces
    piece[6] = 0x000000000000FF00;  //pawn
    piece[7] = 0x0000000000000042;  //knight
    piece[8] = 0x0000000000000024;  //bishop
    piece[9] = 0x0000000000000000000000000000000081;    //rook
    piece[10] = 0x0000000000000000000000000000000008;   //queen
    piece[11] = 0x0000000000000000000000000000000010;   //king

    white_to_move = true;
    wcastle_kingside = true;
    wcastle_queenside = true;
    bcastle_kingside = true;
    bcastle_queenside = true;
    en_passant_square = -1;
}

//Prints the board to the terminal
void Board :: print() {
    cout << endl;
    
    const char* piece_symbols = ".PNBRQKpnbrqk";

    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            int square = (rank * 8) + file;
            int piece_type = 0;

            // Check if there's a piece on this square
            for(int i = 0; i < 12; i++){
                if(piece[i] & (1ULL << square)){
                    piece_type = i + 1;
                    break;
                }
            }

            // Print the piece symbol or a dot if the square is empty
            cout << piece_symbols[piece_type] << " ";
        }
        cout << endl;
    }

    cout << "To move: " << ( white_to_move ? "white" : "black") << endl;
}

//Moves the board
void Board :: move(uint16_t move){
    // Extract the "from" square
    int from = move & 0b111111;
    
    // Extract the "to" square
    int to = (move >> 6) & 0b111111;
    
    // Extract the type of move
    int type = (move >> 12) & 0b1111;

    // Store the ep square before setting it back to -1
    en_passant_square = -1;

    // Find what piece is being moved
    int piece_type = -1;
    for (int i = 0; i < 12; i++) {
        if (piece[i] & (1ULL << from)) {
            piece_type = i;
            break;
        }
    }
    
    if(piece_type == -1){
        //the from is empty and it is an INVALID move
        return;
    }

    // we move the moving peace to the "to" location
    piece[piece_type] &= ~(1ULL << from);
    piece[piece_type] |= (1ULL << to);

    switch(type){
        case 0:     // quiet move
            if(piece_type == 3 || piece_type == 9){
                if(from == 0){
                    bcastle_queenside = false;
                }

                if(from == 7){
                    bcastle_kingside = false;
                }

                if(from == 56){
                    wcastle_queenside = false;
                }

                if(from == 63){
                    wcastle_kingside = false;
                }
            }

            if(piece_type == 5){
                wcastle_kingside = false;
                wcastle_queenside = false;
            }

            if(piece_type == 11){
                bcastle_kingside = false;
                bcastle_queenside = false;
            }

            break;
        case 1:     // double pawn push

            if(white_to_move){
                en_passant_square = from - 8;  
            }else{
                en_passant_square = from + 8;
            }
            
            break;
        case 2:     // king-side castle

            if(piece_type == 5){
                //white king to castle
                //piece[3] = white rooks
                piece[3] &= ~(1ULL << 63);
                piece[3] |= (1ULL << 61);

                wcastle_kingside = false;
                wcastle_queenside = false;
            }else if (piece_type == 11){
                //black king to castle
                //piece[9] = black rooks
                piece[9] &= ~(1ULL << 7);
                piece[9] |= (1ULL << 5);

                bcastle_kingside = false;
                bcastle_queenside = false;
            }

            break;
        case 3:     // queen-side castle

            if(piece_type == 5){
                //white king to castle
                //piece[3] = white rooks
                piece[3] &= ~(1ULL << 56);
                piece[3] |= (1ULL << 59);

                wcastle_kingside = false;
                wcastle_queenside = false;
            }else if (piece_type == 11){
                //black king to castle
                //piece[9] = black rooks
                piece[9] &= ~(1ULL);
                piece[9] |= (1ULL << 3);

                bcastle_kingside = false;
                bcastle_queenside = false;
            }

            break;
        case 4:     // captures
            
            for(int i = 0; i < 12; i++){
                if(piece[i] & (1ULL << to) && i != piece_type){
                    piece[i] &= ~(1ULL << to);
                    break;
                }
            }

            if(piece_type == 3 || piece_type == 9){
                if(from == 0){
                    bcastle_queenside = false;
                }

                if(from == 7){
                    bcastle_kingside = false;
                }

                if(from == 56){
                    wcastle_queenside = false;
                }

                if(from == 63){
                    wcastle_kingside = false;
                }
            }

            if(piece_type == 5){
                wcastle_kingside = false;
                wcastle_queenside = false;
            }

            if(piece_type == 11){
                bcastle_kingside = false;
                bcastle_queenside = false;
            }

            break;
        case 5:     // en passant
            
            if(piece_type == 0){    // white to capture black
                piece[6] &= ~(1ULL << (to + 8));
            }else if (piece_type == 6){     // black to capture white
                piece[0] &= ~(1ULL << (to - 8));
            }

            break;
        case 8:     // knight-promotion

            piece[piece_type] &= ~(1ULL << to);
            if(piece_type == 0){            //white to promote
                piece[1] |= (1ULL << to);   // add a white knight
            }else if(piece_type == 6){      // black to move
                piece[7] |= (1ULL << to);   // add a black knight
            }

            break;
        case 9:     // bishop-promotion
            
            piece[piece_type] &= ~(1ULL << to);
            if(piece_type == 0){            //white to promote
                piece[2] |= (1ULL << to);   // add a white bishop
            }else if(piece_type == 6){      // black to move
                piece[8] |= (1ULL << to);   // add a black bishop
            }

            break;
        case 10:    // rook-promotion
            
            piece[piece_type] &= ~(1ULL << to);
            if(piece_type == 0){            //white to promote
                piece[3] |= (1ULL << to);   // add a white rook
            }else if(piece_type == 6){      // black to move
                piece[9] |= (1ULL << to);   // add a black rook
            }

            break;
        case 11:    // queen-promotion
            
            piece[piece_type] &= ~(1ULL << to);
            if(piece_type == 0){            //white to promote
                piece[4] |= (1ULL << to);   // add a white queen
            }else if(piece_type == 6){      // black to move
                piece[10] |= (1ULL << to);   // add a black queen
            }

            break;
        case 12:    // knight-promotion capture
            
            for(int i = 0; i < 12; i++){
                if(piece[i] & (1ULL << to) && i != piece_type){
                    piece[i] &= ~(1ULL << to);
                    break;
                }
            }
            piece[piece_type] &= ~(1ULL << to);
            if(piece_type == 0){            //white to promote
                piece[1] |= (1ULL << to);   // add a white knight
            }else if(piece_type == 6){      // black to move
                piece[7] |= (1ULL << to);   // add a black knight
            }

            break;
        case 13:    // bishop-promotion capture
            
            for(int i = 0; i < 12; i++){
                if(piece[i] & (1ULL << to) && i != piece_type){
                    piece[i] &= ~(1ULL << to);
                    break;
                }
            }
            piece[piece_type] &= ~(1ULL << to);
            if(piece_type == 0){            //white to promote
                piece[2] |= (1ULL << to);   // add a white bishop
            }else if(piece_type == 6){      // black to move
                piece[8] |= (1ULL << to);   // add a black bishop
            }

            break;
        case 14:    // rook-promotion capture

            for(int i = 0; i < 12; i++){
                if(piece[i] & (1ULL << to) && i != piece_type){
                    piece[i] &= ~(1ULL << to);
                    break;
                }
            }
            piece[piece_type] &= ~(1ULL << to);
            if(piece_type == 0){            //white to promote
                piece[3] |= (1ULL << to);   // add a white rook
            }else if(piece_type == 6){      // black to move
                piece[9] |= (1ULL << to);   // add a black rook
            }

            break;
        case 15:    // queen-promotion capture

            for(int i = 0; i < 12; i++){
                if(piece[i] & (1ULL << to) && i != piece_type){
                    piece[i] &= ~(1ULL << to);
                    break;
                }
            }
            piece[piece_type] &= ~(1ULL << to);
            if(piece_type == 0){            //white to promote
                piece[4] |= (1ULL << to);   // add a white queen
            }else if(piece_type == 6){      // black to move
                piece[10] |= (1ULL << to);   // add a black queen
            }

            break;
    }

    if(white_to_move){
        white_to_move = false;
    }else{
        white_to_move = true;
    }

}

//algebraic move
void Board :: algebraic_move(string Move){
    string from = Move.substr(0, 3);
    char operation = Move[3];
    string to = Move.substr(4, 3);

    // parse the from
    char from_piece = from[0];
    int from_file = from[1] - 'a';
    int from_rank = 7 - (from[2] - '1');
    int from_bit = from_rank * 8 + from_file;

    //parse the to
    char to_piece = to[0];
    int to_file = to[1] - 'a';
    int to_rank = 7 - (to[2] - '1');
    int to_bit = to_rank * 8 + to_file;

    int type;
    // Obviously this lacks a lot of stuff but we'll deal with it later
    switch(operation){
        case '-':
            if(from_piece == 'p'){
                if(to_rank - from_rank == 2){
                    type = 1;
                }
            }else if(from_piece == 'P'){
                if(from_rank - to_rank == 2){
                    type = 1;
                }
            }else type = 0;
            break;
        case 'x':
            type = 4;
            break;
    }

    ///build the 16-bit move from all the parsed data
    uint16_t move_bit = (from_bit | (to_bit << 6) | (type << 12));
    move(move_bit);
}