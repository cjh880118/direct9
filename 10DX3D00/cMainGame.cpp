#include "stdafx.h"
#include "cMainGame.h"
#include "cDeviceManager.h"		/// << : 

// >> :
#include "cCubePC.h"
#include "cCamera.h"
#include "cGrid.h"
//#include "cCubeMan.h"
// << :
#include "cGroup.h"
#include "cObjLoader.h"
#include "cObject.h"
#include "cObjMap.h"

#include "cWoman.h"
#include "cHeightMap.h"

#include "cUIImageView.h"
#include "cUITextView.h"
#include "cUIButton.h"

enum 
{
	E_BUTTON_OK = 11,
	E_BUTTON_CANCEL,
	E_BUTTON_EXIT,
	E_TEXT_VIEW
};

//#include "cAseLoader.h"
//#include "cFrame.h"


cMainGame::cMainGame()
	: //m_pCubePC(NULL)
//	m_pCubeMan(NULL)
	 m_pCamera(NULL)
	, m_pGrid(NULL)
	//, m_pTexture(NULL)
	, m_pMap(NULL)
	//, m_pRootFrame(NULL)
	, m_pWoman(NULL)			//메시 랜더
	, m_pFont(NULL)
	//, m_p3DText(NULL)
	//, m_pMeshSphere(NULL)
	//, m_pMeshTeapot(NULL)
	, m_ObjMesh(NULL)
	, m_pSprite(NULL)
	, m_pUIRoot(NULL)
{

}


cMainGame::~cMainGame()
{
	//SAFE_DELETE(m_pCubePC); 
	//SAFE_DELETE(m_pCubeMan);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);
	SAFE_DELETE(m_pMap);
	SAFE_DELETE(m_pWoman);
	//m_pRootFrame->Destroy();
	
	SAFE_RELEASE(m_pFont);
	//SAFE_RELEASE(m_pTexture);
	//SAFE_RELEASE(m_p3DText);
	//SAFE_RELEASE(m_pMeshSphere);
	//SAFE_RELEASE(m_pMeshTeapot);
	SAFE_RELEASE(m_ObjMesh);
	SAFE_RELEASE(m_pSprite);

	if(m_pUIRoot)
		m_pUIRoot->Destroy();

	for each(auto p in m_vecObjMtlTex) {
		SAFE_RELEASE(p);
	}
	g_pTextureManager->Destroy();
	g_pObjectManager->Destroy();
	g_pFontManager->Destroy();
	g_pDeviceManager->Destroy();

}


void cMainGame::Setup()
{
	SetUp_UI();
	/*
	{
		D3DXCreateTextureFromFile(g_pD3DDevice, "srccodetex.png", &m_pTexture); 
		ST_PT_VERTEX v; 
		v.p = D3DXVECTOR3(0, 0, 0);
		v.t = D3DXVECTOR2(0, 1.0f); 
		m_vecVertex.push_back(v); 

		v.p = D3DXVECTOR3(0, 1, 0);
		v.t = D3DXVECTOR2(0, 0.5f);
		m_vecVertex.push_back(v);

		v.p = D3DXVECTOR3(1, 0, 0);
		v.t = D3DXVECTOR2(1, 1);
		m_vecVertex.push_back(v);
	}
	*/

	/*
	m_pCubeMan = new cCubeMan; 
	m_pCubeMan->Setup();
	*/
	/*
	{
		cCubeMan* pCubeMan = new cCubeMan;
		pCubeMan->Setup();

		m_pCubeMan = pCubeMan;

		
	}
	*/
	SetUp_HeightMap();
	//loadObj.Load(m_vecGroup, "obj", "map.obj");

	//Load_Surface();
	

	//cAseLoader l;
	//m_pRootFrame = l.Load("woman/woman_01_all.ASE");
	SetUp_PickingObj();
	
	m_pWoman = new cWoman;
	m_pWoman->Setup();
	
	m_pCamera = new cCamera;
	m_pCamera->Setup(&m_pWoman->GetPosition());
	
	
	m_pGrid = new cGrid; 
	m_pGrid->Setup();


	SetUp_MeshObject();
	
	Create_Font();
	Set_Light(); 

	
}

