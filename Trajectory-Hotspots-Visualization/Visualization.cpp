#include "window.h"
#include "Trajectory-Hotspots-Wrappers/aabb.h"
#include "Trajectory-Hotspots-Wrappers/trajectory.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QWidget* x = app.activeWindow();
    const std::string window_name = "test";
    
    window* w = new window(x, window_name.c_str());


    std::vector<Vec2> trajectory_points;

    trajectory_points.emplace_back(10.f, 20.f);
    trajectory_points.emplace_back(10.f, 8.f);
    trajectory_points.emplace_back(12.f, 10.5f);
    trajectory_points.emplace_back(8.f, 14.f);
    trajectory_points.emplace_back(6.f, 16.f);
    trajectory_points.emplace_back(4.f, 20.f);
    trajectory_points.emplace_back(4.f, 24.f);

    const trajectory trajectory(trajectory_points);

    const std::vector<Segment>& segments = trajectory.get_ordered_trajectory_segments();
    const aabb hotspot = trajectory.get_hotspot_fixed_length_contiguous(10.f);

    hotspot.draw(*w);
    trajectory.draw(*w);
	
    w->show();
    return app.exec();
}
