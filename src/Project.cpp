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

	inImgsModel = new TableModel(2, this);
	outImgsModel = new TableModel(2, this);

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

	// Open project file.
	QFile xmlFile("prjs/color_space_conv.imgdsp");
	// TODO Check if file is open.
	xmlFile.open(QFile::ReadOnly | QFile::Text);
	QXmlStreamReader xml(&xmlFile);

	QVector<QVariant> r4(4);
	QVector<QVariant> r2(2);
	int nextProg = 0;
	while(!xml.atEnd()) {
		xml.readNext();
		if(xml.isStartElement()){
			if(xml.name() == "program"){
				nextProg++;
				appendProg(xml.attributes().value("name").toString());
			}else if(xml.name() == "param"){
				r4[0] = QString(xml.attributes().value("name").toString());
				// TODO Check is double OK and make error to user if not.
				r4[1] = QString(xml.attributes().value("min").toString())
						.toDouble();
				r4[2] = QString(xml.attributes().value("current").toString())
						.toDouble();
				r4[3] = QString(xml.attributes().value("max").toString())
						.toDouble();
				progSetups[nextProg-1].paramsModel->appendRow(r4);
			}else if(xml.name() == "in_img"){
				QString file_name(
						xml.attributes().value("file_name").toString());
				r2[0] = file_name;
				//TODO Nicer.
				QString full_file_name = QString("in_img/") + file_name;
				r2[1] = QVariant::fromValue(
						new QImage(
								QImage(full_file_name).convertToFormat(
										QImage::Format_RGB888)));
				inImgsModel->appendRow(r2);
			}else if(xml.name() == "out_img"){
				QString file_name(
						xml.attributes().value("file_name").toString());
				r2[0] = file_name;
				//TODO Nicer.
				QString full_file_name = QString("out_img/") + file_name;
				r2[1] = QVariant::fromValue(new QImage());
				outImgsModel->appendRow(r2);
			}
		}
	}


/*
	//TODO Hard-coded for debug.
	QVector<QVariant> r4(4);
	QVector<QVariant> r2(2);
	appendProg("rgb2yuv");
	appendProg("yuv2rgb");
	r2[0] = QString("playing_kitten");
	QImage in = QImage(QString("in_img/playing_kitten.jpg"));
	r2[1] = QVariant::fromValue(
			new QImage(in.convertToFormat(QImage::Format_RGB888)));
	progSetups[0].inImgsModel->appendRow(r2);
	progSetups[1].inImgsModel->appendRow(r2);
	r2[0] = QString("playing_kitten_processed");
	r2[1] = QVariant::fromValue(new QImage());
	progSetups[0].outImgsModel->appendRow(r2);
	progSetups[1].outImgsModel->appendRow(r2);
	r4[0] = "param1";
	r4[1] = 0;
	r4[2] = 0;
	r4[3] = 255;
	progSetups[0].paramsModel->appendRow(r4);
	progSetups[0].paramsModel->appendRow(r4);
	progSetups[0].paramsModel->appendRow(r4);
	progSetups[1].paramsModel->appendRow(r4);
	progSetups[1].paramsModel->appendRow(r4);
	progSetups[1].paramsModel->appendRow(r4);
*/

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
	inImgsProxyModel->setSourceModel(inImgsModel);
	outImgsProxyModel->setSourceModel(outImgsModel);

	imageProcessing();
}

void Project::appendProg(const QString& name) {
	QVector<QVariant> row(1, name);
	progsModel->appendRow(row);

	ProgSetup ps;
	ps.paramsModel = new TableModel(4, this);
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
		QVariant v = paramsProxyModel->data(paramsProxyModel->index(i, 2));
		params[i] = v.toDouble();
	}

	imageProcessingFun(progName, outImgs, inImgs, params);

	emit imageProcessingDone();
}

