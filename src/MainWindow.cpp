#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Project.h"
#include "ImageViewer.h"
#include "UniversalItemDelegate.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
		: QMainWindow(parent), ui(new Ui_mainWindow) {
	ui->setupUi(this);

	project = 0;

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

	UniversalItemDelegate* uid = new UniversalItemDelegate(this);
	ui->inImgsList->setItemDelegate(uid);


	project = new Project("prj/test_prj.xml", this);

	ui->progList->setModel(project->getProgsModel());
	//TODO Use activated() signal?
	connect(
			ui->progList,
			SIGNAL(clicked(QModelIndex)),
			project,
			SLOT(currentProgChanged(QModelIndex)));

}

MainWindow::~MainWindow() {
	delete ui;
}
