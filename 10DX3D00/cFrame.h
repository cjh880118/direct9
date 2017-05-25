#pragma once
#include "cObject.h"
#include "cMtlTex.h"

//>테스트
class cFrame :
	public cObject
{
public:
	cFrame();
	~cFrame();

private:
	SYNTHESIZE_PASS_BY_REF(std::vector<ST_PNT_VERTEX>, m_vecVertex, Vertex);
	SYNTHESIZE_PASS_BY_REF(D3DXMATRIXA16, m_matLocalTM, LocalTM);
	SYNTHESIZE_PASS_BY_REF(D3DXMATRIXA16, m_matWorldTM, WorldTM);
	SYNTHESIZE_PASS_BY_REF(std::vector<ST_POS_SAMPLE>, m_vecPosTrack, PosTrack);
	SYNTHESIZE_PASS_BY_REF(std::vector<ST_ROT_SAMPLE>, m_vecRotTrack, RotTrack);
	SYNTHESIZE_ADD_REF(cMtlTex*, m_pMtlTex, MtlTex);
	std::vector<cFrame*> m_vecChild;

public:
	void Update(int nKeyFrame, D3DXMATRIXA16 * pmatParent);
	void Render();
	void Render2(); //>>테스트
	void AddChild(cFrame* pChild);
	void Destroy();
	void CalcOriginalLocalTm(D3DXMATRIXA16 * pmatParent);

	DWORD m_dwFirstFrame;
	DWORD m_dwLastFrame;
	DWORD m_dwFrameSpeed;
	DWORD m_dwTicksPerFrame;
	int GetKeyFrame();
	void CalcLoacalT(IN int nKeyFrame, OUT D3DXMATRIXA16& matT);
	void CalcLoacalR(IN int nKeyFrame, OUT D3DXMATRIXA16& matR);
private:
	int m_nNumTri;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9 m_pIB;
	//<< 17.05.16
	LPD3DXMESH m_pMesh;
	//<<
public:
	void BuildVB(std::vector<ST_PNT_VERTEX>& vecVertex);
	void BuildIB(std::vector < ST_PNT_VERTEX>& vecVertex);
	//<< 17.05.16
	void BuildMesh(OUT std::vector<ST_PNT_VERTEX>& vecVertex);
	//<<
};

