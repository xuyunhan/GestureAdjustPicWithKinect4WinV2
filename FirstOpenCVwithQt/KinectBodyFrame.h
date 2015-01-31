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
	cv::Mat skeletonImg;//����ͼ
	cv::Mat depthImg;//���ͼ
	IBodyFrame* pBodyFrame;
	IDepthFrame* pDepthFrame;
	IBody *ppBodies[BODY_COUNT] ;//��ÿ��������ָ�룬���6���ˣ�����ֻ��һ��

public:
	int GetCurrentBrightness();
protected:
	void GenerateSkeletonImg();
	double _currentLightness;

public:
	// ��ȡ�����������ppBodies
	void ProcessKinectDataToSkeletonImg();
	void DrawHand(DepthSpacePoint pointPos, HandState handState);
	void DrawCurrentDepth();
	void UserHandClosed(DepthSpacePoint pointPos);
	void UserHandOpened(DepthSpacePoint pointPos);
protected:
	// ��������Ļ�ϵ�λ��
	SHandPos _currentHandPos;
	SHandPos _preHandPos;
	bool _userIsPanning;
};

