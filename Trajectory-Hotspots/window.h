#pragma once

#ifndef CGAL_USE_BASIC_VIEWER
#define CGAL_USE_BASIC_VIEWER
#endif

#include <string>
#include <CGAL/Qt/Basic_viewer_qt.h>
#include <Trajectory_Hotspots/pch.h>
#include <Trajectory_Hotspots/vec2.h>

class window : public CGAL::Basic_viewer_qt
{
	typedef Basic_viewer_qt base;

private:
	std::vector<Vec2>& vecs_;

public:
	window(QWidget* parent, std::vector<Vec2>& vec, const char* name = "Window"): base(parent, name), vecs_(vec)
	{
		base::set_draw_vertices(true);
		compute_elements();
	}

	~window()
	{
	}

	void compute_elements()
	{
		clear();
		for (auto& vec : vecs_)
		{
			CGAL::Simple_cartesian<double>::Point_2 p(vec.x.get_value(), vec.y.get_value());
			// auto p = CGAL::Point_3<CGAL::Epick>(vec.x.get_value(), vec.y.get_value(), 1);
			add_point(p, CGAL::IO::red());
		}
	}

	virtual void keyPressEvent(QKeyEvent* e)
	{
		//arrow keys
		switch (e->key())
		{
		case Qt::Key_Right:
		case Qt::Key_Up:
		case Qt::Key_Down:
		case Qt::Key_Left:
			base::keyPressEvent(e);
			break;
		default:
			break;
		}

		// base::keyPressEvent(e);
	}
};
