//
// Created by Bardio - NHL_STENDEN
//
#include "drawable_trajectory.h"

#include "drawable_segment_search_tree.h"

void drawable_trajectory::draw(window& w) const
{
	for (const Segment& s : get_ordered_trajectory_segments())
	{
		w.draw_point(s.start);
		w.draw_point(s.end);
		w.draw_line(s.start, s.end, CGAL::blue());
	}
}

std::vector<Segment> drawable_trajectory::get_ordered_y_trajectory_segments() const
{
	const std::vector<Segment>& ordered_segments = get_ordered_trajectory_segments();

	std::vector<Segment> y_segments;
	y_segments.reserve(ordered_segments.size());

	for (Segment s : ordered_segments)
	{
		s.start.x = s.start_t;
		s.end.x = s.end_t;

		y_segments.push_back(s);
	}
	return y_segments;
}

void drawable_trajectory::breadth_first_search_node(const std::unique_ptr<Segment_Search_Tree_Node>& node,
                                                    const AABB*& result,
                                                    Float& length,
                                                    const Float& max_length) const
{
	constexpr int not_end = -1;
	// as long as the node is not a leaf
	if (node->segment_index != not_end)
		return;

	//TODO: not sure if it doesn't check the same node if it already knows the length
	if (node->bounding_box.max_size() > max_length)
	{
		breadth_first_search_node(node->left, result, length, max_length);
		breadth_first_search_node(node->right, result, length, max_length);
		return;
	}

	std::vector<const Segment*> segments{};
	drawable_segment_search_tree::get_segments(node, segments);


	// sum the length of all segments in the node
	const Float total_length = std::accumulate(segments.begin(), segments.end(), Float(0),
	                                           [](const Float& sum, const Segment* segment)
	                                           {
		                                           return sum + segment->length();
	                                           });

	if (total_length > length)
	{
		length = total_length;
		result = &node->bounding_box;
	}
}

AABB drawable_trajectory::get_approx_hotspot_fixed_radius_contiguous(const Float& radius) const
{
	// create segment search tree
	const Segment_Search_Tree tree(get_ordered_trajectory_segments());

	// results
	Float length = 0;
	const AABB* result = nullptr;

	breadth_first_search_node(tree.root.left, result, length, radius * 2);
	breadth_first_search_node(tree.root.right, result, length, radius * 2);

	// return the bounding box of the node with the longest length
	// if no node was found, return an empty AABB
	return result ? *result : AABB{};
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
