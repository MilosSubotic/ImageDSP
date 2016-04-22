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
	QVector<QVariant> r(3);
	appendProg("rgb2yuv");
	r[0] = 0;
	r[1] = 50;
	r[2] = 100;
	progSetups[0].paramsModel->appendRow(r);
	r[0] = 553;
	r[1] = 553;
	r[2] = 564;
	progSetups[0].paramsModel->appendRow(r);
	appendProg("yuv2rgb");
	r[0] = 0.0;
	r[1] = 0.1;
	r[2] = 1.0;
	progSetups[1].paramsModel->appendRow(r);
	r[0] = -3.0;
	r[1] = -2.4;
	r[2] = -1.0;
	progSetups[1].paramsModel->appendRow(r);
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

