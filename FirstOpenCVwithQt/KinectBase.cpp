#include "KinectBase.h"

KinectBase::KinectBase():
m_pKinectSensor(nullptr),
m_pCoordinateMapper(nullptr),
m_pBodyFrameReader(nullptr)
{
}


KinectBase::~KinectBase()
{
	SafeRelease(m_pBodyFrameReader);
	SafeRelease(m_pCoordinateMapper);

	if (m_pKinectSensor != nullptr)
	{
		m_pKinectSensor->Close();
	}
	SafeRelease(m_pKinectSensor);
}


HRESULT KinectBase::InitializeKinectSensor()
{
	//用于判断每次读取操作的成功与否
	HRESULT hr;

	//搜索kinect
	hr = GetDefaultKinectSensor(&m_pKinectSensor);
	if (FAILED(hr)){
		return hr;
	}

	//没找到kinect设备返回
	if (m_pKinectSensor == nullptr)
	{
		QMessageBox::information(nullptr, QStringLiteral("失败"), QStringLiteral("找不到Kinect设备"), QMessageBox::Yes);
		return E_FAIL;
	}

	// Initialize the Kinect and get coordinate mapper and the body reader
	IBodyFrameSource* pBodyFrameSource = NULL;//读取骨架
	IDepthFrameSource* pDepthFrameSource = NULL;//读取深度信息
	IBodyIndexFrameSource* pBodyIndexFrameSource = NULL;//读取背景二值图

	//打开kinect
	hr = m_pKinectSensor->Open();

	//coordinatemapper，关节的坐标范围是-1~1，RGB相机与深度相机分辨率不一样，各种需要这个来匹配。
	if (SUCCEEDED(hr))
	{
		hr = m_pKinectSensor->get_CoordinateMapper(&m_pCoordinateMapper);
	}

	//bodyframe
	if (SUCCEEDED(hr))
	{
		hr = m_pKinectSensor->get_BodyFrameSource(&pBodyFrameSource);
	}

	if (SUCCEEDED(hr))
	{
		hr = pBodyFrameSource->OpenReader(&m_pBodyFrameReader);
	}

#if 1 //depthframe and body index frame
	//depth frame
	if (SUCCEEDED(hr)){
		hr = m_pKinectSensor->get_DepthFrameSource(&pDepthFrameSource);
	}

	if (SUCCEEDED(hr)){
		hr = pDepthFrameSource->OpenReader(&m_pDepthFrameReader);
	}

	//body index frame
	if (SUCCEEDED(hr)){
		hr = m_pKinectSensor->get_BodyIndexFrameSource(&pBodyIndexFrameSource);
	}

	if (SUCCEEDED(hr)){
		hr = pBodyIndexFrameSource->OpenReader(&m_pBodyIndexFrameReader);
	}
#endif	
	SafeRelease(pBodyFrameSource);
	SafeRelease(pDepthFrameSource);
	SafeRelease(pBodyIndexFrameSource);

	if (m_pKinectSensor == nullptr || FAILED(hr))
	{
// 		std::cout << "Kinect initialization failed!" << std::endl;
		return E_FAIL;
	}

// 	//skeletonImg,用于画骨架、背景二值图的MAT
// 	skeletonImg.create(cDepthHeight, cDepthWidth, CV_8UC3);
// 	skeletonImg.setTo(0);
// 
// 	//depthImg,用于画深度信息的MAT
// 	depthImg.create(cDepthHeight, cDepthWidth, CV_8UC1);
// 	depthImg.setTo(0);

	return hr;
}
