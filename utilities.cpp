// get least significant bit
#include "utilities.hpp"

uint64_t ranks[8] = {
    18374686479671623680,
    71776119061217280,
    280375465082880,
    1095216660480,
    4278190080,
    16711680,
    65280,
    255
};

uint64_t files[8] = {
    72340172838076673,
    144680345676153346,
    289360691352306692,
    578721382704613384,
    1157442765409226768,
    2314885530818453536,
    4629771061636907072,
    9259542123273814144
};

uint64_t knight_moves[64] = {
    132096,
    329728,
    659712,
    1319424,
    2638848,
    5277696,
    10489856,
    4202496,
    33816580,
    84410376,
    168886289,
    337772578,
    675545156,
    1351090312,
    2685403152,
    1075839008,
    8657044482,
    21609056261,
    43234889994,
    86469779988,
    172939559976,
    345879119952,
    687463207072,
    275414786112,
    2216203387392,
    5531918402816,
    11068131838464,
    22136263676928,
    44272527353856,
    88545054707712,
    175990581010432,
    70506185244672,
    567348067172352,
    1416171111120896,
    2833441750646784,
    5666883501293568,
    11333767002587136,
    22667534005174272,
    45053588738670592,
    18049583422636032,
    145241105196122112,
    362539804446949376,
    725361088165576704,
    1450722176331153408,
    2901444352662306816,
    5802888705324613632,
    11533718717099671552,
    4620693356194824192,
    288234782788157440,
    576469569871282176,
    1224997833292120064,
    2449995666584240128,
    4899991333168480256,
    9799982666336960512,
    1152939783987658752,
    2305878468463689728,
    1128098930098176,
    2257297371824128,
    4796069720358912,
    9592139440717824,
    19184278881435648,
    38368557762871296,
    4679521487814656,
    9077567998918656
};

uint64_t king_moves[64] = {
    770,
    1797,
    3594,
    7188,
    14376,
    28752,
    57504,
    49216,
    197123,
    460039,
    920078,
    1840156,
    3680312,
    7360624,
    14721248,
    12599488,
    50463488,
    117769984,
    235539968,
    471079936,
    942159872,
    1884319744,
    3768639488,
    3225468928,
    12918652928,
    30149115904,
    60298231808,
    120596463616,
    241192927232,
    482385854464,
    964771708928,
    825720045568,
    3307175149568,
    7718173671424,
    15436347342848,
    30872694685696,
    61745389371392,
    123490778742784,
    246981557485568,
    211384331665408,
    846636838289408,
    1975852459884544,
    3951704919769088,
    7903409839538176,
    15806819679076352,
    31613639358152704,
    63227278716305408,
    54114388906344448,
    216739030602088448,
    505818229730443264,
    1011636459460886528,
    2023272918921773056,
    4046545837843546112,
    8093091675687092224,
    16186183351374184448,
    13853283560024178688,
    144959613005987840,
    362258295026614272,
    724516590053228544,
    1449033180106457088,
    2898066360212914176,
    5796132720425828352,
    11592265440851656704,
    4665729213955833856
};

// print out the board for a piece for debugging purposes
void print_bits(const uint64_t& bits){
    cout << endl;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            int index = (i*8) + j;
            if(bits & (1ULL << index)){
                cout << "1 ";
            }else{
                cout << "0 ";
            }
        }
        cout << endl;
    }
    cout << endl;
}

