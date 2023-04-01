#include "MainWindow.h"

MainWindow::MainWindow(){
    pause = true;
    tolerance = 4;

    Grid_ = &Grid::create(0.2);

    // RGB(0.4,0.94,0.96); Cyan
    // RGB(1,0.55,0); // Orange
    Colors[0] = RGB(1,1,1); // White
    Colors[1] = RGB(0,1,0); // Green
    Colors[2] = RGB(0,0,1); // Blue
}

MainWindow::~MainWindow(){
    delete Grid_;
}


void MainWindow::OnCreate(){
    std::cout << "MainWindow::OnCreate()" << std::endl;

    AddChild(new Button("|| / >",Pause), Point(50,40), Point(100,40));
    AddChild(new Button("Clear",Clear), Point(250,40), Point(100,40));
    AddChild(new Button("Random",Random_fill), Point(375,40), Point(100,40));
    AddChild(new Button("Chess",Chess_fill), Point(500,40), Point(100,40));
    AddChild(new Button("--",Decrease_ratio), Point(700,40), Point(50,40));
    AddChild(new Button("+",Increase_ratio), Point(800,40), Point(50,40));

    CreateTimeout(20);
}

void MainWindow::OnSizeChanged(){
    std::cout << "MainWindow::OnSizeChanged()" << std::endl;

    Point size_ = GetSize();

    upper_padding = 100;
    padding = 10;

    uint16_t sc_height, sc_width;
    sc_height = size_.GetY() - padding - upper_padding;
    sc_width = size_.GetX()- padding * 2;

    sq_side = std::min(sc_height/Grid_->Height, sc_width/Grid_->Width);
    padding = (size_.GetX() - sq_side*Grid_->Width) / 2;

    std::cout << "sc_width: " << sc_width << " sc_height: " << sc_height <<  " sc_side: " << sq_side << std::endl;
}

void MainWindow::DrawSquare(Context *cr, int i, int j){
    uint16_t  x, y;
    x = padding + j*sq_side;
    y = upper_padding + i*sq_side;
    cr->SetColor(Colors[Grid_->get_state(i, j)]);

    switch (Grid_->get_happiness(i, j)){
    case Unhappy:
        cr->SetLineWidth(3);
        cr->Line(Point(x, y), Point(x+sq_side, y+sq_side));
        cr->Line(Point(x, y+sq_side), Point(x+sq_side, y));
        break;
    case Happy:
        cr->FillRectangle(Point(x, y), Point(sq_side, sq_side));
        break;
    }
}

void MainWindow::DrawGrid(Context *cr){
    for (int i = 0; i < Grid_->Height; i++)
        for (int j = 0; j < Grid_->Width; j++)
            DrawSquare(cr, i, j);
}

void MainWindow::OnDraw(Context *cr){
    std::cout << "MainWindow::OnDraw()" << std::endl;

    cr->SetColor(RGB(1,1,1));
    cr->FillRectangle(Point(0,0), GetSize());

    cr->SetColor(RGB(0,0,0));

    cr->Text("Ratio:", "arial", 20, Point(775, 30), 0x0);
    cr->Text(Grid_->get_str_ratio().c_str(), "arial", 20, Point(775, 60), 0x0);

    cr->Text("Avg ratio:", "arial", 20, Point(900, 30), 0x0);
    cr->Text(Grid_->get_str_avg_ratio().c_str(), "arial", 20, Point(900, 60), 0x0);

    cr->Text("Happy:", "arial", 20, Point(1050, 30), 0x0);
    cr->Text(Grid_->get_str_happy_percentage().c_str(), "arial", 20, Point(1050, 60), 0x0);

    cr->Text("Num agents:", "arial", 20, Point(1200, 30), 0x0);
    cr->Text(Grid_->get_str_num_agents().c_str(), "arial", 20, Point(1200, 60), 0x0);

    DrawGrid(cr);
}

bool MainWindow::OnLeftMouseButtonClick(const Point &position){
    std::cout << "MainWindow::OnLeftMouseButtonClick()" << std::endl;

    int i, j;
    i = (position.GetY()-upper_padding) / sq_side;
    j = (position.GetX()-padding) / sq_side;
    Grid_->change_state(i, j);
    ReDraw();
	return true;
}

bool MainWindow::OnRightMouseButtonClick(const Point &position){
    std::cout << "MainWindow::OnRightMouseButtonClick()" << std::endl;

    int i, j;
    i = (position.GetY()-upper_padding) / sq_side;
    j = (position.GetX()-padding) / sq_side;
    Grid_->set_neutral_state(i, j);
    ReDraw();
	return true;
}

void MainWindow::OnNotify(Window *child, uint32_t type, const Point &position){
    std::cout << "MainWindow::OnNotify()" << std::endl;

    switch (type){
    case Pause:
        pause = !pause;
        break;
    case Clear:
        Grid_->fill(Empty);
        ReDraw();
        break;
    case Random_fill:
        Grid_->fill(Random);
        ReDraw();
        break;
    case Chess_fill:
        Grid_->fill(Chess);
        ReDraw();
        break;
    case Increase_ratio:
        tolerance = std::max(0, std::min(20, tolerance+1));
        Grid_->set_ratio(double(tolerance)/20);
        ReDraw();
        break;
    case Decrease_ratio:
        tolerance = std::max(0, std::min(20, tolerance-1));
        Grid_->set_ratio(double(tolerance)/20);
        ReDraw();
        break;
    }
}

bool MainWindow::OnKeyPress(uint64_t value){
    std::cout << "MainWindow::OnKeyPress(): " << value << std::endl;

    switch (value){
    case ' ':
        pause = !pause;
        break;
    case 65307: // Esc
        DeleteMe();
        break;
    default:
        ;
    }

    return 1;
}

bool MainWindow::OnTimeout(){
    // std::cout << "MainWindow::OnTimeout()" << std::endl;

    if (!pause){
        Grid_->do_step();
        ReDraw();
    }

    return 1;
}
