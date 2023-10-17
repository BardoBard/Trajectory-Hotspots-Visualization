//
// Created by Bardio - NHL_STENDEN
//
#pragma once

#include "drawable.h"

class window;

class aabb : public AABB, drawable
{
public:
	using AABB::AABB;

	explicit aabb(const AABB& aabb) : AABB(aabb)
	{
	}

	explicit aabb(const AABB&& aabb) : AABB(aabb)
	{
	}

	void draw(window& w) const override
	{
		w.draw_line({min.x, min.y}, {max.x, min.y}, CGAL::IO::black());
		w.draw_line({max.x, min.y}, {max.x, max.y}, CGAL::IO::black());
		w.draw_line({max.x, max.y}, {min.x, max.y}, CGAL::IO::black());
		w.draw_line({min.x, max.y}, {min.x, min.y}, CGAL::IO::black());
	};
};
