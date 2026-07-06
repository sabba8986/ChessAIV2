#ifndef MOVE_LIST
#define MOVE_LIST

#include "move.hpp"
#include <cstdint>

struct MoveList {
private:
    std::uint8_t num_moves;
    Move moves[218];
public:
    MoveList();
    void add_move(Move move);
    int size();
    Move operator[](int i);
};

#endif
