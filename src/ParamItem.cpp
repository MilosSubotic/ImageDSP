
#include "ParamItem.h"
#include "ui_ParamItem.h"

#include <QDebug>

ParamItem::ParamItem(QWidget* parent)
	: ItemWidget(parent), ui(new Ui_ParamItem) {
	ui->setupUi(this);
}

ItemWidget* ParamItem::clone(QWidget* parent) {
	return new ParamItem(parent);
}

void ParamItem::setIndex(const QModelIndex& index) {
	double min = getField(index, 0);
	double current = getField(index, 1);
	double max = getField(index, 2);

	Q_ASSERT(min <= max);
	Q_ASSERT(min <= current && current <= max);

	ui->minEdit->setText(QString::number(min));
	ui->currentEdit->setText(QString::number(current));
	ui->maxEdit->setText(QString::number(max));

	int slider_current = qRound64((current - min)/(max - min)*100);
	Q_ASSERT(0 <= slider_current && slider_current <= 100);

	ui->paramSlider->setValue(slider_current);
}

double ParamItem::getField(const QModelIndex& index, int column) {
	const QAbstractItemModel* model = index.model();
	//TODO Handle errors.
	bool doubleOk;
	double d = model->data(
			model->index(index.row(), column, QModelIndex()),
			Qt::DisplayRole).toDouble(&doubleOk);
	Q_ASSERT(doubleOk);
	return d;
}


///////////////////////////////////////////////////////////////////////////////
