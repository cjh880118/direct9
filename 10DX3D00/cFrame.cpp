#include "stdafx.h"
#include "cFrame.h"
//>>테스트
//>>한줄추가
//>>추가
cFrame::cFrame()
	:m_pMtlTex(NULL)
	,m_nNumTri(0)
	,m_pVB(NULL)
	,m_pIB(NULL)
	,m_pMesh(NULL)
{
	D3DXMatrixIdentity(&m_matLocalTM);
	D3DXMatrixIdentity(&m_matWorldTM);
}


cFrame::~cFrame()
{
	SAFE_RELEASE(m_pMtlTex);
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);
	SAFE_RELEASE(m_pMesh);
}

void cFrame::Update(int nKeyFrame, D3DXMATRIXA16 * pmatParent)
{
	D3DXMATRIXA16 matR, matT;
	/*
	{
		D3DXMatrixIdentity(&matR);
		matR = m_matLocalTM;
		matR._41 = 0.0f;
		matR._42 = 0.0f;
		matR._43 = 0.0f;

		D3DXMatrixIdentity(&matT);
		matT._41 = m_matLocalTM._41;
		matT._42 = m_matLocalTM._42;
		matT._43 = m_matLocalTM._43;
	}
	*/
	CalcLoacalR(nKeyFrame, matR);
	CalcLoacalT(nKeyFrame, matT);
	m_matLocalTM = matR * matT;

	m_matWorldTM = m_matLocalTM;

	if (pmatParent) {
		m_matWorldTM *= (*pmatParent);
	}

	for each(auto c in m_vecChild) {
		c->Update(nKeyFrame, &m_matWorldTM);
	}
}

void cFrame::Render()
{

	if (m_pMtlTex) {
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorldTM);
		g_pD3DDevice->SetTexture(0, m_pMtlTex->GetTexture());
		g_pD3DDevice->SetMaterial(&m_pMtlTex->GetMaterial());
		g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);

		//>>17.05.17 10000번 반복
		
		m_pMesh->DrawSubset(0);
		
		//g_pD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(ST_PNT_VERTEX));
		//g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_nNumTri);
		
		/*
		g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
			m_vecVertex.size() / 3,
			&m_vecVertex[0],
			sizeof(ST_PNT_VERTEX)
		);
		*/

	}

	for each(auto c in m_vecChild) {
		c->Render();
	}
}


//>> 차이 구별 테스트용 랜더
void cFrame::Render2()
{
	if (m_pMtlTex) {
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorldTM);
		g_pD3DDevice->SetTexture(0, m_pMtlTex->GetTexture());
		g_pD3DDevice->SetMaterial(&m_pMtlTex->GetMaterial());
		g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);	
		g_pD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(ST_PNT_VERTEX));
	
		//>>17.05.17 10000번 반복
	
		g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_nNumTri);	

		/*
		g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
		m_vecVertex.size() / 3,
		&m_vecVertex[0],
		sizeof(ST_PNT_VERTEX)
		);
		*/
	}

	for each(auto c in m_vecChild) {
		c->Render();
	}
}
//<<

void cFrame::AddChild(cFrame * pChild)
{
	m_vecChild.push_back(pChild);
}

void cFrame::Destroy()
{
	for each(auto c in m_vecChild) {
		c->Destroy();
	}
	this->Release();	
}

void cFrame::CalcOriginalLocalTm(D3DXMATRIXA16 * pmatParent)
{
	m_matLocalTM = m_matWorldTM;
	if (pmatParent) {
		D3DXMATRIXA16 matInvParent;
		D3DXMatrixInverse(&matInvParent, 0, pmatParent);
		m_matLocalTM = m_matWorldTM * matInvParent;
	}

	for each(auto c in m_vecChild) {
		c->CalcOriginalLocalTm(&m_matWorldTM);
	}
}

int cFrame::GetKeyFrame()
{
	int first = m_dwFirstFrame * m_dwTicksPerFrame;
	int last = m_dwLastFrame * m_dwTicksPerFrame;
	return GetTickCount() % (last - first) + first;
}

void cFrame::CalcLoacalT(IN int nKeyFrame, OUT D3DXMATRIXA16 & matT)
{	
	D3DXMatrixIdentity(&matT);
	if (m_vecPosTrack.empty()) {
		matT._41 = m_matLocalTM._41;
		matT._42 = m_matLocalTM._42;
		matT._43 = m_matLocalTM._43;
		return;
	}

	if (nKeyFrame <= m_vecPosTrack.front().n) {
		matT._41 = m_vecPosTrack.front().v.x;
		matT._42 = m_vecPosTrack.front().v.y;
		matT._43 = m_vecPosTrack.front().v.z;
		return;
	}

	if (nKeyFrame >= m_vecPosTrack.back().n) {
		matT._41 = m_vecPosTrack.back().v.x;
		matT._42 = m_vecPosTrack.back().v.y;
		matT._43 = m_vecPosTrack.back().v.z;
		return;
	}
	
	int nNextIndex = 0;
	for (size_t i = 0; i < m_vecPosTrack.size(); i++) {
		if (nKeyFrame < m_vecPosTrack[i].n) {
			nNextIndex = i;
			break;
		}
	}

	int nPrevIndex = nNextIndex - 1;

	float t = (nKeyFrame - m_vecPosTrack[nPrevIndex].n) /
		(float)(m_vecPosTrack[nNextIndex].n - m_vecPosTrack[nPrevIndex].n);

	D3DXVECTOR3 v;
	D3DXVec3Lerp(&v, &m_vecPosTrack[nPrevIndex].v,
					 &m_vecPosTrack[nNextIndex].v,
					 t);
	matT._41 = v.x;
	matT._42 = v.y;
	matT._43 = v.z;
}

