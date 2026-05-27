#include "attacks.hpp"
#include "constants.hpp"

std::uint64_t attacks::rook_attacks(int sq, std::uint64_t allies, std::uint64_t enemies){
    const auto& info = tables::rook_magics[sq];
    std::uint64_t occupancy = (allies | enemies) & info.mask;
    return tables::rook_attacks[tables::get_index_from_magic(occupancy, info)] & ~allies;
}

std::uint64_t attacks::bishop_attacks(int sq, std::uint64_t allies, std::uint64_t enemies){
    const auto& info = tables::bishop_magics[sq];
    std::uint64_t occupancy = (allies | enemies) & info.mask;
    return tables::bishop_attacks[tables::get_index_from_magic(occupancy, info)] & ~allies;
}

std::uint64_t attacks::queen_attacks(int sq, std::uint64_t allies, std::uint64_t enemies){
    return attacks::bishop_attacks(sq, allies, enemies) | attacks::rook_attacks(sq, allies, enemies);
}

std::uint64_t attacks::knight_attacks(int sq, std::uint64_t allies, std::uint64_t enemies){
    return tables::knight_attacks[sq] & ~allies;
}

//no check functionality added yet
std::uint64_t attacks::king_attacks(int sq, std::uint64_t allies, std::uint64_t enemies){
    return tables::king_attacks[sq] & ~allies;
}




