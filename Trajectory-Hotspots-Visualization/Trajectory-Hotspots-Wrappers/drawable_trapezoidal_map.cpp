//
// Created by Bardio - NHL_STENDEN
//
#include "drawable_trapezoidal_map.h"

#include "drawable_aabb.h"

std::vector<const Trapezoidal_Leaf_Node*> drawable_trapezoidal_map::get_leaf_nodes() const
{
	std::queue<const Trapezoidal_Node*> node_queue;
	std::unordered_set<const Trapezoidal_Node*> visited_nodes;
	std::vector<const Trapezoidal_Leaf_Node*> leaf_nodes;

	node_queue.push(root.get());
	while (!node_queue.empty())
	{
		const auto base_ptr = node_queue.front();
		node_queue.pop(); //pop the front node

		// if (visited_nodes.contains(base_ptr)) continue; //TODO: check if this needs to be here

		//check if current node is y
		if (const auto current_node = dynamic_cast<const Trapezoidal_Y_Node*>(base_ptr))
		{
			visited_nodes.insert(current_node);

			if (current_node->above != nullptr && !visited_nodes.contains(current_node->above.get()))
				node_queue.push(current_node->above.get());
			if (current_node->below != nullptr && !visited_nodes.contains(current_node->below.get()))
				node_queue.push(current_node->below.get());
		}
		//check if current node is x
		else if (const auto current_node = dynamic_cast<const Trapezoidal_X_Node*>(base_ptr))
		{
			visited_nodes.insert(current_node);

			if (current_node->left != nullptr && !visited_nodes.contains(current_node->left.get()))
				node_queue.push(current_node->left.get());
			if (current_node->right != nullptr && !visited_nodes.contains(current_node->right.get()))
				node_queue.push(current_node->right.get());
		}
		//check if current node is a leaf node
		else if (const auto current_node = dynamic_cast<const Trapezoidal_Leaf_Node*>(base_ptr))
			leaf_nodes.push_back(current_node);
	}
	return leaf_nodes;
}

Trapezoidal_Leaf_Node* drawable_trapezoidal_map::query_point(const Vec2& point, window& w) const
{
	const Vec2 inf_vec = {inf, inf};
	w.draw_point(point, CGAL::yellow(), true);

	const auto trapezoidal_leaf_node = root->query_point(point);
	if (trapezoidal_leaf_node != nullptr)
	{
		if (trapezoidal_leaf_node->left_segment->start < inf_vec && trapezoidal_leaf_node->left_segment->start > -
			inf_vec)
		{
			Vec2 p = {trapezoidal_leaf_node->left_segment->y_intersect(point.y), point.y};

			w.draw_line({point, p}, CGAL::blue());
		}
		if (trapezoidal_leaf_node->right_segment->start < inf_vec && trapezoidal_leaf_node->right_segment->start > -
			inf_vec)
		{
			Vec2 p = {trapezoidal_leaf_node->right_segment->y_intersect(point.y), point.y};

			w.draw_line({point, p}, CGAL::blue());
		}
	}
	return trapezoidal_leaf_node;
}

void drawable_trapezoidal_map::draw(window& w) const
{
	const auto leaf_nodes = get_leaf_nodes();

	const Vec2 inf_vec = {inf, inf};
	auto min = inf_vec;
	auto max = -inf_vec;
	for (const Trapezoidal_Leaf_Node* const leaf : leaf_nodes)
	{
		const Segment* left = leaf->left_segment;

		if (left->start < inf_vec && left->start > -inf_vec)
		{
			const auto& start = left->start;
			const auto& end = left->end;

			//draw stuff
			w.draw_line({start, end}, CGAL::red());
			w.draw_point(start, CGAL::black(), true);

			query_point(left->start, w);
			query_point(left->start - Vec2{Float::fixed_epsilon * 100, 0}, w); //nudge the point a bit to the left

			if (start.x < min.x)
				min.x = start.x;
			if (start.y < min.y)
				min.y = start.y;
			if (end.x > max.x)
				max.x = end.x;
			if (end.y > max.y)
				max.y = end.y;
		}
	}

	if (min != inf_vec && max != -inf_vec)
	{
		const drawable_aabb bounding_box = {min, max};
		bounding_box.draw(w);
	}
}
