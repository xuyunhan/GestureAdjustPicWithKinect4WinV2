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
	cv::Mat cv_img;//原始图片
	cv::Mat cv_img_processed;//操作后的图片对象
	cv::Mat skeletonImg;//放骨骼的图
	QImage img; //显示的图片
	QLabel *label;
	KinectBodyFrame *kinect4skeleton;//放kinect对象
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