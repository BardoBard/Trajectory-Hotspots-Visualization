#include "window.h"
#include "Trajectory-Hotspots-Wrappers/aabb.h"
#include "Trajectory-Hotspots-Wrappers/trajectory.h"
#include "Trajectory-Hotspots-Wrappers/trapezoidal_map.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	QWidget* active_window = app.activeWindow();
	const std::string window_name = "test";
	const std::string window_name2 = "tm";

	window w = window(active_window, window_name.c_str());
	window tm_window = window(active_window, window_name2.c_str());


	std::vector<Vec2> trajectory_points;

	trajectory_points.emplace_back(10.f, 20.f);
	trajectory_points.emplace_back(10.f, 8.f);
	trajectory_points.emplace_back(12.f, 10.5f);
	trajectory_points.emplace_back(8.f, 14.f);
	trajectory_points.emplace_back(6.f, 16.f);
	trajectory_points.emplace_back(4.f, 20.f);
	trajectory_points.emplace_back(4.f, 24.f);

	const trajectory trajectory(trajectory_points);

	auto ordered_y = trajectory.get_ordered_y_trajectory_segments();
	trapezoidal_map tm = trapezoidal_map(ordered_y);

	const aabb hotspot = trajectory.get_hotspot_fixed_length_contiguous(10.f);

	tm.initialize_leaf_nodes();

	hotspot.draw(w);
	trajectory.draw(w);
	tm.draw(tm_window);
	tm.query_point(Vec2(5, 7), tm_window);
	tm.query_point(Vec2(9, 15), tm_window);

	w.show();
	tm_window.show();
	return app.exec();
}
