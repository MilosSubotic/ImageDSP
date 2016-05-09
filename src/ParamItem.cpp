
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
	double min = getDoubleField(1);
	double current = getDoubleField(2);
	double max = getDoubleField(3);
	Q_ASSERT(min <= max);
	Q_ASSERT(min <= current && current <= max);

	ui->paramName->setText(getStringField(0));

	ui->minEdit->setText(QString::number(min));

	ui->currentEdit->setText(QString::number(current));

	ui->maxEdit->setText(QString::number(max));

	int sliderCurrent = qRound64((current - min) / (max - min) * 100);
	Q_ASSERT(0 <= sliderCurrent && sliderCurrent <= 100);
	ui->paramSlider->setValue(sliderCurrent);
}

double ParamItem::getDoubleField(int column) {
	const QAbstractItemModel* model = currentIndex.model();
	//TODO Handle errors.
	bool doubleOk;
	double d = model->data(
			model->index(currentIndex.row(), column, QModelIndex())).toDouble(
			&doubleOk);
	Q_ASSERT(doubleOk);
	return d;
}

QString ParamItem::getStringField(int column) {
	const QAbstractItemModel* model = currentIndex.model();
	//TODO Handle errors.
	QString s =
			model->data(
					model->index(currentIndex.row(), column, QModelIndex()))
					.toString();
	return s;
}

void ParamItem::setField(int column, double value) {
	QAbstractItemModel* model
		= const_cast<QAbstractItemModel*>(currentIndex.model());

	model->setData(
			model->index(currentIndex.row(), column, QModelIndex()), value);
}

void ParamItem::minEditEditingFinished() {
	double min = ui->minEdit->text().toDouble();
	double current = getDoubleField(2);
	double max = getDoubleField(3);

	if(max < min) {
		max = min;
		setField(3, max);
	}

	if(current < min) {
		current = min;
		setField(2, current);
	}

	setField(1, min);

	update();
}

void ParamItem::currentEditEditingFinished() {
	double min = getDoubleField(1);
	double current = ui->currentEdit->text().toDouble();
	double max = getDoubleField(3);

	if(current < min){
		current = min;
	}

	if(current > max){
		current = max;
	}

	// Snap it to slider resolution.
	int sliderCurrent = qRound64((current - min) / (max - min) * 100);
	current = double(sliderCurrent)/100*(max - min) + min;

	setField(2, current);

	update();
}

void ParamItem::maxEditEditingFinished() {
	double min = getDoubleField(1);
	double current = getDoubleField(2);
	double max = ui->maxEdit->text().toDouble();

	if(min > max) {
		min = max;
		setField(1, min);
	}

	if(current > max) {
		current = max;
		setField(2, current);
	}

	setField(3, max);

	update();
}

void ParamItem::paramSliderReleased() {
	int sliderCurrent = ui->paramSlider->sliderPosition();
	Q_ASSERT(0 <= sliderCurrent && sliderCurrent <= 100);

	double min = getDoubleField(1);
	double max = getDoubleField(3);
	double current = double(sliderCurrent)/100*(max - min) + min;

	setField(2, current);

	update();
}

///////////////////////////////////////////////////////////////////////////////
