//
// Created by Bardio - NHL_STENDEN
//
#pragma once

#include "../Interfaces/drawable.h"

class drawable_segment_search_tree : public Segment_Search_Tree, drawable
{
private:
	void breadth_first_search_node(std::vector<int>& result_out,
	                               const std::unique_ptr<Segment_Search_Tree_Node>& node,
	                               window& w, Vec2& offset) const;

	std::vector<int> breadth_first_search(const std::unique_ptr<Segment_Search_Tree_Node>& lhs,
	                                      const std::unique_ptr<Segment_Search_Tree_Node>& rhs, window& w, Vec2& origin) const;
	void draw_tree(window& w) const;

public:
	typedef Segment_Search_Tree base;
	using Segment_Search_Tree::Segment_Search_Tree;

	/// \brief draws the segment search tree
	/// \param w window to draw on
	void draw(window& w) const override;
};
