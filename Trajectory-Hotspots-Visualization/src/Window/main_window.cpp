#include <Window/main_window.h>
#include "CGAL/Qt/resources/qglviewer-icon.xpm"

void main_window::initialize_table_widget(const file_pick_table_widget::t_file_pick_callback& callback, const int width,
                                          const int height)
{
	helpWidget_ = new file_pick_table_widget(nullptr, callback);
	static QString label[] = {
		tr("&Help"),
		tr("&Keyboard"),
		tr("&Mouse"),
		tr("&About QT")
	};

	helpWidget_->setWindowTitle(tr("Help", "Help window"));

	for (int i = 0; i < 4; ++i)
	{
		QTextEdit* tab = new QTextEdit(nullptr);
		tab->setReadOnly(true);

		helpWidget_->insertTab(i, tab, label[i]);
		if (i == 3)
		{
			QPixmap pixmap(qglviewer_icon);
			tab->document()->addResource(QTextDocument::ImageResource,
			                             QUrl("mydata://qglviewer-icon.xpm"),
			                             QVariant(pixmap));
		}
	}

	helpWidget_->resize(width, height);
}

void main_window::initialize_key_bindings()
{
	keyboardBinding_.clear(); // remove default bindings

	mouseBinding_.clear(); // remove default bindings
	wheelBinding_.clear();
	clickBinding_.clear();

	mouseDescription_.clear();
	keyDescription_.clear();
}

main_window::main_window(QWidget* parent, const file_pick_table_widget::t_file_pick_callback& file_pick_callback,
                         const window_description& description,
                         const char* name, const int width, const int height) : window_description_(description)
{
	initialize_table_widget(file_pick_callback, width, height);
	initialize_key_bindings();
}

QString main_window::helpString() const
{
	return helpString(window_description_.title);
}

QString main_window::helpString(const char* title) const
{
	return
		"<center>"
		"<h1>" + QString(title) + "</h1>" +
		window_description_.description +
		"<br>"
		"<h3>press <b>ctrl + o</b> and navigate to the config.txt file</h3>"
		"</center>";
}

void main_window::setMouseDescription(const std::vector<mouse_binding>& bindings)
{
	mouseDescription_.clear();

	for (const auto& [modifiers, button, double_click, description] : bindings)
		setMouseBindingDescription(modifiers, button, description, double_click);
}
