#include "pch.h"
#include "CScene_Tool.h"

#include "CKeyMgr.h"
#include "CTile.h"

#include "CCore.h"
#include "CResMgr.h"
#include "CSceneMgr.h"
#include "CPathMgr.h"

#include "resource.h"

#include "CUIMgr.h"
#include "CPanelUI.h"
#include "CBtnUI.h"

void ChangeScene(DWORD_PTR, DWORD_PTR);

CScene_Tool::CScene_Tool()
	: m_pUI(nullptr)
{
}

CScene_Tool::~CScene_Tool()
{
}

void CScene_Tool::Enter()
{
	// 툴 Scene에서 사용할 메뉴를 붙인다.
	CCore::GetInst()->DockMenu();

	// 타일 생성
	CreateTile(5, 5);

	// UI 하나 만들어 보기
	Vec2 vResolution = CCore::GetInst()->GetResolution();		// 윈도우 해상도

	CUI* pPanelUI = new CPanelUI;
	pPanelUI->SetName(L"ParentUI");
	pPanelUI->SetScale(Vec2(300.f, 150.f));
	pPanelUI->SetPos(Vec2(vResolution.x- pPanelUI->GetScale().x, 0.f));	// 우측 상단에 위치하려면 해상도가 필요함 ↑위에서 얻어오자
	
	// 자식 UI
	CBtnUI* pBtnUI = new CBtnUI;
	pBtnUI->SetName(L"ChildUI");
	pBtnUI->SetScale(Vec2(100.f, 40.f));
	pBtnUI->SetPos(Vec2(0.f, 0.f));
	((CBtnUI*)pBtnUI)->SetClickedCallBack(this, (SCENE_MEMFUNC)&CScene_Tool::SaveTileData);
	pPanelUI->AddChild(pBtnUI);
	AddObject(pPanelUI, GROUP_TYPE::UI);

	// 복사본 UI
	//CUI* pClonePanel = pPanelUI->Clone();
	//pClonePanel->SetPos(pPanelUI->GetPos() + Vec2(-300.f, 0.f));
	//((CBtnUI*)(pClonePanel->GetChildUI()[0]))->SetClickedCallBack(&ChangeScene, 0, 0); // 복사본 UI의 첫 번째 자식 UI 버튼 클릭 시 씬 변경
	//AddObject(pClonePanel, GROUP_TYPE::UI);

	//m_pUI = pClonePanel;

	// Camera Look 지정
	CCamera::GetInst()->SetLookAt(vResolution / 2.f);
}

void CScene_Tool::Exit()
{
	CCore::GetInst()->DivideMenu();
	DeleteAll();
}


void CScene_Tool::update()
{
	CScene::update();
	SetTileIdx();

	//if (KEY_TAP(KEY::LSHIFT))
	//{
	//	//CUIMgr::GetInst()->SetFocusedUI(m_pUI);
	//	SaveTileData();
	//	
	//}


	if (KEY_TAP(KEY::CTRL))
	{
		LoadTileData();
	}

}

void CScene_Tool::SetTileIdx()
{
	if (KEY_TAP(KEY::LBTN))
	{
		Vec2 vMousePos = MOUSE_POS;
		vMousePos = CCamera::GetInst()->GetRealPos(vMousePos);	// 마우스 클릭 실제 좌표 얻기

		int iTileX = (int)GetTileX();
		int iTileY = (int)GetTileY();

		int iCol = (int)vMousePos.x / TILE_SIZE;
		int iRow = (int)vMousePos.y / TILE_SIZE;

		// Tile 바깥을 찍으면 에러가 나서 예외 처리를 해주자
		if (vMousePos.x < 0.f || iTileX <= iCol
			|| vMousePos.y < 0.f || iTileY <= iRow)
		{
			return;
		}

		UINT iIdx = iRow * iTileX + iCol;

		const vector<CObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);
		((CTile*)vecTile[iIdx])->AddImgIdx();
	}
}

void CScene_Tool::SaveTileData()
{
	wchar_t szName[256] = {};

	OPENFILENAME ofn = {};

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CCore::GetInst()->GetMainHwnd();
	ofn.lpstrFile = szName;
	ofn.nMaxFile = sizeof(szName);
	ofn.lpstrFilter = L"All\0*.*\0Tile\0*.tile\0";
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = nullptr; 
	ofn.nMaxFileTitle = 0;

	wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
	strTileFolder += L"Tile";
	
	ofn.lpstrInitialDir = strTileFolder.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Modal 방식 
	if(GetSaveFileName(&ofn))
	{
		SaveTile(szName);
	}	
}

void CScene_Tool::SaveTile(const wstring & _strFilePath)
{	
	// 커널 오브젝트
	FILE* pFile = nullptr;

	_wfopen_s(&pFile, _strFilePath.c_str(), L"wb");	// Stream 열기, 쓰기 모드면 파일이 없으면 파일을 만들어 버린다.
	assert(pFile);

	// 타일 가로세로 개수 저장
	UINT xCount = GetTileX();
	UINT yCount = GetTileY();

	fwrite(&xCount, sizeof(UINT), 1, pFile);
	fwrite(&yCount, sizeof(UINT), 1, pFile);

	// 모든 타일들을 개별적으로 저장할 데이터를 저장하게 함
	const vector<CObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);
	for (size_t i = 0; i < vecTile.size(); ++i)
	{
		((CTile*)vecTile[i])->Save(pFile);	// 각자 타일에게 저장하도록 함
	}

	fclose(pFile);	// Stream 닫기
}

void CScene_Tool::LoadTileData()
{
	wchar_t szName[256] = {};

	OPENFILENAME ofn = {};

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CCore::GetInst()->GetMainHwnd();
	ofn.lpstrFile = szName;
	ofn.nMaxFile = sizeof(szName);
	ofn.lpstrFilter = L"All\0*.*\0Tile\0*.tile\0";
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;

	wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
	strTileFolder += L"Tile";

	ofn.lpstrInitialDir = strTileFolder.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Modal 방식 
	if (GetOpenFileName(&ofn))
	{
		wstring strRelativePath = CPathMgr::GetInst()->GetRelativePath(szName);
		LoadTile(strRelativePath);
	}
}

void ChangeScene(DWORD_PTR, DWORD_PTR)
{
	ChangeScene(SCENE_TYPE::START);
}


// =======================
// Titlt Count Window Proc
// =======================
INT_PTR CALLBACK TileCountProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)	// Dialog 박스에서 확인 버튼을 누르거나 취소 버튼을 누르면
		{
			UINT iXCount = GetDlgItemInt(hDlg, IDC_EDIT1, nullptr, false);
			UINT iYCount = GetDlgItemInt(hDlg, IDC_EDIT2, nullptr, false);

			CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

			// Tool Scene 확인
			CScene_Tool* pToolScene = dynamic_cast<CScene_Tool*>(pCurScene);	 // dynamic case는 실패 시 nullptr 반환
			assert(pToolScene);

			pToolScene->DeleteGroup(GROUP_TYPE::TILE); // 기존 타일 삭제
			pToolScene->CreateTile(iXCount, iYCount);

			EndDialog(hDlg, LOWORD(wParam));						// 본인을 종료
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}