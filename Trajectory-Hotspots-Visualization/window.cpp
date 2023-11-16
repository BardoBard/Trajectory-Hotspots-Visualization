#include "window.h"

cgal_point2d window::vec2_to_point(const Vec2& vec) const
{
	return cgal_point2d(vec.x.get_value(), vec.y.get_value());
}

cgal_point2d window::vec2_to_point(const float x, const float y) const
{
	return cgal_point2d(x, y);
}

void window::moveEvent(QMoveEvent* event)
{
	switch (event->type())
	{
	case QEvent::Scroll:
		base::moveEvent(event);
		break;
	default:
		break;
	}
}

Vec2 window::get_local_mouse_position() const
{
	//get mouse position
	const QPoint mouse_pos = QCursor::pos();
	//get window pos
	const QPoint window_pos = this->pos();

	//get local mouse position
	const QPoint local_mouse_pos = mouse_pos - window_pos;

	return {static_cast<float>(local_mouse_pos.x()), static_cast<float>(local_mouse_pos.y())};
}

void window::mouseReleaseEvent(QMouseEvent* event)
{
	const auto local_mouse_pos = get_local_mouse_position();
	const auto diff = local_mouse_pos - mouse_press_position_;
	//move
	this->camera()->frame()->translate(camera()->frame()->inverseTransformOf(
		CGAL::qglviewer::Vec( static_cast<double>(diff.x.get_value()) / 10 * camera()->flySpeed(), static_cast<double>(diff.y.get_value()) / 10 * camera()->flySpeed(), 0.0)));
	update();
}

void window::mousePressEvent(QMouseEvent* event)
{
	mouse_press_position_ = get_local_mouse_position();
}

void window::keyPressEvent(QKeyEvent* e)
{
	//arrow keys
	switch (e->key())
	{
	case Qt::Key_Escape:
		close();
		break;

	case Qt::Key_T:
		base::set_draw_text(text_visible_ = !text_visible_);
		redraw();
		break;

	case Qt::Key_Plus:
		camera()->setFlySpeed(camera()->flySpeed() * 1.5);
		break;
	case Qt::Key_Minus:
		camera()->setFlySpeed(camera()->flySpeed() * 0.75);
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

void window::draw(const std::vector<class drawable>& drawables)
{
	for (const auto& drawable : drawables)
		drawable.draw(*this);
}

void window::draw_line(const Segment& segment, const CGAL::IO::Color& color)
{
	draw_line(segment.start, segment.end, color);
}

void window::draw_line(const Vec2& start, const Vec2& end, const CGAL::IO::Color& color)
{
	const auto p1 = vec2_to_point(start);
	const auto p2 = vec2_to_point(end);
	base::add_segment(p1, p2, color);
}

void window::draw_point(const Vec2& point, const CGAL::IO::Color& color, const bool show_text)
{
	const auto p = vec2_to_point(point);
	base::add_point(p, color);

	if (show_text)
		base::add_text(
			p, "(" + std::to_string(static_cast<int>(p.x())) + "," + std::to_string(static_cast<int>(p.y())) + ")");
}

void window::draw_box(const Vec2& p1, const Vec2& p2, const Vec2& p3, const Vec2& p4, const CGAL::IO::Color& color)
{
	draw_line(p1, p2, color);
	draw_line(p2, p3, color);
	draw_line(p3, p4, color);
	draw_line(p4, p1, color);
}

void window::draw_box(const AABB& box, const CGAL::IO::Color& color)
{
	draw_box(box.min, Vec2(box.min.x, box.max.y), box.max, Vec2(box.max.x, box.min.y), color);
}

void window::draw_box_filled(const Vec2& p1, const Vec2& p2, const Vec2& p3, const Vec2& p4,
                             const CGAL::IO::Color& color)
{
	draw_filled_area(color, p1, p2, p3, p4);
}

void window::draw_box_filled(const AABB& box, const CGAL::IO::Color& color)
{
	draw_filled_area(color, box.min, Vec2(box.min.x, box.max.y), box.max, Vec2(box.max.x, box.min.y));
}

void window::draw_text(const Vec2& point, const std::string& text)
{
	const auto p = vec2_to_point(point);
	base::add_text(p, text);
}
