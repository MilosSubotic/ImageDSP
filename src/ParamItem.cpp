
#include "ParamItem.h"
#include "ui_ParamItem.h"

#include <QDebug>

ParamItem::ParamItem(QWidget* parent)
	: ItemWidget(parent), ui(new Ui_ParamItem) {
	ui->setupUi(this);

	ui->minEdit->setValidator(new QDoubleValidator());
	ui->currentEdit->setValidator(new QDoubleValidator());
	ui->maxEdit->setValidator(new QDoubleValidator());

	ui->paramSlider->setTracking(false);

	connect(
			ui->minEdit,
			SIGNAL(textEdited(QString)),
			this,
			SLOT(minEditTextEdited(QString)));
	connect(
			ui->currentEdit,
			SIGNAL(textEdited(QString)),
			this,
			SLOT(currentEditTextEdited(QString)));
	connect(
			ui->maxEdit,
			SIGNAL(textEdited(QString)),
			this,
			SLOT(maxEditTextEdited(QString)));

	connect(
			ui->paramSlider,
			SIGNAL(valueChanged(int)),
			this,
			SLOT(paramSliderChanged(int)));
}

ItemWidget* ParamItem::clone(QWidget* parent) {
	return new ParamItem(parent);
}

void ParamItem::setIndex(const QModelIndex& index) {
	currentIndex = index;
	qDebug() << "setIndex";
	update();
}

void ParamItem::update() {
	qDebug() << "update";
	// TODO Too much calling dataChanged().

	double min = getField(0);
	double current = getField(1);
	double max = getField(2);
	Q_ASSERT(min <= max);
	Q_ASSERT(min <= current && current <= max);

	ui->minEdit->setText(QString::number(min));

	ui->currentEdit->setText(QString::number(current));

	ui->maxEdit->setText(QString::number(max));

	int sliderCurrent = qRound64((current - min) / (max - min) * 100);
	Q_ASSERT(0 <= sliderCurrent && sliderCurrent <= 100);
	ui->paramSlider->setValue(sliderCurrent);
}

double ParamItem::getField(int column) {
	const QAbstractItemModel* model = currentIndex.model();
	//TODO Handle errors.
	bool doubleOk;
	double d = model->data(
			model->index(currentIndex.row(), column, QModelIndex())).toDouble(
			&doubleOk);
	Q_ASSERT(doubleOk);
	return d;
}

void ParamItem::setField(int column, double value) {
	QAbstractItemModel* model
		= const_cast<QAbstractItemModel*>(currentIndex.model());

	model->setData(
			model->index(currentIndex.row(), column, QModelIndex()), value);
}

void ParamItem::minEditTextEdited(const QString& text) {
	qDebug() << "minEditTextEdited start";

	double min = text.toDouble();
	double current = getField(1);
	double max = getField(2);

	if(max < min) {
		max = min;
		setField(2, max);
		ui->maxEdit->setText(QString::number(max));
	}

	if(current < min) {
		current = min;
		setField(1, current);
		ui->currentEdit->setText(QString::number(current));
		int sliderCurrent = qRound64((current - min) / (max - min) * 100);
		Q_ASSERT(0 <= sliderCurrent && sliderCurrent <= 100);
		ui->paramSlider->setValue(sliderCurrent);
	}

	setField(0, min);

	qDebug() << "minEditTextEdited end";
}

void ParamItem::currentEditTextEdited(const QString& text) {
	qDebug() << "currentEditTextEdited start";

	double min = getField(0);
	double current = text.toDouble();
	double max = getField(2);

	if(current < min){
		current = min;
	}

	if(current > max){
		current = max;
	}

	setField(1, current);
	int sliderCurrent = qRound64((current - min) / (max - min) * 100);
	Q_ASSERT(0 <= sliderCurrent && sliderCurrent <= 100);
	ui->paramSlider->setValue(sliderCurrent);

	qDebug() << "currentEditTextEdited end";
}

void ParamItem::maxEditTextEdited(const QString& text) {
	double min = getField(0);
	double current = getField(1);
	double max = text.toDouble();

	if(min > max) {
		min = max;
		setField(0, min);
		ui->minEdit->setText(QString::number(min));
	}

	if(current > max) {
		current = max;
		setField(1, current);
		ui->currentEdit->setText(QString::number(current));
		int sliderCurrent = qRound64((current - min) / (max - min) * 100);
		Q_ASSERT(0 <= sliderCurrent && sliderCurrent <= 100);
		ui->paramSlider->setValue(sliderCurrent);
	}

	setField(2, max);
}

void ParamItem::paramSliderChanged(int sliderCurrent) {
	qDebug() << "paramSliderChanged start";
	Q_ASSERT(0 <= sliderCurrent && sliderCurrent <= 100);

	double min = getField(0);
	double max = getField(2);
	double current = double(sliderCurrent)/100*(max - min) + min;

	setField(1, current);

	update();
	qDebug() << "paramSliderChanged end";
}

///////////////////////////////////////////////////////////////////////////////
