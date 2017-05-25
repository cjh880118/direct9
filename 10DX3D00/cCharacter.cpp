#include "stdafx.h"
#include "cCharacter.h"
#include "cObjLoader.h"
#include "cGroup.h"

cCharacter::cCharacter()
	// : to do someting
	: m_fRotY(0.0f) ,
	m_vDirection(0,0,1), 
	m_vPosition(0,0,0)
{
	D3DXMatrixIdentity(&m_matWorld);
}


cCharacter::~cCharacter()
{
}

void cCharacter::Setup()
{
	

}
void cCharacter::Update(iMap* pMap) 
{
	//m_pMap = pMap;

	// : to do someting
	if (GetKeyState('A') & 0x8000)
	{
		m_fRotY -= 0.1f;
	}
	if (GetKeyState('D') & 0x8000)
	{
		m_fRotY += 0.1f;
	}

	//>>
	D3DXVECTOR3 vPosition = m_vPosition;
	//<<

	if (GetKeyState('W') & 0x8000)
	{
		vPosition = m_vPosition + (m_vDirection * 0.1f);

	}
	if (GetKeyState('S') & 0x8000)
	{
		vPosition = m_vPosition - (m_vDirection * 0.1f);
	}

	RECT rc;
	GetClientRect(g_hWnd, &rc);
	
	D3DXMATRIXA16 matR, matT;
	
	D3DXMatrixRotationY(&matR, m_fRotY);
	m_vDirection = D3DXVECTOR3(0, 0, -1);
	D3DXVec3TransformNormal(&m_vDirection, &m_vDirection, &matR);


	//>>
	//m_vPosition = vPosition;

	if (pMap) {
		if (pMap->GetHeight(vPosition.x, vPosition.y, vPosition.z)) {
			m_vPosition = vPosition;
		}
	}

	D3DXMatrixTranslation(&matT, m_vPosition.x, 
								m_vPosition.y,
								m_vPosition.z);
	m_matWorld = matR * matT;

}
void cCharacter::Render()
{

}
D3DXVECTOR3 & cCharacter::GetPosition()
{
	// : to do someting
	return m_vPosition;
}
