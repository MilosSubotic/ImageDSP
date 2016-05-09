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

	tabifyDockWidget(ui->outImgsListDock, ui->inImgsListDock);
	tabifyDockWidget(ui->outImgsListDock, ui->progListDock);

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
	//TODO Separate view  and editor,
	// view to have labels and editor line edits.
	UniversalItemDelegate* paramsListDelegate = new UniversalItemDelegate(
			paramItem,
			paramItem,
			this);
	paramsList->setItemDelegate(paramsListDelegate);

	paramsList->setModel(project->getParamsModel());

	//TODO Remove add and remove buttons to make more space.
	// Maybe add some smaller button like Eclipse have.

	//TODO Setup QListView for in and out images and make them without editors.
	// Make possible to open new ImageView on double click.
	inputImageViewer = new ImageViewer(this);
	ui->inputImgViewLayout->addWidget(inputImageViewer);

	outputImageViewer = new ImageViewer(this);
	ui->outputImgViewLayout->addWidget(outputImageViewer);
}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::updateImageViews() {
	//TODO Add map of out and in QImage* to ImageViewer* fo updating.

	QAbstractItemModel* mi = project->getInImgsModel();
	QImage* ii = mi->data(mi->index(0, 1)).value<QImage*>();
	inputImageViewer->setImage(*ii);

	QAbstractItemModel* mo = project->getOutImgsModel();
	QImage* io = mo->data(mo->index(0, 1)).value<QImage*>();
	outputImageViewer->setImage(*io);
}
