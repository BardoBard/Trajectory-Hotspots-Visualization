//
// Created by Bardio - NHL_STENDEN
//
#pragma once

#include "../Interfaces/drawable.h"

class drawable_segment_search_tree : public Segment_Search_Tree, drawable
{
private:
	void draw_tree(window& w) const;

public:
	typedef Segment_Search_Tree base;
	using Segment_Search_Tree::Segment_Search_Tree;

	static void get_segments(const std::unique_ptr<Segment_Search_Tree_Node>& node,
	                         std::vector<const Segment*>& segments);

	/// \brief draws the segment search tree
	/// \param w window to draw on
	void draw(window& w) const override;
};
