#include "PictureAdjustwithKinect.h"
#include <QPixmap>
#include <QtCore>
#include <cv.h>


FirstOpenCVwithQt::FirstOpenCVwithQt(QWidget *parent)
	: QMainWindow(parent),cv_img(cv::imread("D:/62a7d933c895d143cfbebbd773f082025baf077d.jpg"))
{
	ui.setupUi(this);

//     cv_img = cv::imread("D:/62a7d933c895d143cfbebbd773f082025baf077d.jpg");
// 	cv_img_processed = cv_img.clone();
// 	cv::namedWindow("image", CV_WINDOW_AUTOSIZE);  
// 	cv::imshow("image", cv_img);
//     cv::cvtColor(cv_img, cv_img_processed, CV_RGB2RGBA);
	cv::cvtColor(cv_img, cv_img, CV_RGB2RGBA);
	cv_img_processed=cv_img.clone();

	label  = new QLabel(this);
	label->move(200, 50);//ͼ���ڴ�����������λ��
	img = QImage((const unsigned char*)(cv_img_processed.data), cv_img_processed.cols, cv_img_processed.rows, QImage::Format_RGB32); 
	label->setPixmap(QPixmap::fromImage(img));  
	label->resize(label->pixmap()->size());
	label->show();

 	kinect4skeleton = new KinectBodyFrame();

	//�½���ʱ��
	QTimer *timer = new QTimer(this);

	//������ʱ�������źź���Ӧ�Ĳۺ���
	connect(timer, SIGNAL(timeout()), this, SLOT(timerFuncToUpdateBrightnessValue()));

	//��ʱ����ʼ��ʱ������1000��ʾ1000ms��1��
	timer->start(10);
}

FirstOpenCVwithQt::~FirstOpenCVwithQt()
{

}

void FirstOpenCVwithQt::SliderChanged(int newValue)
{
//     ui.lineEdit->setText(QString ("%1").arg(newValue));
	ProcessImg(newValue);
	img = QImage((const unsigned char*)(cv_img_processed.data), cv_img_processed.cols, cv_img_processed.rows, QImage::Format_RGB32); 

	label->setPixmap(QPixmap::fromImage(img));  
}

void FirstOpenCVwithQt::ProcessImg(double newValue)
{
	ProcessLightness(newValue);
}

void FirstOpenCVwithQt::ProcessLightness( double newValue )
{
	// 	const uchar *data = cv_img.ptr<uchar>(0);
	// 	uchar *data_processed = cv_img_processed.ptr<uchar>(0);
	const uchar *data = cv_img.data;
	uchar *data_processed = cv_img_processed.data;

	const int total = cv_img.cols * cv_img.rows * cv_img.channels();
	if (newValue > 255)
	{
		newValue = 255;
	}
	if (newValue < 0)
	{
		newValue = 0;
	}
	for (int i = 0;i < total;++i)
	{
		*data_processed++ = *data++*(newValue)/255;
	}
}

void FirstOpenCVwithQt::ProcessEdge(int newValue)
{
	cv::cvtColor(cv_img, cv_img_processed, CV_RGBA2GRAY);
	cv::Mat re  =  cv_img_processed.clone();

#if 0
	//�����˲�
	cv::morphologyEx(cv_img_processed, cv_img_processed, cv::MORPH_GRADIENT, cv::Mat());
	//��ֵ��
	cv::threshold(cv_img_processed, cv_img_processed, newValue, 255, cv::THRESH_BINARY);
#endif

#if 0
	//Canny�㷨�������
	cv::Canny(cv_img, cv_img_processed, ((double)newValue)/255.0 * 1000.0, 500);
#endif
	
	cv_img_processed += re;

	label->setPixmap(
		QPixmap::fromImage(QImage((const unsigned char*)(cv_img_processed.data), 
		cv_img_processed.cols, cv_img_processed.rows, QImage::Format_Indexed8)
		)
		);
}

void FirstOpenCVwithQt::timerFuncToUpdateBrightnessValue()
{
	SliderChanged(kinect4skeleton->GetCurrentBrightness());
	kinect4skeleton->DrawCurrentDepth();
	kinect4skeleton->ProcessKinectDataToSkeletonImg();
	//QMessageBox::information(nullptr, QStringLiteral("ʧ��"), QStringLiteral("�Ҳ���Kinect�豸"), QMessageBox::Yes);
}

HRESULT FirstOpenCVwithQt::Initialize()//��ʼ��Kinect��
{
	return kinect4skeleton->InitializeKinectSensorForBodyFrame();
}
