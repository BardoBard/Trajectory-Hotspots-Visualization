#include <Window/main_window.h>

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
	catch (const parser::parsing_error& e)
	{
		std::cerr << "Error parsing the file: " << e.what() << std::endl;
	}
	catch (...)
	{
		std::cerr << "Something went wrong with the window" << std::endl;
	}
	return 0;
}
