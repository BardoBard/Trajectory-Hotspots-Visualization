//
// Created by Bardio - NHL_STENDEN
//
#pragma once
#include "../Window/window.h"

class window;

/// \brief drawable is an interface for all drawable objects
class drawable
{
public:
	virtual ~drawable() = default; //virtual destructor because we are using inheritance
	/// \brief here we draw the object on the window
	/// \param w window to draw on
	virtual void draw(window& w) const = 0;
};
