#ifndef FAKTURATOR_H
#define FAKTURATOR_H

#include <QtWidgets/QMainWindow>
#include "ui_fakturator.h"

#include <QFile>

typedef void (*SerializeString)(QLineEdit * line, QFile & file);
typedef void (*SerializeDouble)(QDoubleSpinBox * spinBox, QFile & file);
typedef void (*SerializeDate)(QDateEdit * dateBox, QFile & file);

struct SerializeCallBack {
	QFile file;
	SerializeString serializeString;
	SerializeDouble serializeDouble;
	SerializeDate serializeDate;
};

class Fakturator : public QMainWindow
{
	Q_OBJECT

public:
	Fakturator(QWidget *parent = 0);
	~Fakturator();

private:
	Ui::FakturatorClass ui;

	QString _template;
	QString _css;

	void LoadTemplates();
	void SerializeBin(SerializeCallBack & stream);
	void Save(QString & str);
	void Load(QString & str);

public slots:
	void ExportSlot();
	void SaveSlot();
	void LoadSlot();
};

#endif // FAKTURATOR_H
