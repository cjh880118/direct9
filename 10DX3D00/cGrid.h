#pragma once

class cPyramid; 

class cGrid
{
public:
	cGrid();
	~cGrid();

private:
	std::vector<ST_PC_VERTEX>	m_vecVertex;
	std::vector<cPyramid*>		m_vecPyramid;


	std::vector<ST_PT_VERTEX> m_vecBottomVertex;
	LPDIRECT3DTEXTURE9 m_pTexture;
	D3DMATERIAL9	   m_stMtl;	// << : 13.

public:
	void Setup(int nNumHalfTile = 15, float fInterval = 1.0f);
	void Render();
	void TextureRender();
private:
	int m_nNumTri;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
};

