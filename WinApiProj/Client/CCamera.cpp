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

	// Ű���� ����Ű�� ���� ������ �� �������� �ٲ� ������ ȭ���� ���� �ִ� ��ġ�� ���� �ȼ� ����ŭ ������ 
	if (KEY_HOLD(KEY::UP))
		m_vLookAt.y -= 500.f * fDT;
	if (KEY_HOLD(KEY::DOWN))
		m_vLookAt.y += 500.f * fDT;
	if (KEY_HOLD(KEY::LEFT))
		m_vLookAt.x -= 500.f * fDT;
	if (KEY_HOLD(KEY::RIGHT))
		m_vLookAt.x += 500.f * fDT;

	// ȭ�� �߾���ǥ�� ī�޶� LookAt ��ǥ ���� ���̰� ���
	CalDiff();
}

void CCamera::render(HDC _dc)
{
	if (m_listCamEffect.empty())
		return;

	tCamEffect& effect = m_listCamEffect.front();

	// �ð� ����  ���� üũ�ؼ� 	
	effect.fCurTime += fDT;


	float fRatio = 0.f;		// ����Ʈ ���� ����
	fRatio = effect.fCurTime / effect.fDuration;

	// fRatio ������ 0~1���̿� �ֵ��� �ϱ�
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

	// ��� �ð��� ����Ʈ �ִ� ���� �ð��� �Ѿ ���
	if (effect.fDuration < effect.fCurTime)
	{
		// ȿ�� ����
		m_listCamEffect.pop_front();
	}


}

void CCamera::CalDiff()
{
	// ���� LookAt�� ���� LookAt�� ���̰��� �����ؼ� ���� LookAt�� ���Ѵ�.
	m_fAccTime += fDT;
	if (m_fTime <= m_fAccTime)
	{
		m_vCurLookAt = m_vLookAt;	// �̵��ϴµ� ������ �ð��� ������ ���� �ð� �̻��̸� �׳� ���� �������� ������ �̵�
	}
	else
	{
		Vec2 vLookDir = m_vLookAt - m_vPrevLookAt;	// ���� ���� (x������ �̵��ؾ� �� �Ÿ�, y������ �̵��ؾ� �� �Ÿ�)
		if (!vLookDir.IsZero())
		{
			m_vCurLookAt = m_vPrevLookAt + vLookDir.Nomalize() * m_fSpeed * fDT;	// m_fSpeed �ӵ��� vLookDir ���� ����(Nomalize)���� �̵�
		}

	}

	Vec2 vResolution = CCore::GetInst()->GetResolution();
	Vec2 vCenter = vResolution / 2.f;

	m_vDiff = m_vCurLookAt - vCenter;
	m_vPrevLookAt = m_vCurLookAt;
}