void cFrame::CalcLoacalR(IN int nKeyFrame, OUT D3DXMATRIXA16 & matR)
{
	D3DXMatrixIdentity(&matR);
	if (m_vecRotTrack.empty()) {
		matR = m_matLocalTM;
		matR._41 = 0.0f;
		matR._42 = 0.0f;
		matR._43 = 0.0f;
		return;
	}

	if (nKeyFrame <= m_vecRotTrack.front().n) {
		D3DXMatrixRotationQuaternion(&matR,
							&m_vecRotTrack.front().q);
		return;
	}

	if (nKeyFrame >= m_vecRotTrack.back().n) {
		D3DXMatrixRotationQuaternion(&matR,
			&m_vecRotTrack.back().q);
		return;
	}

	int nNextIndex = 0;

	for (size_t i = 0; i < m_vecRotTrack.size(); ++i) {
		if (nKeyFrame < m_vecRotTrack[i].n) {
			nNextIndex = i;
			break;
		}
	}

	
	int nPrevIndex = nNextIndex - 1;
	
	float t = (nKeyFrame - m_vecRotTrack[nPrevIndex].n) /
		(float)(m_vecRotTrack[nNextIndex].n - m_vecRotTrack[nPrevIndex].n);


	D3DXQUATERNION q;

	D3DXQuaternionSlerp(&q,
		&m_vecRotTrack[nPrevIndex].q,
		&m_vecRotTrack[nNextIndex].q,
		t);

	D3DXMatrixRotationQuaternion(&matR, &q);
}

void cFrame::BuildVB(std::vector<ST_PNT_VERTEX>& vecVertex)
{
	m_nNumTri = vecVertex.size() / 3;
	g_pD3DDevice->CreateVertexBuffer(
		vecVertex.size() * sizeof(ST_PNT_VERTEX)
		, 0
		, ST_PNT_VERTEX::FVF
		, D3DPOOL_MANAGED
		, &m_pVB
		, NULL
		);
	ST_PNT_VERTEX* pV = NULL;
	m_pVB->Lock(0, 0, (LPVOID*)&pV, 0);
	memcpy(pV, &vecVertex[0], vecVertex.size() * sizeof(ST_PNT_VERTEX));

	m_pVB->Unlock();

}

void cFrame::BuildIB(std::vector<ST_PNT_VERTEX>& vecVertex)
{
	/*
	g_pD3DDevice->CreateIndexBuffer(
		vecVertex.size() * sizeof(ST_PNT_VERTEX) * 3
		, D3DUSAGE_WRITEONLY
		, D3DFMT_INDEX16
		, D3DPOOL_MANAGED
		, &m_pIB
		, NULL
	);
	WORD* pI = NULL;
	m_pIB->Lock(0, 0, (LPVOID*)&pI, 0);
	memcpy(pI, &vecVertex[0], vecVertex.size() * sizeof(ST_PNT_VERTEX)*3);

	m_pIB->Unlock();
	*/
}


//>>17.05.16 메시 생성
void cFrame::BuildMesh(OUT std::vector<ST_PNT_VERTEX>& vecVertex)
{
	std::vector<DWORD> vecAttrBuf;
	D3DXCreateMeshFVF(vecVertex.size() / 3,
		vecVertex.size(),
		D3DXMESH_MANAGED,
		ST_PNT_VERTEX::FVF,
		g_pD3DDevice,
		&m_pMesh);


	ST_PNT_VERTEX *pV = NULL;
	m_pMesh->LockVertexBuffer(0, (LPVOID*)&pV);
	memcpy(pV, &vecVertex[0], vecVertex.size() * sizeof(ST_PNT_VERTEX));
	m_pMesh->UnlockVertexBuffer();

	WORD* pI = NULL;
	m_pMesh->LockIndexBuffer(0, (LPVOID*)&pI);
	for (int i = 0; i < vecVertex.size(); i++) {
		pI[i] = i;
	}
	m_pMesh->UnlockIndexBuffer();
	/*
	DWORD* pA = NULL;
	pMesh->LockAttributeBuffer(0, &pA);
	memcpy(pA, &vecAttrBuf[0], vecAttrBuf.size() * sizeof(DWORD));
	pMesh->UnlockAttributeBuffer();
	*/
	std::vector<DWORD> vecAdj(vecVertex.size());
	m_pMesh->GenerateAdjacency(0.0f, &vecAdj[0]);

	m_pMesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		&vecAdj[0], 0, 0, 0);	
}

//>>