#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <list>
#include <cstdint>

#include "board.hpp"

using namespace std;

//get the index of the least significant bit
inline int get_lsb(uint64_t bits){
    return __builtin_ctzll(bits);
}

inline int pop_count(uint64_t bits){
    return __builtin_popcountll(bits);
}

void print_bits(const uint64_t&);

// translates from, to, type to uint16_t
inline uint16_t move_encode(const int& from, const int& to,const int& type){
    return( from | (to << 6) | (type << 12));
}

string move_decode(const uint16_t&, const Board&);

extern uint64_t ranks[8];
extern uint64_t files[8];
extern uint64_t knight_moves[64];
extern uint64_t king_moves[64];


#endif