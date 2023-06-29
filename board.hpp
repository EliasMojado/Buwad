#ifndef BOARD_H
#define BOARD_H

#include <cstdint>
#include <string>

using namespace std;

// Define a struct to hold the entire board
class Board {
    public: 
    uint64_t piece[12];
    bool white_to_move;
    bool wcastle_kingside;
    bool wcastle_queenside;
    bool bcastle_kingside;
    bool bcastle_queenside;
    int8_t en_passant_square;

    Board(){

    }

    void board_initialize();
    void print();
    void move(uint16_t);
    void algebraic_move(string);
};

#endif
