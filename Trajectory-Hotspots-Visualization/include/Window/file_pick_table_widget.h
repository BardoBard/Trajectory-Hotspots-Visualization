//
// Created by Bardio - NHL_STENDEN
//

#pragma once

/// \brief This is a class that inherits from QTabWidget, it is used to pick a file
class file_pick_table_widget : public QTabWidget
{
public:
	typedef std::function<void(const std::string&)> t_file_pick_callback;

private:
	typedef QTabWidget base;

	t_file_pick_callback file_pick_callback_ = nullptr;

public:
	using QTabWidget::QTabWidget; // inherit constructors
	file_pick_table_widget(QWidget* parent, const t_file_pick_callback& callback);

	void set_file_pick_callback(const t_file_pick_callback& callback);

	/// \brief Here we override keypress to open a file
	/// \param e key event
	///	\details This function opens a file dialog when the user presses ctrl + o
	void keyPressEvent(QKeyEvent* e) override;
};
