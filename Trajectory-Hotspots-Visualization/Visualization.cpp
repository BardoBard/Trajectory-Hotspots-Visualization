#include "Trajectory-Hotspots-Wrappers/drawable_trajectory.h"
#include "Trajectory-Hotspots-Wrappers/drawable_trapezoidal_map.h"
#include "Window/window.h"
#include "Window/main_window.h"

QMap<unsigned int, QString> keyboard_bindings{
	{::Qt::Key_L, QString("Toggles line display")},
	{::Qt::Key_P, QString("Toggles points display")},
	{::Qt::Key_T, QString("Toggles text display")},
	{::Qt::Key_Minus, QString("Decrease camera move speed (for arrow keys)")},
	{::Qt::Key_Plus, QString("Increase camera move speed (for arrow keys)")},
	{::Qt::Key_Up, QString("Move up")},
	{::Qt::Key_Down, QString("Move down")},
	{::Qt::Key_Left, QString("Move left")},
	{::Qt::Key_Right, QString("Move right")},
	{::Qt::Key_Q, QString("Exit")}
};
std::vector<main_window::mouse_binding> mouse_bindings{
	main_window::mouse_binding(Qt::NoModifier, Qt::LeftButton, false, QString("Rotate plane")),
	main_window::mouse_binding(Qt::NoModifier, Qt::RightButton, false, QString("Move camera")),
	main_window::mouse_binding(Qt::NoModifier, Qt::LeftButton, true, QString("Align plane")),
	main_window::mouse_binding(Qt::NoModifier, Qt::RightButton, true, QString("Show entire scene")),
	main_window::mouse_binding(Qt::NoModifier, Qt::MiddleButton, false, QString("Zoom in on specific part")),
	main_window::mouse_binding(Qt::NoModifier, Qt::MiddleButton, true, QString("Show entire scene"))
};

std::vector<std::unique_ptr<window>> windows{};

static inline file_pick_table_widget::t_file_pick_callback visualization_callback = [&](const std::string& path)
{
	std::vector<parser::parsed_trajectory> trajectories = parser::parser::parse_config(path, ' ');
	QWidget* active_window = QApplication::activeWindow();

	for (parser::parsed_trajectory& parsed_trajectory : trajectories)
	{
		windows.emplace_back(
			std::make_unique<window>(active_window, ("trapezoidal map - " + parsed_trajectory.name).data()));
		window* tm_window = windows.back().get();

		windows.emplace_back(
			std::make_unique<window>(active_window, ("trajectory- " + parsed_trajectory.name).data()));
		window* t_window = windows.back().get();


		//trajectory
		const drawable_trajectory drawable_traject(parsed_trajectory.trajectory);
		drawable_traject.draw(*t_window);

		//hotspots
		const drawable_aabb hotspot(parsed_trajectory.run_trajectory_function());
		const drawable_aabb approx_hotspot(
			drawable_trajectory(parsed_trajectory.trajectory).get_approx_hotspot_fixed_radius_contiguous(
				parsed_trajectory.length));

		//draw hotspots
		if (hotspot.min != Vec2{0, 0} && hotspot.max != Vec2{0, 0})
		{
			hotspot.draw(*t_window);
			std::cout << "hotspot: " << hotspot.min.x.get_value() << " " << hotspot.min.y.get_value() << " "
				<< hotspot.max.x.get_value() << " " << hotspot.max.y.get_value() << std::endl;
		}
		if (approx_hotspot.min != Vec2{0, 0} && approx_hotspot.max != Vec2{0, 0})
		{
			// approx_hotspot.draw(*t_window);
			std::cout << "approx hotspot: " << approx_hotspot.min.x.get_value() << " "
				<< approx_hotspot.min.y.get_value() << " " << approx_hotspot.max.x.get_value() << " "
				<< approx_hotspot.max.y.get_value() << std::endl;
		}
		//trapezoidal map
		auto ordered_y = drawable_traject.get_ordered_y_trajectory_segments();
		drawable_trapezoidal_map tm = drawable_trapezoidal_map(ordered_y);
		tm.draw(*tm_window);

		//show windows
		t_window->show();
		tm_window->show();
	}
};

int main(int argc, char* argv[])
{
	QApplication app(argc, nullptr);
	QWidget* active_window = QApplication::activeWindow();
	try
	{
		main_window::window_description description = {
			"Trajectory Hotspots Visualization",
			"This is a visualization tool for the Hotspots algorithm. <br> The algorithm is used to find hotspots in a trajectory."
		};

		main_window main_window(active_window, visualization_callback, description, "Help");
		main_window.setKeyboardDescription(keyboard_bindings);
		main_window.setMouseDescription(mouse_bindings);
		main_window.show();

		app.exec();
	}
	catch (...)
	{
		std::cout << "Something went wrong with the window" << std::endl;
	}

	for (auto& window : windows)
		window.release();

	windows.clear();

	return 0;
}
