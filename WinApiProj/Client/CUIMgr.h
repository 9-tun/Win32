#pragma once

class CUI;

class CUIMgr
{
	SINGLE(CUIMgr);

private:
	CUI*		m_pFocusedUI;

public:
	void update();

	void SetFocusedUI(CUI* _pUI);

private:
	
	CUI* GetFocusedUI();
	// 부모 UI 내에서 실제로 타겟팅된 UI를 찾아서 반환한다.
	CUI* GetTargetedUI(CUI* _pParentUI);	
};

