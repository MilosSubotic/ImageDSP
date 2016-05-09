
#include "ImageProcessing.h"

#include <QDebug>

static void RGBtoYUV444(const uchar rgbImg[], int x, int y, uchar Y_buff[], char U_buff[], char V_buff[]) 
{
	uchar R, G, B;
	for(int i = 0; i<x; i++)
	{
		for(int j = 0; j<y; j+=1)
		{
			R = rgbImg[i*3*y+j*3];
			G = rgbImg[i*3*y+j*3 + 1];
			B = rgbImg[i*3*y+j*3 + 2];

			Y_buff[i*y+j] =  0.299*R + 0.587*G + 0.114*B;
			U_buff[i*y+j] =  - 0.14713*R - 0.28886*G + 0.436*B;
			V_buff[i*y+j] =  R*0.615 - 0.51499*G - 0.10001*B;
		}
	}
}

static void YUV444toRGB(const uchar Y_buff[], const char U_buff[], const char V_buff[], int x, int y, uchar rgbImg[]) 
{
	double R,G,B;
	double Y, U, V;
	for(int i = 0; i<x; i++)
	{
		for(int j = 0; j<y; j+=1)
		{
			Y = 1.0*Y_buff[i*y+j];
			U = U_buff[i*y+j];
			V = V_buff[i*y+j];

			R = Y + 1.13983*V;
			G = Y - 0.39465*U - 0.58060*V;
			B = Y + 2.03211*U;
			
			if (R < 0)
				R = 0;
			else if (R > 255)
				R = 255;
			if (G< 0)
				G = 0;
			else if (G> 255)
				G = 255;
			if (B < 0)
				B = 0;
			else if (B > 255)
				B = 255;


			rgbImg[i*3*y+j*3] =  R;
			rgbImg[i*3*y+j*3 + 1] = G;
			rgbImg[i*3*y+j*3 + 2] =  B;
	
		}
	}
}

static void RGBtoYUV422(const uchar rgbImg[], int x, int y, uchar Y_buff[], char U_buff[], char V_buff[]) 
{
	uchar R, G, B;
	for(int i = 0; i<x; i++)
	{
		for(int j = 0; j<y; j+=2)
		{
			R = rgbImg[i*3*y+j*3];
			G = rgbImg[i*3*y+j*3 + 1];
			B = rgbImg[i*3*y+j*3 + 2];

			Y_buff[i*y+j] =  0.299*R + 0.587*G + 0.114*B;
			U_buff[i*y/2+j/2] =  (- 0.14713*R - 0.28886*G + 0.436*B)/2;
			V_buff[i*y/2+j/2] =  (R*0.615 - 0.51499*G - 0.10001*B)/2;

			R = rgbImg[i*3*y+(j+1)*3];
			G = rgbImg[i*3*y+(j+1)*3 + 1];
			B = rgbImg[i*3*y+(j+1)*3 + 2];

			Y_buff[i*y+(j+1)] =  0.299*R + 0.587*G + 0.114*B;
			U_buff[i*y/2+j/2] +=  (- 0.14713*R - 0.28886*G + 0.436*B)/2;
			V_buff[i*y/2+j/2] +=  (R*0.615 - 0.51499*G - 0.10001*B)/2;
		}
	}
}

