//
// Created by Bardio - NHL_STENDEN
//
#include "drawable_trajectory.h"

void drawable_trajectory::draw(window& w) const
{
	for (const Segment& s : get_ordered_trajectory_segments())
	{
		w.draw_point(s.start);
		w.draw_point(s.end);
		w.draw_line(s.start, s.end);
	}
}

std::vector<Segment> drawable_trajectory::get_ordered_y_trajectory_segments() const
{
	const std::vector<Segment>& ordered_segments = get_ordered_trajectory_segments();

	std::vector<Segment> y_segments;
	y_segments.reserve(ordered_segments.size());

	std::set<Float> y_values;
	Segment* prev_seg_t = nullptr;

	for (Segment s : ordered_segments)
	{
		while (y_values.contains(s.start.y))
		{
			s.start.y += 1;
			prev_seg_t->end.y += 1;
		}

		y_values.insert(s.start.y);
		
		s.start.x = s.start_t;
		s.end.x = s.end_t;

		y_segments.push_back(s);
		prev_seg_t = &y_segments.back();
	}
	return y_segments;
}

std::vector<Segment> drawable_trajectory::get_ordered_x_trajectory_segments() const
{
	const std::vector<Segment>& ordered_segments = get_ordered_trajectory_segments();

	std::vector<Segment> x_segments;
	x_segments.reserve(ordered_segments.size());

	for (Segment s : ordered_segments)
	{
		s.start.y = s.start.x;
		s.end.y = s.end.x;

		s.start.x = s.start_t;
		s.end.x = s.end_t;

		x_segments.push_back(s);
	}
	return x_segments;
}
