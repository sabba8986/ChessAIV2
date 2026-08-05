#ifndef PERFT_RESULTS
#define PERFT_RESULTS
#include <iostream>

struct PerftResults{
    unsigned long long nodes = 0;
    unsigned long long captures = 0;
    unsigned long long en_passants = 0;
    unsigned long long castles = 0;
    unsigned long long promotions = 0;
    unsigned long long checks = 0;
};


inline std::ostream& operator<<(std::ostream& os, const PerftResults& stats){
    os << "{\n";
    os << "\tnodes: " << stats.nodes << "\n";
    os << "\tcaptures: " << stats.captures << "\n";
    os << "\ten_passants: " << stats.en_passants << "\n";
    os << "\tcastles: " << stats.castles << "\n";
    os << "\tpromotions: " << stats.promotions << "\n";
    os << "\tchecks: " << stats.checks << "\n}";
    return os;
}


#endif