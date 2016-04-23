
#include "ImageProcessing.h"

#include <QDebug>

static void rgb2yuv(
		QImage& yuvImg,
		const QImage& rgbImg,
		const QVector<double>& params) {

	yuvImg = QImage(rgbImg.size(), rgbImg.format());
	for(int y = 0; y < yuvImg.height(); y++){
		for(int x = 0; x < yuvImg.width(); x++){
			QRgb p = rgbImg.pixel(x, y);
			int r = qMin(qRed(p) + int(params[0]), 255);
			int g = qMin(qGreen(p) + int(params[1]), 255);
			int b = qMin(qBlue(p) + int(params[2]), 255);
			yuvImg.setPixel(x, y, qRgb(r, g, b));
		}
	}
}

void imageProcessingFun(
		const QString& progName,
		const QVector<QImage*>& outImgs,
		const QVector<const QImage*>& inImgs,
		const QVector<double>& params) {
	if(progName == "rgb2yuv") {
		rgb2yuv(*outImgs[0], *inImgs[0], params);
	}
}

