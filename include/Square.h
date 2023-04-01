#ifndef SQUARE_H
#define SQUARE_H

enum State{
    Neutral = 0,
    First = 1,
    Second = 2
};
const int Num_states = 3;

State& operator++(State& st, int);

enum Happiness{
    Unhappy = 0,
    Happy = 1
};

class Square{
public:
    Square(State st = State::Neutral, Happiness h = Happy);
    ~Square();

    State state;
    Happiness happiness;
};

#endif // SQUARE_H
