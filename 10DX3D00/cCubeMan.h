#pragma once

#include "cCharacter.h"
class cCubeNode; 

class cCubeMan  : public cCharacter
{
public:
	cCubeMan();
	~cCubeMan();

public :
	cCubeNode*	m_pRoot; 
	LPDIRECT3DTEXTURE9 m_pTexture;  // << : 
	D3DMATERIAL9		m_stMtl; 
	// : 

	virtual void Setup() override;
	virtual void Update(iMap* pMap) override;
	virtual void Render() override;

	void Set_Material(); // << : 

};

