#include "Grid.h"

Grid* Grid::instance_ptr = nullptr;

Grid::~Grid(){}

Grid::Grid(double ratio_){
    ratio = ratio_;

    UnhappyLst = {};
    FreeLst = {};

    fill(Empty);
}

Grid& Grid::create(double ratio){
    if (instance_ptr){
        delete instance_ptr;
    }
    instance_ptr = new Grid(ratio);
    return *instance_ptr;
}

State Grid::get_state(int i, int j){return Squares[i][j].state;}
Happiness Grid::get_happiness(int i, int j){return Squares[i][j].happiness;}

void Grid::fill(Method method){
    switch (method){
    case Empty:
        for (int i = 0; i < Height; i++)
            for (int j = 0; j < Width; j++){
                Squares[i][j].state = Neutral;
                Squares[i][j].happiness = Happy;
            }
        break;
    case Random:
        srand(time(NULL));
        for (int i = 0; i < Height; i++)
            for (int j = 0; j < Width; j++){
                if (!(rand()%Config::EmptySquareRatio))
                    Squares[i][j].state = Neutral;
                else
                    Squares[i][j].state = State(rand()%2 + 1);
            }
        break;
    case Chess:
        srand(time(NULL));
        int count = 0;
        for (int i = 0; i < Height; i++){
            for (int j = 0; j < Width; j++){
                Squares[i][j].state = State(count%2 + 1);
                count++;
            }
            count += (Width+1) % 2;
        }
        for (int i = 0; i < Width*Height/Config::EmptySquareRatio; i++){
            Squares[rand()%Height][rand()%Width].state = Neutral;
        }
        // Squares[Height/2][Width/4].state = Squares[Height/2][Width/2+Width/4].state = Neutral;
        break;
    }
    upd_grid();
}

double Grid::upd_sq_happiness(int i, int j){
    if (Squares[i][j].state == Neutral){
        Squares[i][j].happiness = Happy;
        return 0.;
    }

    int own_color_cnt = 0, opposite_color_cnt = 0;
    State own_st = Squares[i][j].state, opposite_st = State(own_st % 2 + 1);

    for (int near_i = i-1; near_i <= i+1; near_i++){
        for (int near_j = j-1; near_j <= j+1; near_j++)
            if ((near_i != i || near_j != j) && 0 <= near_i && near_i < Height && 0 <= near_j && near_j < Width){
                if (Squares[near_i][near_j].state == own_st)
                    own_color_cnt += 1;
                else if (Squares[near_i][near_j].state == opposite_st)
                    opposite_color_cnt += 1;
            }
    }

    if (own_color_cnt || opposite_color_cnt){
        double ratio_ = double(own_color_cnt) / (double(own_color_cnt+opposite_color_cnt));
        Squares[i][j].happiness = Happiness(ratio_ >= ratio);
        return ratio_;
    }
    Squares[i][j].happiness = Happy;
    return 1.;


}

void Grid::upd_grid(){
    int happy_agents = 0, unhappy_agents = 0;
    double sum_ratio = 0.;

    UnhappyLst.clear();
    FreeLst.clear();

    for (int i = 0; i < Height; i++)
        for (int j = 0; j < Width; j++){
            sum_ratio += upd_sq_happiness(i, j);
            if (Squares[i][j].state == Neutral)
                FreeLst.push_back(std::make_tuple(i, j));
            else if (Squares[i][j].happiness == Unhappy){
                UnhappyLst.push_back(std::make_tuple(i, j));
                unhappy_agents++;
            }
            else
                happy_agents++;
        }

    num_agents = happy_agents + unhappy_agents;
    happy_percentage = double(happy_agents) / double(num_agents);
    avg_ratio = sum_ratio / double(num_agents);
}

void Grid::move_rand_agent(){
    srand(time(NULL));
    int from_ind, to_ind;
    from_ind = rand() % UnhappyLst.size();
    to_ind = rand() % FreeLst.size();

    std::tuple<int,int> from_tuple, to_tuple;
    from_tuple = UnhappyLst[from_ind];
    to_tuple = FreeLst[to_ind];

    UnhappyLst.erase(UnhappyLst.begin()+from_ind);
    FreeLst.erase(FreeLst.begin()+to_ind);

    int from_i, from_j, to_i, to_j;
    from_i = std::get<0>(from_tuple);
    from_j = std::get<1>(from_tuple);
    to_i = std::get<0>(to_tuple);
    to_j = std::get<1>(to_tuple);

    Squares[to_i][to_j].state = Squares[from_i][from_j].state;
    Squares[from_i][from_j].state = Neutral;
}

void Grid::do_step(){
    while(!(UnhappyLst.empty() || FreeLst.empty()))
        move_rand_agent();

    upd_grid();
}

void Grid::change_state(int i, int j){
    Squares[i][j].state++;
    upd_grid();
}

void Grid::set_neutral_state(int i, int j){
    Squares[i][j].state = Neutral;
    upd_grid();
}

void Grid::upd_neighbour_happiness(int i, int j){
    for (int near_i = i-1; near_i <= i+1; near_i++)
        for (int near_j = j-1; near_j <= j+1; near_j++)
            upd_sq_happiness(near_i, near_j);
}

std::string Grid::get_str_ratio(){
    std::ostringstream StreamObj;
    StreamObj << std::setprecision(2) << ratio;
    return StreamObj.str();
}

void Grid::set_ratio(double value){
    ratio = value;
    upd_grid();
}

std::string Grid::get_str_happy_percentage(){
    std::ostringstream StreamObj;
    StreamObj << std::setprecision(4) << happy_percentage;
    return StreamObj.str();
}

std::string Grid::get_str_num_agents(){
    return std::to_string(num_agents);
}

std::string Grid::get_str_avg_ratio(){
    std::ostringstream StreamObj;
    StreamObj << std::setprecision(4) << avg_ratio;
    return StreamObj.str();
}
