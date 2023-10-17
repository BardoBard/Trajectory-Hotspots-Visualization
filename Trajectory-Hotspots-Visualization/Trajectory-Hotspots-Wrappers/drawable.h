//
// Created by Bardio - NHL_STENDEN
//
#pragma once
#include "window.h"

class window;

class drawable
{
public:
	virtual void draw(window& w) const = 0;
};
