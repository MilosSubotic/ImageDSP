#include "UniversalItemDelegate.h"

#include <QPainter>

#include <QDebug>

UniversalItemDelegate::UniversalItemDelegate(
		ItemWidget* widget,
		ItemWidget* editor,
		QObject* parent)
		:
				QStyledItemDelegate(parent),
				editingRow(-1),
				widget(widget),
				editor(editor) {
}

QSize UniversalItemDelegate::sizeHint(
		const QStyleOptionViewItem& option,
		const QModelIndex& index) const {
	Q_UNUSED(index);

	if(editingRow == index.row()){
		widget->setContent(index);
		return widget->sizeHint();
	}else{
		editor->setContent(index);
		return editor->sizeHint();
	}
}

void UniversalItemDelegate::paint(
		QPainter* painter,
		const QStyleOptionViewItem& option,
		const QModelIndex& index) const {

	if(editingRow == index.row()){
		return;
	}

	widget->setContent(index);

	QStyleOptionViewItemV4 opt(option);
	initStyleOption(&opt, index);

	QPaintDevice* originalPainterDevice = painter->device();

	if(option.state & QStyle::State_Selected){
		painter->fillRect(option.rect, option.palette.highlight());
	}

	widget->setGeometry(option.rect);

	painter->end();

	widget->render(
			painter->device(),
			option.rect.topLeft(),
			QRegion(0, 0, option.rect.width(), option.rect.height()),
			QWidget::DrawChildren);

	painter->begin(originalPainterDevice);
}

QWidget* UniversalItemDelegate::createEditor(
		QWidget* parent,
		const QStyleOptionViewItem& option,
		const QModelIndex& index) const {
	editingRow = index.row();
	return editor->clone(parent);
}

void UniversalItemDelegate::updateEditorGeometry(
		QWidget* editor,
		const QStyleOptionViewItem& option,
		const QModelIndex& index) const {
	Q_UNUSED(index);
	editor->setGeometry(option.rect);
}

void UniversalItemDelegate::setEditorData(
		QWidget* editor,
		const QModelIndex& index) const {
	dynamic_cast<ItemWidget*>(editor)->setContent(index);
	//TODO editor->connectToModel(index);
}

void UniversalItemDelegate::setModelData(
		QWidget* editor,
		QAbstractItemModel* model,
		const QModelIndex& index) const {
	Q_ASSERT(editingRow == index.row());
	editingRow = -1;

	//TODO editor->updateModel(model, index)
}

