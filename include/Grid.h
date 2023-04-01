#ifndef GRID_H
#define GRID_H

#include "time.h"
#include "stdlib.h"

#include "vector"
#include "tuple"

#include <string>
#include <iomanip>

#include "Config.h"
#include "Square.h"

enum Method{
    Empty,
    Random,
    Chess
};

class Grid{
public:
    ~Grid();
    static Grid& create(double ratio);

    void fill(Method method);
    void do_step();

    void change_state(int i, int j);
    void set_neutral_state(int i, int j);

    State get_state(int i, int j);
    Happiness get_happiness(int i, int j);

    std::string get_str_ratio();
    void set_ratio(double value);

    std::string get_str_happy_percentage();
    std::string get_str_num_agents();
    std::string get_str_avg_ratio();

    const int Width = Config::Width;
    const int Height = Config::Height;
private:
    Grid(double ratio_);

    void upd_grid();
    void upd_neighbour_happiness(int i, int j);
    double upd_sq_happiness(int i, int j);
    void upd_sq_state(int i, int j);

    void move_rand_agent();

    std::vector <std::tuple<int, int>> UnhappyLst;
    std::vector <std::tuple<int, int>> FreeLst;

    static Grid* instance_ptr;

    Square Squares[Config::Height][Config::Width];

    int num_agents;
    double ratio, happy_percentage, avg_ratio;
};

#endif // GRID_H
