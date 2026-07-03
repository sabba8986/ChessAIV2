#ifndef MOVE
#define MOVE

#include "piece.hpp"
#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#endif


struct Move{
#ifdef __cplusplus
    static constexpr std::uint16_t from_mask = 63;
    static constexpr std::uint16_t to_mask = 63 << 6;
    static constexpr std::uint16_t promotion_mask = 7 << 13;
    std::uint16_t move;

public:
    static constexpr std::uint16_t promotion_flag = 1ull << 12;
    static constexpr std::uint16_t en_passant_flag = 1u << 13;
    static constexpr std::uint16_t castle_flag = 1u << 14;
    Move() = default;
    Move(int from, int to, PieceType promoted_type);
    Move(int from, int to, std::uint16_t flags = 0);
    int from();
    int to();
    bool is_en_passant();
    bool is_promotion();
    PieceType promoted_type();
    bool is_castle();
#else
    uint16_t move;
#endif
};

#endif