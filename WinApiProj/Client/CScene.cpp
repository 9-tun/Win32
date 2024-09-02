#include "pch.h"
#include "CScene.h"

#include "CObject.h"
#include "CTile.h"
#include "CResMgr.h"
#include "CPathMgr.h"

#include "CCore.h"
#include "CCamera.h"

CScene::CScene()
	: m_iTileX(0)
	, m_iTileY(0)
	, m_pPlayer(nullptr)
{
}

CScene::~CScene()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		for (UINT j = 0; j < m_arrObj[i].size(); ++j)
		{
			// m_arrObj[i] 그룹 벡터의 j물체 삭제
			delete m_arrObj[i][j];
		}
	}
}

void CScene::start()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		for (UINT j = 0; j < m_arrObj[i].size(); ++j)
		{
			m_arrObj[i][j]->start();	
		}
	}
}

void CScene::update()	// 객체 위치 업데이트
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		for (UINT j = 0; j < m_arrObj[i].size(); ++j)
		{
			if (!m_arrObj[i][j]->IsDead())	// 객체가 죽은 상태가 아니라면
			{
				m_arrObj[i][j]->update();	// CObject* 로 Scene에 표시되는 Obj의 위치 값 업데이트 
			}
		}
	}
}

void CScene::finalupdate()	// 콤포넌트 위치 업데이트 (예: 충돌체)
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		for (UINT j = 0; j < m_arrObj[i].size(); ++j)
		{
			m_arrObj[i][j]->finalupdate();	// 각 Object가 가진 final update 실행
		}
	}
}


void CScene::render(HDC _dc)
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		if ((UINT)GROUP_TYPE::TILE == i)
		{
			render_tile(_dc);
			continue;
		}

		vector<CObject*>::iterator iter = m_arrObj[i].begin();
		for (; iter != m_arrObj[i].end(); )
		{
			if (!(*iter)->IsDead())	// 객체가 죽은 상태가 아니라면
			{
				(*iter)->render(_dc);	// Scene의 각 Object 렌더링
				++iter;					// 다음 iter로
			}
			else
			{
				iter = m_arrObj[i].erase(iter);	//삭제하고 나서는 다음 iter 가리킬 필요 없음
			}
		}
	}
}

void CScene::render_tile(HDC _dc)
{
	const vector<CObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);

	Vec2 vCamLook = CCamera::GetInst()->GetLookAt();
	Vec2 vResolution = CCore::GetInst()->GetResolution();
	Vec2 vLeftTop = vCamLook - vResolution / 2.f;			// 좌상단 좌표

	int iTileSize = TILE_SIZE;

	int iLTCol = (int)vLeftTop.x / iTileSize;			// 카메라 좌상단까지 x축 타일 갯수
	int iLTRow = (int)vLeftTop.y / iTileSize;			// 카메라 좌상단까지 y축 타일 갯수

	int iClientWidth = ((int)vResolution.x / iTileSize) + 1;	// 카메라 화면의 가로 타일 갯수
	int iClientHeight = ((int)vResolution.y / iTileSize) + 1;	// 카메라 화면의 세로 타일 갯수

	for (int iCurRow = iLTRow; iCurRow < (iLTRow + iClientHeight); ++iCurRow)
	{
		for (int iCurCol = iLTCol; iCurCol < (iLTCol + iClientWidth); ++iCurCol)
		{
			// Tile 표시 화면 밖으로 나간 경우, 예외 처리
			if (iCurCol < 0 || m_iTileX <= iCurCol
				|| iCurRow < 0 || m_iTileY <= iCurRow)
			{
				continue;
			}
			int iIdx = (m_iTileX * iCurRow) + iCurCol;
			vecTile[iIdx]->render(_dc);
		}
	}

}

void CScene::DeleteGroup(GROUP_TYPE _eTarget)
{
	Safe_Delete_Vec(m_arrObj[(UINT)_eTarget]);
}

void CScene::DeleteAll()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		DeleteGroup((GROUP_TYPE)i);
	}
}

void CScene::CreateTile(UINT _iXCount, UINT _iYCount)
{
	DeleteGroup(GROUP_TYPE::TILE);

	m_iTileX = _iXCount;
	m_iTileY = _iYCount;

	CTexture* pTileTex = CResMgr::GetInst()->LoadTexture(L"Tile", L"texture\\tile\\TILE.bmp");

	// 타일 생성
	for (UINT i = 0; i < _iYCount; ++i)
	{
		for (UINT j = 0; j < _iXCount; ++j)
		{
			CTile* pTile = new CTile();
			pTile->SetPos(Vec2((float)(j*TILE_SIZE), (float)(i*TILE_SIZE)));
			pTile->SetTexture(pTileTex);
			AddObject(pTile, GROUP_TYPE::TILE);
		}
	}
}

void CScene::LoadTile(const wstring & _strRelativePath)
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	// 커널 오브젝트
	FILE* pFile = nullptr;

	_wfopen_s(&pFile, strFilePath.c_str(), L"rb");	// Stream 열기, 읽기 모드에서 파일이 없으면 오류가 난다.
	assert(pFile);

	// 타일 가로 세로 개수 불러오기
	UINT xCount = 0;
	UINT yCount = 0;

	fread(&xCount, sizeof(UINT), 1, pFile);
	fread(&yCount, sizeof(UINT), 1, pFile);

	// 불러온 개수에 맞게 EmptyFile 만들어 두기
	CreateTile(xCount, yCount);

	// 만들어진 타일 개별로 필요한 정보를 불러오게 함
	const vector<CObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);
	for (size_t i = 0; i < vecTile.size(); ++i)
	{
		((CTile*)vecTile[i])->Load(pFile);	// 각자 타일에서 불러오도록 함
	}
	fclose(pFile);	// Stream 닫기
}
