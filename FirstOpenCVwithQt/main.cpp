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

	int value = 50;//kinect�ı�����ֵ��OpenCV�������ı�ͼƬ��ֵ��0~99

	return a.exec();
}