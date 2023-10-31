//
// Created by Bardio - NHL_STENDEN
//
#include "trapezoidal_map.h"

#include "aabb.h"

void trapezoidal_map::breadth_first_search() {
	leaf_nodes_.clear(); //clear nodes from previous search
	std::queue<const Trapezoidal_Node*> node_queue;
	std::unordered_set<const Trapezoidal_Node*> visited_nodes;

	node_queue.push(root.get());
	while (!node_queue.empty())
	{
		const auto base_ptr = node_queue.front();
		node_queue.pop(); //pop the front node

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
			leaf_nodes_.push_back(current_node);
	}
}
Trapezoidal_Leaf_Node* trapezoidal_map::query_point(const Vec2& point, window& w) const {
	const Vec2 inf_vec = { inf, inf };
	w.draw_point(point, CGAL::yellow(), true);

	const auto trapezoidal_leaf_node = root->query_point(point);
	if (trapezoidal_leaf_node != nullptr)
	{
		if (trapezoidal_leaf_node->left_segment->start < inf_vec && trapezoidal_leaf_node->left_segment->start > -
			inf_vec)
		{
			Vec2 p = { trapezoidal_leaf_node->left_segment->y_intersect(point.y), point.y };

			w.draw_point(p, CGAL::black());
			w.draw_line({ point, p }, CGAL::red());
		}
		if (trapezoidal_leaf_node->right_segment->start < inf_vec && trapezoidal_leaf_node->right_segment->start > -
			inf_vec)
		{
			Vec2 p = { trapezoidal_leaf_node->right_segment->y_intersect(point.y), point.y };

			w.draw_point(p, CGAL::black());
			w.draw_line({ point, p }, CGAL::red());
		}
	}
	return trapezoidal_leaf_node;
}

void trapezoidal_map::draw(window& w) const {
	const Vec2 inf_vec = { inf, inf };
	auto min = inf_vec;
	auto max = -inf_vec;
	for (const auto leaf : leaf_nodes_)
	{
		const auto& s1 = *leaf->left_segment;
		//TODO: remove magic numbers
		if (s1.start < inf_vec && s1.start > -inf_vec)
		{
			const auto& start = s1.start;
			const auto& end = s1.end;
			w.draw_line({ start, end }, CGAL::red());
			w.draw_point(start, CGAL::black(), true);
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
		const aabb bounding_box = { min, max };
		bounding_box.draw(w);
	}
}
