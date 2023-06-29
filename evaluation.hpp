#ifndef EVALUATION_HPP
#define EVALUATION_HPP

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
#include "search.hpp"

float evaluate(Board);

#endif