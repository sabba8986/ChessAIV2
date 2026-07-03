#ifndef MOVE_LIST
#define MOVE_LIST

#include "move.hpp"
#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#endif

struct MoveList {
#ifdef __cplusplus
private:
    std::uint8_t num_moves;
    Move moves[218];
public:
    MoveList();
    void add_move(Move move);
#else
    uint8_t num_moves;
    struct Move moves[218];
#endif    
};

#endif