static void YUV422toRGB(const uchar Y_buff[], const char U_buff[], const char V_buff[], int x, int y, uchar rgbImg[]) 
{
	double R,G,B;
	double Y, U, V;
	for(int i = 0; i<x; i++)
	{
		for(int j = 0; j<y; j+=2)
		{
			U = U_buff[i*y/2+j/2];
			V = V_buff[i*y/2+j/2];

			Y = Y_buff[i*y+j];

			R = Y + 1.13983*V;
			G = Y -0.39465*U - 0.58060*V;
			B = Y + 2.03211*U;

			if (R < 0)
				R = 0;
			else if (R > 255)
				R = 255;
			if (G < 0)
				G = 0;
			else if (G> 255)
				G = 255;
			if (B < 0)
				B = 0;
			else if (B > 255)
				B = 255;

			rgbImg[i*3*y+j*3] =  R;
			rgbImg[i*3*y+j*3 + 1] = G;
			rgbImg[i*3*y+j*3 + 2] = B;

			Y = Y_buff[i*y+(j+1)];

			R = Y + 1.13983*V;
			G = Y -0.39465*U - 0.58060*V;
			B = Y + 2.03211*U;

			if (R < 0)
				R = 0;
			else if (R > 255)
				R = 255;
			if (G< 0)
				G = 0;
			else if (G> 255)
				G = 255;
			if (B < 0)
				B = 0;
			else if (B > 255)
				B = 255;

			rgbImg[i*3*y+(j+1)*3] =  R;
			rgbImg[i*3*y+(j+1)*3 + 1] = G;
			rgbImg[i*3*y+(j+1)*3 + 2] = B;
		}
	}
}


static void RGBtoYUV420(const uchar rgbImg[], int x, int y, uchar Y_buff[], char U_buff[], char V_buff[]) 
{
	uchar R, G, B;
	for(int i = 0; i<x; i+=2)
	{
		for(int j = 0; j<y; j+=2)
		{	
			R = rgbImg[i*3*y+j*3];
			G = rgbImg[i*3*y+j*3 + 1];
			B = rgbImg[i*3*y+j*3 + 2];

			Y_buff[i*y+j] =  0.299*R + 0.587*G + 0.114*B;
			U_buff[(i/2)*y/2+j/2] =  (- 0.14713*R - 0.28886*G + 0.436*B);
			V_buff[(i/2)*y/2+j/2] =  (R*0.615 - 0.51499*G - 0.10001*B);

			R = rgbImg[i*3*y+(j+1)*3];
			G = rgbImg[i*3*y+(j+1)*3 + 1];
			B = rgbImg[i*3*y+(j+1)*3 + 2];

			Y_buff[i*y+(j+1)] =  0.299*R + 0.587*G + 0.114*B;
		//	U_buff[(i/2)*y/2+j/2] +=  (- 0.14713*R - 0.28886*G + 0.436*B)/4;
		//	V_buff[(i/2)*y/2+j/2] +=  (R*0.615 - 0.51499*G - 0.10001*B)/4;

			R = rgbImg[(i+1)*3*y+j*3];
			G = rgbImg[(i+1)*3*y+j*3 + 1];
			B = rgbImg[(i+1)*3*y+j*3 + 2];

			Y_buff[(i+1)*y+j] =  0.299*R + 0.587*G + 0.114*B;
		//	U_buff[(i/2)*y/2+j/2] +=  (- 0.14713*R - 0.28886*G + 0.436*B)/4;
		//	V_buff[(i/2)*y/2+j/2] +=  (R*0.615 - 0.51499*G - 0.10001*B)/4;

			R = rgbImg[(i+1)*3*y+(j+1)*3];
			G = rgbImg[(i+1)*3*y+(j+1)*3 + 1];
			B = rgbImg[(i+1)*3*y+(j+1)*3 + 2];

			Y_buff[(i+1)*y+(j+1)] =  0.299*R + 0.587*G + 0.114*B;
		//	U_buff[(i/2)*y/2+j/2] +=  (- 0.14713*R - 0.28886*G + 0.436*B)/4;
		//	V_buff[(i/2)*y/2+j/2] +=  (R*0.615 - 0.51499*G - 0.10001*B)/4;
		}
	}
}

