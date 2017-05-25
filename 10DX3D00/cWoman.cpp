#include "stdafx.h"
#include "cWoman.h"
#include "cAseLoader.h"
#include "cFrame.h"


cWoman::cWoman()
	:m_pRootFrame(NULL)
{
	
}


cWoman::~cWoman()
{
	m_pRootFrame->Destroy();
}

void cWoman::Setup()
{
	cCharacter::Setup();
	cAseLoader l;
	m_pRootFrame = l.Load("woman/woman_01_all.ASE");
}

void cWoman::Update(iMap* pMap)
{
	cCharacter::Update(pMap);
	if (m_pRootFrame)m_pRootFrame->Update(m_pRootFrame->GetKeyFrame(), &m_matWorld);
}


void cWoman::Render()
{
	if (m_pRootFrame)m_pRootFrame->Render();
}

void cWoman::Render2()
{
	if (m_pRootFrame)m_pRootFrame->Render2();
}

