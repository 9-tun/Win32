#include "pch.h"
#include "CCamera.h"

#include "CObject.h"
#include "CCore.h"

#include "CCore.h"
#include "CtimeMgr.h"
#include "CKeyMgr.h"

#include "CResMgr.h"
#include "CTexture.h"

CCamera::CCamera()
	: m_pTargetObj(nullptr)
	, m_fTime(0.5f)
	, m_fSpeed(0.f)
	, m_fAccTime(0.5f)
	, m_pVeilTex(nullptr)
{

}

CCamera::~CCamera()
{

}

void CCamera::init()
{
	Vec2 vResolution = CCore::GetInst()->GetResolution();
	m_pVeilTex = CResMgr::GetInst()->CreateTexture(L"CameraVeil",(UINT)vResolution.x, (UINT)vResolution.y);
}

void CCamera::update()
{
	if (m_pTargetObj)
	{
		if (m_pTargetObj->IsDead())
		{
			m_pTargetObj = nullptr;
		}
		else
		{
			m_vLookAt = m_pTargetObj->GetPos();
		}
	}

	// 키보드 방향키가 눌려 있으면 한 프레임이 바뀔 때마다 화면이 보고 있는 위치를 설정 픽셀 값만큼 움직임 
	if (KEY_HOLD(KEY::UP))
		m_vLookAt.y -= 500.f * fDT;
	if (KEY_HOLD(KEY::DOWN))
		m_vLookAt.y += 500.f * fDT;
	if (KEY_HOLD(KEY::LEFT))
		m_vLookAt.x -= 500.f * fDT;
	if (KEY_HOLD(KEY::RIGHT))
		m_vLookAt.x += 500.f * fDT;

	// 화면 중앙좌표와 카메라 LookAt 좌표 간의 차이값 계산
	CalDiff();
}

void CCamera::render(HDC _dc)
{
	if (m_listCamEffect.empty())
		return;

	tCamEffect& effect = m_listCamEffect.front();

	// 시간 누적  값을 체크해서 	
	effect.fCurTime += fDT;


	float fRatio = 0.f;		// 이펙트 진행 비율
	fRatio = effect.fCurTime / effect.fDuration;

	// fRatio 범위가 0~1사이에 있도록 하기
	if (fRatio < 0.f)
		fRatio = 0.f;
	if (fRatio > 1.f)
		fRatio = 1.f;

	int iAlpha = 0;

	if (CAM_EFFECT::FADE_OUT == effect.eEffect)
	{		   
		iAlpha = (int)(255.f * fRatio);
	}
	else if (CAM_EFFECT::FADE_IN == effect.eEffect)
	{
		iAlpha = (int)(255.f * (1.f - fRatio));
	}

	BLENDFUNCTION bf = {};
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = iAlpha;

	AlphaBlend(_dc, 0, 0
		, m_pVeilTex->Width()
		, m_pVeilTex->Height()
		, m_pVeilTex->GetDC()
		, 0,0
		, m_pVeilTex->Width()
		, m_pVeilTex->Height()
		, bf);

	// 경과 시간이 이펙트 최대 지정 시간을 넘어선 경우
	if (effect.fDuration < effect.fCurTime)
	{
		// 효과 종료
		m_listCamEffect.pop_front();
	}


}

void CCamera::CalDiff()
{
	// 이전 LookAt과 현재 LookAt의 차이값을 보정해서 현재 LookAt을 구한다.
	m_fAccTime += fDT;
	if (m_fTime <= m_fAccTime)
	{
		m_vCurLookAt = m_vLookAt;	// 이동하는데 누적된 시간이 설정한 지속 시간 이상이면 그냥 최종 목적지로 완전히 이동
	}
	else
	{
		Vec2 vLookDir = m_vLookAt - m_vPrevLookAt;	// 방향 벡터 (x축으로 이동해야 할 거리, y축으로 이동해야 할 거리)
		if (!vLookDir.IsZero())
		{
			m_vCurLookAt = m_vPrevLookAt + vLookDir.Nomalize() * m_fSpeed * fDT;	// m_fSpeed 속도로 vLookDir 단위 방향(Nomalize)으로 이동
		}

	}

	Vec2 vResolution = CCore::GetInst()->GetResolution();
	Vec2 vCenter = vResolution / 2.f;

	m_vDiff = m_vCurLookAt - vCenter;
	m_vPrevLookAt = m_vCurLookAt;
}
