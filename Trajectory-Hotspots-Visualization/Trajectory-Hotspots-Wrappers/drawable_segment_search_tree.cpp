//
// Created by Bardio - NHL_STENDEN
//
#include "drawable_segment_search_tree.h"

#include "drawable_aabb.h"

void drawable_segment_search_tree::draw_tree(window& w) const
{
	return;
}

void drawable_segment_search_tree::get_segments(const std::unique_ptr<Segment_Search_Tree_Node>& node,
                                                std::vector<const Segment*>& segments)
{
	constexpr int not_end = -1;
	if (node->segment_index == not_end)
	{
		get_segments(node->left, segments);
		get_segments(node->right, segments);
		return;
	}
	segments.push_back(&node->segment_list[node->segment_index]);
}

void drawable_segment_search_tree::draw(window& w) const
{
	draw_tree(w);
}
