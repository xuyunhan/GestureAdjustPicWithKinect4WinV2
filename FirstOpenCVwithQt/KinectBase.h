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

	IKinectSensor*					m_pKinectSensor;//kinectԴ
	ICoordinateMapper*			m_pCoordinateMapper;//��������任
	IBodyFrameReader*			m_pBodyFrameReader;//���ڹǼ����ݶ�ȡ
	IDepthFrameReader*			m_pDepthFrameReader;//����������ݶ�ȡ
	IBodyIndexFrameReader*	m_pBodyIndexFrameReader;//���ڱ�����ֵͼ��ȡ

};

