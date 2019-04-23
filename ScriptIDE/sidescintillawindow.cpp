#include "sidescintillawindow.h"

sIDEScintillaWindow::sIDEScintillaWindow(sIDEDocument *document, QWidget *parent) : sIDEWindow (document, parent)
{

	scintilla = new ScintillaEdit(this);
	//setCentralWidget(scintilla);
	connect(scintilla, SIGNAL(macroRecord(int,uptr_t,sptr_t)), this, SLOT(on_macroRecord(int,uptr_t,sptr_t)));

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

	void onData(const QString &bytes) {
		target->addText(bytes.size(), (const char*)bytes.constData());
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
