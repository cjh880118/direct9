#pragma once

#define g_pTextureManager cTextureManager::GetInstance() 

class cTextureManager
{
/*
public:
	cTextureManager();
	~cTextureManager();
*/
private :
	SINGLETONE(cTextureManager);
	
	std::map<std::string, LPDIRECT3DTEXTURE9> m_mapTexture; 

	std::map<std::string, D3DXIMAGE_INFO> m_mapImageInfo;
public :
	LPDIRECT3DTEXTURE9	GetTexture(char* szFullPath); 
	LPDIRECT3DTEXTURE9	GetTexture(std::string& sFullPath); 
	void Destroy();

	LPDIRECT3DTEXTURE9	GetTexture(char* szFullPath, D3DXIMAGE_INFO* pImageInfo);
	LPDIRECT3DTEXTURE9	GetTexture(std::string& sFullPath, D3DXIMAGE_INFO* pImageInfo);
};

