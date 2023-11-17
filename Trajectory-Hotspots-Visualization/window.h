//
// Created by Bardio - NHL_STENDEN
//
#pragma once
#include "Interfaces/drawable.h"
#ifndef CGAL_USE_BASIC_VIEWER
#define CGAL_USE_BASIC_VIEWER
#endif

class window : public CGAL::Basic_viewer_qt
{
	typedef Basic_viewer_qt base;

private:
	[[nodiscard]] cgal_point2d vec2_to_point(const Vec2& vec) const;
	[[nodiscard]] cgal_point2d vec2_to_point(const float x, const float y) const;
	bool text_visible_{false};

public:
	/// \brief generates a window
	/// \param parent where the window is a child of, usually the screen of the computer
	/// \param name name of the window
	window(QWidget* parent, const char* name = "Window"): base(parent, name)
	{
		base::set_draw_vertices(true);
		base::set_draw_edges(true);
		base::set_draw_faces(true);
		base::set_draw_rays(true);
		base::set_draw_lines(true);
		base::set_draw_text(text_visible_);
	}

	void moveEvent(QMoveEvent* event) override;

	void mouseDoubleClickEvent(QMouseEvent*) override
	{
	}

	/// \brief gets the mouse position relative to the window
	/// \return local mouse position
	[[nodiscard]] Vec2 get_local_mouse_position() const;

private:
	Vec2 prev_mouse_click_position_;

public:
	void mouseReleaseEvent(QMouseEvent* event) override;

	void mousePressEvent(QMouseEvent* event) override;

	void keyPressEvent(QKeyEvent* e) override;

	/**
	 * \brief draws a collection of drawable objects
	 * \param drawables collection of drawable objects
	 */
	void draw(const std::vector<class drawable>& drawables);

	void draw_line(const Segment& segment, const CGAL::IO::Color& color = CGAL::IO::blue());
	void draw_line(const Vec2& start, const Vec2& end, const CGAL::IO::Color& color = CGAL::IO::blue());

	void draw_point(const Vec2& point, const CGAL::IO::Color& color = CGAL::IO::red(), bool show_text = true);

	void draw_box(const Vec2& p1, const Vec2& p2, const Vec2& p3, const Vec2& p4, const CGAL::IO::Color& color);
	void draw_box(const AABB& box, const CGAL::IO::Color& color = CGAL::IO::red());

	void draw_box_filled(const Vec2& p1, const Vec2& p2, const Vec2& p3, const Vec2& p4, const CGAL::IO::Color& color);
	void draw_box_filled(const AABB& box, const CGAL::IO::Color& color = CGAL::IO::red());

	template <typename... Vec2>
		requires (std::is_same_v<Vec2, class Vec2> && ...)
	void draw_filled_area(const CGAL::IO::Color& color, const Vec2&... vecs);

	void draw_text(const Vec2& point, const std::string& text);
};

template <typename... Vec2>
	requires (std::is_same_v<Vec2, class Vec2> && ...)
void window::draw_filled_area(const CGAL::IO::Color& color, const Vec2&... vecs)
{
	if (sizeof...(vecs) == 0)
		return;

	face_begin(color);
	for (const auto& vec : {vecs...})
	{
		// draw_line(vec, CGAL::black());
		base::add_point_in_face(vec2_to_point(vec));
	}
	face_end();
}
