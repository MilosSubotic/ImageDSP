
#include "ParamItem.h"
#include "ui_ParamItem.h"

#include <QDebug>

ParamItem::ParamItem(QWidget* parent)
	: ItemWidget(parent), ui(new Ui_ParamItem) {
	ui->setupUi(this);

	ui->CurrentValueEdit->setValidator(new QDoubleValidator());
	
	connect(
			ui->CurrentValueEdit,
			SIGNAL(editingFinished()),
			this,
			SLOT(currentEditEditingFinished()));
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

	//TODO: Update control name

	ui->MinLabel->setText(QString::number(min));

	ui->CurrentValueEdit->setText(QString::number(current));

	ui->MaxLabel->setText(QString::number(max));

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


void ParamItem::currentEditEditingFinished() {
	double min = getField(0);
	double current = ui->CurrentValueEdit->text().toDouble();
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
