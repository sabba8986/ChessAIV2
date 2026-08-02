#ifndef ARR 
#define ARR
#include <array>
#include <cstddef>


template<typename T, std::size_t N>
struct EnumArr{
private:
    std::array<T, N> data;

public:
    typedef std::array<T, N>::iterator iterator;
    typedef std::array<T, N>::const_iterator const_iterator;

    constexpr EnumArr(const std::array<T, N>& data): data{data}{}
    constexpr EnumArr(const EnumArr<T, N>& other) = default;
    constexpr EnumArr(): data{}{};

    constexpr EnumArr<T, N>& operator=(const std::array<T, N>& arr){
        data = arr;
        return *this;
    }

    constexpr T& operator[](std::size_t i){
        return data[i];
    }

    constexpr const T& operator[](std::size_t i) const{
        return data[i];
    }

    template <typename E>
    requires std::is_enum_v<E>
    constexpr T& operator[](E i){
        return data[to_idx(i)];
    }

    template <typename E>
    requires std::is_enum_v<E>
    constexpr const T& operator[](E i) const{
        return data[to_idx(i)];
    }

    constexpr iterator begin(){ return data.begin();}
    constexpr const_iterator begin() const{ return data.cbegin();}

    constexpr iterator end(){ return data.end();}
    constexpr const_iterator end() const{ return data.cend();}

    constexpr std::array<T, N> to_array(){ return data;}
};


#endif