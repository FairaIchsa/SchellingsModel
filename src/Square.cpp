#include "Square.h"

State& operator++(State& st, int){
    return st = State((st) % 2 + 1);
}

Square::~Square(){}

Square::Square(State st, Happiness h)
{
    state = st;
    happiness = h;
}
