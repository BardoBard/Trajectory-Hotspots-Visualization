#include "window.h"

cgal_point2d window::vec2_to_point(const Vec2& vec) const
{
	return cgal_point2d(vec.x.get_value(), vec.y.get_value());
}

cgal_point2d window::vec2_to_point(const float x, const float y) const
{
	return cgal_point2d(x, y);
}

void window::keyPressEvent(QKeyEvent* e)
{
	//arrow keys
	switch (e->key())
	{
	case Qt::Key_T:
		base::set_draw_text(text_visible_ = !text_visible_);
		redraw();
		break;
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

void window::draw(const std::vector<drawable>& drawables)
{
	for (const auto& drawable : drawables)
		drawable.draw(*this);
}

void window::draw_line(const Segment& segment, const CGAL::IO::Color& color)
{
	draw_line(segment.start, segment.end, color);
}

void window::draw_line(const Vec2& lhs, const Vec2& rhs, const CGAL::IO::Color& color)
{
	const auto p1 = vec2_to_point(lhs);
	const auto p2 = vec2_to_point(rhs);
	base::add_segment(p1, p2, color);
}

void window::draw_point(const Vec2& point, const CGAL::IO::Color& color)
{
	trajectory_points_.push_back(point);
	const auto p = vec2_to_point(point);
	base::add_text(p, "(" + std::to_string(int(p.x())) + "," + std::to_string(int(p.y())) + ")");
	base::add_point(p, color);
}
