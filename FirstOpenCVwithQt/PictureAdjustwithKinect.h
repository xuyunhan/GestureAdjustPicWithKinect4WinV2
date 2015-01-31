#ifndef FIRSTOPENCVWITHQT_H
#define FIRSTOPENCVWITHQT_H

#include <QtWidgets/QMainWindow>
#include "ui_firstopencvwithqt.h"
#include <highgui.h>
#include "qlabel.h"
#include <KinectBodyFrame.h>

class FirstOpenCVwithQt : public QMainWindow
{
	Q_OBJECT
		
public:
	FirstOpenCVwithQt(QWidget *parent = 0);
	~FirstOpenCVwithQt();
	void ProcessLightness( double newValue );

private:
	Ui::FirstOpenCVwithQtClass ui;
	cv::Mat cv_img;//ԭʼͼƬ
	cv::Mat cv_img_processed;//�������ͼƬ����
	cv::Mat skeletonImg;//�Ź�����ͼ
	QImage img; //��ʾ��ͼƬ
	QLabel *label;
	KinectBodyFrame *kinect4skeleton;//��kinect����
	void ProcessImg(double newValue);

public:
	HRESULT Initialize();

public slots:
	void SliderChanged(int newValue);
	void ProcessEdge(int newValue);

private slots:
	void timerFuncToUpdateBrightnessValue();
};

#endif // FIRSTOPENCVWITHQT_H