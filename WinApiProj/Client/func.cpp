#include "pch.h"
#include "func.h"

#include "CEventMgr.h"

void CreateObject(CObject* _pObj, GROUP_TYPE _eGroup)
{
	tEvent evn = {};
	evn.eEven = EVENT_TYPE::CREATE_OBJECT;
	evn.lParam = (DWORD_PTR)_pObj;
	evn.wParam = (DWORD_PTR)_eGroup;

	CEventMgr::GetInst()->AddEvent(evn);
}

void DeleteObject(CObject * _pObj)
{
	tEvent evn = {};
	evn.eEven = EVENT_TYPE::DELETE_OBJECT;
	evn.lParam = (DWORD_PTR)_pObj;

	CEventMgr::GetInst()->AddEvent(evn);
}

void ChangeScene(SCENE_TYPE _eNext)
{
	tEvent evn = {};
	evn.eEven = EVENT_TYPE::SCENE_CHANGE;
	evn.lParam = (DWORD_PTR)_eNext;

	CEventMgr::GetInst()->AddEvent(evn);
}

void ChangeAIState(AI * _pAI, MON_STATE _eNextState)
{
	tEvent evn = {};
	evn.eEven = EVENT_TYPE::CHANGE_AI_STATE;
	evn.lParam = (DWORD_PTR)_pAI;
	evn.wParam = (DWORD_PTR)_eNextState;

	CEventMgr::GetInst()->AddEvent(evn);
}

void FScanf(char * _pOutBuff, FILE * _pFile)
{
	// 파일에서 개행까지 한 줄을 읽어오는 코드
	int i = 0;
	while (true)
	{
		char c = (char)getc(_pFile);
		if (c == '\n')				// 개행을 만나면 break
		{
			_pOutBuff[i++] = '\0';	// 이번에 읽어온 데 까지가 어디인지 알 수 있도록 표시, 이게 없으면 전에 읽었던 것도 계속 읽음 (예, 첨에 5글자 읽고 그 다음에 3글자 읽은 경우)
			break;
		}
		_pOutBuff[i++] = c;
	}
}

void SaveWString(const wstring & _str, FILE * _pFile)
{
	// Animation의 이름을 저장한다. (데이터 직렬화: 주소값이 아닌 객체의 상태를 바이트 스트림으로 변환하여 저장))
	// fwrite(&m_strName, sizeof(wstring), 1, pFile); 그냥 이렇게 저장하면 오류가 난다. 왜냐면 wstring에 담을 문자열은 애초에 크기가 정해져 있지 않기 때문
	const wchar_t*	pStrName = _str.c_str();
	size_t iLen = _str.length();

	// 문자 길이 저장
	fwrite(&iLen, sizeof(size_t), 1, _pFile);

	// 문자열 저장
	fwrite(pStrName, sizeof(wchar_t), iLen, _pFile);	// sizeof(wchar_t) 2byte씩 iLen(글자수)만큼 문자를 저장한다.
}

void LoadWString(wstring & _str, FILE * _pFile)
{
	size_t iLen = 0;
	fread(&iLen, sizeof(size_t), 1, _pFile);
	wchar_t szBuff[256] = {};
	fread(szBuff, sizeof(wchar_t), iLen, _pFile); 

	_str = szBuff;
}
