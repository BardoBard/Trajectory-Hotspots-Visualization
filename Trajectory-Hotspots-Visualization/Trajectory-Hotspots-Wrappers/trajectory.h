//
// Created by Bardio - NHL_STENDEN
//
#pragma once

#include "drawable.h"

class window;

class trajectory : public Trajectory, drawable
{
public:
	using Trajectory::Trajectory;

	explicit trajectory(const Trajectory& t) : Trajectory(t)
	{
	}

	explicit trajectory(const Trajectory&& t) : Trajectory(t)
	{
	}

	void draw(window& w) const override
	{
		for (const Segment& s : get_ordered_trajectory_segments())
		{
			w.draw_point(s.start);
			w.draw_point(s.end);
			w.draw_line(s.start, s.end);
		}
	}

	aabb get_hotspot_fixed_length_contiguous(const float length) const
	{
		return aabb(Trajectory::get_hotspot_fixed_length_contiguous(length));
	}
};
