#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <iostream>
#include <algorithm>
#include <cstdint>
#include <list>
#include <stack>
#include <vector>

#include "utilities.hpp"
#include "board.hpp"
#include "magic_bits.hpp"
#include "movegen.hpp"
#include "evaluation.hpp"

using namespace std;

extern int max_depth;
extern int traversed;
extern int pruned;

float search(Board, int, float, float, bool, uint16_t&);

#endif