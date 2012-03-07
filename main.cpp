#include <QtGui/QApplication>
#include "Widget.h"
#include <QTextCodec>
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF8"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
	Widget w;
	w.show();
	
	return a.exec();
}
