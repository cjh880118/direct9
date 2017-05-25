#include "stdafx.h"
#include "cGrid.h"
#include "cPyramid.h"

cGrid::cGrid()
	: m_nNumTri(0)
	, m_pVB(NULL)
	, m_pTexture(NULL)
	, m_vecBottomVertex(NULL)
{
}


cGrid::~cGrid()
{
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pTexture);
	for each(auto p in m_vecPyramid)
	{
		SAFE_DELETE(p);
	}
}

void cGrid::Setup(int nNumHalfTile, float fInterval)
{
	float fMax = nNumHalfTile * fInterval;
	float fMin = -nNumHalfTile * fInterval;

	D3DXCreateTextureFromFile(g_pD3DDevice, "batman.png",
		&m_pTexture);

	{	/// : material 설정
		ZeroMemory(&m_stMtl, sizeof(D3DMATERIAL9));
		m_stMtl.Ambient = D3DXCOLOR(0.0f, 0.7f, 0.7f, 1.0f);
		m_stMtl.Diffuse = D3DXCOLOR(0.0f, 0.7f, 0.7f, 1.0f);
		m_stMtl.Specular = D3DXCOLOR(0.0f, 0.7f, 0.7f, 1.0f);
	}

	ST_PT_VERTEX vT;
	
	vT.p = D3DXVECTOR3(fMax, 0, fMax); 
	vT.t = D3DXVECTOR2(0, 0);
	m_vecBottomVertex.push_back(vT);
	vT.p = D3DXVECTOR3(fMax, 0, -fMax);
	vT.t = D3DXVECTOR2(0, 1.0f);
	m_vecBottomVertex.push_back(vT);
	vT.p = D3DXVECTOR3(-fMax, 0, fMax);
	vT.t = D3DXVECTOR2(1.0f, 1.0f);
	m_vecBottomVertex.push_back(vT);
	
	vT.p = D3DXVECTOR3(-fMax, 0, fMax);
	vT.t = D3DXVECTOR2(1.0f, 1.0f);
	m_vecBottomVertex.push_back(vT);
	vT.p = D3DXVECTOR3(fMax, 0, -fMax);
	vT.t = D3DXVECTOR2(0, 1.0f);
	m_vecBottomVertex.push_back(vT);
	vT.p = D3DXVECTOR3(-fMax, 0, -fMax);
	vT.t = D3DXVECTOR2(0, 1.0f);
	m_vecBottomVertex.push_back(vT);
	
	

	ST_PC_VERTEX v;

	for (int i = 1; i <= nNumHalfTile; ++i)
	{
		
		if (i % 5 == 0)
			v.c = D3DCOLOR_XRGB(255, 255, 255);
		else
			v.c = D3DCOLOR_XRGB(128, 128, 128);
		
		v.p = D3DXVECTOR3(fMin, 0, i * fInterval); m_vecVertex.push_back(v);

		v.p = D3DXVECTOR3(fMax, 0, i * fInterval); m_vecVertex.push_back(v);

		v.p = D3DXVECTOR3(fMin, 0, -i * fInterval); m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(fMax, 0, -i * fInterval); m_vecVertex.push_back(v);

		v.p = D3DXVECTOR3(i * fInterval, 0, fMin); m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(i * fInterval, 0, fMax); m_vecVertex.push_back(v);

		v.p = D3DXVECTOR3(-i * fInterval, 0, fMin); m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(-i * fInterval, 0, fMax); m_vecVertex.push_back(v);
	}

	v.c = D3DCOLOR_XRGB(255, 0, 0);
	v.p = D3DXVECTOR3(fMin, 0, 0); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(fMax, 0, 0); m_vecVertex.push_back(v);

	v.c = D3DCOLOR_XRGB(0, 255, 0);
	v.p = D3DXVECTOR3(0, fMin, 0); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(0, fMax, 0); m_vecVertex.push_back(v);

	v.c = D3DCOLOR_XRGB(0, 0, 255);
	v.p = D3DXVECTOR3(0, 0, fMin); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(0, 0, fMax); m_vecVertex.push_back(v);
	
	cPyramid* pPyramid = NULL;
	D3DXMATRIXA16 matR;
	
	pPyramid = new cPyramid;
	D3DXMatrixRotationZ(&matR, D3DX_PI / 2.0f);
	pPyramid->Setup(D3DCOLOR_XRGB(255, 0, 0), matR);
	m_vecPyramid.push_back(pPyramid);

	pPyramid = new cPyramid;
	D3DXMatrixRotationZ(&matR, D3DX_PI);
	pPyramid->Setup(D3DCOLOR_XRGB(0, 255, 0), matR);
	m_vecPyramid.push_back(pPyramid);

	pPyramid = new cPyramid;
	D3DXMatrixRotationX(&matR, -D3DX_PI / 2.0f);
	pPyramid->Setup(D3DCOLOR_XRGB(0, 0, 255), matR);
	m_vecPyramid.push_back(pPyramid);
	
	{
		m_nNumTri = m_vecVertex.size() / 2;
		g_pD3DDevice->CreateVertexBuffer(
			m_vecVertex.size() * sizeof(ST_PC_VERTEX)
			, 0
			, ST_PT_VERTEX::FVF
			, D3DPOOL_MANAGED
			, &m_pVB
			, NULL
		);
		ST_PC_VERTEX* pV = NULL;
		m_pVB->Lock(0, 0, (LPVOID*)&pV, 0);
		memcpy(pV, &m_vecVertex[0], m_vecVertex.size() * sizeof(ST_PC_VERTEX));
		m_pVB->Unlock();
	}
}

void cGrid::Render()
{
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false); // << : 13.
	D3DXMATRIXA16 matI;
	D3DXMatrixIdentity(&matI);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matI);
	// : 이번 제외 g_pD3DDevice->SetTexture(0, NULL);
	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);
	g_pD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(ST_PC_VERTEX));
	g_pD3DDevice->DrawPrimitive(D3DPT_LINELIST, 0, m_nNumTri);
	
	/*
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST,
		m_vecVertex.size() / 2,
		&m_vecVertex[0],
		sizeof(ST_PT_VERTEX));
	
	*/
	
	for each(auto p in m_vecPyramid)
	{
		p->Render();
	}
	
}

void cGrid::TextureRender()
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	g_pD3DDevice->SetMaterial(&m_stMtl);
	g_pD3DDevice->SetTexture(0, m_pTexture);
	g_pD3DDevice->SetFVF(ST_PT_VERTEX::FVF);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
		m_vecBottomVertex.size() / 3,
		&m_vecBottomVertex[0],
		sizeof(ST_PT_VERTEX));

	g_pD3DDevice->SetTexture(0, NULL);
}
