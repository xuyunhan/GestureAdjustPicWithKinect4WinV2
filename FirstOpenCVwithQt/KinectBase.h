#pragma once
#include <stdafx.h>
#include <iostream>
#include <Kinect.h>

#include <qmessagebox.h>
using namespace std;

class KinectBase
{
public:
	KinectBase();
	~KinectBase();

	// Safe release for interfaces
	template<class Interface>
	inline void SafeRelease(Interface *& pInterfaceToRelease)
	{
		if (pInterfaceToRelease != nullptr)
		{
			pInterfaceToRelease->Release();
			pInterfaceToRelease = nullptr;
		}
	}

	HRESULT InitializeKinectSensor();

	IKinectSensor*					m_pKinectSensor;//kinect源
	ICoordinateMapper*			m_pCoordinateMapper;//用于坐标变换
	IBodyFrameReader*			m_pBodyFrameReader;//用于骨架数据读取
	IDepthFrameReader*			m_pDepthFrameReader;//用于深度数据读取
	IBodyIndexFrameReader*	m_pBodyIndexFrameReader;//用于背景二值图读取

};

