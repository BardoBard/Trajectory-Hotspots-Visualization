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
	bool text_visible_{true};

public:
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

	void mousePressEvent(QMouseEvent* e) override
	{
	}

	void mouseDoubleClickEvent(QMouseEvent*) override
	{
	}

	void keyPressEvent(QKeyEvent* e) override;

	void draw(const std::vector<class drawable>& drawables);

	void draw_line(const Segment& segment, const CGAL::IO::Color& color = CGAL::IO::blue());
	void draw_line(const Vec2& lhs, const Vec2& rhs, const CGAL::IO::Color& color = CGAL::IO::blue());

	void draw_point(const Vec2& point, const CGAL::IO::Color& color = CGAL::IO::red(), bool show_text = true);
	void draw_text(const Vec2& point, const std::string& text);
};
