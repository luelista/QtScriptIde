#include "macrorecorder.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSettings>
#include <QTextEdit>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonDocument>
#include <QQmlEngine>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	macroEngine.installExtensions(QJSEngine::TranslationExtension | QJSEngine::ConsoleExtension);
	QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
	macroEngine.globalObject().setProperty("window", macroEngine.newQObject(this));

	scDebug = createScDockWindow("dw_DebugOutput", "Debug Output");
	scMacro = createScDockWindow("dw_Macro", "Macro Recorder");
	QFile f("macros.txt");
	if (f.open(QIODevice::ReadOnly)) {
		QByteArray bytes = f.readAll();
		scMacro->appendText(bytes.length(), bytes.data());
	}

	//QQmlEngine::setObjectOwnership(scDebug, QQmlEngine::CppOwnership);
	//macroEngine.globalObject().setProperty("scDebug", macroEngine.newQObject(scDebug));

	connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));

	readSettings();

	on_actionNew_Script_triggered();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::setCurrentDocument(sIDEDocument *doc)
{
	/*if (currentDocument == doc) return;

	currentDocument = doc;
	setWindowTitle(doc->filespec);
	scintilla->setDocPointer((sptr_t)doc->pointer());
	updateDocumentList();
	scintilla->targetWholeDocument();**/
}

void MainWindow::updateDocumentList()
{
	/*ui->lwOpenFiles->clear();
	int i=0;
	foreach(sIDEDocument * doc, documents) {
		ui->lwOpenFiles->addItem(doc->filespec);
		if (doc == currentDocument)
			ui->lwOpenFiles->setCurrentRow(i);
		i++;
	}*/
}

void MainWindow::openFile(QString filespec)
{
	foreach(sIDEDocument * doc, documents) {
		if (filespec == doc->filespec) {
			setCurrentDocument(doc);
			return;
		}
	}
	sIDEDocument * doc = new sIDEDocument();
	doc->filespec = filespec;
	QFile f(filespec);
	f.open(QIODevice::ReadOnly);
	QByteArray bytes = f.readAll();
	doc->insert_string(0, bytes);
	documents.append(doc);

	setCurrentDocument(doc);
}

void MainWindow::closeCurrentDocument()
{
	sIDEDocument * closing = currentDocument;
	documents.removeOne(closing);
	if (documents.count() == 0) {
		on_actionNew_Script_triggered();
	} else {
		setCurrentDocument(documents.at(0));
	}
	delete closing;
}

QStringList MainWindow::getDocuments()
{
	QStringList lst;
	foreach(sIDEDocument * doc, documents) {
		lst.append(doc->filespec);
	}
	return lst;
}

/*void MainWindow::test(const char *str)
{
	if (str == NULL) str = "(null)";
	QMessageBox::information(this, "Script-Dialog", QString::fromUtf8(str));
}*/

void MainWindow::alert(QString str)
{
	QMessageBox::warning(this, "Script-Dialog", str);
}

void MainWindow::log(QString str)
{
	str += "\n";
	QByteArray a = str.toUtf8();
	scDebug->appendText(a.length(), a.data());
}

void MainWindow::clearLog()
{
	scDebug->clear();
}

ScintillaEdit *MainWindow::createScDockWindow(QString objectName, QString title)
{
	QDockWidget *dw1 = new QDockWidget(title, this);
	dw1->setObjectName(objectName);
	ScintillaEdit * edit = new ScintillaEdit;
	dw1->setWidget(edit);
	addDockWidget(Qt::TopDockWidgetArea, dw1);
	resizeDocks({dw1}, {70}, Qt::Vertical);
	return edit;
}

void MainWindow::on_actionOpen_Files_triggered()
{
	ui->dwOpenFiles->show();
}

void MainWindow::on_actionFile_Explorer_triggered()
{
	ui->dwExplorer->show();
}

void MainWindow::on_actionTable_Of_Contents_triggered()
{
	ui->dwTOC->show();
}

void MainWindow::on_actionSearch_Results_triggered()
{
	ui->dwSearchResults->show();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	QSettings settings("Weller IT", "ScriptIDE Q");
	settings.setValue("geometry", saveGeometry());
	settings.setValue("windowState", saveState());
	QMainWindow::closeEvent(event);
	QFile f("macros.txt");
	if (f.open(QIODevice::Truncate | QIODevice::WriteOnly)) {
		QByteArray bytes = scMacro->getText(scMacro->length() + 1);
		f.write(bytes);
	}
}

void MainWindow::readSettings()
{
	QSettings settings("Weller IT", "ScriptIDE Q");
	bool ok=restoreGeometry(settings.value("geometry").toByteArray());
	qDebug() << "restoreGeometry " << ok;
	restoreState(settings.value("windowState").toByteArray());
	qDebug() << "restoreState " << ok;
}

void MainWindow::on_actionNew_Script_triggered()
{
	sIDEDocument * doc = new sIDEDocument();
	doc->filespec = "untitled " + QString::number(untitledCounter++);
	documents.append(doc);

	setCurrentDocument(doc);
}

void MainWindow::on_actionOpen_triggered()
{
	QStringList lst = QFileDialog::getOpenFileNames(this, "Select one or more files to open");
	foreach(QString file, lst) {
		openFile(file);
	}
}

void MainWindow::on_macroRecord(int msg, uptr_t wparam, sptr_t lparam)
{
	QString s;
	char *t = (char*) lparam;
	if (t == NULL) {
		s = QString("sc.send(%1, %2, %3);\n")
		        .arg(QString::number(msg), QString::number(wparam), QString::number(lparam));
	} else {
		QString para = MacroRecorder::escapedString(QString::fromUtf8(t));
		QJsonDocument doc;

		s = QString("sc.sendqs(%1, %2, \"%3\");\n")
		        .arg(QString::number(msg), QString::number(wparam), para);
	}
	QByteArray a = s.toUtf8();
	scMacro->appendText(a.length(), a.data());
}

void MainWindow::on_actionStart_macro_recording_triggered(bool checked)
{
	if (checked) {
		scintilla->startRecord();
	} else {
		scintilla->stopRecord();
	}
}

void MainWindow::on_actionRun_macro_triggered()
{
	QString macroScript(scMacro->getText(scMacro->length() + 1));

	QJSValue result = macroEngine.evaluate(macroScript, "Macro");
	if (result.isError()) {
		QMessageBox::critical(this, "Script Error", result.toString());
	}
}




void MainWindow::on_lwOpenFiles_itemActivated(QListWidgetItem *item)
{

}

void MainWindow::on_actionClose_triggered()
{
	closeCurrentDocument();
}

void MainWindow::on_lwOpenFiles_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
	if (current == nullptr) return;
	QString text = current->text();
	foreach(sIDEDocument * doc, documents) {
		if (text == doc->filespec) {
			setCurrentDocument(doc);
		}
	}
}
