//
// Created by Bardio - NHL_STENDEN
//
#pragma once

#include "drawable_aabb.h"
#include "../Interfaces/drawable.h"

class window;

class drawable_trajectory : public Trajectory, drawable
{
private:
	void breadth_first_search_node(
		const std::unique_ptr<Segment_Search_Tree_Node>& node,
		const AABB*& result,
		Float& length,
		const Float& max_length) const;

public:
	using Trajectory::Trajectory;

	explicit drawable_trajectory(const Trajectory& t) : Trajectory(t.get_ordered_trajectory_segments())
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
	[[nodiscard]] drawable_aabb get_hotspot_fixed_length_contiguous(const float length) const
	{
		return drawable_aabb(Trajectory::get_hotspot_fixed_length_contiguous(length));
	}

	/// \brief this is a naive implementation of fixed radius contiguous, it's not perfectly accurate but it's fast
	/// \verbatim
	/// given size: 20
	///			     [root]
	///			   /       \ (1)
	///			  20       10 <-- 20 will be the result because it has the biggest length within the radius
	///			 /  \     /  \ (2)
	///			5    10  5    5
	///
	/// \endverbatim
	/// \param diameter 2 * radius of the hotspot
	/// \return approximate hotspot of the trajectory with a fixed diameter
	[[nodiscard]] AABB get_approx_hotspot_fixed_diameter_contiguous(const Float& diameter) const;
};
