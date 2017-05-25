#pragma once
#include "cCubePNT.h"

class cCubeNode : public cCubePNT
{
public:
	cCubeNode();
	
protected :
	float			m_fRotX; // : 
	D3DXVECTOR3		m_vLocalPos; 
	D3DXMATRIXA16	m_matLocalTM; 
	D3DXMATRIXA16	m_matWorldTM;
	std::vector<cCubeNode*> m_vecChild; 

	SYNTHESIZE(D3DXMATRIXA16*, m_pParentWorldTM, ParentWorldTM);
	SYNTHESIZE(float, m_fRotDeltaX, RotDeltaX);

public :
	virtual ~cCubeNode();


	virtual void Destroy();
	void AddChild(cCubeNode* pChild);
	virtual void Setup() override;
	virtual void Update() override;
	virtual void Render() override;
};

