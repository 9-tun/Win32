#include "CGround.h"

#include "CCollider.h"
#include "CGravity.h"

CGround::CGround()
{
	CreateCollider();
}

CGround::~CGround()
{
}

void CGround::start()
{
	GetCollider()->SetScale(Vec2(GetScale()));
}

void CGround::update()
{
}

void CGround::OnCollisionEnter(CCollider * _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"Player")
	{
		pOtherObj->GetGravity()->SetGround(true);

		Vec2 vObjPos = _pOther->GetFinalPos();		// Ground�� �浹�� ��ü�� ��ġ
		Vec2 vObjScale = _pOther->GetScale();		// Ground�� �浹�� ��ü�� ũ��

		Vec2 vPos = GetCollider()->GetFinalPos();	// Ground ��ü ��ġ
		Vec2 vScale = GetCollider()->GetScale();	// Ground ��ü ũ��
		
		float fLen = abs(vObjPos.y - vPos.y);						// �� ��ü ���� �Ÿ� 
		float fValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fLen;	// �� ��ü�� y������ ��ģ ����

		// �浹�� ���� ���·� �����ϱ� ���ؼ� �Ϻη� 1�ȼ��� �� �÷���
		vObjPos = pOtherObj->GetPos();	// Ground�� �浹�� ��ü�� ��ġ�� 
		vObjPos.y -= (fValue);	// ��ģ ��ŭ y������ �÷��ֱ�

		pOtherObj->SetPos(vObjPos);
	}
}

void CGround::OnCollision(CCollider * _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"Player")
	{
		pOtherObj->GetGravity()->SetGround(true);

		Vec2 vObjPos = _pOther->GetFinalPos();		// Ground�� �浹�� ��ü�� ��ġ
		Vec2 vObjScale = _pOther->GetScale();		// Ground�� �浹�� ��ü�� ũ��

		Vec2 vPos = GetCollider()->GetFinalPos();	// Ground ��ü ��ġ
		Vec2 vScale = GetCollider()->GetScale();	// Ground ��ü ũ��

		float fLen = abs(vObjPos.y - vPos.y);						// �� ��ü ���� �Ÿ� 
		float fValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fLen;	// �� ��ü�� y������ ��ģ ����

		// �浹�� ���� ���·� �����ϱ� ���ؼ� �Ϻη� 1�ȼ��� �� �÷���
		vObjPos = pOtherObj->GetPos();	// Ground�� �浹�� ��ü�� ��ġ�� 
		vObjPos.y -= (fValue);	

		pOtherObj->SetPos(vObjPos);
	}
}

void CGround::OnCollisionExit(CCollider * _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"Player")
	{
		pOtherObj->GetGravity()->SetGround(false);
	}
}

