#ifndef MOVEGEN_H
#define MOVEGEN_H

#include <iostream>
#include <list>
#include <vector>

#include "utilities.hpp"
#include "board.hpp"
#include "evaluation.hpp"
#include "magic_bits.hpp"

using namespace std;
namespace{
    const magic_bits :: Attacks attacks;
}

int is_legal(const Board board, int&);
vector <uint16_t> generate_moves(const Board& board);
vector <uint16_t> generate_ordered_moves(Board board);

#endif