#pragma once
class cCubePNT
{
public:
	cCubePNT();

protected :
	std::vector<ST_PNT_VERTEX>	m_vecVertex; 

public :
	virtual ~cCubePNT(); 
	virtual	void Setup();
	virtual	void Update();
	virtual	void Render(); 
};

