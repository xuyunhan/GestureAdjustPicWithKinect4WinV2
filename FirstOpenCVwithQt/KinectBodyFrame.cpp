#include "KinectBodyFrame.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>  
using namespace std;
//���������

KinectBodyFrame::KinectBodyFrame()
{
	for (int i = 0; i < BODY_COUNT; ++i	)
	{
		ppBodies[i] = 0;
	}
	//ppBodies[0] = nullptr;// new IBody *[BODY_COUNT];
	skeletonImg.create(424, 512, CV_8UC3);//��������ͬһ���ֱ���
	depthImg.create(424, 512, CV_8UC3);

	_currentHandPos.x = _currentHandPos.y = 0;
	_userIsPanning = false;

	_currentLightness = 255;
}

KinectBodyFrame::~KinectBodyFrame()
{
}

HRESULT KinectBodyFrame::InitializeKinectSensorForBodyFrame()
{
	if (SUCCEEDED(KinectBase::InitializeKinectSensor()))
	{
		return S_OK;
	}
	else
	{
		return E_FAIL;
	}

}

int KinectBodyFrame::GetCurrentBrightness()//�������Ȳ�ֵ
{
	return _currentLightness;
// 	if (_userIsPanning)
// 	{
// 		return _currentHandPos.x - _startHandPos.x;
// 	}
// 	return 0;
}

//��������������ɻ�ͼ
void KinectBodyFrame::GenerateSkeletonImg()
{
	skeletonImg.setTo(0);

	HRESULT hr = E_FAIL;
	int n = _countof(ppBodies);
	for (int i = 0; i < n; ++i)//��ÿ��Bodyѭ��
	{
		IBody *pBody = ppBodies[i];
	
		if (pBody == nullptr)
		{
			QMessageBox::information(nullptr, "GenerateSkeletonImg Error", 0);
		}
	
		BOOLEAN pTracked = false;
	
		hr = pBody->get_IsTracked(&pTracked);
	
		if (FAILED(hr) || !pTracked)
		{
			continue;// QMessageBox::information(nullptr, "GenerateSkeletonImg Error", 0);
		}
	
		Joint joints[JointType_Count];//�洢�ؽڵ����
			HandState leftHandState = HandState_Unknown;//����״̬
			HandState rightHandState = HandState_Unknown;//����״̬
	
		DepthSpacePoint *depthSpacePos = new DepthSpacePoint [_countof(joints)];//��ؽ���Depth�ռ����λ��
		if (SUCCEEDED(pBody->GetJoints(_countof(joints), joints)))
		{
			for (int j = 0; j < _countof(joints); ++j)
			{
				m_pCoordinateMapper->MapCameraPointToDepthSpace(joints[j].Position, &depthSpacePos[j]);//����ӳ��
			}
		}
	
		//��ȡ������״̬
			pBody->get_HandLeftState(&leftHandState);
			pBody->get_HandRightState(&rightHandState);
	
// 		DrawHand(depthSpacePos[JointType_HandLeft], leftHandState);
		DrawHand(depthSpacePos[JointType_HandRight], rightHandState);

		delete[] depthSpacePos;

	}
}

// ��ȡ������������ɻ�ͼ
void KinectBodyFrame::ProcessKinectDataToSkeletonImg()
{
	HRESULT hr = m_pBodyFrameReader->AcquireLatestFrame(&pBodyFrame);//��ùǼ���Ϣ

	if (SUCCEEDED(hr))
	{
// 		ppBodies[0] = nullptr;//��ÿ��������ָ�룬���6���ˣ�����ֻ��һ��
// 		IBody* ppBodies[BODY_COUNT] = { 0 };//ÿһ��IBody����׷��һ���ˣ��ܹ�����׷��������

		hr = pBodyFrame->GetAndRefreshBodyData(_countof(ppBodies), ppBodies);//��kinect׷�ٵ����˵���Ϣ���ֱ�浽ÿһ��IBody��
	}
	if (FAILED(hr))
	{
		return;// QMessageBox::information(nullptr, QStringLiteral("ʧ��"), QStringLiteral("��ȡ����ʧ��"), QMessageBox::Yes);
	}

	GenerateSkeletonImg();

	for (int i = 0; i < _countof(ppBodies); ++i)
	{
		SafeRelease(ppBodies[i]);//�ͷ����й���
	}

	SafeRelease(pBodyFrame);
}

void KinectBodyFrame::DrawHand(DepthSpacePoint pointPos, HandState handState)
{
	CvScalar color;// = cvScalar(0, 0, 0);
	switch (handState)
	{
	case HandState_Closed:
		color = cvScalar(255, 0, 0);
		UserHandClosed(pointPos);
		break;
	case  HandState_Lasso:
		color = cvScalar(0, 255, 0);
		UserHandOpened(pointPos);
		break;
	case  HandState_Open:
		color = cvScalar(0, 0, 255);
		UserHandOpened(pointPos);
		break;
	default:
		UserHandOpened(pointPos);
		break;
	}
	
	circle(skeletonImg, cvPoint(pointPos.X, pointPos.Y), 20, color, -1);//����λ��
	cv::imshow("hand", skeletonImg);
	//throw std::logic_error("The method or operation is not implemented.");
}

void KinectBodyFrame::DrawCurrentDepth()
{
	HRESULT hr = m_pDepthFrameReader->AcquireLatestFrame(&pDepthFrame);

	cv::Mat bufferMat(424, 512, CV_16UC1);
	const int width = 512;
	const int height = 424;
	unsigned int bufferSize = width * height * sizeof(unsigned short);

	if (SUCCEEDED(hr)){
		hr = pDepthFrame->AccessUnderlyingBuffer(&bufferSize, reinterpret_cast<UINT16**>(&bufferMat.data));
		if (SUCCEEDED(hr))
		{
			bufferMat.convertTo(depthImg,  CV_8UC3,  - 255.0f / 4500.0f, 255.0f);
		}
	}

	SafeRelease(pDepthFrame);

	cv::imshow("DeipthImg", depthImg);
	//throw std::logic_error("The method or operation is not implemented.");
}

void KinectBodyFrame::UserHandClosed(DepthSpacePoint pointPos)
{
	if (!_userIsPanning)
	{
		_preHandPos.x = pointPos.X;
	}
	_userIsPanning = true;

	_currentHandPos.x = pointPos.X;

	_currentLightness += (_currentHandPos.x - _preHandPos.x) * 2;

	_preHandPos.x = _currentHandPos.x;
}

void KinectBodyFrame::UserHandOpened(DepthSpacePoint pointPos)
{
	_userIsPanning = false;
}