// translates a uint16_t move to algebraic notation
string move_decode(const uint16_t& move, const Board& board){
    string symbols = "PNBRQKpnbrqk";
    // Extract the "from" square
    int from = move & 0b111111;
    
    // Extract the "to" square
    int to = (move >> 6) & 0b111111;
    
    // Extract the type of move
    int type = (move >> 12) & 0b1111;

    //identify what the piece is
    int piece = 0;
    for(int i = 0; i < 12; i++){
        if(board.piece[i] & (1ULL << from)){
            piece = i;
            break;
        }
    }

    string algebraic;
    if(type == 0){
        algebraic += symbols[piece];
        algebraic += 'a' + (from % 8);
        algebraic += '8' - (from / 8);
        algebraic += '-';
        algebraic += symbols[piece];
        algebraic += 'a' + (to % 8);
        algebraic += '8' - (to / 8);
            
        return algebraic;
    }else if(type == 1){ 
        algebraic += symbols[piece];
        algebraic += 'a' + (from % 8);
        algebraic += '8' - (from / 8);
        algebraic += '-';
        algebraic += symbols[piece];
        algebraic += 'a' + (to % 8);
        algebraic += '8' - (to / 8);
            
        return algebraic;
    }else if (type == 2){ 
        algebraic = "O-O";
        return algebraic;
    }else if (type == 3){
        algebraic = "O-O-O";
        return algebraic;
    }else if (type == 4){
        int opp_piece = 0;
        for(int i = 0; i < 12; i++){
            if(board.piece[i] & (1ULL << to)){
                opp_piece = i;
                break;
            }
        }

        algebraic += symbols[piece];
        algebraic += 'a' + (from % 8);
        algebraic += '8' - (from / 8);
        algebraic += 'x';
        algebraic += symbols[opp_piece];
        algebraic += 'a' + (to % 8);
        algebraic += '8' - (to / 8);
            
        return algebraic;
    }else if (type == 5){
        int opp_piece = 1;
        if(piece = 1){
            opp_piece = 6;
        }

        algebraic += symbols[piece];
        algebraic += 'a' + (from % 8);
        algebraic += '8' - (from / 8);
        algebraic += 'x';
        algebraic += symbols[opp_piece];
        algebraic += 'a' + (to % 8);

        if(piece = 1){
            algebraic += '7' - (to / 8);
        }else{
            algebraic += '9' - (to / 8);
        }

        return algebraic;
    }else if ( type == 8){
        algebraic += symbols[piece];
        algebraic += 'a' + (from % 8);
        algebraic += '8' - (from / 8);
        algebraic += '-';
        algebraic += symbols[piece];
        algebraic += 'a' + (to % 8);
        algebraic += '8' - (to / 8);
            
        return algebraic; 
    }else if (type == 9){
        algebraic+= symbols[piece];
        algebraic+= 'a' + (from % 8);
        algebraic+= '8' - (from / 8);
        algebraic+= '-';
        algebraic+= symbols[piece];
        algebraic+= 'a' + (to % 8);
        algebraic+= '8' - (to / 8);
            
        return algebraic; 
    }else if (type == 10){
        algebraic+= symbols[piece];
        algebraic+= 'a' + (from % 8);
        algebraic+= '8' - (from / 8);
        algebraic+= '-';
        algebraic+= symbols[piece];
        algebraic+= 'a' + (to % 8);
        algebraic+= '8' - (to / 8);
            
        return algebraic; 
    }else if (type == 11){
        algebraic+= symbols[piece];
        algebraic+= 'a' + (from % 8);
        algebraic+= '8' - (from / 8);
        algebraic+= '-';
        algebraic+= symbols[piece];
        algebraic+= 'a' + (to % 8);
        algebraic+= '8' - (to / 8);
            
        return algebraic; 
    }else if (type == 12){
        int opp_piece = 0;
        for(int i = 0; i < 12; i++){
            if(board.piece[i] & (1ULL << to)){
                opp_piece = i;
                break;
            }
        }

        algebraic+= symbols[piece];
        algebraic+= 'a' + (from % 8);
        algebraic+= '8' - (from / 8);
        algebraic+= 'x';
        algebraic+= symbols[opp_piece];
        algebraic+= 'a' + (to % 8);
        algebraic+= '8' - (to / 8);
            
        return algebraic; 

    }else if (type == 13){
        int opp_piece = 0;
        for(int i = 0; i < 12; i++){
            if(board.piece[i] & (1ULL << to)){
                opp_piece = i;
                break;
            }
        }

        algebraic+= symbols[piece];
        algebraic+= 'a' + (from % 8);
        algebraic+= '8' - (from / 8);
        algebraic+= 'x';
        algebraic+= symbols[opp_piece];
        algebraic+= 'a' + (to % 8);
        algebraic+= '8' - (to / 8);
            
        return algebraic;
    } else if (type == 14){
        int opp_piece = 0;
        for(int i = 0; i < 12; i++){
            if(board.piece[i] & (1ULL << to)){
                opp_piece = i;
                break;
            }
        }

        algebraic+= symbols[piece];
        algebraic+= 'a' + (from % 8);
        algebraic+= '8' - (from / 8);
        algebraic+= 'x';
        algebraic+= symbols[opp_piece];
        algebraic+= 'a' + (to % 8);
        algebraic+= '8' - (to / 8);
            
        return algebraic; 
    } else if (type == 15){
        int opp_piece = 0;
        for(int i = 0; i < 12; i++){
            if(board.piece[i] & (1ULL << to)){
                opp_piece = i;
                break;
            }
        }

        algebraic+= symbols[piece];
        algebraic+= 'a' + (from % 8);
        algebraic+= '8' - (from / 8);
        algebraic+= 'x';
        algebraic+= symbols[opp_piece];
        algebraic+= 'a' + (to % 8);
        algebraic+= '8' - (to / 8);
            
        return algebraic; 
    }
}