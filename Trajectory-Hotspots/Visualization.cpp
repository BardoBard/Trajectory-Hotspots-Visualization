#include "window.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QWidget* x = app.activeWindow();
    const std::string window_name = "test";
    window* w = new window(x, window_name);

    w->show();
    return app.exec();
}
