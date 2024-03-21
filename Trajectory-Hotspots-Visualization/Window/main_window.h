//
// Created by Bardio - NHL_STENDEN
//
#pragma once
#include "window.h"
#include "file_pick_table_widget.h"
#include "Trajectory-Hotspots-Visualization/Trajectory-Hotspots-Wrappers/drawable_aabb.h"
#include "Trajectory-Hotspots-Visualization/Trajectory-Hotspots-Wrappers/drawable_trajectory.h"
#include "Trajectory-Hotspots-Visualization/Trajectory-Hotspots-Wrappers/drawable_trapezoidal_map.h"

class main_window : public window
{
private:
	typedef window base;

private:
	std::vector<std::unique_ptr<window>> windows_{};

private:
	void initialize_table_widget();
	void initialize_key_bindings();

public:
	/// \brief generates a help window
	/// \param parent where the window is a child of, usually the screen of the computer
	/// \param name name of the window
	explicit main_window(QWidget* parent, const char* name = "Help");

	/// \brief this is an override of the base class function
	/// \return string with the help text
	QString helpString() const override;

	/// \brief this is an override of the base class function
	/// \param title title of the help window
	/// \return string with the help text
	QString helpString(const char* title) const override;

	void show()
	{
		help();
	}

	void closeAllWindows()
	{
		for (const auto& window : windows_)
			window->close();
		windows_.clear();
	}

private:
	file_pick_table_widget::t_file_pick_callback callback_ = [this](const std::string& path)
	{
		std::vector<parser::parsed_trajectory> trajectories = parser::parse_config(path, ' ');
		QWidget* active_window = parentWidget();

		for (parser::parsed_trajectory& parsed_trajectory : trajectories)
		{
			windows_.emplace_back(
				std::make_unique<window>(active_window, ("trapezoidal map - " + parsed_trajectory.name).data()));
			window* tm_window = windows_.back().get();

			windows_.emplace_back(
				std::make_unique<window>(active_window, ("trajectory- " + parsed_trajectory.name).data()));
			window* t_window = windows_.back().get();

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
				hotspot.draw(*t_window);
			if (approx_hotspot.min != Vec2{0, 0} && approx_hotspot.max != Vec2{0, 0})
				approx_hotspot.draw(*t_window);

			//trapezoidal map
			auto ordered_y = drawable_traject.get_ordered_y_trajectory_segments();
			drawable_trapezoidal_map tm = drawable_trapezoidal_map(ordered_y);
			tm.draw(*tm_window);

			//show windows
			t_window->show();
			tm_window->show();
		}
	};
};
