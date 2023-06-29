#include "movegen.hpp"

// checks if a move is legal while also assisting ordering of the moves
int is_legal(const Board board, int& strength){
    uint64_t white_attacked_squares = 0;
    uint64_t black_attacked_squares = 0;
    uint64_t attacking_piece = 0;
    uint64_t occupied = 0;
    uint64_t white_occupied = 0;
    uint64_t black_occupied = 0;
    uint64_t possible_moves = 0;
    int square;

    int white_strength = 0;
    int black_strength = 0;


    //get all the white occupied
        for(int i = 0; i < 6; i++){
        white_occupied |= board.piece[i];
    }

    //get all the black occupied
    for(int i = 6; i < 12; i++){
        black_occupied |= board.piece[i];
    }

    //get all the occupied 
    occupied = white_occupied | black_occupied;

    /* ♛ WHITE QUEEN ATTACKS ♛ */
    attacking_piece = board.piece[4];
    while(attacking_piece){
        white_strength += 10;
        square = get_lsb(attacking_piece);
        attacking_piece &= attacking_piece - 1;

        possible_moves = attacks.Queen(occupied, square);
        white_attacked_squares |= possible_moves;
    }

    /*  ♜ WHITE ROOK ATTACKS ♜ */
    attacking_piece = board.piece[3];
    while(attacking_piece){
        white_strength += 5;
        square = get_lsb(attacking_piece);
        attacking_piece &= attacking_piece - 1;

        possible_moves = attacks.Rook(occupied, square);
        white_attacked_squares |= possible_moves;
    }

    /*  ♝ WHITE BISHOP ATTACKS ♝ */
    attacking_piece = board.piece[2];
    while(attacking_piece){
        white_strength += 3;
        square = get_lsb(attacking_piece);
        attacking_piece &= attacking_piece - 1;

        possible_moves = attacks.Bishop(occupied, square);
        white_attacked_squares |= possible_moves;
    }

    /*  ♞ WHITE KNIGHT ATTACKS ♞ */
    attacking_piece = board.piece[1];
    while(attacking_piece){
        white_strength += 3;
        square = get_lsb(attacking_piece);
        attacking_piece &= attacking_piece - 1;

        possible_moves = knight_moves[square];
        white_attacked_squares |= possible_moves;
    }

    /* ♟︎ WHITE PAWN ATTACKS ♟︎ */
    attacking_piece = board.piece[0];
    white_strength += pop_count(attacking_piece);
    uint64_t left_target = attacking_piece & 18374403900871474942;
    left_target = (left_target >> 9);
    uint64_t right_target = attacking_piece & 9187201950435737471;
    right_target = (right_target >> 7);
    white_attacked_squares |= left_target;
    white_attacked_squares |= right_target;

    /*  ♚ WHITE KING ATTACKS ♚ */
    attacking_piece = board.piece[5];
    while(attacking_piece){
        square = get_lsb(attacking_piece);
        attacking_piece &= attacking_piece - 1;

        possible_moves = king_moves[square];
        white_attacked_squares |= possible_moves;
    }

    if(board.white_to_move){
        if(white_attacked_squares & board.piece[11]){
            return 0;
        }
    }

    /* ♕  BLACK QUEEN ATTACKS ♕  */
    attacking_piece = board.piece[10];
    while(attacking_piece){
        black_strength += 10;
        square = get_lsb(attacking_piece);
        attacking_piece &= attacking_piece - 1;

        possible_moves = attacks.Queen(occupied, square);
        black_attacked_squares |= possible_moves;
    }

    /*  ♖ BLACK ROOK ATTACKS ♖ */
    attacking_piece = board.piece[9];
    while(attacking_piece){
        black_strength += 5;
        square = get_lsb(attacking_piece);
        attacking_piece &= attacking_piece - 1;

        possible_moves = attacks.Rook(occupied, square);
        black_attacked_squares |= possible_moves;
    }

    /*  ♗ BLACK BISHOP ATTACKS ♗ */
    attacking_piece = board.piece[8];
    while(attacking_piece){
        black_strength += 3;
        square = get_lsb(attacking_piece);
        attacking_piece &= attacking_piece - 1;

        possible_moves = attacks.Bishop(occupied, square);
        black_attacked_squares |= possible_moves;
    }

    /*  ♘ BLACK KNIGHT ATTACKS ♘ */
    attacking_piece = board.piece[7];
    while(attacking_piece){
        black_strength += 3;
        square = get_lsb(attacking_piece);
        attacking_piece &= attacking_piece - 1;

        possible_moves = knight_moves[square];
        black_attacked_squares |= possible_moves;
    }

    /* ♙ BLACK PAWN ATTACKS ♙ */
    attacking_piece = board.piece[6];
    black_strength += pop_count(attacking_piece);
    left_target = attacking_piece & 18374403900871474942;
    left_target = (left_target << 7);
    right_target = attacking_piece & 9187201950435737471;
    right_target = (right_target << 9);
    black_attacked_squares |= left_target;
    black_attacked_squares |= right_target;


    /*  ♔ BLACK KING ATTACKS ♔ */
    attacking_piece = board.piece[11];
    while(attacking_piece){
        square = get_lsb(attacking_piece);
        attacking_piece &= attacking_piece - 1;

        possible_moves = king_moves[square];
        black_attacked_squares |= possible_moves;
    }

    if(!board.white_to_move){
        if(black_attacked_squares & board.piece[5]){
            return 0;
        }
    }
    
    strength = 25*(white_strength-black_strength) 
                + 1.5 * (pop_count(white_attacked_squares) - pop_count(black_attacked_squares))
                + 3 * (pop_count(white_attacked_squares & black_occupied) - pop_count(black_attacked_squares & white_occupied)); 
    return 1;
}

