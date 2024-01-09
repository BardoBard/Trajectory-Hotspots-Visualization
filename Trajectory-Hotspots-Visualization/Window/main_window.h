//
// Created by Bardio - NHL_STENDEN
//
#pragma once
#include "window.h"
#include "file_pick_table_widget.h"

class main_window : public window
{
public:
	/// \brief struct for the description of a window
	struct window_description
	{
		const char* title;
		const char* description;
	};

	struct mouse_binding
	{
		mouse_binding(const ::Qt::KeyboardModifiers modifiers, const ::Qt::MouseButton button,
		              const bool double_click, const QString& description) : modifiers(modifiers), button(button),
		                                                                     double_click(double_click),
		                                                                     description(description)
		{
		}

		::Qt::KeyboardModifiers modifiers;
		::Qt::MouseButton button;
		bool double_click = false;
		QString description;
	};

private:
	typedef window base;
	window_description window_description_;

private:
	void initialize_table_widget(const file_pick_table_widget::t_file_pick_callback& callback, const int width,
	                             const int height);
	void initialize_key_bindings();

public:
	/// \brief generates a help window
	/// \param parent where the window is a child of, usually the screen of the computer
	/// \param file_pick_callback callback function for when the config file is picked
	///	\param description description of the window
	/// \param name name of the window
	/// \param width width, default is 600
	///	\param height height, default is 400
	explicit main_window(QWidget* parent, const file_pick_table_widget::t_file_pick_callback& file_pick_callback,
	                     const window_description& description,
	                     const char* name = "Help", const int width = 600, const int height = 400);

	/// \brief this is an override of the base class function
	/// \return string with the help text
	QString helpString() const override;

	/// \brief this is an override of the base class function
	/// \param title title of the help window
	/// \return string with the help text
	QString helpString(const char* title) const override;

	/// \brief sets the keyboard bindings
	/// \param map map of key bindings
	void setKeyboardDescription(QMap<unsigned int, QString> map)
	{
		keyDescription_ = std::move(map);
	}

	/// \brief sets the key bindings
	/// \param bindings vector of key bindings
	void setMouseDescription(const std::vector<mouse_binding>& bindings);

	void show()
	{
		help();
	}
};
