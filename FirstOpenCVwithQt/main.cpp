#include "PictureAdjustwithKinect.h"
#include <QtWidgets/QApplication>
#include "highgui.h"


int BtnTestwithOCV();

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	FirstOpenCVwithQt w;

	if (!SUCCEEDED(w.Initialize()))
	{
		;
	}

	w.show();

	int value = 50;//kinect改变它的值，OpenCV根据它改变图片的值，0~99

	return a.exec();
}