
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
			SIGNAL(editingFinished()),
			this,
			SLOT(minEditEditingFinished()));
	connect(
			ui->currentEdit,
			SIGNAL(editingFinished()),
			this,
			SLOT(currentEditEditingFinished()));
	connect(
			ui->maxEdit,
			SIGNAL(editingFinished()),
			this,
			SLOT(maxEditEditingFinished()));

	connect(
			ui->paramSlider,
			SIGNAL(sliderReleased()),
			this,
			SLOT(paramSliderReleased()));
}

ItemWidget* ParamItem::clone(QWidget* parent) {
	return new ParamItem(parent);
}

void ParamItem::setIndex(const QModelIndex& index) {
	currentIndex = index;
	update();
}

void ParamItem::update() {
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

void ParamItem::minEditEditingFinished() {
	double min = ui->minEdit->text().toDouble();
	double current = getField(1);
	double max = getField(2);

	if(max < min) {
		max = min;
		setField(2, max);
	}

	if(current < min) {
		current = min;
		setField(1, current);
	}

	setField(0, min);

	update();
}

void ParamItem::currentEditEditingFinished() {
	double min = getField(0);
	double current = ui->currentEdit->text().toDouble();
	double max = getField(2);

	if(current < min){
		current = min;
	}

	if(current > max){
		current = max;
	}

	// Snap it to slider resolution.
	int sliderCurrent = qRound64((current - min) / (max - min) * 100);
	current = double(sliderCurrent)/100*(max - min) + min;

	setField(1, current);

	update();
}

void ParamItem::maxEditEditingFinished() {
	double min = getField(0);
	double current = getField(1);
	double max = ui->maxEdit->text().toDouble();

	if(min > max) {
		min = max;
		setField(0, min);
	}

	if(current > max) {
		current = max;
		setField(1, current);
	}

	setField(2, max);

	update();
}

void ParamItem::paramSliderReleased() {
	int sliderCurrent = ui->paramSlider->sliderPosition();
	Q_ASSERT(0 <= sliderCurrent && sliderCurrent <= 100);

	double min = getField(0);
	double max = getField(2);
	double current = double(sliderCurrent)/100*(max - min) + min;

	setField(1, current);

	update();
}

///////////////////////////////////////////////////////////////////////////////
