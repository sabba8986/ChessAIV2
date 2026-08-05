#include "bitboard_moves.hpp"
#include "pch.hpp"


std::uint64_t bitboard_moves::pawn_captures(int sq, Color c, std::uint64_t enemies){
    using namespace bitboard;
    std::uint64_t pos = 1ull << sq;
    return ((c == Color::WHITE) ? slide<NE>(pos) | slide<NW>(pos) : slide<SE>(pos) | slide<SW>(pos)) & enemies;
}

std::uint64_t bitboard_moves::pawn_en_passant(int sq, Color c, int en_passant_sq){
    return pawn_captures(sq, c, 1ull << en_passant_sq);
}

std::uint64_t bitboard_moves::pawn(int sq, Color c, std::uint64_t allies, std::uint64_t enemies){
    using namespace bitboard;
    constexpr std::uint64_t white_two_square_mask = slide<N>(tables::defaults::bitboards_init[static_cast<int>(Piece::WHITE_PAWN)]);
    constexpr std::uint64_t black_two_square_mask = slide<S>(tables::defaults::bitboards_init[static_cast<int>(Piece::BLACK_PAWN)]);
    std::uint64_t pos = 1ull << sq;
    std::uint64_t occupancy = allies | enemies;
    std::uint64_t forward;
    if(c == Color::WHITE){
        forward = slide<N>(pos) & ~occupancy;
        forward |= slide<N>(forward & white_two_square_mask) & ~occupancy;
    }
    else{
        forward = slide<S>(pos) & ~occupancy;
        forward |= slide<S>(forward & black_two_square_mask) & ~occupancy;
    }
    return forward | pawn_captures(sq, c, enemies);
}

std::uint64_t bitboard_moves::rook(int sq, std::uint64_t allies, std::uint64_t enemies){
    using namespace tables;
    const auto& info = magics::rook[sq];
    std::uint64_t occupancy = (allies | enemies) & info.mask;
    return attacks::rook[magics::get_index_from_magic(occupancy, info)] & ~allies;
}

std::uint64_t bitboard_moves::bishop(int sq, std::uint64_t allies, std::uint64_t enemies){
    using namespace tables;
    const auto& info = magics::bishop[sq];
    std::uint64_t occupancy = (allies | enemies) & info.mask;
    return attacks::bishop[magics::get_index_from_magic(occupancy, info)] & ~allies;
}

std::uint64_t bitboard_moves::queen(int sq, std::uint64_t allies, std::uint64_t enemies){
    return bitboard_moves::bishop(sq, allies, enemies) | bitboard_moves::rook(sq, allies, enemies);
}

std::uint64_t bitboard_moves::knight(int sq, std::uint64_t allies, std::uint64_t enemies){
    return tables::attacks::knight[sq] & ~allies;
}

std::uint64_t bitboard_moves::king(int sq, std::uint64_t allies, std::uint64_t enemies){
    return tables::attacks::king[sq] & ~allies;
}

