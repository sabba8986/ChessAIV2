#ifndef DEFINES
#define DEFINES 

//Indices for each piece in white/black attributes in the Board class
#define PAWN_INDEX 0
#define ROOK_INDEX 1
#define KNIGHT_INDEX 2  
#define BISHOP_INDEX 3
#define QUEEN_INDEX 4  
#define KING_INDEX 5  

//Bitmasks for whether the given knight move can be made from each position
#define KNIGHT_1_VALID 18229723555195321344ULL //2 right, 1 down
#define KNIGHT_2_VALID 71209857637481724ULL //2 right, 1 up
#define KNIGHT_3_VALID 280371153272574ULL //2 up, 1 right
#define KNIGHT_4_VALID 140185576636287ULL //2 up, 1 left
#define KNIGHT_5_VALID 17802464409370431ULL //2 left, 1 up
#define KNIGHT_6_VALID 4557430888798830336ULL //2 left, 1 down
#define KNIGHT_7_VALID 9187201950435704832ULL //2 down, 1 left
#define KNIGHT_8_VALID 18374403900871409664ULL //2 down, 1 right
  
//Sets the initial positions for each piece type
#define INIT_BLACK_ROOKS 9295429630892703744ULL
#define INIT_BLACK_KNIGHTS 4755801206503243776ULL
#define INIT_BLACK_BISHOPS 2594073385365405696ULL
#define INIT_BLACK_QUEEN 1152921504606846976ULL
#define INIT_BLACK_KING 576460752303423488ULL
#define INIT_BLACK_PAWNS 71776119061217280ULL
#define INIT_WHITE_PAWNS 65280
#define INIT_WHITE_KING 8
#define INIT_WHITE_QUEEN 16
#define INIT_WHITE_BISHOPS 36
#define INIT_WHITE_KNIGHTS 66
#define INIT_WHITE_ROOKS 129

//Top left position of the bitboard (only most significant bit toggled on)
#define BITBOARD_TOP_LEFT 9223372036854775808ULL

//Bitmasks for multiple positions on the board
#define EDGE_OF_BOARD 36822278289781621759ULL //right, left, top, bottom edges bitwised ORed
#define BITBOARD_RIGHT_EDGE 72340172838076673ULL
#define BITBOARD_LEFT_EDGE 9259542123273814144ULL
#define BITBOARD_TOP_EDGE 18374686479671623680ULL
#define BITBOARD_BOTTOM_EDGE 255



#define WHITE_PAWN_ROW 65280
#define BLACK_PAWN_ROW 71776119061217280ULL

//Codes for method returns
#define EMPTY_TILE -1
#define INVALID_TILE 6
#define NO_THREAT_DIR 8

#define TURN 1 
#define CHECK 2  
#define CHECKMATE 4

#endif
