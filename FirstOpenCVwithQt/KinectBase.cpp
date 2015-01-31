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
	//�����ж�ÿ�ζ�ȡ�����ĳɹ����
	HRESULT hr;

	//����kinect
	hr = GetDefaultKinectSensor(&m_pKinectSensor);
	if (FAILED(hr)){
		return hr;
	}

	//û�ҵ�kinect�豸����
	if (m_pKinectSensor == nullptr)
	{
		QMessageBox::information(nullptr, QStringLiteral("ʧ��"), QStringLiteral("�Ҳ���Kinect�豸"), QMessageBox::Yes);
		return E_FAIL;
	}

	// Initialize the Kinect and get coordinate mapper and the body reader
	IBodyFrameSource* pBodyFrameSource = NULL;//��ȡ�Ǽ�
	IDepthFrameSource* pDepthFrameSource = NULL;//��ȡ�����Ϣ
	IBodyIndexFrameSource* pBodyIndexFrameSource = NULL;//��ȡ������ֵͼ

	//��kinect
	hr = m_pKinectSensor->Open();

	//coordinatemapper���ؽڵ����귶Χ��-1~1��RGB������������ֱ��ʲ�һ����������Ҫ�����ƥ�䡣
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

// 	//skeletonImg,���ڻ��Ǽܡ�������ֵͼ��MAT
// 	skeletonImg.create(cDepthHeight, cDepthWidth, CV_8UC3);
// 	skeletonImg.setTo(0);
// 
// 	//depthImg,���ڻ������Ϣ��MAT
// 	depthImg.create(cDepthHeight, cDepthWidth, CV_8UC1);
// 	depthImg.setTo(0);

	return hr;
}
