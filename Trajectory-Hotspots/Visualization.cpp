#include "window.h"
#include <Trajectory_Hotspots/vec2.h>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QWidget* x = app.activeWindow();
    const std::string window_name = "test";
    std::vector<Vec2> vecs = { Vec2(0,1), Vec2(1,0), Vec2(0,-1), Vec2(-1,0) };
    window* w = new window(x, vecs, window_name.c_str());

    w->show();
    return app.exec();
}
