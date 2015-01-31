#pragma once
#include "KinectBase.h"
#include "G:\opencv-2.4.10-extract\opencv\include\opencv2\core\core.hpp"

struct SHandPos
{
	float x, y;
};

class KinectBodyFrame : public KinectBase
{
public:
	KinectBodyFrame();
	~KinectBodyFrame();
	HRESULT InitializeKinectSensorForBodyFrame();
protected:
	cv::Mat skeletonImg;//骨骼图
	cv::Mat depthImg;//深度图
	IBodyFrame* pBodyFrame;
	IDepthFrame* pDepthFrame;
	IBody *ppBodies[BODY_COUNT] ;//放每个骨骼的指针，最多6个人，这里只放一个

public:
	int GetCurrentBrightness();
protected:
	void GenerateSkeletonImg();
	double _currentLightness;

public:
	// 获取人物骨骼放入ppBodies
	void ProcessKinectDataToSkeletonImg();
	void DrawHand(DepthSpacePoint pointPos, HandState handState);
	void DrawCurrentDepth();
	void UserHandClosed(DepthSpacePoint pointPos);
	void UserHandOpened(DepthSpacePoint pointPos);
protected:
	// 放手在屏幕上的位置
	SHandPos _currentHandPos;
	SHandPos _preHandPos;
	bool _userIsPanning;
};

