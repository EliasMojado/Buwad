#include "evaluation.hpp"

float evaluate(Board board){
    float white_value = 0, black_value = 0;

    uint64_t attacked_squares = 0;
    uint64_t attacking_piece = 0;

    uint64_t occupied = 0;
    uint64_t white_occupied = 0;
    uint64_t black_occupied = 0;
    uint64_t possible_moves = 0;
    int square;

    for(int i = 0; i < 6; i++){
        white_occupied |= board.piece[i];
    }

    for(int i = 6; i < 12; i++){
        black_occupied |= board.piece[i];
    }

    occupied = white_occupied | black_occupied;
    float num_of_pieces = pop_count(occupied);

    /* ♛ WHITE QUEEN ATTACKS ♛ */
    attacking_piece = board.piece[4];
    while(attacking_piece){
        white_value += 10;
        square = get_lsb(attacking_piece);
        attacking_piece &= attacking_piece - 1;

        possible_moves = attacks.Queen(occupied, square);
        attacked_squares |= possible_moves;
    }

    white_value += pop_count(attacked_squares) * (0.1/num_of_pieces);
    white_value += pop_count(attacked_squares & black_occupied) * 0.1;
    attacked_squares = 0;

    /*  ♜ WHITE ROOK ATTACKS ♜ */
    attacking_piece = board.piece[3];
    while(attacking_piece){
        white_value += 5;
        square = get_lsb(attacking_piece);
        attacking_piece &= attacking_piece - 1;

        possible_moves = attacks.Rook(occupied, square);
        attacked_squares |= possible_moves;
    }

    white_value += pop_count(attacked_squares) * (0.75/num_of_pieces);
    white_value += pop_count(attacked_squares & black_occupied) * 0.2;
    attacked_squares = 0;

    /*  ♝ WHITE BISHOP ATTACKS ♝ */
    attacking_piece = board.piece[2];
    while(attacking_piece){
        white_value += 3;
        square = get_lsb(attacking_piece);
        attacking_piece &= attacking_piece - 1;

        possible_moves = attacks.Bishop(occupied, square);
        attacked_squares |= possible_moves;
    }

    white_value += pop_count(attacked_squares) * (0.75/num_of_pieces);
    white_value += pop_count(attacked_squares & black_occupied) * 0.3;
    attacked_squares = 0;

    /*  ♞ WHITE KNIGHT ATTACKS ♞ */
    attacking_piece = board.piece[1];
    while(attacking_piece){
        white_value += 2.75;
        square = get_lsb(attacking_piece);
        attacking_piece &= attacking_piece - 1;

        possible_moves = knight_moves[square];
        attacked_squares |= possible_moves;
    }

    white_value += pop_count(attacked_squares) * (num_of_pieces/750);
    white_value += pop_count(attacked_squares & black_occupied) * 0.3;
    attacked_squares = 0;
    
    /* ♟︎ WHITE PAWN ATTACKS ♟︎ */
    attacking_piece = board.piece[0];
    white_value += pop_count(attacking_piece) * 1;

    for(int i = 0; i < 8; i++){
        uint64_t pawn_file = attacking_piece & files[i];
        int doubled_pawn = pop_count(pawn_file);
        if(doubled_pawn > 1){
            white_value -= doubled_pawn * 0.5;
        }

        uint64_t pawn_rank = attacking_piece & ranks[i];
        int advance_pawn = pop_count(pawn_rank);
        white_value += advance_pawn * (i * 0.1);
    }

    uint64_t left_target = attacking_piece & 18374403900871474942;
    left_target = (left_target >> 9);
    uint64_t right_target = attacking_piece & 9187201950435737471;
    right_target = (right_target >> 7);
    attacked_squares |= left_target;
    attacked_squares |= right_target;

    uint64_t center_control = attacked_squares & 1006632960;
    white_value += pop_count(center_control) * (num_of_pieces/200);
    white_value += pop_count(attacked_squares & black_occupied) * 0.5;
    attacked_squares = 0;



/*#######################################################################################################################################################################################################################*/



    /* ♕  BLACK QUEEN ATTACKS ♕  */
    attacking_piece = board.piece[10];
    while(attacking_piece){
        black_value += 10;
        square = get_lsb(attacking_piece);
        attacking_piece &= attacking_piece - 1;

        possible_moves = attacks.Queen(occupied, square);
        attacked_squares |= possible_moves;
    }

    black_value += pop_count(attacked_squares) * (0.1/num_of_pieces);
    black_value += pop_count(attacked_squares & white_occupied) * 0.1;
    attacked_squares = 0;

    /*  ♖ BLACK ROOK ATTACKS ♖ */
    attacking_piece = board.piece[9];
    while(attacking_piece){
        black_value += 5;
        square = get_lsb(attacking_piece);
        attacking_piece &= attacking_piece - 1;

        possible_moves = attacks.Rook(occupied, square);
        attacked_squares |= possible_moves;
    }

    black_value += pop_count(attacked_squares) * (0.75/num_of_pieces);
    black_value += pop_count(attacked_squares & white_occupied) * 0.2;
    attacked_squares = 0;

    /*  ♗ BLACK BISHOP ATTACKS ♗ */
    attacking_piece = board.piece[8];
    while(attacking_piece){
        black_value += 3;
        square = get_lsb(attacking_piece);
        attacking_piece &= attacking_piece - 1;

        possible_moves = attacks.Bishop(occupied, square);
        attacked_squares |= possible_moves;
    }

    black_value += pop_count(attacked_squares) * (0.75/num_of_pieces);
    black_value += pop_count(attacked_squares & white_occupied) * 0.3;
    attacked_squares = 0;

    /*  ♘ BLACK KNIGHT ATTACKS ♘ */
    attacking_piece = board.piece[7];
    while(attacking_piece){
        black_value += 2.75;
        square = get_lsb(attacking_piece);
        attacking_piece &= attacking_piece - 1;

        possible_moves = knight_moves[square];
        attacked_squares |= possible_moves;
    }

    black_value += pop_count(attacked_squares) * (num_of_pieces/750);
    black_value += pop_count(attacked_squares & white_occupied) * 0.3;
    attacked_squares = 0;

    /* ♙ BLACK PAWN ATTACKS ♙ */
    attacking_piece = board.piece[6];
    black_value += pop_count(attacking_piece) * 1;

    for(int i = 0; i < 8; i++){
        uint64_t pawn_file = attacking_piece & files[i];
        int doubled_pawn = pop_count(pawn_file);
        if(doubled_pawn > 1){
            black_value -= doubled_pawn * 0.5;
        }

        uint64_t pawn_rank = attacking_piece & ranks[i];
        int advance_pawn = pop_count(pawn_rank);
        white_value += advance_pawn * ((7 - i) * 0.1);
    }

    left_target = attacking_piece & 18374403900871474942;
    left_target = (left_target << 7);
    right_target = attacking_piece & 9187201950435737471;
    right_target = (right_target << 9);
    attacked_squares |= left_target;
    attacked_squares |= right_target;

    center_control = attacked_squares & 257698037760;
    black_value += pop_count(center_control) * (num_of_pieces/200);
    black_value += pop_count(attacked_squares & white_occupied) * 0.5;
    attacked_squares = 0;

    return white_value - black_value;
}
