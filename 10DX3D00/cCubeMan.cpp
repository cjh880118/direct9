#include "stdafx.h"
#include "cCubeMan.h"

#include "cBody.h"
#include "cHead.h"
#include "cLeftArm.h"
#include "cLeftLeg.h"
#include "cRightArm.h"
#include "cRightLeg.h"


cCubeMan::cCubeMan()
	// : to do someting 
	: m_pRoot(NULL)
{
}


cCubeMan::~cCubeMan()
{
	// : to do someting 
	if (m_pRoot)
		m_pRoot->Destroy();

	SAFE_RELEASE(m_pTexture); 

}

void cCubeMan::Setup()
{
	// : to do someting 
	cCharacter::Setup(); 

	Set_Material(); 

	D3DXCreateTextureFromFile(g_pD3DDevice, "batman.png",
		&m_pTexture); 

	cBody * pBody = new cBody; 
	pBody->Setup();
	pBody->SetParentWorldTM(&m_matWorld); 
	m_pRoot = pBody; 

	cHead * pHead = new cHead;
	pHead->Setup();
	m_pRoot->AddChild(pHead); 

	cLeftArm * pLeftArm = new cLeftArm;
	pLeftArm->Setup();
	pLeftArm->SetRotDeltaX(0.1f); 
	m_pRoot->AddChild(pLeftArm);

	cRightArm * pRightArm = new cRightArm;
	pRightArm->Setup();
	pRightArm->SetRotDeltaX(-0.1f);
	m_pRoot->AddChild(pRightArm);

	cLeftLeg * pLeftLeg = new cLeftLeg;
	pLeftLeg->Setup();
	pLeftLeg->SetRotDeltaX(-0.1f);
	m_pRoot->AddChild(pLeftLeg);

	cRightLeg * pRightLeg = new cRightLeg;
	pRightLeg->Setup();
	pRightLeg->SetRotDeltaX(0.1f);
	m_pRoot->AddChild(pRightLeg);
}

void cCubeMan::Update(iMap* pMap)
{
	// : to do someting 
	cCharacter::Update(pMap);
	if (m_pRoot)
		m_pRoot->Update(); 
}

void cCubeMan::Render()
{
	// : to do someting 
	
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true); 
	g_pD3DDevice->SetMaterial(&m_stMtl); 

	cCharacter::Render();

	D3DXMATRIXA16 matWorld; 
	D3DXMatrixIdentity(&matWorld); 
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld); 
	g_pD3DDevice->SetTexture(0, m_pTexture); 

	if (m_pRoot)
		m_pRoot->Render(); 
}

void cCubeMan::Set_Material()
{
	ZeroMemory(&m_stMtl, sizeof(D3DMATERIAL9));
	m_stMtl.Ambient = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	m_stMtl.Diffuse = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	m_stMtl.Specular = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
}
