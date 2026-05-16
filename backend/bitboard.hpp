#ifndef MOVES
#define MOVES
#include <cstdint>
#include <array>
#include "Piece.hpp"


namespace bitboard{
    enum Direction{
        N = 8, 
        NE = 7, 
        E = -1, 
        SE = -9, 
        S = -8, 
        SW = -7, 
        W = 1, 
        NW = 9,
        K1 = -10, 
        K2 = 6, 
        K3 = 15, 
        K4 = 17,
        K5 = 10, 
        K6 = -6, 
        K7 = -15,
        K8 = -17
    };



    constexpr std::uint64_t up_edge{18374686479671623680u};
    constexpr std::uint64_t down_edge{255};
    constexpr std::uint64_t right_edge{72340172838076673}; 
    constexpr std::uint64_t left_edge{9259542123273814144u};

    template<Direction dir>
    constexpr std::uint64_t boundary(){
        if constexpr(dir == N){
            return up_edge;
        }
        else if constexpr(dir == NE){
            return up_edge | right_edge;
        }
        else if constexpr(dir == E){
            return right_edge;
        }
        else if constexpr(dir == SE){
            return down_edge | right_edge;
        }
        else if constexpr(dir == S){
            return down_edge; 
        }
        else if constexpr(dir == SW){
            return down_edge | left_edge; 
        }
        else if constexpr(dir == W){ 
            return left_edge;
        }
        else{
            return left_edge | up_edge;
        }
    }

    template<Direction dir>
    constexpr std::uint64_t shift(std::uint64_t pos){
        if constexpr(dir < 0){
            return pos & boundary<dir>() ? 0 : pos >> (-1 * dir);
        }
        else{
            return pos & boundary<dir>() ? 0 : pos << dir;
        }
    }

    template<Direction dir> 
    constexpr std::uint64_t ray(std::uint64_t pos){
        std::uint64_t moves = 0;
        while((pos = shift<dir>(pos))) moves |= pos;
        return moves; 
    }

    template<Direction dir>
    constexpr std::uint64_t internal_move(std::uint64_t pos){
        return shift<dir>(pos) & ~boundary<dir>();
    }

    template<Direction dir>
    constexpr std::uint64_t internal_ray(std::uint64_t pos){
        return ray<dir>(pos) & ~boundary<dir>();
    }

    
    template<PieceType p>
    constexpr std::array<Direction, 4> get_directions(){
        static_assert(p == PieceType::ROOK | p == PieceType::BISHOP);
        if constexpr(p == PieceType::ROOK){
            return {N, E, S, W};
        }
        else{
            return {NE, SE, SW, NW};
        }   
    }

    template<PieceType p>
    constexpr std::uint64_t get_attack(std::uint64_t pos, std::uint64_t occupancy){
        static_assert(p == PieceType::ROOK | p == PieceType::BISHOP);
        constexpr auto dirs = get_directions<p>();
        constexpr auto d1 = dirs[0], d2 = dirs[1], d3 = dirs[2], d4 = dirs[3];
        std::uint64_t attack = 0;
        for(std::uint64_t a = shift<d1>(pos); a && !(a & occupancy); a = shift<d1>(a)){
            attack |= a;
        }
        for(std::uint64_t a = shift<d2>(pos); a && !(a & occupancy); a = shift<d2>(a)){
            attack |= a;
        }
        for(std::uint64_t a = shift<d3>(pos); a && !(a & occupancy); a = shift<d3>(a)){
            attack |= a;
        }
        for(std::uint64_t a = shift<d4>(pos); a && !(a & occupancy); a = shift<d4>(a)){
            attack |= a;
        }
        return attack; 
    }
};

#endif
