#ifndef MOVE
#define MOVE

#include "piece.hpp"
#include <cstdint>

// This Move struct stores bits for whether the move is a promotion, capture/quiet, en passant, or castle
// If the move type is a promotion (i.e. promotion bit is ON), then the en passant and castle bits (2 MSB) are meaningless and instead are used to store the promoted type
// Thus, to check if a move is an en passant or a castle, make sure the move is not a promotion beforehand
// In addition Calling `promoted_type` on a Move that is not a promotion results in undefined behavior
struct Move{
private:
    static constexpr std::uint16_t src_mask = 63;
    static constexpr std::uint16_t dest_mask = 63 << 6;
    static constexpr std::uint16_t promotion_mask = 3 << 14;
    std::uint16_t move;

public:
    static constexpr std::uint16_t promotion_flag = 1u << 12;
    static constexpr std::uint16_t capture_flag = 1u << 13;
    static constexpr std::uint16_t en_passant_flag = 1u << 14;
    static constexpr std::uint16_t castle_flag = 1u << 15;
    Move() = default;
    Move(int src, int dest, std::uint16_t flags = 0, PieceType promoted_type = PieceType::EMPTY);
    int src();
    int dest();
    bool is_en_passant();
    bool is_promotion();
    PieceType promoted_type();
    bool is_castle();
    bool is_quiet();
    bool is_capture();
};

#endif