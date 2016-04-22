#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Project.h"
#include "ImageViewer.h"
#include "UniversalItemDelegate.h"
#include "HoverEditTriggerListView.h"
#include "ParamItem.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
		: QMainWindow(parent), ui(new Ui_mainWindow) {
	ui->setupUi(this);

	project = 0;


	project = new Project("prj/test_prj.xml", this);

	ui->progList->setModel(project->getProgsModel());
	//TODO Use activated() signal?
	connect(
			ui->progList,
			SIGNAL(clicked(QModelIndex)),
			project,
			SLOT(currentProgChanged(QModelIndex)));
	//TODO Must select first item, best when file is loaded.
	// Not sure why this is not working.
	/*
	ui->progList->setCurrentIndex(
		project->getProgsModel()->index(0, 0, QModelIndex())
	);
	*/


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


	/////
	// TODO Just for debug.

	ImageViewer* imageViewer = new ImageViewer(this);

	ui->imgViewLayout->addWidget(imageViewer);

	QString fileName("in_img/playing_cat.jpg");
	QImage image(fileName);
	if(image.isNull()){
		QMessageBox::information(
				this,
				tr("Image Viewer"),
				tr("Cannot open image \"%1\"!").arg(fileName));
		return;
	}
	imageViewer->setImage(image);
	imageViewer->adjustSize();

	/////
}

MainWindow::~MainWindow() {
	delete ui;
}
