#ifndef SIDEWINDOW_H
#define SIDEWINDOW_H

#include "sidedocument.h"

#include <QObject>
#include <QWidget>

class sIDEWindow : public QWidget
{
	Q_OBJECT
public:
	explicit sIDEWindow(sIDEDocument *doc, QWidget *parent = nullptr);
	~sIDEWindow();

	sIDEDocument *document() { return doc; }

	Q_PROPERTY(sIDEDocument *document READ document)

	virtual void load();

signals:

public slots:

private:
	sIDEDocument *doc;
};

#endif // SIDEWINDOW_H