vector<uint16_t> generate_ordered_moves(Board board) {
    vector<pair<uint16_t, int>> moves_and_strengths;
    vector<uint16_t> possible_moves = generate_moves(board);

    for (auto move : possible_moves) {
        Board newboard = board;
        newboard.move(move);
        int strength;
        if (is_legal(newboard, strength) != 0) {
            if (!board.white_to_move) {
                strength = -strength; // negate strength if black to move
            }
            moves_and_strengths.push_back(make_pair(move, strength));
        }
    }

    // sort moves based on strength
    stable_sort(moves_and_strengths.begin(), moves_and_strengths.end(), [](const pair<uint16_t, int>& a, const pair<uint16_t, int>& b) {
        return a.second > b.second;
    });

    // create vector of moves from sorted pairs
    vector<uint16_t> ordered_moves;
    for (auto move_and_strength : moves_and_strengths) {
        ordered_moves.push_back(move_and_strength.first);
    }

    return ordered_moves;
}


// generate all pseudo-legal moves
vector <uint16_t> generate_moves(const Board& board) {
    vector <uint16_t> moves;
    uint64_t moving_piece = 0;
    uint64_t occupied = 0;
    uint64_t empty = 0;
    uint64_t possible_moves = 0; 
    uint64_t white_occupied = 0;
    uint64_t black_occupied = 0;
    int square;

    //get the white occupied
    for(int i = 0; i < 6; i++){
        white_occupied |= board.piece[i];
    }

    //get the black occupied
    for(int i = 6; i < 12; i++){
        black_occupied |= board.piece[i];
    }

    //get the occupied squares
    occupied = white_occupied | black_occupied;

    //get the empty squares
    empty = ~occupied;

    if(board.white_to_move){

        /*
            ♟︎ WHITE PAWN CAPTURES ♟︎
        */
        moving_piece = board.piece[0];
        {
            uint64_t left_target = moving_piece & 18374403900871474942;
            left_target = (left_target >> 9) & black_occupied;
            uint64_t right_target = moving_piece & 9187201950435737471;
            right_target = (right_target >> 7) & black_occupied;

            while(left_target){
                square = get_lsb(left_target);
                // if pawn is in the 7th rank
                if(square < 8){
                    moves.push_back(move_encode(square + 9, square, 12));
                    moves.push_back(move_encode(square + 9, square, 13));
                    moves.push_back(move_encode(square + 9, square, 14));
                    moves.push_back(move_encode(square + 9, square, 15));
                }else{
                    moves.push_back(move_encode(square + 9, square, 4));
                }

                left_target &= left_target - 1;
            }

            while(right_target){
                square = get_lsb(right_target);
                // if pawn is in the seventh rank
                if(square < 8){
                    moves.push_back(move_encode(square + 7, square, 12));     
                    moves.push_back(move_encode(square + 7, square, 13));     
                    moves.push_back(move_encode(square + 7, square, 14));     
                    moves.push_back(move_encode(square + 7, square, 15));     
                }else{
                    moves.push_back(move_encode(square + 7, square, 4));     
                }

                right_target &= right_target - 1;
            }

            // en passant move
            if(board.en_passant_square >= 0){
                left_target = (moving_piece >> 9) & black_occupied;
                right_target = (moving_piece >> 7) & black_occupied;

                left_target &= (1ULL << board.en_passant_square);
                if(left_target){
                    moves.push_back(move_encode(get_lsb(left_target) + 9, get_lsb(left_target), 5));
                }

                right_target &= (1ULL << board.en_passant_square);
                if(right_target){
                    moves.push_back(move_encode(get_lsb(right_target) + 7, get_lsb(right_target), 5));
                }
            }
        }

        /*
            ♞ WHITE KNIGHT CAPTURES ♞ 
        */

         moving_piece = board.piece[1];
        {
            while(moving_piece){
                // get the index square of the knight and pop it
                square = get_lsb(moving_piece);
                moving_piece &= moving_piece - 1;

                // get the possible moves
                possible_moves = knight_moves[square] & black_occupied;
                while(possible_moves){
                    moves.push_back(move_encode(square ,get_lsb(possible_moves), 4));
                    possible_moves &= possible_moves - 1;
                }
            }
            
        }

        /*
            ♝ WHITE BISHOP CAPTURES ♝
        */
        
        moving_piece = board.piece[2];
        {
            while(moving_piece){
                square = get_lsb(moving_piece);
                moving_piece &= moving_piece - 1;

                possible_moves = attacks.Bishop(occupied, square);
                possible_moves &= black_occupied;
                while(possible_moves){
                    moves.push_back(move_encode(square, get_lsb(possible_moves), 4));
                    possible_moves &= possible_moves -1;
                }
            }
        }
        
        /*
            ♜ WHITE ROOK CAPTURES ♜
        */

        moving_piece = board.piece[3];
        // Capture moves
        {
            while(moving_piece){
                square = get_lsb(moving_piece);
                moving_piece &= moving_piece - 1;

                possible_moves = attacks.Rook(occupied, square);
                possible_moves &= black_occupied;
                while(possible_moves){
                    moves.push_back(move_encode(square, get_lsb(possible_moves), 4));
                    possible_moves &= possible_moves -1;
                }
            }
        }

        /*
            ♛ WHITE QUEEN CAPTURES ♛
        */

        moving_piece = board.piece[4];
        // Capture moves
        {
            while(moving_piece){
                square = get_lsb(moving_piece);
                moving_piece &= moving_piece - 1;

                possible_moves = attacks.Queen(occupied, square);
                possible_moves &= black_occupied;
                while(possible_moves){
                    moves.push_back(move_encode(square, get_lsb(possible_moves), 4));
                    possible_moves &= possible_moves -1;
                }
            }
        }

        /*
            ♛ WHITE QUEEN MOVES ♛
        */

        moving_piece = board.piece[4];
        // Silent moves
        {
            while(moving_piece){
                square = get_lsb(moving_piece);
                moving_piece &= moving_piece - 1;

                possible_moves = attacks.Queen(occupied, square);
                possible_moves &= empty;
                while(possible_moves){
                    moves.push_back(move_encode(square, get_lsb(possible_moves), 0));
                    possible_moves &= possible_moves -1;
                }
            }
        }

        /*
            ♜ WHITE ROOK MOVES ♜
        */
        moving_piece = board.piece[3];
        // Silent moves
        {
            while(moving_piece){
                square = get_lsb(moving_piece);
                moving_piece &= moving_piece - 1;

                possible_moves = attacks.Rook(occupied, square);
                possible_moves &= empty;
                while(possible_moves){
                    moves.push_back(move_encode(square, get_lsb(possible_moves), 0));
                    possible_moves &= possible_moves -1;
                }
            }
        }

        /*
            ♝ WHITE BISHOP MOVES ♝
        */

        moving_piece = board.piece[2];
        // Silent moves
        {
            while(moving_piece){
                square = get_lsb(moving_piece);
                moving_piece &= moving_piece - 1;

                possible_moves = attacks.Bishop(occupied, square);
                possible_moves &= empty;
                while(possible_moves){
                    moves.push_back(move_encode(square, get_lsb(possible_moves), 0));
                    possible_moves &= possible_moves -1;
                }
            }
        }

        /*
            ♞ WHITE KNIGHT MOVES ♞ 
        */

        moving_piece = board.piece[1];
        // silent moves
        {
            while(moving_piece){
                // get the index square of the knight and pop it
                square = get_lsb(moving_piece);
                moving_piece &= moving_piece - 1;
                
                // get the possible moves
                possible_moves = knight_moves[square] & empty;
                while(possible_moves){
                    moves.push_back(move_encode(square ,get_lsb(possible_moves), 0));
                    possible_moves &= possible_moves - 1;
                }
            }
        }

        /*
            ♟︎ WHITE PAWN PUSH ♟︎
        */
       
        moving_piece = board.piece[0];
        //one-push pawn move
        {
            uint64_t target_squares = moving_piece >> 8;
            possible_moves = target_squares & empty;
            while(possible_moves){
                //add one-push moves to the list

                //if pawn is in the 7th rank
                square = get_lsb(possible_moves);
                if(square < 8){
                    moves.push_back(move_encode(square + 8, square, 8));
                    moves.push_back(move_encode(square + 8, square, 9));
                    moves.push_back(move_encode(square + 8, square, 10));
                    moves.push_back(move_encode(square + 8, square, 11));
                }else{
                    moves.push_back(move_encode(square + 8, square, 0));
                }

                possible_moves &= possible_moves - 1;
            }
        }

        // double-push pawn move
        {
            uint64_t target_squares = (moving_piece >> 16) & empty; 
            uint64_t third_rank = (moving_piece >> 8) & empty;
            possible_moves = target_squares & (third_rank >> 8);
            while(possible_moves){
                //add double-push moves to the list
                square = get_lsb(possible_moves);
                if(square >= 32 && square <= 39){
                    moves.push_back(move_encode(square + 16, square, 1));
                }

                possible_moves &= possible_moves - 1;
            }
        }

        /*
            ♚ WHITE KING MOVES ♚
        */

        moving_piece = board.piece[5];
        // silent moves
        {
            while(moving_piece){
                square = get_lsb(moving_piece);
                moving_piece &= moving_piece - 1;

                possible_moves = king_moves[square] & empty;
                while(possible_moves){
                    moves.push_back(move_encode(square, get_lsb(possible_moves), 0));
                    possible_moves &= possible_moves - 1;
                }
            }
        }

        moving_piece = board.piece[5];
        // captures
        {
            while(moving_piece){
                square = get_lsb(moving_piece);
                moving_piece &= moving_piece - 1;

                possible_moves = king_moves[square] & black_occupied;
                while(possible_moves){
                    moves.push_back(move_encode(square, get_lsb(possible_moves), 4));
                    possible_moves &= possible_moves - 1;
                }
            }
        }

        // queen castle
        {   
            uint64_t queenside = occupied & 1008806316530991104;
            if(queenside == 0 && board.wcastle_queenside == true){
                moves.push_back(move_encode(60, 58, 3));
            }
        }

        // king castle
        {
            uint64_t kingside = occupied & 6917529027641081856;
            if(kingside == 0 && board.wcastle_kingside == true){
                moves.push_back(move_encode(60, 62, 2));
            }
        }

    }else{

        /*
            ♙ BLACK PAWN CAPTURES ♙
        */

        moving_piece = board.piece[6];
        // pawn capture move
        {
            uint64_t left_target = moving_piece & 18374403900871474942;
            left_target = (left_target << 7) & white_occupied;
            uint64_t right_target = moving_piece & 9187201950435737471;
            right_target = (right_target << 9) & white_occupied;

            while(left_target){
                square = get_lsb(left_target);
                // if pawn is in the seventh rank
                if(square > 55){
                    moves.push_back(move_encode(square - 7, square, 12));
                    moves.push_back(move_encode(square - 7, square, 13));
                    moves.push_back(move_encode(square - 7, square, 14));
                    moves.push_back(move_encode(square - 7, square, 15));
                }else{
                    moves.push_back(move_encode(square - 7, square, 4));
                }

                left_target &= left_target - 1;
            }

            while(right_target){
                square = get_lsb(right_target);
                // if pawn is in the seventh rank
                if(square > 55){
                    moves.push_back(move_encode(square - 9, square, 12));
                    moves.push_back(move_encode(square - 9, square, 13));
                    moves.push_back(move_encode(square - 9, square, 14));
                    moves.push_back(move_encode(square - 9, square, 15));
                }else{
                    moves.push_back(move_encode(square - 9, square, 4));
                }

                right_target &= right_target - 1;
            }

            // en passant move
            if(board.en_passant_square >= 0){
                left_target &= (1ULL << board.en_passant_square);
                if(left_target){
                    moves.push_back(move_encode(get_lsb(left_target) - 7, get_lsb(left_target), 5));
                }

                right_target &= (1ULL << board.en_passant_square);
                if(right_target){
                    moves.push_back(move_encode(get_lsb(right_target) - 9, get_lsb(right_target), 5));
                }
            }
        }

        /*
            ♘ BLACK KNIGHT CAPTURES ♘
        */

        moving_piece = board.piece[7];
        // capture moves
        {   
            uint64_t possible_moves;
            while(moving_piece){
                // get the index square of the knight and pop it
                square = get_lsb(moving_piece);
                moving_piece &= moving_piece-1;

                // get the possible moves
                possible_moves = knight_moves[square] & white_occupied;
                
                while(possible_moves){
                    moves.push_back(move_encode(square ,get_lsb(possible_moves), 4));
                    possible_moves &= possible_moves -1;
                }
            }
            
        }

        /*
            ♗ BLACK BISHOP CAPTURES ♗
        */
        moving_piece = board.piece[8];
        // Capture moves
        {
            while(moving_piece){
                square = get_lsb(moving_piece);
                moving_piece &= moving_piece - 1;

                possible_moves = attacks.Bishop(occupied, square);
                possible_moves &= white_occupied;
                while(possible_moves){
                    moves.push_back(move_encode(square, get_lsb(possible_moves), 4));
                    possible_moves &= possible_moves -1;
                }
            }
        }

        /*
            ♖ BLACK ROOK CAPTURES ♖
        */

        moving_piece = board.piece[9];
        // Capture moves
        {
            while(moving_piece){
                square = get_lsb(moving_piece);
                moving_piece &= moving_piece - 1;

                possible_moves = attacks.Rook(occupied, square);
                possible_moves &= white_occupied;
                while(possible_moves){
                    moves.push_back(move_encode(square, get_lsb(possible_moves), 4));
                    possible_moves &= possible_moves -1;
                }
            }
        }

        /*
            ♕ BLACK QUEEN CAPTURES ♕
        */

        moving_piece = board.piece[10];
        // Capture moves
        {
            while(moving_piece){
                square = get_lsb(moving_piece);
                moving_piece &= moving_piece - 1;

                possible_moves = attacks.Queen(occupied, square);
                possible_moves &= white_occupied;
                while(possible_moves){
                    moves.push_back(move_encode(square, get_lsb(possible_moves), 4));
                    possible_moves &= possible_moves -1;
                }
            }
        }

        /*
            ♕ BLACK QUEEN MOVES ♕
        */

        moving_piece = board.piece[10];
        // Silent moves
        {
            while(moving_piece){
                square = get_lsb(moving_piece);
                moving_piece &= moving_piece - 1;

                possible_moves = attacks.Queen(occupied, square);
                possible_moves &= empty;
                while(possible_moves){
                    moves.push_back(move_encode(square, get_lsb(possible_moves), 0));
                    possible_moves &= possible_moves -1;
                }
            }
        }

        /*
            ♖ BLACK ROOK MOVES ♖
        */

        moving_piece = board.piece[9];
        // Silent moves
        {
            while(moving_piece){
                square = get_lsb(moving_piece);
                moving_piece &= moving_piece - 1;

                possible_moves = attacks.Rook(occupied, square);
                possible_moves &= empty;
                while(possible_moves){
                    moves.push_back(move_encode(square, get_lsb(possible_moves), 0));
                    possible_moves &= possible_moves -1;
                }
            }
        }

        /*
            ♗ BLACK BISHOP MOVES ♗
        */

        moving_piece = board.piece[8];
        // Silent moves
        {
            while(moving_piece){
                square = get_lsb(moving_piece);
                moving_piece &= moving_piece - 1;

                possible_moves = attacks.Bishop(occupied, square);
                possible_moves &= empty;
                while(possible_moves){
                    moves.push_back(move_encode(square, get_lsb(possible_moves), 0));
                    possible_moves &= possible_moves -1;
                }
            }
        }
    


        /*
            ♘ BLACK KNIGHT MOVES ♘
        */

        moving_piece = board.piece[7];
        // silent moves
        {
            uint64_t possible_moves;
            while(moving_piece){
                // get the index square of the knight and pop it
                square = get_lsb(moving_piece);
                moving_piece &= moving_piece-1;
                
                // get the possible moves
                possible_moves = knight_moves[square] & empty;
                while(possible_moves){
                    moves.push_back(move_encode(square ,get_lsb(possible_moves), 0));
                    possible_moves &= possible_moves-1;
                }
            }
        }    

        /*
            ♙ BLACK PAWN MOVES ♙
        */

        moving_piece = board.piece[6];
        //one-push pawn move
        {
            uint64_t target_squares = moving_piece << 8;
            possible_moves = target_squares & empty;
            while(possible_moves){
                //add one-push moves to the list
                
                //if pawn is in the 7th rank
                square = get_lsb(possible_moves);
                if(square > 55){
                    moves.push_back(move_encode(square - 8, square, 8));
                    moves.push_back(move_encode(square - 8, square, 9));
                    moves.push_back(move_encode(square - 8, square, 10));
                    moves.push_back(move_encode(square - 8, square, 11));
                }else{
                    moves.push_back(move_encode(square - 8, square, 0));
                }

                possible_moves &= possible_moves - 1;
            }
        }

        // double-push pawn move
        {
            uint64_t target_squares = moving_piece << 16 & empty; 
            uint64_t third_rank = (moving_piece << 8) & empty;
            possible_moves = target_squares & (third_rank << 8);
            while(possible_moves){
                //add double-push moves to the list
                square = get_lsb(possible_moves);
                if(square >= 24 && square <= 31){
                    moves.push_back(move_encode(square - 16, square, 1));
                }

                possible_moves &= possible_moves - 1;
            }
        }

        /*
            ♔ BLACk KING MOVES ♔
        */

        moving_piece = board.piece[11];
        // silent moves
        {
            while(moving_piece){
                square = get_lsb(moving_piece);
                moving_piece &= moving_piece - 1;

                possible_moves = king_moves[square] & empty;
                while(possible_moves){
                    moves.push_back(move_encode(square, get_lsb(possible_moves), 0));
                    possible_moves &= possible_moves - 1;
                }
            }
        }

        moving_piece = board.piece[11];
        // captures
        {
            while(moving_piece){
                square = get_lsb(moving_piece);
                moving_piece &= moving_piece - 1;

                possible_moves = king_moves[square] & white_occupied;
                while(possible_moves){
                    moves.push_back(move_encode(square, get_lsb(possible_moves), 4));
                    possible_moves &= possible_moves - 1;
                }
            }
        }

        // queen castle
        {
            uint64_t queenside = occupied & 14;
            if(queenside == 0 && board.bcastle_queenside == true){
                moves.push_back(move_encode(4, 2, 3));
            }
        }

        // king castle
        {
            uint64_t kingside = occupied & 96;
            if(kingside == 0 && board.bcastle_kingside == true){
                moves.push_back(move_encode(4, 6, 2));
            }
        }
    }

    return moves;
}