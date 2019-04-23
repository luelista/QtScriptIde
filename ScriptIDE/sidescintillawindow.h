#ifndef SIDESCINTILLAWINDOW_H
#define SIDESCINTILLAWINDOW_H

#include "sidewindow.h"

#include <QObject>
#include <ScintillaEdit.h>

class sIDEScintillaWindow : public sIDEWindow
{
public:
	explicit sIDEScintillaWindow(sIDEDocument *document, QWidget *parent = nullptr);
	ScintillaEdit *scintilla;
	Q_PROPERTY(ScintillaEdit *scintilla MEMBER scintilla)

	virtual void load();
};

#endif // SIDESCINTILLAWINDOW_H
