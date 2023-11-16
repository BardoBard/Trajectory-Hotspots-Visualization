#include "window.h"
#include "Trajectory-Hotspots-Wrappers/drawable_aabb.h"
#include "Trajectory-Hotspots-Wrappers/drawable_trajectory.h"
#include "Trajectory-Hotspots-Wrappers/drawable_trapezoidal_map.h"


int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cerr << "Please provide a file path to the config file" << std::endl;
		return 0;
	}
	const std::string path = argv[1];

	try
	{
		QApplication app(argc, argv);
		QWidget* active_window = QApplication::activeWindow();

		const std::vector<parser::parsed_trajectory> trajectories = parser::parse_config(path, ' ');

		for (const auto& parsed_trajectory : trajectories)
		{
			window tm_window = window(active_window, "trapezoidal map");
			window t_window = window(active_window, "trajectory");

			const drawable_trajectory drawable_trajectory(parsed_trajectory.trajectory);
			auto ordered_y = drawable_trajectory.get_ordered_y_trajectory_segments();

			drawable_trapezoidal_map tm = drawable_trapezoidal_map(ordered_y);

			const drawable_aabb hotspot(parsed_trajectory.run_trajectory_function());

			hotspot.draw(t_window);
			drawable_trajectory.draw(t_window);
			std::cout << ordered_y.size() << std::endl;
			std::cout << "hotspot: " << hotspot.min.x.get_value() << ", " << hotspot.min.y.get_value() << " - "
				<< hotspot.max.x.get_value() << ", " << hotspot.max.y.get_value() << std::endl;

			tm.draw(tm_window);
			tm.query_point({10, 4.5}, tm_window);

			t_window.show();
			tm_window.show();
			app.exec();
		}
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	}
	return 0;
}
