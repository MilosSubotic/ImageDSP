#ifndef PARAMITEM_H_
#define PARAMITEM_H_

#include "ItemWidget.h"

// Forward declaration.
class Ui_ParamItem;

class ParamItem : public ItemWidget {
	Q_OBJECT

public:
	explicit ParamItem(QWidget* parent = 0);
	~ParamItem() {}

	virtual ItemWidget* clone(QWidget* parent);
	virtual void setIndex(const QModelIndex& index);

private:
	Ui_ParamItem* ui;

	QModelIndex currentIndex;

	double getField(int column);
	void setField(int column, double value);
	void update();

protected slots:
	void currentEditEditingFinished();
	void paramSliderReleased();
};

#endif // PARAMITEM_H_
