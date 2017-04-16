#include "fakturator.h"

Fakturator::Fakturator(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.exportButton, SIGNAL(clicked()), this, SLOT(ExportSlot()));
	connect(ui.actionLoad, SIGNAL(triggered()), this, SLOT(LoadSlot()));
	connect(ui.actionSave, SIGNAL(triggered()), this, SLOT(SaveSlot()));
	LoadTemplates();
	ui.exVal->setDate(QDate::currentDate());
	ui.dateVal->setDate(QDate::currentDate());
}

#include <QMessageBox>
#include <QFile>

void Fakturator::LoadTemplates()
{
	QFile file("data\\faktura.html");
	if (!file.open(QIODevice::ReadOnly)) {
		QMessageBox::information(0, "Unable to load template", file.errorString());
		return;
	}
	_template = file.readAll();
	QFile cssFile("data\\faktura.css");
	if (!cssFile.open(QIODevice::ReadOnly)) {
		QMessageBox::information(0, "Unable to load css file", file.errorString());
		return;
	}
	_css = file.readAll();
}

#ifndef QT_NO_PRINTER
#include <QPrinter>
#endif // !QT_NO_PRINTER

#include <QFileInfo>
#include <QFileDialog>
#include <QTextDocument>

void LoadString(QLineEdit * line, QFile & file)
{
	QString str = file.readLine();
	line->setText(str);
}
void SaveString(QLineEdit * line, QFile & file)
{
	QString str = line->text();
	file.write(str.toStdString().c_str());
	file.write("\n");
}

void LoadDouble(QDoubleSpinBox * spinBox, QFile & file)
{
	QVariant var = file.readLine();
	double d = var.toDouble();
	spinBox->setValue(d);
}
void SaveDouble(QDoubleSpinBox * spinBox, QFile & file)
{
	QVariant var = spinBox->value();
	file.write(var.toString().toStdString().c_str());
	file.write("\n");
}

void LoadDate(QDateEdit * dateBox, QFile & file)
{
	QDate date = QDate::fromString(file.readLine());
	dateBox->setDate(date);
}
void SaveDate(QDateEdit * dateBox, QFile & file)
{
	QString date = dateBox->date().toString();
	file.write(date.toStdString().c_str());
	file.write("\n");
}

void Fakturator::SerializeBin(SerializeCallBack & stream)
{
	stream.serializeString(ui.accountVal, stream.file);
	stream.serializeDate(ui.exVal,stream.file);
	stream.serializeString(ui.senderValue, stream.file);
	stream.serializeDouble(ui.sumVal, stream.file);
	stream.serializeString(ui.sumStrVal, stream.file);
	stream.serializeString(ui.factoryNumberVal, stream.file);
	stream.serializeString(ui.accountVal, stream.file);
	stream.serializeString(ui.bankVal, stream.file);
	stream.serializeString(ui.formNumberVal, stream.file);
	stream.serializeString(ui.formNumberCodeVal, stream.file);
	stream.serializeString(ui.purposeVal, stream.file);
}

void Fakturator::Save(QString& name)
{
	SerializeCallBack saveCallback;
	saveCallback.serializeDouble = &SaveDouble;
	saveCallback.serializeDate = &SaveDate;
	saveCallback.serializeString = &SaveString;
	saveCallback.file.setFileName(name);
	if (saveCallback.file.open(QIODevice::WriteOnly) == false)
	{
		QMessageBox::warning(0,"ERROR","Unable to open file for writing");
		return;
	}
	SerializeBin(saveCallback);
}

void Fakturator::Load(QString& name)
{
	SerializeCallBack loadCallback;
	loadCallback.serializeDouble = &LoadDouble;
	loadCallback.serializeDate = &LoadDate;
	loadCallback.serializeString = &LoadString;
	loadCallback.file.setFileName(name);
	if (loadCallback.file.open(QIODevice::ReadOnly) == false)
	{
		QMessageBox::warning(0,"Error","Unable to open file");
		return;
	}
	SerializeBin(loadCallback);
}

void Fakturator::SaveSlot()
{
	QString fileName = QFileDialog::getSaveFileName((QWidget*)0, "Save template", QString(), "*.template");
	
	if (fileName.length() == 0)
		return;

	if (QFileInfo(fileName).suffix().isEmpty()) {
		fileName.append(".template");
	}
	
	Save(fileName);
	QMessageBox::information(0, "Done", "Template saved");
}

void Fakturator::LoadSlot()
{
	QString fileName = QFileDialog::getOpenFileName((QWidget*)0, "Load template", QString(), "*.template");
	if (fileName.length() == 0)
		return;
	Load(fileName);
}

void Fakturator::ExportSlot()
{
	QString fileName = QFileDialog::getSaveFileName((QWidget*)0, "Export PDF", QString(), "*.pdf");
	if (fileName.length() == 0)
		return;
	if (QFileInfo(fileName).suffix().isEmpty()) { 
		fileName.append(".pdf"); 
	}

	QPrinter printer(QPrinter::PrinterResolution);
	printer.setOutputFormat(QPrinter::PdfFormat);
	printer.setPaperSize(QPrinter::A4);
	printer.setOutputFileName(fileName);

	QTextDocument doc;

	QDate d = ui.dateVal->date();
	QDate ex = ui.exVal->date();
	QString fullHtml = _template.asprintf(
		_template.toStdString().c_str(), 
		ui.accountVal->text().toStdString().c_str(), 
		ui.bankVal->text().toStdString().c_str(), 
		ui.formNumberVal->text().toStdString().c_str(),
		ui.formNumberCodeVal->text().toStdString().c_str(),
		ui.factoryNumberVal->text().toStdString().c_str(),
		ui.senderValue->text().toStdString().c_str(),
		d.day(),
		d.month(),
		d.year(),
		d.day(),
		d.month(),
		d.year(),
		ui.purposeVal->text().toStdString().c_str(),
		ex.day(),
		ex.month(),
		ex.year(),
		d.day(),
		d.month(),
		d.year(),
		ui.sumVal->value(),
		ui.sumStrVal->text().toStdString().c_str(),
		d.day(),
		d.month(),
		d.year(),
		d.day(),
		d.month(),
		d.year(),
		d.day(),
		d.month(),
		d.year(),
		d.day(),
		d.month(),
		d.year(),
		d.day(),
		d.month(),
		d.year()
	);
	doc.setDefaultStyleSheet(_css);
	doc.setHtml(fullHtml);
	doc.setPageSize(printer.pageRect().size()); // This is necessary if you want to hide the page number
	doc.print(&printer);
	QMessageBox::information(0, "Done", "Please see exported pdf");
}
Fakturator::~Fakturator()
{
}
