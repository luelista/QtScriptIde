#include "sidewindow.h"

sIDEWindow::sIDEWindow(sIDEDocument *document, QWidget *parent) : QWidget(parent)
{
	this->doc = document;
	this->doc->refCount++;
}

sIDEWindow::~sIDEWindow()
{
	if (this->doc) {
		this->doc->refCount--;
		if (this->doc->refCount == 0) delete this->doc;
	}
}

void sIDEWindow::load()
{

}
