#pragma once
#include "Asciitok.h"
class cMtlTex;
class cFrame;

class cAseLoader
{
public:
	cAseLoader();
	~cAseLoader();
public:
	FILE*							m_fp;
	char							m_szToken[1024];
	std::vector<cMtlTex*>			m_vecMtlTex;
	std::map<std::string, cFrame*>  m_mapFrame;
public:
	cFrame* Load(IN char* szFullPath);
private:
	char* GetToken();
	int GetInteger();
	float GetFloat();
	bool IsWhite(IN char c);
	bool IsEqual(IN char* str1, IN char* str2);
	void SkipBlock();
	void ProcessMATERIAL_LIST();
	void ProcessMATERIAL(OUT cMtlTex* pMtlTex);
	void ProcessMAP_DIFFUSE(OUT cMtlTex* pMtlTex);
	cFrame* ProcessGEOMOBJECT();
	void ProcessMESH(OUT cFrame* pFrame);
	void ProcessMESH_VERTEX_LIST(OUT std::vector<D3DXVECTOR3>& vecV);
	void ProcessMESH_FACE_LIST(OUT std::vector<ST_PNT_VERTEX>& vecverTex, 
								IN std::vector<D3DXVECTOR3>& vecV);

	void ProcessMESH_TVERTLIST(OUT std::vector<D3DXVECTOR2>& vecVT);
	void ProcessMESH_TFACELIST(OUT std::vector<ST_PNT_VERTEX>& vecVertex,
								IN std::vector<D3DXVECTOR2>& vecVT);
	void ProcessMESH_NORMALS(OUT std::vector<ST_PNT_VERTEX>& vecVertex);

	void ProcessNODE_TM(OUT cFrame* pFrame);



	DWORD m_dwFirstFrame;
	DWORD m_dwLastFrame;
	DWORD m_dwFrameSpeed;
	DWORD m_dwTicksPerFrame;
	void Set_SceneFrame(OUT cFrame* pRoot);
	void ProcessScene();

	void ProcessTM_ANIMATION(OUT cFrame* pFrame);
	void ProcessCONTROL_POS_TRACK(OUT cFrame* pFrame);
	void ProcessCONTROL_ROT_TRACK(OUT cFrame* pFrame);
};