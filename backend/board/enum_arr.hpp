#ifndef ARR 
#define ARR
#include <array>
#include <cstddef>


template<typename T, std::size_t size>
struct EnumArr{
private:
    std::array<T, size> data;

public:
    //default constructor provided, and empty move assignment provided as well
};


#endif