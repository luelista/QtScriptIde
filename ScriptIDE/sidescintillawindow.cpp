#include "sidescintillawindow.h"

#include <qboxlayout.h>

sIDEScintillaWindow::sIDEScintillaWindow(sIDEDocument *document, QWidget *parent) : sIDEWindow (document, parent)
{

	scintilla = new ScintillaEdit(this);
    QGridLayout *layout = new QGridLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(scintilla);

	//setCentralWidget(scintilla);
    //connect(scintilla, SIGNAL(macroRecord(int,uptr_t,sptr_t)), this, SLOT(on_macroRecord(int,uptr_t,sptr_t)));

	scintilla->setMultipleSelection(true);
	scintilla->setAdditionalSelectionTyping(true);
	scintilla->setSearchFlags(0);
	scintilla->assignCmdKey((SCMOD_CTRL << 16) | 'D', SCI_MULTIPLESELECTADDNEXT);
	scintilla->assignCmdKey((SCMOD_CTRL << 16) | 'E', SCI_SCROLLTOEND);
	scintilla->assignCmdKey(((SCMOD_CTRL|SCMOD_SHIFT) << 16) | 'D', SCI_MULTIPLESELECTADDEACH);

	//macroEngine.installExtensions(QJSEngine::TranslationExtension | QJSEngine::ConsoleExtension);
	//QQmlEngine::setObjectOwnership(scintilla, QQmlEngine::CppOwnership);
}

class PScintillaPipeInput : public IPipeInput<QString> {
public:
	ScintillaEdit *target;

    void onData(const QString &string) {
        QByteArray inUtf8 = string.toUtf8();
        target->addText(inUtf8.size(), (const char*)inUtf8.constData());
	}
	void onEnd() {}
};

void sIDEScintillaWindow::load()
{
	PTextDecoder dec(QTextCodec::codecForName("UTF-8"));
	//PHEXEncoder dec;
	PScintillaPipeInput sciPipe;
	sciPipe.target = scintilla;
	dec.pipeInto(&sciPipe);

	document()->read(&dec);
}
