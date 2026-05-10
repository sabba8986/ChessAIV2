#ifndef MOVES
#define MOVES
#include <cstdint>
#include <array>
#include "Piece.hpp"


namespace moves{
    enum Direction{
        N = 8, 
        NE = 7, 
        E = -1, 
        SE = -9, 
        S = -8, 
        SW = -7, 
        W = 1, 
        NW = 9
    };

    constexpr std::uint64_t up_edge{18374686479671623680u};
    constexpr std::uint64_t down_edge{255};
    constexpr std::uint64_t right_edge{72340172838076673}; 
    constexpr std::uint64_t left_edge{9259542123273814144u};

    template<Direction dir>
    constexpr std::uint64_t edge(){
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
            return pos & edge<dir>() ? 0 : pos >> (-1 * dir);
        }
        else{
            return pos & edge<dir>() ? 0 : pos << dir;
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
        return shift<dir>(pos) & ~edge<dir>();
    }

    template<Direction dir>
    constexpr std::uint64_t internal_ray(std::uint64_t pos){
        return ray<dir>(pos) & ~edge<dir>();
    }

    template<Piece p>
    constexpr std::array<Direction, 4> get_directions(){
        static_assert(p == Piece::ROOK | p == Piece::BISHOP);
        if constexpr(p == Piece::ROOK){
            return {N, E, S, W};
        }
        else {
            return {NE, SE, SW, NW};
        }  
    }

    template<Piece p>
    constexpr std::uint64_t get_attack_from_blockers(std::uint64_t pos, std::uint64_t occupancy){
        static_assert(p == Piece::ROOK | p == Piece::BISHOP);
        constexpr auto dirs = get_directions<p>();
        constexpr auto d1 = dirs[0], d2 = dirs[1], d3 = dirs[2], d4 = dirs[3];
        std::uint64_t attack = 0;
        for(std::uint64_t a = move<d1>(pos); a && !(a & occupancy); a = move<d1>(a)){
            attack |= a;
        }
        for(std::uint64_t a = move<d2>(pos); a && !(a & occupancy); a = move<d2>(a)){
            attack |= a;
        }
        for(std::uint64_t a = move<d3>(pos); a && !(a & occupancy); a = move<d3>(a)){
            attack |= a;
        }
        for(std::uint64_t a = move<d4>(pos); a && !(a & occupancy); a = move<d4>(a)){
            attack |= a;
        }
        return attack; 
    }
};

#endif
