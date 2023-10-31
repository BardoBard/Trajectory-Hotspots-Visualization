//
// Created by Bardio - NHL_STENDEN
//
#pragma once

#include "drawable.h"

class trapezoidal_map : public Trapezoidal_Map, drawable
{
private:
	/// \brief breadth first search to find all leaf nodes
	void breadth_first_search();
	std::vector<const Trapezoidal_Leaf_Node*> leaf_nodes_{};

public:
	typedef Trapezoidal_Map base;
	using Trapezoidal_Map::Trapezoidal_Map;

	/// \brief draws the trapezoidal map
	/// \param w window to draw on
	void draw(window& w) const override;

	/// \brief queries the trapezoidal map for a point and draws the result
	/// \param point point to query
	/// \param w window to draw queried point on
	/// \return leaf node
	Trapezoidal_Leaf_Node* query_point(const Vec2& point, window& w) const;

	//TODO: perhaps move this to ctor
	/// \brief initializes the leaf nodes
	void initialize_leaf_nodes() { breadth_first_search(); }

	/// \brief returns the leaf nodes
	/// \return leaf nodes
	const std::vector<const Trapezoidal_Leaf_Node*>& get_leaf_nodes() { return leaf_nodes_; }
};