void cMainGame::Update()
{
	//if (m_pCubeMan)	m_pCubeMan->Update(m_pMap); 
	if (m_pCamera) m_pCamera->Update();	
	if (m_pWoman) m_pWoman->Update(m_pMap);

	if (m_pUIRoot) m_pUIRoot->Update();
	//if (m_pRootFrame) m_pRootFrame->Update(m_pRootFrame->GetKeyFrame(), NULL);
}

void cMainGame::Render()
{
	g_pD3DDevice->Clear(NULL,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(47, 121, 112),
		1.0f, 0);

	g_pD3DDevice->BeginScene();

	
	


	if (m_pGrid) m_pGrid->Render(); 
//	m_pGrid->TextureRender();


	//>메쉬로 랜더
	if (m_pWoman) m_pWoman->Render();	




	
	//Mesh_Render();
	
	//PickingObj_Render();
	
	
	if (m_pMap) {
		m_pMap->Render();
	}
	

	UI_Render();
	//Obj_Render(); // << : 
	
//	Text_Render();
	//if (m_pRootFrame)m_pRootFrame->Render();
	//if (m_pCubeMan) m_pCubeMan->Render(); 
	
	/*
	{
		D3DXMATRIXA16 matWorld; 
		D3DXMatrixIdentity(&matWorld); 
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld); 
		g_pD3DDevice->SetTexture(0, m_pTexture);
		g_pD3DDevice->SetFVF(ST_PT_VERTEX::FVF); 
		g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
			m_vecVertex.size() / 3,
			&m_vecVertex[0],
			sizeof(ST_PT_VERTEX)); 

		g_pD3DDevice->SetTexture(0, NULL);
	}
	*/

	g_pD3DDevice->EndScene();
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}
// << : 
#include "cRay.h"
void cMainGame::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pCamera)
	{
		m_pCamera->WndProc(hwnd, message, wParam, lParam); 
	}

	//>>:

	switch (message)
	{
	case WM_LBUTTONDOWN:
		{
			cRay r = cRay::RayAtWorldSpace(LOWORD(lParam), HIWORD(lParam));
			for (int i = 0; i < m_vecSphere.size(); i++) {
				m_vecSphere[i].isPicked = r.IsPicked(&m_vecSphere[i]);
			}
		}
		break;
	case WM_RBUTTONDOWN:
		{
			cRay r = cRay::RayAtWorldSpace(LOWORD(lParam), HIWORD(lParam));
			for (int i = 0; i < m_vecPlaneVertex.size(); i+=3) {
				D3DXVECTOR3 v(0, 0, 0);
				if (r.IntersectTri(m_vecPlaneVertex[i + 0].p
					, m_vecPlaneVertex[i + 1].p
					, m_vecPlaneVertex[i + 2].p
					, v));
				{
					m_vPickedPosition = v;
				}
			}

		}
		break;

	default:
		break;
	}
}

void cMainGame::Set_Light()
{
	D3DLIGHT9 stLight;
	{
		ZeroMemory(&stLight, sizeof(D3DLIGHT9));
		stLight.Type = D3DLIGHT_DIRECTIONAL;
		stLight.Ambient = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
		stLight.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
		stLight.Specular = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
		D3DXVECTOR3 vDir(1.0f, -1.0f, 1.0f);
		D3DXVec3Normalize(&vDir, &vDir);
		stLight.Direction = vDir;
		g_pD3DDevice->SetLight(0, &stLight);
	}

	g_pD3DDevice->LightEnable(0, true);
}

void cMainGame::Obj_Render()
{
	D3DXMATRIXA16 matWorld, matS, matR, matT;
	D3DXMatrixScaling(&matS, 0.01f, 0.01f, 0.01f); 
	D3DXMatrixRotationX(&matR, -D3DX_PI / 2.0F); 
	D3DXMatrixTranslation(&matT, 5, 0, 5);
	matWorld = matS * matR * matT;
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld); 
	for each(auto p in m_vecGroup)
	{
		p->Render(); 
	}
}


void cMainGame::Load_Surface()
{
	
	D3DXMATRIXA16 matWorld, matS, matR, matT;
	D3DXMatrixScaling(&matS, 0.01f, 0.01f, 0.01f);
	D3DXMatrixRotationX(&matR, -D3DX_PI / 2.0f);
	D3DXMatrixTranslation(&matT, 5, 0, 5);
	matWorld = matS * matR * matT;

	//m_pMap = new cObjMap("obj", "map_surface.obj", &matWorld);
	
}

