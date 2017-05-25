#pragma once


#include "cCharacter.h"
class cFrame;

class cWoman : public cCharacter
{
public:
	cWoman();
	~cWoman();
private:
	cFrame*		m_pRootFrame;


public:
	//virtual void Destroy();
	virtual void Setup() override;
	virtual void Render() override;
	virtual void Render2();
	virtual void Update(iMap* pMap);
};