static void YUV420toRGB(const uchar Y_buff[], const char U_buff[], const char V_buff[], int x, int y, uchar rgbImg[]) 
{
	double R,G,B;
	double Y, U, V;
	for(int i = 0; i<x; i+=2)
	{
		for(int j = 0; j<y; j+=2)
		{
			U = U_buff[i/2*y/2+j/2];
			V = V_buff[i/2*y/2+j/2];

			Y = Y_buff[i*y+j];

			R = Y + 1.13983*V;
			G = Y -0.39465*U - 0.58060*V;
			B = Y + 2.03211*U;

			if (R < 0)
				R = 0;
			else if (R > 255)
				R = 255;
			if (G < 0)
				G = 0;
			else if (G> 255)
				G = 255;
			if (B < 0)
				B = 0;
			else if (B > 255)
				B = 255;

			rgbImg[i*3*y+j*3] =  R;
			rgbImg[i*3*y+j*3 + 1] = G;
			rgbImg[i*3*y+j*3 + 2] = B;

			Y = Y_buff[i*y+(j+1)];

			R = Y + 1.13983*V;
			G = Y -0.39465*U - 0.58060*V;
			B = Y + 2.03211*U;

			if (R < 0)
				R = 0;
			else if (R > 255)
				R = 255;
			if (G< 0)
				G = 0;
			else if (G> 255)
				G = 255;
			if (B < 0)
				B = 0;
			else if (B > 255)
				B = 255;

			rgbImg[i*3*y+(j+1)*3] =  R;
			rgbImg[i*3*y+(j+1)*3 + 1] = G;
			rgbImg[i*3*y+(j+1)*3 + 2] = B;

			Y = Y_buff[(i+1)*y+j];

			R = Y + 1.13983*V;
			G = Y -0.39465*U - 0.58060*V;
			B = Y + 2.03211*U;

			if (R < 0)
				R = 0;
			else if (R > 255)
				R = 255;
			if (G < 0)
				G = 0;
			else if (G> 255)
				G = 255;
			if (B < 0)
				B = 0;
			else if (B > 255)
				B = 255;

			rgbImg[(i+1)*3*y+j*3] =  R;
			rgbImg[(i+1)*3*y+j*3 + 1] = G;
			rgbImg[(i+1)*3*y+j*3 + 2] = B;

			Y = Y_buff[(i+1)*y+(j+1)];

			R = Y + 1.13983*V;
			G = Y -0.39465*U - 0.58060*V;
			B = Y + 2.03211*U;

			if (R < 0)
				R = 0;
			else if (R > 255)
				R = 255;
			if (G< 0)
				G = 0;
			else if (G> 255)
				G = 255;
			if (B < 0)
				B = 0;
			else if (B > 255)
				B = 255;

			rgbImg[(i+1)*3*y+(j+1)*3] =  R;
			rgbImg[(i+1)*3*y+(j+1)*3 + 1] = G;
			rgbImg[(i+1)*3*y+(j+1)*3 + 2] = B;
		}
	}
}

static void procesing_YUV444(uchar Y_buff[], char U_buff[], char V_buff[], int x, int y, double Y, double U, double V)
{
	for(int i = 0; i<x; i++)
	{
		for(int j = 0; j<y; j++)
		{
			Y_buff[i*y+j] *= Y; 
			U_buff[i*y+j] *= U; 
			V_buff[i*y+j] *= V; 
		}
	}

}

static void procesing_YUV422(uchar Y_buff[], char U_buff[], char V_buff[], int x, int y, double Y, double U, double V)
{
	for(int i = 0; i<x; i++)
	{
		for(int j = 0; j<y; j+=2)
		{
			Y_buff[i*y+j] *= Y; 
			Y_buff[i*y+j+1] *= Y; 
			U_buff[i*y/2+j/2] *= U; 
			V_buff[i*y/2+j/2] *= V; 
		}
	}

}


static void procesing_YUV420(uchar Y_buff[], char U_buff[], char V_buff[], int x, int y, double Y, double U, double V)
{
	for(int i = 0; i<x; i+=2)
	{
		for(int j = 0; j<y; j+=2)
		{
			Y_buff[i*y+j] *= Y; 
			Y_buff[i*y+j+1] *= Y; 
			Y_buff[(i+1)*y+j] *= Y; 
			Y_buff[(i+1)*y+j+1] *= Y; 

			U_buff[i/2*y/2+j/2] *= U; 
			V_buff[i/2*y/2+j/2] *= V; 
		}
	}

}