void cMainGame::Create_Font()
{
	/*
	{
		D3DXFONT_DESC fd;
		ZeroMemory(&fd, sizeof(D3DXFONT_DESC));
		fd.Height = 50;
		fd.Width = 25;
		fd.Weight = FW_MEDIUM;
		fd.Italic = false;
		fd.CharSet = DEFAULT_CHARSET;
		fd.OutputPrecision = OUT_DEFAULT_PRECIS;
		fd.PitchAndFamily = FF_DONTCARE;
		AddFontResource("font/umberto.ttf");
		strcpy_s(fd.FaceName, "umberto");

		D3DXCreateFontIndirect(g_pD3DDevice, &fd, &m_pFont);
	}
	{
		HDC hdc = CreateCompatibleDC(0);
		LOGFONT lf;
		ZeroMemory(&lf, sizeof(LOGFONT));
		lf.lfHeight = 25;
		lf.lfWidth = 12;
		lf.lfWeight = 500; //0~1000
		lf.lfItalic = false;
		lf.lfUnderline = false;
		lf.lfStrikeOut = false;
		lf.lfCharSet = DEFAULT_CHARSET;
		strcpy_s(lf.lfFaceName, "굴림체");

		HFONT hFont;
		HFONT hFontOld;
		hFont = CreateFontIndirect(&lf);
		hFontOld = (HFONT)SelectObject(hdc, hFont);
		D3DXCreateText(g_pD3DDevice,
			hdc,
			"Direct3D",
			0.001f,
			0.01f,
			&m_p3DText,
			0,
			0
		);
		
		SelectObject(hdc, hFontOld);
		DeleteObject(hFont);
		DeleteDC(hdc);
	}
	*/
}

void cMainGame::Text_Render()
{
	/*
	{
		std::string sText(std::to_string(m_nCount1) + "//" + std::to_string(m_nCount2));
		RECT rc;
		SetRect(&rc, 100, 100, 100, 100);

	

		m_pFont->DrawTextA(NULL, 
			sText.c_str(), 
			sText.length(), 
			&rc, 
			DT_LEFT | DT_TOP | DT_NOCLIP,
			D3DCOLOR_XRGB(255, 255, 0)
			);
	}
	{
		D3DXMATRIXA16 matWorld, matS, matR, matT;
		D3DXMatrixIdentity(&matS);
		D3DXMatrixIdentity(&matR);
		D3DXMatrixIdentity(&matT);

		D3DXMatrixScaling(&matS, 1.0f, 1.0f, 100.0f);
		D3DXMatrixRotationX(&matR, -D3DX_PI / 4.0f);
		D3DXMatrixTranslation(&matT, -2.0f, 2.0f, 0.0f);

		matWorld = matS * matR * matT;

		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
		m_p3DText->DrawSubset(0);
	}
	*/
}

void cMainGame::SetUp_MeshObject()
{
	/*
	D3DXCreateTeapot(g_pD3DDevice, &m_pMeshTeapot, NULL);
	D3DXCreateSphere(g_pD3DDevice, 0.5f, 10, 10, &m_pMeshSphere, NULL);

	//머테리얼 생성
	ZeroMemory(&m_stMtlTeapot, sizeof(D3DMATERIAL9));
	ZeroMemory(&m_stMtlSphere, sizeof(D3DMATERIAL9));

	m_stMtlTeapot.Ambient = D3DXCOLOR(0.0f, 0.7f, 0.7f, 1.0f);
	m_stMtlTeapot.Diffuse = D3DXCOLOR(0.0f, 0.7f, 0.7f, 1.0f);
	m_stMtlTeapot.Specular = D3DXCOLOR(0.0f, 0.7f, 0.7f, 1.0f);

	m_stMtlSphere.Ambient = D3DXCOLOR(0.7f, 0.7f, 0.0f, 1.0f);
	m_stMtlSphere.Diffuse = D3DXCOLOR(0.7f, 0.7f, 0.0f, 1.0f);
	m_stMtlSphere.Specular = D3DXCOLOR(0.7f, 0.7f, 0.0f, 1.0f);

	//>>17.05.16
	cObjLoader loadMesh;
	m_ObjMesh = loadMesh.LoadMesh(m_vecObjMtlTex, "obj", "map.obj");
//	loadMesh.Load(m_vecGroup, "obj", "map.obj");
	//<<
	*/
}

