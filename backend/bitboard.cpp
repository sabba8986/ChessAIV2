#include "bitboard.hpp"

constexpr std::uint64_t bitboard::boundary(Direction dir){
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

