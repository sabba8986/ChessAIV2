#pragma once
#include <cstdint>


enum class GameState: std::uint8_t{
    ONGOING,
    CHECKMATE,
    STALEMATE,
    DRAW_HUNDRED_MOVE_CLOCK,
    DRAW_LACK_MATERIAL,
    DRAW_REPEATED_POSITION
};
