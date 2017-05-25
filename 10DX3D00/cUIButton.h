#pragma once
#include "cUIObject.h"
class cUIButton;
class iButtonDelegate
{
public:
	virtual void OnClick(cUIButton* pSender) = 0;
};



class cUIButton :
	public cUIObject
{
public:
	cUIButton();
	virtual ~cUIButton();
protected:
	enum eButtonState
	{
		E_NORMAL,
		E_MOUSEOVER,
		E_SELECTED,
		E_STATE_CNT
	};

	eButtonState m_eButtonState;
	LPDIRECT3DTEXTURE9 m_aTexture[E_STATE_CNT];

	//>>버튼 클릭 처리
	SYNTHESIZE(iButtonDelegate*, m_pDelegate, Delegate);
	//<<
public:
	virtual void SetTexture(char* szNor, char* szOvr, char* szSel);
	virtual void Update() override;
	virtual void Render(LPD3DXSPRITE pSprite) override;
};

