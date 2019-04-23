#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "sidedocument.h"

#include <QMainWindow>
#include <ScintillaEdit.h>
#include <QJSEngine>
#include <QListWidget>


Q_DECLARE_METATYPE(sptr_t)
Q_DECLARE_METATYPE(uptr_t)

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	ScintillaEdit *scMacro;
	ScintillaEdit *scDebug;
	Q_PROPERTY(ScintillaEdit *scLog MEMBER scDebug)

	void setCurrentDocument(sIDEDocument *doc);
	void updateDocumentList();
	Q_INVOKABLE void openFile(QString filespec);
	Q_INVOKABLE void closeCurrentDocument();
	Q_INVOKABLE QStringList getDocuments();

	//Q_INVOKABLE void test(const char* str = 0);
	Q_INVOKABLE void alert(QString str);
	Q_INVOKABLE void log(QString str);
	Q_INVOKABLE void clearLog();


	ScintillaEdit * createScDockWindow(QString objectName, QString title);

private slots:
	void on_actionOpen_Files_triggered();

	void on_actionFile_Explorer_triggered();

	void on_actionTable_Of_Contents_triggered();

	void on_actionSearch_Results_triggered();

	void on_actionNew_Script_triggered();

	void on_actionOpen_triggered();
	void on_macroRecord(int msg,uptr_t wparam,sptr_t lparam);
	void on_actionStart_macro_recording_triggered(bool checked);

	void on_actionRun_macro_triggered();

	void on_lwOpenFiles_itemActivated(QListWidgetItem *item);

	void on_actionClose_triggered();

	void on_lwOpenFiles_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
	void closeEvent(QCloseEvent *event);
	void readSettings();
	Ui::MainWindow *ui;
	QList<sIDEDocument *> documents;
	sIDEDocument * currentDocument;
	QJSEngine macroEngine;
	int untitledCounter = 1;
};

#endif // MAINWINDOW_H
