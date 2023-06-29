#include <iostream>
#include <string>
#include <list>
#include <chrono>
#include <stdlib.h>

#include "search.hpp"
#include "board.hpp"
#include "magic_bits.hpp"
#include "movegen.hpp"
#include "utilities.hpp"

using namespace std;

int main() {
    // Create an initial board position
    Board board;
    board.board_initialize();

    vector<uint16_t> moves;
    uint16_t move;

    cout << "DEPTH: ";
    cin >> max_depth;

    string game_type;

    cout << "W / B / WB ?: ";
    cin >> game_type;
    system("cls");
    while(true){
        board.print();
        moves = generate_ordered_moves(board);
        
        cout << endl << "possible moves   " << "code" << endl;
        for(auto it = moves.begin(); it != moves.end(); ++it){
            cout << move_decode(*it, board) << "          " << *it << endl;
        }
        
        uint16_t best_move = 0;
        float strength = 0;
        traversed = 0;
        pruned = 0;
        cout << endl << "Top engine move: " << endl;

        auto start = chrono::high_resolution_clock::now();

        if(board.white_to_move && game_type == "W"){
            strength = search(board, max_depth, -1000000, 1000000, board.white_to_move, best_move);
            cout << move_decode(best_move, board) << "          " << best_move << endl;
            cout << "strength: " << strength << endl;
        }else if(board.white_to_move == false && game_type == "B"){
            strength = search(board,  max_depth, -1000000, 1000000, board.white_to_move, best_move);
            cout << move_decode(best_move, board) << "          " << best_move << endl;
            cout << "strength: " << strength << endl;
        }else if (game_type == "WB"){
            strength = search(board, max_depth, -1000000, 1000000, board.white_to_move, best_move);
            cout << move_decode(best_move, board) << "          " << best_move << endl;
            cout << "strength: " << strength << endl;
        }

        auto finish = chrono::high_resolution_clock::now();
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(finish-start).count() << "ns\n";
        
        cout << "traversed: " << traversed << endl;
        cout << "pruned: " << pruned << endl;

        cout << "your move : ";
        cin >> move;
        board.move(move);
        system("cls");
    }
    
    return 0;
}