static void decimate_Y(uchar Y_buff[], int x, int y)
{
	uchar YY;
	for(int i = 0; i<x; i+=2)
	{
		for(int j = 0; j<y; j+=2)
		{
			YY = (int)Y_buff[i*y+j];
			Y_buff[i*y+j] = YY;
			Y_buff[(i+1)*y+j] = YY;
			Y_buff[i*y+j+1] = YY;
			Y_buff[(i+1)*y+j+1] = YY;
		}
	}

}

void imageProcessingFun(const QString& progName, const QVector<QImage*>& outImgs, const QVector<const QImage*>& inImgs, const QVector<double>& params) 
{
	uchar* Y_buff;
	char* U_buff;
	char* V_buff;
	
	QImage * outImage = new QImage(inImgs[0]->width(), inImgs[0]->height(), inImgs[0]->format());
	*outImgs[0] = *outImage;
	
	if(progName == "YUV444") 
	{	
		Y_buff = new uchar[inImgs[0]->width()*inImgs[0]->height()];
		U_buff = new char[inImgs[0]->width()*inImgs[0]->height()];
		V_buff = new char[inImgs[0]->width()*inImgs[0]->height()];

		RGBtoYUV444(inImgs[0]->bits(), inImgs[0]->width(), inImgs[0]->height(), Y_buff, U_buff, V_buff);
		decimate_Y(Y_buff, inImgs[0]->width(), inImgs[0]->height());
		procesing_YUV444(Y_buff, U_buff, V_buff, inImgs[0]->width(), inImgs[0]->height(), params[0], params[1], params[2]);
		YUV444toRGB(Y_buff, U_buff, V_buff, inImgs[0]->width(), inImgs[0]->height(), outImgs[0]->bits());

		delete[] Y_buff;
		delete[] U_buff;
		delete[] V_buff;

	}
	else if (progName == "YUV422") 
	{
		Y_buff = new uchar[inImgs[0]->width()*inImgs[0]->height()];
		U_buff = new char[inImgs[0]->width()*inImgs[0]->height()/2];
		V_buff = new char[inImgs[0]->width()*inImgs[0]->height()/2];

		RGBtoYUV422(inImgs[0]->bits(), inImgs[0]->width(), inImgs[0]->height(), Y_buff, U_buff, V_buff);
		procesing_YUV422(Y_buff, U_buff, V_buff, inImgs[0]->width(), inImgs[0]->height(), params[0], params[1], params[2]);
		YUV422toRGB(Y_buff, U_buff, V_buff, inImgs[0]->width(), inImgs[0]->height(), outImgs[0]->bits());
		
		delete[] Y_buff;
		delete[] U_buff;
		delete[] V_buff;
	}
	else if (1)//progName == "YUV420") 
	{
		Y_buff = new uchar[inImgs[0]->width()*inImgs[0]->height()];
		U_buff = new char[inImgs[0]->width()*inImgs[0]->height()/4];
		V_buff = new char[inImgs[0]->width()*inImgs[0]->height()/4];

		RGBtoYUV420(inImgs[0]->bits(), inImgs[0]->width(), inImgs[0]->height(), Y_buff, U_buff, V_buff);
		procesing_YUV420(Y_buff, U_buff, V_buff, inImgs[0]->width(), inImgs[0]->height(), params[0], params[1], params[2]);
		YUV420toRGB(Y_buff, U_buff, V_buff, inImgs[0]->width(), inImgs[0]->height(), outImgs[0]->bits());

		delete[] Y_buff;
		delete[] U_buff;
		delete[] V_buff;
	}
	else
	{
		*outImgs[0] = QImage(*inImgs[0]);
	}
}

