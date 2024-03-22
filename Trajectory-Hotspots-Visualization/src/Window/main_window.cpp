#include <Window/main_window.h>
#include "CGAL/Qt/resources/qglviewer-icon.xpm"

void main_window::initialize_table_widget()
{
	helpWidget_ = new file_pick_table_widget(nullptr, callback_);
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

	helpWidget_->resize(600, 400);
}

void main_window::initialize_key_bindings()
{
	keyboardBinding_.clear(); // remove default bindings
	keyDescription_.clear();

	mouseBinding_.clear(); // remove default bindings
	wheelBinding_.clear();
	clickBinding_.clear();
	mouseDescription_.clear();

	//add custom key descriptions
	setKeyDescription(::Qt::Key_L, "Toggles line display");
	setKeyDescription(::Qt::Key_P, "Toggles points display");
	setKeyDescription(::Qt::Key_T, "Toggles text display");
	setKeyDescription(::Qt::Key_Minus, "Decrease camera move speed (for arrow keys)");
	setKeyDescription(::Qt::Key_Plus, "Increase camera move speed (for arrow keys)");
	setKeyDescription(::Qt::Key_Up, "Move up");
	setKeyDescription(::Qt::Key_Down, "Move down");
	setKeyDescription(::Qt::Key_Left, "Move left");
	setKeyDescription(::Qt::Key_Right, "Move right");
	setKeyDescription(::Qt::Key_Q, "Exit");

	//add custom mouse description
	setMouseBindingDescription(::Qt::NoModifier, ::Qt::LeftButton, "Rotate plane");
	setMouseBindingDescription(::Qt::NoModifier, ::Qt::RightButton, "Move camera");
	setMouseBindingDescription(::Qt::NoModifier, ::Qt::LeftButton, "Align plane", true);
	setMouseBindingDescription(::Qt::NoModifier, ::Qt::RightButton, "Show entire scene", true);
	setMouseBindingDescription(::Qt::NoModifier, ::Qt::MiddleButton, "Zoom in on specific part");
	setMouseBindingDescription(::Qt::NoModifier, ::Qt::MiddleButton, "Show entire scene", true);
}

main_window::main_window(QWidget* parent, const char* name) : base(parent, name)
{
	initialize_table_widget();
	initialize_key_bindings();
}

QString main_window::helpString() const
{
	return helpString("Trajectory Hotspots Visualization");
}

QString main_window::helpString(const char* title) const
{
	return
		"<center>"
		"<h1>" + QString(title) + "</h1>"
		"This is a visualization tool for the Hotspots algorithm."
		"<br>"
		"The algorithm is used to find hotspots in a trajectory."
		"<br>"
		"<h3>press <b>ctrl + o</b> and navigate to the config.txt file</h3>"
		"</center>";
}
