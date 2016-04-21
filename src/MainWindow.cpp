
#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "ImageViewer.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui_mainWindow) {
    ui->setupUi(this);

    ImageViewer* imageViewer = new ImageViewer(this);

    ui->imgViewLayout->addWidget(imageViewer);

    QString fileName("in_img/playing_cat.jpg");
    QImage image(fileName);
    if(image.isNull()){
        QMessageBox::information(this, tr("Image Viewer"),
                tr("Cannot open image \"%1\"!").arg(fileName));
        return;
    }
    imageViewer->setImage(image);
    imageViewer->adjustSize();
}

MainWindow::~MainWindow() {
    delete ui;
}
