//
// Created by Bardio - NHL_STENDEN
//
#pragma once

#include "aabb.h"
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

	/// \brief draws the trajectory on the window
	/// \param w window to draw on
	void draw(window& w) const override;

	/// \brief orders the segments of the trajectory by x, it fills this in the y plane
	/// \return ordered vector of segments of the trajectory for x
	[[nodiscard]] std::vector<Segment> get_ordered_x_trajectory_segments() const;

	/// \brief orders the segments of the trajectory by y, it fills this in the y plane
	/// \return ordered vector of segment of the trajectory for y 
	[[nodiscard]] std::vector<Segment> get_ordered_y_trajectory_segments() const;

	/// \brief finds a hotspot of a contiguous trajectory with a fixed length
	/// \param length length of the hotspot
	/// \return hotspot of the trajectory with a fixed length
	[[nodiscard]] aabb get_hotspot_fixed_length_contiguous(const float length) const
	{
		return aabb(Trajectory::get_hotspot_fixed_length_contiguous(length));
	}
};