void cMainGame::Mesh_Render()
{
	
	D3DXMATRIXA16 matWorld, matS, matR;
	/*
	{
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixIdentity(&matS);
		D3DXMatrixIdentity(&matR);
		D3DXMatrixScaling(&matS, 1.0f, 1.0f, 1.0f);
		matWorld = matS * matR;
		D3DXMatrixTranslation(&matWorld, 0, 0, 10);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
		g_pD3DDevice->SetMaterial(&m_stMtlTeapot);
		m_pMeshTeapot->DrawSubset(0);
		//>>
	//	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
	*/
	/*
	{
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixIdentity(&matS);
		D3DXMatrixIdentity(&matR);
		D3DXMatrixScaling(&matS, 1.0f, 1.0f, 1.0f);
		matWorld = matS * matR;
		D3DXMatrixTranslation(&matWorld, 0, 0.5, -5);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
		g_pD3DDevice->SetMaterial(&m_stMtlSphere);
		m_pMeshSphere->DrawSubset(0);

	}
	*/
	/*
	{
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matS, 0.01f, 0.01f, 0.01f);
		D3DXMatrixRotationX(&matR, -D3DX_PI / 2.0F);
		matWorld = matS * matR;
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
		
		for (size_t i = 0; i < m_vecObjMtlTex.size(); i++) {
			g_pD3DDevice->SetMaterial(&m_vecObjMtlTex[i]->GetMaterial());
			g_pD3DDevice->SetTexture(0, m_vecObjMtlTex[i]->GetTexture());
			m_ObjMesh->DrawSubset(i);
		}
	}
	*/
}

void cMainGame::SetUp_PickingObj()
{
	for (int i = 0; i < 10; i++)
	{
		ST_SPHERE s;
		s.fRadius = 0.5f;
		s.vCenter = D3DXVECTOR3(0, 0, -10 + 2 * i);
		m_vecSphere.push_back(s);
	}

	ZeroMemory(&m_stMtlNone, sizeof(D3DMATERIAL9));
	m_stMtlNone.Ambient = D3DXCOLOR(0.7f, 0.7f, 0.0f, 1.0f);
	m_stMtlNone.Diffuse = D3DXCOLOR(0.7f, 0.7f, 0.0f, 1.0f);
	m_stMtlNone.Specular = D3DXCOLOR(0.7f, 0.7f, 0.0f, 1.0f);

	ZeroMemory(&m_stMtlPicked, sizeof(D3DMATERIAL9));
	m_stMtlPicked.Ambient = D3DXCOLOR(0.7f, 0.0f, 0.0f, 1.0f);
	m_stMtlPicked.Diffuse = D3DXCOLOR(0.7f, 0.0f, 0.0f, 1.0f);
	m_stMtlPicked.Specular = D3DXCOLOR(0.7f, 0.0f, 0.0f, 1.0f);

	ZeroMemory(&m_stMtlPlane, sizeof(D3DMATERIAL9));
	m_stMtlPlane.Ambient = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	m_stMtlPlane.Diffuse = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	m_stMtlPlane.Specular = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);

	ST_PN_VERTEX v;
	v.n = D3DXVECTOR3(0, 1, 0);
	v.p = D3DXVECTOR3(-10, 0, -10); m_vecPlaneVertex.push_back(v);
	v.p = D3DXVECTOR3(-10, 0, 10); m_vecPlaneVertex.push_back(v);
	v.p = D3DXVECTOR3(10, 0, 10); m_vecPlaneVertex.push_back(v);

	v.p = D3DXVECTOR3(-10, 0, -10); m_vecPlaneVertex.push_back(v);
	v.p = D3DXVECTOR3(10, 0, 10); m_vecPlaneVertex.push_back(v);
	v.p = D3DXVECTOR3(10, 0, -10); m_vecPlaneVertex.push_back(v);
	
}


