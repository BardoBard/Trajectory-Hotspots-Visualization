//
// Created by Bardio - NHL_STENDEN
//
#include "drawable_segment_search_tree.h"

#include "drawable_aabb.h"

constexpr int c_offset = 15;

void drawable_segment_search_tree::breadth_first_search_node(
	std::vector<int>& result_out,
	const std::unique_ptr<Segment_Search_Tree_Node>& node,
	window& w, Vec2& offset) const
{
	constexpr int not_end = -1;
	if (node->segment_index == not_end)
	{
		std::vector<int> bfs_result = breadth_first_search(node->left, node->right, w, offset);
		std::vector<Segment> segments;

		offset.y += c_offset;
		for (const int i : bfs_result)
		{
			Segment s = node->segment_list[i];
			// w.draw_text(
			// 	s.end + offset, std::to_string((int)s.end.x.get_value()) + " " + std::to_string((int)s.end.y.get_value()));

			// int local_offset = node->left->segment_index == not_end ? 0 : 15;
			s.start += offset - Vec2(c_offset * 1.5, 0);
			s.end += offset - Vec2(c_offset * 1.5, 0);
			// w.draw_text(
			// 	s.start, std::to_string((int)offset.x.get_value()) + " " + std::to_string((int)offset.y.get_value()));
			w.draw_line(s);
		}
		drawable_aabb box = drawable_aabb(offset - Vec2(c_offset * 1.5, 0), offset + Vec2(-c_offset/2, c_offset));
		box.draw(w);

		if (node->left)
			w.draw_line(offset - Vec2(c_offset * 2, 0), offset - Vec2(c_offset, -c_offset), CGAL::black());
		if (node->right)
			w.draw_line(offset, offset - Vec2(c_offset, -c_offset), CGAL::black());
		result_out.insert(result_out.end(), bfs_result.begin(), bfs_result.end());
	}
	else //at the end
	{
		Segment s = node->segment_list[node->segment_index];

		s.start += offset;
		s.end += offset;

		// drawable_aabb(offset - Vec2(c_offset , c_offset ), offset + Vec2(c_offset * 1.5, c_offset * 1.5)).draw(w);
		// w.draw_text(
		// 	s.start, std::to_string((int)offset.x.get_value()) + " " + std::to_string((int)offset.y.get_value()));

		w.draw_line(s);

		offset.x += c_offset;
		result_out.push_back(node->segment_index);
	}
}

std::vector<int> drawable_segment_search_tree::breadth_first_search(
	const std::unique_ptr<Segment_Search_Tree_Node>& lhs,
	const std::unique_ptr<Segment_Search_Tree_Node>& rhs,
	window& w, Vec2& origin) const
{
	std::vector<int> result;

	breadth_first_search_node(result, lhs, w, origin);

	breadth_first_search_node(result, rhs, w, origin);

	return result;
}

void drawable_segment_search_tree::draw_tree(window& w) const
{
	Vec2 origin = {0, 0};
	std::vector<int> result;
	breadth_first_search_node(result, root.left, w, origin);
	result.clear();
	origin.y += c_offset;
	for (const auto& s : root.segment_list)
		w.draw_line((s.start + origin), (s.end + origin));
	if (root.left)
		w.draw_line(origin - Vec2(c_offset * 2, 0), origin - Vec2(c_offset, -c_offset), CGAL::black());

	origin.y = 0;
	// origin.x += c_offset;
	breadth_first_search_node(result, root.right, w, origin);
	if (root.right)
		w.draw_line(origin, origin - Vec2(c_offset, -c_offset), CGAL::black());
}

void drawable_segment_search_tree::draw(window& w) const
{
	draw_tree(w);
	// std::vector<Segment> segments;
	// for (const int i : bfs_result)
	// {
	// 	Segment s = root.segment_list[i];
	// 	w.draw_line(s);
	// }
}
