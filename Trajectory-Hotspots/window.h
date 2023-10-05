#pragma once

#include <string>
#ifndef CGAL_USE_BASIC_VIEWER
#define CGAL_USE_BASIC_VIEWER
#endif

#include <CGAL/Qt/Basic_viewer_qt.h>

class window : public CGAL::Basic_viewer_qt
{
private:
public:
    window(QWidget* parent, const std::string& name): Basic_viewer_qt(parent, name.c_str())
    {
    }

    ~window()
    {
    }
};
