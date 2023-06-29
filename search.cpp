#include "search.hpp"

int max_depth = 0;
int traversed = 0;
int pruned = 0;

float quiescence_search(Board board, float alpha, float beta, bool maximizingPlayer, bool quietMove){
    traversed++;
    if(quietMove){
        return evaluate(board);
    }

    vector <uint16_t> possible_moves = generate_ordered_moves(board);

    if(maximizingPlayer){
        float maxEval = -100000000;

        for(auto it = possible_moves.begin(); it != possible_moves.end(); ++it){
            if((*it >> 12) & 0b1111 == 4){
                Board new_board = board;
                new_board.move(*it);
                float temp_eval;
                temp_eval = quiescence_search(new_board, alpha, beta, false, false); 

                if (temp_eval > maxEval) {
                    maxEval = temp_eval;
                }

                alpha = max(alpha, maxEval);
                if(beta <= alpha){
                    pruned++;
                    break;
                }
            }else{
                Board new_board = board;
                new_board.move(*it);
                float temp_eval;
                temp_eval = quiescence_search(new_board, alpha, beta, false, true); 

                if (temp_eval > maxEval) {
                    maxEval = temp_eval;
                }

                alpha = max(alpha, maxEval);
                if(beta <= alpha){
                    pruned++;
                    break;
                }
            }
        }

        return maxEval;
    }else{
        float minEval = 100000000;

        for(auto it = possible_moves.begin(); it != possible_moves.end(); ++it){
            if((*it >> 12) & 0b1111 == 4){;
                Board new_board = board;
                new_board.move(*it);
                float temp_eval;
                temp_eval = quiescence_search(new_board, alpha, beta, true, false); 

                if (temp_eval < minEval) {
                    minEval = temp_eval;
                }

                beta = min(beta, minEval);
                if(beta <= alpha){
                    pruned++;
                    break;
                }
            }else{
                Board new_board = board;
                new_board.move(*it);
                float temp_eval;
                temp_eval = quiescence_search(new_board, alpha, beta, true, true); 

                if (temp_eval < minEval) {
                    minEval = temp_eval;
                }

                beta = min(beta, minEval);
                if(beta <= alpha){
                    pruned++;
                    break;
                }
            }
        }

        return minEval;
    }
}

float search(Board board, int depth, float alpha, float beta, bool maximizingPlayer, uint16_t& best_move) {
    traversed++;
    if (depth == 0) {
        // if(maximizingPlayer){
        //     return quiescence_search(board, alpha, beta, true, false);
        // }else{
        //     return quiescence_search(board, alpha, beta, false, false);
        // }

        return evaluate(board);
    }

    vector <uint16_t> possible_moves = generate_ordered_moves(board);

    if (maximizingPlayer) {
        float maxEval = -100000000;

        for(auto it = possible_moves.begin(); it != possible_moves.end(); ++it){
            Board new_board = board;
            new_board.move(*it);
            float temp_eval;
            temp_eval = search(new_board, depth-1, alpha, beta, false, best_move); 

            if (temp_eval > maxEval) {
                maxEval = temp_eval;
                if(depth == max_depth){
                    best_move = *it;
                }
            }
            alpha = max(alpha, maxEval);
            if(beta <= alpha){
                pruned++;
                break;
            }
        }
        return maxEval;
    } else {
        float minEval = 100000000;

        for(auto it = possible_moves.begin(); it != possible_moves.end(); ++it){
            Board new_board = board;
            new_board.move(*it);
            float temp_eval;
            temp_eval = search(new_board, depth-1, alpha, beta, true, best_move); 

            if (temp_eval < minEval) {
                minEval = temp_eval;
                if(depth == max_depth){
                    best_move = *it;
                }
            }
            beta = min(beta, minEval);
            if(beta <= alpha){
                pruned++;
                break;
            }
        }
        return minEval; 
    }
}


