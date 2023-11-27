//
// Created by Bardio - NHL_STENDEN
//
#pragma once

#include <string>
#include <vector>
#include <CGAL/Qt/Basic_viewer_qt.h>

#include <Trajectory_Hotspots/pch.h>
#include <Trajectory_Hotspots/vec2.h>
#include <Trajectory_Hotspots/Float.h>
#include <Trajectory_Hotspots/AABB.h>
#include <Trajectory_Hotspots/segment.h>
#include <Trajectory_Hotspots/segment_search_tree.h>
#include <Trajectory_Hotspots/trapezoidal_map.h>
#include <Trajectory_Hotspots/trajectory.h>

#include <file_parser.h>

typedef CGAL::Simple_cartesian<double>::Point_2 cgal_point2d;

constexpr auto inf = std::numeric_limits<float>::infinity();
