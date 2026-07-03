#include "bitboard_moves.hpp"
#include "constants.hpp"


std::uint64_t bitboard_moves::pawn_captures(int sq, Color c, std::uint64_t enemies){
    using namespace bitboard;
    std::uint64_t pos = 1ull << sq;
    return ((c == WHITE) ? slide<NE>(pos) | slide<NW>(pos) : slide<SE>(pos) | slide<SW>(pos)) & enemies;
}


std::uint64_t bitboard_moves::pawn_en_passant(int sq, Color c, int en_passant_sq){
    return pawn_captures(sq, c, 1ull << en_passant_sq);
}


std::uint64_t bitboard_moves::pawn(int sq, Color c, std::uint64_t allies, std::uint64_t enemies){
    using namespace bitboard;
    constexpr std::uint64_t white_two_square_mask = slide<N>(defaults::white_init[PAWN]);
    constexpr std::uint64_t black_two_square_mask = slide<S>(defaults::black_init[PAWN]);
    std::uint64_t pos = 1ull << sq;
    std::uint64_t occupancy = allies | enemies;
    std::uint64_t forward;
    if(c == WHITE){
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
    const auto& info = tables::rook_magics[sq];
    std::uint64_t occupancy = (allies | enemies) & info.mask;
    return tables::rook_attacks[tables::get_index_from_magic(occupancy, info)] & ~allies;
}

std::uint64_t bitboard_moves::bishop(int sq, std::uint64_t allies, std::uint64_t enemies){
    const auto& info = tables::bishop_magics[sq];
    std::uint64_t occupancy = (allies | enemies) & info.mask;
    return tables::bishop_attacks[tables::get_index_from_magic(occupancy, info)] & ~allies;
}

std::uint64_t bitboard_moves::queen(int sq, std::uint64_t allies, std::uint64_t enemies){
    return bitboard_moves::bishop(sq, allies, enemies) | bitboard_moves::rook(sq, allies, enemies);
}

std::uint64_t bitboard_moves::knight(int sq, std::uint64_t allies, std::uint64_t enemies){
    return tables::knight_attacks[sq] & ~allies;
}


std::uint64_t bitboard_moves::king(int sq, std::uint64_t allies, std::uint64_t enemies){
    return tables::king_attacks[sq] & ~allies;
}

