#include "KinectBodyFrame.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>  
using namespace std;
//人物骨骼类

KinectBodyFrame::KinectBodyFrame()
{
	for (int i = 0; i < BODY_COUNT; ++i	)
	{
		ppBodies[i] = 0;
	}
	//ppBodies[0] = nullptr;// new IBody *[BODY_COUNT];
	skeletonImg.create(424, 512, CV_8UC3);//与深度相机同一个分辨率
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

int KinectBodyFrame::GetCurrentBrightness()//返回亮度差值
{
	return _currentLightness;
// 	if (_userIsPanning)
// 	{
// 		return _currentHandPos.x - _startHandPos.x;
// 	}
// 	return 0;
}

//根据人物骨骼生成绘图
void KinectBodyFrame::GenerateSkeletonImg()
{
	skeletonImg.setTo(0);

	HRESULT hr = E_FAIL;
	int n = _countof(ppBodies);
	for (int i = 0; i < n; ++i)//对每个Body循环
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
	
		Joint joints[JointType_Count];//存储关节点对象
			HandState leftHandState = HandState_Unknown;//左手状态
			HandState rightHandState = HandState_Unknown;//右手状态
	
		DepthSpacePoint *depthSpacePos = new DepthSpacePoint [_countof(joints)];//存关节在Depth空间里的位置
		if (SUCCEEDED(pBody->GetJoints(_countof(joints), joints)))
		{
			for (int j = 0; j < _countof(joints); ++j)
			{
				m_pCoordinateMapper->MapCameraPointToDepthSpace(joints[j].Position, &depthSpacePos[j]);//坐标映射
			}
		}
	
		//获取左右手状态
			pBody->get_HandLeftState(&leftHandState);
			pBody->get_HandRightState(&rightHandState);
	
// 		DrawHand(depthSpacePos[JointType_HandLeft], leftHandState);
		DrawHand(depthSpacePos[JointType_HandRight], rightHandState);

		delete[] depthSpacePos;

	}
}

// 获取人物骨骼并生成绘图
void KinectBodyFrame::ProcessKinectDataToSkeletonImg()
{
	HRESULT hr = m_pBodyFrameReader->AcquireLatestFrame(&pBodyFrame);//获得骨架信息

	if (SUCCEEDED(hr))
	{
// 		ppBodies[0] = nullptr;//放每个骨骼的指针，最多6个人，这里只放一个
// 		IBody* ppBodies[BODY_COUNT] = { 0 };//每一个IBody可以追踪一个人，总共可以追踪六个人

		hr = pBodyFrame->GetAndRefreshBodyData(_countof(ppBodies), ppBodies);//把kinect追踪到的人的信息，分别存到每一个IBody中
	}
	if (FAILED(hr))
	{
		return;// QMessageBox::information(nullptr, QStringLiteral("失败"), QStringLiteral("获取骨骼失败"), QMessageBox::Yes);
	}

	GenerateSkeletonImg();

	for (int i = 0; i < _countof(ppBodies); ++i)
	{
		SafeRelease(ppBodies[i]);//释放所有骨骼
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
	
	circle(skeletonImg, cvPoint(pointPos.X, pointPos.Y), 20, color, -1);//画手位置
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
