#pragma once

// >> : 
class cCubePC; 
class cCamera; 
class cGrid; 
// << : 
//class cCubeMan; 
class cGroup; 
//class cFrame;
class cWoman;
class cMtlTex;
//class cCharacter;

#include "cUIButton.h"

class cMainGame : public iButtonDelegate
{
public:
	cMainGame();
	~cMainGame();

	// >> :
private :

public :
	void Setup();
	void Update(); 
	void Render(); 
	// << :

		// >> : 
private : 
	
	//cCubePC*	m_pCubePC; 
	cCamera*	m_pCamera; 
	cGrid*		m_pGrid; 
//	cCubeMan*	m_pCubeMan;	
	//cCharacter* m_pCubeMan;
	std::vector<cGroup*> m_vecGroup;	 
	iMap* m_pMap;
	// >> : 
	//LPDIRECT3DTEXTURE9	 m_pTexture; 
	std::vector<ST_PT_VERTEX> m_vecVertex; 
	// << : 
	//cFrame*		m_pRootFrame;
	cWoman*			m_pWoman;

	LPD3DXFONT m_pFont;
	//ID3DXMesh* m_p3DText;

	//>>: 17_05_15
	/*
	LPD3DXMESH m_pMeshTeapot;
	LPD3DXMESH m_pMeshSphere;
	D3DMATERIAL9 m_stMtlTeapot;
	D3DMATERIAL9 m_stMtlSphere;
	*/
	//<<
	
	//>>: 17_05_16
	LPD3DXMESH m_ObjMesh;
	std::vector<cMtlTex *> m_vecObjMtlTex;
	//<<

	std::vector<ST_SPHERE> m_vecSphere;
	D3DMATERIAL9		   m_stMtlNone;
	D3DMATERIAL9		   m_stMtlPicked;
	D3DMATERIAL9		   m_stMtlPlane;
	std::vector<ST_PN_VERTEX> m_vecPlaneVertex;
	D3DXVECTOR3	m_vPickedPosition;

	//>>
	LPD3DXSPRITE	m_pSprite;
	D3DXIMAGE_INFO	m_stImageInfo;
	cUIObject* m_pUIRoot;

	

public :
	void WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	void Set_Light();
	void Obj_Render(); // << : 
	// << : 

	void Load_Surface();

	void Create_Font();
	void Text_Render();


	//>>:17_05_15
public:
	void SetUp_MeshObject();
	void Mesh_Render();
	void SetUp_PickingObj();
	void PickingObj_Render();
	void SetUp_HeightMap();
	void SetUp_UI();
	void UI_Render();
	virtual void OnClick(cUIButton* pSender) override;
	//<<
};

