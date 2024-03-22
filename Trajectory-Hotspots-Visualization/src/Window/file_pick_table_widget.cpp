#include <Window/file_pick_table_widget.h>

file_pick_table_widget::file_pick_table_widget(QWidget* parent, const t_file_pick_callback& callback): base(parent),
	file_pick_callback_(callback)
{
}

void file_pick_table_widget::set_file_pick_callback(const t_file_pick_callback& callback)
{
	file_pick_callback_ = callback;
}

void file_pick_table_widget::keyPressEvent(QKeyEvent* e)
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
				catch (const parser::parsing_error& e)
				{
					QMessageBox::critical(this, "Error", e.what());
				}
				catch (...)
				{
					QMessageBox::critical(this, "Error",
					                      "Something went wrong while loading the file, make sure you've got the right configuration");
				}
		}
	}
	else
		base::keyPressEvent(e);
}
