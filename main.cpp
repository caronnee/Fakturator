#include "fakturator.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Fakturator w;
	w.show();
	return a.exec();
}
