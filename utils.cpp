#include "utils.hpp"



bool operator==(const IntCoords& a, const IntCoords& b){
    return a.x == b.x && a.y == b.y;
}


std::size_t IntCoordsHash::operator()(const IntCoords& c) const noexcept {
        auto h1 = std::hash<int>{}(c.x);
        auto h2 = std::hash<int>{}(c.y);
        return h1 ^ (h2 << 1);
}


