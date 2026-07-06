#ifndef BITBOARD
#define BITBOARD
#include <cstdint>
#include <vector>
#include <string>
#include "piece.hpp"


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



    constexpr std::uint64_t up_edge{18374686479671623680ull};
    constexpr std::uint64_t down_edge{255};
    constexpr std::uint64_t right_edge{72340172838076673ull}; 
    constexpr std::uint64_t left_edge{9259542123273814144ull};


    template<Direction dir>
    constexpr std::uint64_t boundary(){
        if constexpr(dir == N) return up_edge;
        else if constexpr(dir == NE) return up_edge | right_edge;
        else if constexpr(dir == E) return right_edge;
        else if constexpr(dir == SE) return down_edge | right_edge;
        else if constexpr(dir == S) return down_edge;
        else if constexpr(dir == SW) return down_edge | left_edge;
        else if constexpr(dir == W) return left_edge;
        else if constexpr(dir == NW) return left_edge | up_edge;
        else return 0;
    }


    constexpr std::uint64_t boundary(Direction dir){
        switch(dir){
            case N: return boundary<N>();
            case NE: return boundary<NE>();
            case E: return boundary<E>();
            case SE: return boundary<SE>();
            case S: return boundary<S>();
            case SW: return boundary<SW>();
            case W: return boundary<W>();
            case NW: return boundary<NW>();
            default: return 0;
        }
    }

    template<int shft>
    constexpr std::uint64_t shift(std::uint64_t pos){
        if constexpr(shft < 0) return pos >> (-1 * shft);
        else return pos << shft;
    }

    constexpr std::uint64_t shift(std::uint64_t pos, int shft){
        return shft < 0 ? pos >> (-1 * shft) : pos << shft;
    }

    template<Direction dir>
    constexpr std::uint64_t slide(std::uint64_t pos){
        if constexpr(dir == N || dir == S) return shift<dir>(pos);
        else return pos & boundary<dir>() ? 0 : shift<dir>(pos);
    }

    constexpr std::uint64_t slide(std::uint64_t pos, Direction dir){
        if(dir == N || dir == S) return shift(pos, dir);
        else return pos & boundary(dir) ? 0 : shift(pos, dir);
    }

    template<Direction dir>
    constexpr std::uint64_t ray(std::uint64_t pos){
        std::uint64_t moves = 0;
        while((pos = slide<dir>(pos))) moves |= pos;
        return moves; 
    }

    constexpr std::uint64_t ray(std::uint64_t pos, Direction dir){
        std::uint64_t moves = 0;
        while((pos = slide(pos, dir))) moves |= pos;
        return moves; 
    }

    template<Direction dir>
    constexpr std::uint64_t internal_slide(std::uint64_t pos){
        return slide<dir>(pos) & ~boundary<dir>();
    }

    constexpr std::uint64_t internal_slide(std::uint64_t pos, Direction dir){
        return slide(pos, dir) & ~boundary(dir);
    }

    template<Direction dir>
    constexpr std::uint64_t internal_ray(std::uint64_t pos){
        return ray<dir>(pos) & ~boundary<dir>();
    }

    constexpr std::uint64_t internal_ray(std::uint64_t pos, Direction dir){
        return ray(pos, dir) & ~boundary(dir);
    }

    constexpr std::vector<Direction> get_directions(PieceType p){
        switch(p){
            case ROOK: return {N, E, S, W};
            case BISHOP: return {NE, SE, SW, NW};
            case KING: return {N, NE, E, SE, S, SW, W, NW};
            case KNIGHT: return {K1, K2, K3, K4, K5, K6, K7, K8};
            default: return {};
        } 
    }


    constexpr std::uint64_t raycast_attack(std::uint64_t pos, PieceType p, std::uint64_t occupancy){
        std::vector<Direction> dirs = get_directions(p);
        std::uint64_t attack = 0;
        for(Direction d: dirs){
            for(std::uint64_t a = slide(pos, d); ; a = slide(a, d)){
                attack |= a;
                if(!a || a & occupancy){
                    break;
                }
            } 
        }
        return attack; 
    }

    [[gnu::used]]
    inline std::string to_string(const std::uint64_t bitboard){
        std::string result;
        result.reserve(78);
        std::uint64_t trav = (1ull << 63);
        for(int i = 0; i < 8; i++){
            for(int j = 0; j < 8; j++, trav >>= 1){
                result += trav & bitboard ? "1" : "0";
            }
            result += "\n";
        }
        return result;
    }

};

#endif
