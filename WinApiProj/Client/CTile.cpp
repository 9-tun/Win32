#include "CTile.h"

#include "CTexture.h"

CTile::CTile()
	: m_pTileTex(nullptr)
	, m_iImgIdx(0)
{
	SetScale(Vec2((float)TILE_SIZE, (float)TILE_SIZE));
}

CTile::~CTile()
{
}

void CTile::update()
{
}

void CTile::render(HDC _dc)
{
	if (nullptr == m_pTileTex || -1 == m_iImgIdx)
		return;

	UINT iWidth = m_pTileTex->Width();
	UINT iHeight = m_pTileTex->Height();

	UINT iMaxCol = iWidth / (UINT)TILE_SIZE;
	UINT iMaxRow = iHeight / (UINT)TILE_SIZE;

	UINT iCurRow = (UINT)m_iImgIdx / iMaxCol;
	UINT iCurCol = (UINT)m_iImgIdx % iMaxCol;

	// �̹��� ������ ��� �ε���
	if (iMaxCol <= iCurRow)
		assert(nullptr);

	Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(GetPos());	// ��ü�� ȭ��� ǥ�� ��ǥ�� ��´�
	Vec2 vScale = GetScale();	// ��ü ũ��

	// Ÿ���� ��ü�� �»� ������ ����
	BitBlt(_dc
		, int(vRenderPos.x)
		, int(vRenderPos.y)
		, int(vScale.x)
		, int(vScale.y)
		, m_pTileTex->GetDC()
		, iCurCol*(INT)TILE_SIZE
		, iCurRow*(INT)TILE_SIZE
		, SRCCOPY);

}

void CTile::Save(FILE* _pFile)
{
	fwrite(&m_iImgIdx, sizeof(int), 1, _pFile);
}

void CTile::Load(FILE* _pFile)
{
	fread(&m_iImgIdx, sizeof(int), 1, _pFile);
}
