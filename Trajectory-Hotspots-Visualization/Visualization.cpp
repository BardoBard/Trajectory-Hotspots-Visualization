#include "Window/main_window.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, nullptr);
	QWidget* active_window = QApplication::activeWindow();
	try
	{
		main_window main_window(active_window, "Help");
		main_window.show();

		return app.exec();
	}
	catch (...)
	{
		std::cout << "Something went wrong with the window" << std::endl;
	}
	return 0;
}
