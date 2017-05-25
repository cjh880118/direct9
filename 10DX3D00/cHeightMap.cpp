#include "stdafx.h"
#include "cHeightMap.h"


cHeightMap::cHeightMap()
	: m_pMesh(NULL)
	, m_pTexture(NULL)
{
}


cHeightMap::~cHeightMap()
{
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pTexture);
}

void cHeightMap::Setup(char * szFolder, char * szRaw, char * szTex, DWORD dwBytePerPixel)
{
	std::string sFolder(szFolder);
	std::string sRaw = sFolder + std::string(szRaw);
	std::string sTex = sFolder + std::string(szTex);

	m_pTexture = g_pTextureManager->GetTexture(sTex);

	FILE* fp = NULL;
	fopen_s(&fp, sRaw.c_str(), "rb");


	//>>파일 사이즈를 구함 끝으로 보냄
	fseek(fp, 0, SEEK_END);
	int nFileSize = ftell(fp);
	//
	int nNumVertex = nFileSize / dwBytePerPixel;

	int nRow = (int)(sqrt((float)nNumVertex) + 0.0001f);
	int nCol = nRow;
	int nTileN = nRow - 1;
	m_nTileN = nTileN;
	//처음으로 보냄
	fseek(fp, 0, SEEK_SET);

	std::vector<ST_PNT_VERTEX> vecVertex(nNumVertex);
	m_vecVertex.resize(nNumVertex);

	std::vector<DWORD> vecIndex;
	//면 2 점 3
	vecIndex.reserve(nTileN * nTileN * 2 * 3);

	for (int i = 0; i < nNumVertex; i++) {
		ST_PNT_VERTEX v;
		v.p = D3DXVECTOR3(i % nCol,
			((unsigned char) fgetc(fp)) / 10.0f, i/nCol);
		v.n = D3DXVECTOR3(0, 1, 0);
		v.t = D3DXVECTOR2((i % nCol) / (float)nCol, (i / nCol) / (float)nCol);

		vecVertex[i] = v;
		m_vecVertex[i] = v.p;
		if (dwBytePerPixel == 3) {
			fgetc(fp);
			fgetc(fp);
		}
	}

	fclose(fp);

	for (int x = 1; x < nTileN; x++)
	{
		for (int z = 1; z < nTileN; z++)
		{
			int left  = (z + 0) * nCol + x - 1;
			int right = (z + 0) * nCol + x + 1;
			int up    = (z + 1) * nCol + x + 0;
			int down  = (z - 1) * nCol + x + 0;

			D3DXVECTOR3 leftToRight = m_vecVertex[right] - m_vecVertex[left];
			D3DXVECTOR3 downToUp = m_vecVertex[up] - m_vecVertex[down];
			D3DXVECTOR3 normal;
			D3DXVec3Cross(&normal, &downToUp, &leftToRight);
			D3DXVec3Normalize(&normal, &normal);

			int nIndex = z * nCol + x;
			vecVertex[nIndex].n = normal;
		}
	}

	for (int x = 0; x < nTileN ; x++) {
		for (int z = 0; z < nTileN ; z++)
		{
			int _0 = (z + 0) * nCol + x + 0;
			int _1 = (z + 1) * nCol + x + 0;
			int _2 = (z + 1) * nCol + x + 1;
			int _3 = (z + 0) * nCol + x + 1;

			vecIndex.push_back(_0);
			vecIndex.push_back(_1);
			vecIndex.push_back(_3);

			vecIndex.push_back(_2);
			vecIndex.push_back(_3);
			vecIndex.push_back(_1);
		}
	}

	D3DXCreateMeshFVF(
		vecIndex.size() / 3
		, vecVertex.size()
		, D3DXMESH_MANAGED | D3DXMESH_32BIT
		, ST_PNT_VERTEX::FVF
		, g_pD3DDevice
		, &m_pMesh);

	ST_PNT_VERTEX* pV = NULL;

	m_pMesh->LockVertexBuffer(0, (LPVOID*)&pV);
	memcpy(pV, &vecVertex[0], vecVertex.size() * sizeof(ST_PNT_VERTEX));
	m_pMesh->UnlockVertexBuffer();

	DWORD* pI = NULL;
	m_pMesh->LockIndexBuffer(0, (LPVOID*)&pI);
	memcpy(pI, &vecIndex[0], vecIndex.size() * sizeof(DWORD));
	m_pMesh->UnlockIndexBuffer();

	DWORD* pA = NULL;
	m_pMesh->LockAttributeBuffer(0, &pA);
	ZeroMemory(pA, (vecIndex.size() / 3 ) * sizeof(DWORD));
	m_pMesh->UnlockAttributeBuffer();

	std::vector<DWORD> vecAdj(vecIndex.size());
	m_pMesh->GenerateAdjacency(0.0f, &vecAdj[0]);

	m_pMesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		&vecAdj[0],
		0, 0, 0);

	ZeroMemory(&m_stMtl, sizeof(D3DMATERIAL9));
	m_stMtl.Ambient = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	m_stMtl.Diffuse = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	m_stMtl.Specular = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
}

void cHeightMap::Render()
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);

	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	g_pD3DDevice->SetMaterial(&m_stMtl);
	g_pD3DDevice->SetTexture(0, m_pTexture);
	m_pMesh->DrawSubset(0);
}

bool cHeightMap::GetHeight(IN float x, OUT float & y, IN float z)
{
	if (x < 0.0f || z < 0.0f || x >= m_nTileN || z >= m_nTileN) {
		y = 0;
		return false;
	}
	int nX = x;
	int nZ = z;

	float fDeltaX = x - nX;
	float fDeltaZ = z - nZ;

	int _0 = (nZ + 0) * (m_nTileN + 1) + nX + 0;
	int _1 = (nZ + 1) * (m_nTileN + 1) + nX + 0;
	int _2 = (nZ + 1) * (m_nTileN + 1) + nX + 1;
	int _3 = (nZ + 0) * (m_nTileN + 1) + nX + 1;

	if (fDeltaX + fDeltaZ < 1.0f) {
		D3DXVECTOR3 v01 = m_vecVertex[_1] - m_vecVertex[_0];
		D3DXVECTOR3 v03 = m_vecVertex[_3] - m_vecVertex[_0];
		
		y = m_vecVertex[_0].y + (v01 * fDeltaZ + v03 * fDeltaX).y;
		return true;
	}
	else {
		fDeltaX = 1.0f - fDeltaX;
		fDeltaZ = 1.0f - fDeltaZ;
		D3DXVECTOR3 v21 = m_vecVertex[_1] - m_vecVertex[_2];
		D3DXVECTOR3 v23 = m_vecVertex[_3] - m_vecVertex[_2];

		y = m_vecVertex[_2].y + (v21 * fDeltaX + v23 * fDeltaZ).y;
		return true;
	}
	return true;
}
