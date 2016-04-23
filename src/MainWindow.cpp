#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Project.h"
#include "ImageViewer.h"
#include "UniversalItemDelegate.h"
#include "HoverEditTriggerListView.h"
#include "ParamItem.h"

#include <QMessageBox>
#include <QDebug>

Q_DECLARE_METATYPE(QImage*);

MainWindow::MainWindow(QWidget* parent)
		: QMainWindow(parent), ui(new Ui_mainWindow) {
	ui->setupUi(this);

	project = new Project("prj/test_prj.xml", this);

	ui->progList->setModel(project->getProgsModel());
	//TODO Use activated() signal?
	connect(
			ui->progList,
			SIGNAL(clicked(QModelIndex)),
			project,
			SLOT(currentProgChanged(QModelIndex)));
	/*
	ui->progList->setCurrentIndex(
		project->getProgsModel()->index(0, 0, QModelIndex())
	);
	*/
	connect(
			project,
			SIGNAL(imageProcessingDone()),
			this,
			SLOT(updateImageViews()));


	//UniversalItemDelegate* uid = new UniversalItemDelegate(this);
	//ui->inImgsList->setItemDelegate(uid);

	QListView* paramsList = new HoverEditTriggerListView(this);
	ui->paramsListLayout->addWidget(paramsList);
	ItemWidget* paramItem = new ParamItem();
	UniversalItemDelegate* paramsListDelegate = new UniversalItemDelegate(
			paramItem,
			paramItem,
			this);
	paramsList->setItemDelegate(paramsListDelegate);

	paramsList->setModel(project->getParamsModel());


	imageViewer = new ImageViewer(this);
	ui->imgViewLayout->addWidget(imageViewer);
}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::updateImageViews() {
	QAbstractItemModel* m = project->getOutImgsModel();
	QVariant v = m->data(m->index(0, 1));
	QImage* i = v.value<QImage*>();

	imageViewer->setImage(*i);
	//imageViewer->adjustSize();
}
