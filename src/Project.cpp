#include "Project.h"

#include "TableModel.h"

#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include <QSortFilterProxyModel>

#include <QDebug>

Project::Project(const QString& fileName, QObject* parent)
		: QObject(parent), fileName(fileName) {
	progsModel = new TableModel(1, this);

	paramsProxyModel = new QSortFilterProxyModel(this);

	//TODO Filled for debug.
	appendProg("rgb2yuv");
	QVector<QVariant> r0(3);
	r0[0] = 0;
	r0[1] = 100;
	r0[2] = 50;
	progSetups[0].paramsModel->appendRow(r0);
	appendProg("yuv2rgb");
	QVector<QVariant> r1(3);
	r1[0] = 0.0;
	r1[1] = 0.1;
	r1[2] = 1.0;
	progSetups[0].paramsModel->appendRow(r1);
}

QAbstractItemModel* Project::getProgsModel() const {
	return progsModel;
}

QAbstractItemModel* Project::getParamsModel() const {
	return paramsProxyModel;
}

void Project::currentProgChanged(const QModelIndex& index) {
	qDebug() << index.row();
	paramsProxyModel->setSourceModel(progSetups[index.row()].paramsModel);
}

void Project::appendProg(const QString& name) {
	QVector<QVariant> row(1, name);
	progsModel->appendRow(row);

	ProgSetup ps;
	ps.paramsModel = new TableModel(3, this);
	progSetups.push_back(ps);
}

