//
// Created by Bardio - NHL_STENDEN
//
#pragma once

#include "../Interfaces/drawable.h"

class drawable_trapezoidal_map : public Trapezoidal_Map, drawable
{
private:
	/// \brief breadth first search to find all leaf nodes
	std::vector<const Trapezoidal_Leaf_Node*> get_leaf_nodes() const;

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
};
