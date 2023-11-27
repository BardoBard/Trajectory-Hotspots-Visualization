#include "window.h"
#include "Trajectory-Hotspots-Wrappers/drawable_aabb.h"
#include "Trajectory-Hotspots-Wrappers/drawable_trajectory.h"
#include "Trajectory-Hotspots-Wrappers/drawable_trapezoidal_map.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	QWidget* active_window = QApplication::activeWindow();

	window t_window = window(active_window, "trajectory");
	window tm_window = window(active_window, "trapezoidal map");

	std::vector<Vec2> trajectory_points;

	trajectory_points.emplace_back(10.f, 20.f);
	trajectory_points.emplace_back(10.f, 8.f);
	trajectory_points.emplace_back(12.f, 10.5f);
	trajectory_points.emplace_back(8.f, 14.f);
	trajectory_points.emplace_back(6.f, 16.f);
	trajectory_points.emplace_back(4.f, 20.f);
	trajectory_points.emplace_back(4.f, 24.f);

	const drawable_trajectory trajectory(trajectory_points);

	auto ordered_y = trajectory.get_ordered_y_trajectory_segments();
	trapezoidal_map tm = trapezoidal_map(ordered_y);

	const drawable_aabb hotspot = trajectory.get_hotspot_fixed_length_contiguous(10.f);
	
	hotspot.draw(t_window);
	trajectory.draw(t_window);

	tm.draw(tm_window);
	tm.query_point(Vec2(5, 7), tm_window);
	tm.query_point(Vec2(9, 15), tm_window);

	t_window.show();
	// tm_window.show();
	return app.exec();
}