void cMainGame::PickingObj_Render()
{
	/*
	D3DXMATRIXA16 matWorld;

	g_pD3DDevice->SetFVF(ST_PN_VERTEX::FVF);
	g_pD3DDevice->SetMaterial(&m_stMtlPlane);
	D3DXMatrixIdentity(&matWorld);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	g_pD3DDevice->SetTexture(0, 0);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &m_vecPlaneVertex[0],
		sizeof(ST_PN_VERTEX));

	for (int i = 0; i < m_vecSphere.size(); i++) {
		D3DXMatrixIdentity(&matWorld);
		matWorld._41 = m_vecSphere[i].vCenter.x;
		matWorld._42 = m_vecSphere[i].vCenter.y;
		matWorld._43 = m_vecSphere[i].vCenter.z;
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
		g_pD3DDevice->SetMaterial(m_vecSphere[i].isPicked ?
		&m_stMtlPicked : &m_stMtlNone);
		m_pMeshSphere->DrawSubset(0);
	}
	g_pD3DDevice->SetMaterial(&m_stMtlNone);
	D3DXMatrixTranslation(&matWorld, m_vPickedPosition.x, m_vPickedPosition.y, m_vPickedPosition.z);

	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	m_pMeshSphere->DrawSubset(0);
	*/
}

void cMainGame::SetUp_HeightMap()
{
	
	cHeightMap* pMap = new cHeightMap;
	pMap->Setup("HeightMapData/", "HeightMap.raw", "terrain.jpg");
	m_pMap = pMap;
	
}


void cMainGame::SetUp_UI()
{
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);

	/*
	{
		m_pTexture = g_pTextureManager->GetTexture("UI/김태희.jpg");
	}
	*/
	/*
	{
		D3DXCreateTextureFromFileEx(
			g_pD3DDevice
			, "UI/김태희.jpg"
			, D3DX_DEFAULT_NONPOW2
			, D3DX_DEFAULT_NONPOW2
			, D3DX_DEFAULT
			, 0
			, D3DFMT_UNKNOWN
			, D3DPOOL_MANAGED
			, D3DX_FILTER_NONE
			, D3DX_DEFAULT
			, 0
			, &m_stImageInfo
			, NULL
			, &m_pTexture
		);
	}
	*/
	cUIImageView* pImageView = new cUIImageView;
	pImageView->SetPosition(D3DXVECTOR3(0, 0, 0));
	pImageView->SetTexture("UI/panel-info.png");
	m_pUIRoot = pImageView;

	cUITextView* pTextView = new cUITextView;
	pTextView->SetText("좀비 출현");
	pTextView->SetSize(ST_SZIEN(300, 200));
	pTextView->SetPosition(D3DXVECTOR3(100,100,0));
	pTextView->SetDrawTextFormat(DT_CENTER | DT_VCENTER | DT_WORDBREAK);
	pTextView->SetTextColor(D3DCOLOR_XRGB(255, 255, 0));
	pTextView->SetTag(E_TEXT_VIEW);
	m_pUIRoot->AddChild(pTextView);

	cUIButton* pButtonOK = new cUIButton;
	pButtonOK->SetTexture("UI/btn-med-up.png",
		"UI/btn-med-over.png", 
		"UI/btn-med-down.png");
	pButtonOK->SetPosition(D3DXVECTOR3(135, 330, 0));
	pButtonOK->SetDelegate(this);
	pButtonOK->SetTag(E_BUTTON_OK);
	m_pUIRoot->AddChild(pButtonOK);


}

void cMainGame::UI_Render()
{
	/*
	{
		m_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

		D3DXMATRIXA16 matT, matR, matS, mat;
		D3DXMatrixTranslation(&matT, 100, 100, 0);

		
		static float frotZ = 0.01f;
		frotZ += 0.1f;
		D3DXMatrixRotationZ(&matR, frotZ);

		mat = matT * matR;

		m_pSprite->SetTransform(&mat);

		RECT rc;
		SetRect(&rc, 0, 0, m_stImageInfo.Width, m_stImageInfo.Height);
		
		m_pSprite->Draw(m_pTexture,
			&rc,
			&D3DXVECTOR3(0, 0, 0),
			&D3DXVECTOR3(0, 0, 0),
			D3DCOLOR_ARGB(120, 255, 255, 255));
		m_pSprite->End();

	}
	*/
	if (m_pUIRoot)
		m_pUIRoot->Render(m_pSprite);
}

void cMainGame::OnClick(cUIButton * pSender)
{
	cUITextView* pTextView = (cUITextView*)m_pUIRoot->FindChildByTag(E_TEXT_VIEW);

	if (pSender->GetTag() == E_BUTTON_OK) {
		pTextView->SetText("확인");
	}
	else {

	}
}




/*
	1. 팝업
	2. 구조 - 트리
	3. 드래그 가능
	4. 파업위 마우스 카메라
	5. 버튼이미지 상태 (액션)


*/