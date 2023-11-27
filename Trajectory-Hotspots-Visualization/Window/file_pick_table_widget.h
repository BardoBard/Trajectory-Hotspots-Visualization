//
// Created by Bardio - NHL_STENDEN
//
#pragma once

class file_pick_table_widget : public QTabWidget
{
public:
	typedef std::function<void(const std::string&)> t_file_pick_callback;

private:
	typedef QTabWidget base;

	t_file_pick_callback file_pick_callback_ = nullptr;

public:
	using QTabWidget::QTabWidget; // inherit constructors
	file_pick_table_widget(QWidget* parent, const t_file_pick_callback& callback) : base(parent),
		file_pick_callback_(callback)
	{
	}

	void set_file_pick_callback(const t_file_pick_callback& callback)
	{
		file_pick_callback_ = callback;
	}

	// override so we can handle key presses
	void keyPressEvent(QKeyEvent* e) override
	{
		// choose file
		if (e->modifiers() == ::Qt::KeyboardModifier::ControlModifier && e->key() == ::Qt::Key_O)
		{
			const QString file_name = QFileDialog::getOpenFileName(this, "Open a file", QDir::homePath(),
			                                                       "Config file (config.txt) ;; Config file (*.txt)");
			if (!file_name.isEmpty())
			{
				const std::string file_path = file_name.toStdString();

				if (file_pick_callback_)
					try
					{
						file_pick_callback_(file_path);
					}
					catch (const std::exception& e)
					{
						QMessageBox::critical(this, "Error", e.what());
					}
					catch (...)
					{
						QMessageBox::critical(this, "Error", "Something went wrong while loading the file");
					}
			}
		}
		else
			base::keyPressEvent(e);
	}
};
