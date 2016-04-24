#include "Project.h"

#include "TableModel.h"

#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include <QSortFilterProxyModel>

#include <QDebug>

#include "ImageProcessing.h"

Q_DECLARE_METATYPE(QImage*);

Project::Project(const QString& fileName, QObject* parent)
		: QObject(parent), fileName(fileName) {
	progsModel = new TableModel(1, this);

	outImgsProxyModel = new QSortFilterProxyModel(this);
	inImgsProxyModel = new QSortFilterProxyModel(this);
	paramsProxyModel = new QSortFilterProxyModel(this);

	connect(
			progsModel,
			SIGNAL(dataChanged(QModelIndex, QModelIndex)),
			this,
			SLOT(dataChanged(QModelIndex, QModelIndex)));

	connect(
			paramsProxyModel,
			SIGNAL(dataChanged(QModelIndex, QModelIndex)),
			this,
			SLOT(dataChanged(QModelIndex, QModelIndex)));

	//TODO Filled for debug.
	QVector<QVariant> r3(3);
	QVector<QVariant> r2(2);
	appendProg("rgb2yuv");
	r2[0] = QString("playing_cat_processed");
	r2[1] = QVariant::fromValue(new QImage());
	progSetups[0].outImgsModel->appendRow(r2);
	r2[0] = QString("playing_cat");
	r2[1] = QVariant::fromValue(new QImage(QString("in_img/playing_cat.jpg")));
	progSetups[0].inImgsModel->appendRow(r2);
	r3[0] = 0;
	r3[1] = 0;
	r3[2] = 255;
	progSetups[0].paramsModel->appendRow(r3);
	progSetups[0].paramsModel->appendRow(r3);
	progSetups[0].paramsModel->appendRow(r3);

	appendProg("yuv2rgb");
	r3[0] = 0;
	r3[1] = 50;
	r3[2] = 100;
	progSetups[1].paramsModel->appendRow(r3);
	r3[0] = 0.0;
	r3[1] = 0.1;
	r3[2] = 1.0;
	progSetups[1].paramsModel->appendRow(r3);
	r3[0] = -3.0;
	r3[1] = -2.4;
	r3[2] = -1.0;
	progSetups[1].paramsModel->appendRow(r3);
}

QAbstractItemModel* Project::getProgsModel() const {
	return progsModel;
}
QAbstractItemModel* Project::getOutImgsModel() const {
	return outImgsProxyModel;
}
QAbstractItemModel* Project::getInImgsModel() const {
	return inImgsProxyModel;
}
QAbstractItemModel* Project::getParamsModel() const {
	return paramsProxyModel;
}

void Project::currentProgChanged(const QModelIndex& index) {
	currentProg = index.row();
	paramsProxyModel->setSourceModel(progSetups[currentProg].paramsModel);
	inImgsProxyModel->setSourceModel(progSetups[currentProg].inImgsModel);
	outImgsProxyModel->setSourceModel(progSetups[currentProg].outImgsModel);

	imageProcessing();
}

void Project::appendProg(const QString& name) {
	QVector<QVariant> row(1, name);
	progsModel->appendRow(row);

	ProgSetup ps;
	ps.paramsModel = new TableModel(3, this);
	ps.inImgsModel = new TableModel(2, this);
	ps.outImgsModel = new TableModel(2, this);
	progSetups.push_back(ps);
}


void Project::dataChanged(
		const QModelIndex& topLeft,
		const QModelIndex& bottomRight) {
	Q_UNUSED(topLeft);
	Q_UNUSED(bottomRight);
	imageProcessing();
}

void Project::imageProcessing() {

	QString progName = progsModel->get(currentProg, 0).toString();

	int outImgsCount = outImgsProxyModel->rowCount();
	QVector<QImage*> outImgs(outImgsCount);
	for(int i = 0; i < outImgsCount; i++){
		QVariant v = outImgsProxyModel->data(outImgsProxyModel->index(i, 1));
		outImgs[i] = v.value<QImage*>();
	}

	int inImgsCount = inImgsProxyModel->rowCount();
	QVector<const QImage*> inImgs(inImgsCount);
	for(int i = 0; i < inImgsCount; i++){
		QVariant v = inImgsProxyModel->data(inImgsProxyModel->index(i, 1));
		inImgs[i] = v.value<QImage*>();
	}

	int paramsCount = paramsProxyModel->rowCount();
	QVector<double> params(paramsCount);
	for(int i = 0; i < paramsCount; i++){
		QVariant v = paramsProxyModel->data(paramsProxyModel->index(i, 1));
		params[i] = v.toDouble();
	}

	imageProcessingFun(progName, outImgs, inImgs, params);

	emit imageProcessingDone();
}

