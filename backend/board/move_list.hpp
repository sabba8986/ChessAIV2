#ifndef MOVE_LIST
#define MOVE_LIST

#include "move.hpp"
#include <cstdint>
#include <array>

struct MoveList {
private:
    std::uint8_t m_num_moves;
    std::array<Move, 256> m_moves;
public:
    MoveList();
    void add_move(Move move);
    int size();
    Move operator[](int i);
};

#endif
