#include "bitboard_moves.hpp"
#include "pch.hpp"

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

