#pragma once

class CObject;
class AI;

void CreateObject(CObject* _pObj, GROUP_TYPE _eGroup);	// 총알과 같은 객체 생성
void DeleteObject(CObject* _pObj);						// 충돌 발생 시, 객체 삭제에 사용
void ChangeScene(SCENE_TYPE _eNext);
void ChangeAIState(AI* _pAI, MON_STATE _eNextState);


template<typename T>
void Safe_Delete_Vec(vector<T>& _vec)	// 참조값으로 받아야 삭제가 가능
{
	for (size_t i = 0; i < _vec.size(); ++i)
	{
		if (nullptr != _vec[i])
			delete _vec[i];
	}
	_vec.clear();
}


template<typename T1, typename T2>
void Safe_Delete_Map(map<T1,T2>& _map)
{
	typename map<T1, T2>::iterator iter = _map.begin();	// map<T1, T2>::iterator가 타입인지 변수인지 명확히 해주기 위해서 앞에 typename을 적어 줌 (iterator는 타입임)
	for (; iter != _map.end(); ++iter)
	{
		if(nullptr != iter->second)
			delete iter->second;
	}
	_map.clear();
}

void FScanf(char* _pOutBuff, FILE* _pFile);

void SaveWString(const wstring& _str, FILE* _pFile);
void LoadWString(wstring& _str, FILE* _pFile);