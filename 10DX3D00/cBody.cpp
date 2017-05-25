#include "stdafx.h"
#include "cBody.h"


cBody::cBody()
{
}


cBody::~cBody()
{
}

void cBody::Setup()
{
	cCubeNode::Setup();

	{
		// : ¾Õ À­»ï°¢
		m_vecVertex[0].t.x = (float)32 / 64;	m_vecVertex[0].t.y = (float)32 / 32;
		m_vecVertex[1].t.x = (float)32 / 64;	m_vecVertex[1].t.y = (float)20 / 32;
		m_vecVertex[2].t.x = (float)40 / 64;	m_vecVertex[2].t.y = (float)20 / 32;

		// : ¾Õ ¾Æ·¡ »ï°¢
		m_vecVertex[3].t.x = (float)32 / 64;	m_vecVertex[3].t.y = (float)32 / 32;
		m_vecVertex[4].t.x = (float)40 / 64;	m_vecVertex[4].t.y = (float)20 / 32;
		m_vecVertex[5].t.x = (float)40 / 64;	m_vecVertex[5].t.y = (float)32 / 32;

		// : µÞ À§ »ï°¢
		m_vecVertex[6].t.x = (float)28 / 64;	m_vecVertex[6].t.y = (float)32 / 32;
		m_vecVertex[7].t.x = (float)20 / 64;	m_vecVertex[7].t.y = (float)20 / 32;
		m_vecVertex[8].t.x = (float)28 / 64;	m_vecVertex[8].t.y = (float)20 / 32;

		// : µÚ ¾Æ·¡ »ï°¢
		m_vecVertex[9].t.x = (float)28 / 64;	m_vecVertex[9].t.y = (float)32 / 32;
		m_vecVertex[10].t.x = (float)20 / 64;	m_vecVertex[10].t.y = (float)32 / 32;
		m_vecVertex[11].t.x = (float)20 / 64;	m_vecVertex[11].t.y = (float)20 / 32;

		// : ¿ÞÂÊ À§
		m_vecVertex[12].t.x = (float)28 / 64;	m_vecVertex[12].t.y = (float)32 / 32;
		m_vecVertex[13].t.x = (float)28 / 64;	m_vecVertex[13].t.y = (float)20 / 32;
		m_vecVertex[14].t.x = (float)32 / 64;	m_vecVertex[14].t.y = (float)20 / 32;

		// : ¿ÞÂÊ ¾Æ·¡
		m_vecVertex[15].t.x = (float)28 / 64;	m_vecVertex[15].t.y = (float)32 / 32;
		m_vecVertex[16].t.x = (float)32 / 64;	m_vecVertex[16].t.y = (float)20 / 32;
		m_vecVertex[17].t.x = (float)32 / 64;	m_vecVertex[17].t.y = (float)32 / 32;

		// : ¿À¸¥ÂÊ 
		m_vecVertex[18].t.x = (float)16 / 64;	m_vecVertex[18].t.y = (float)32 / 32;
		m_vecVertex[19].t.x = (float)16 / 64;	m_vecVertex[19].t.y = (float)20 / 32;
		m_vecVertex[20].t.x = (float)20 / 64;	m_vecVertex[20].t.y = (float)20 / 32;

		m_vecVertex[21].t.x = (float)16 / 64;	m_vecVertex[21].t.y = (float)32 / 32;
		m_vecVertex[22].t.x = (float)20 / 64;	m_vecVertex[22].t.y = (float)20 / 32;
		m_vecVertex[23].t.x = (float)20 / 64;	m_vecVertex[23].t.y = (float)32 / 32;

		// : 
		m_vecVertex[24].t.x = (float)28 / 64;	m_vecVertex[24].t.y = (float)16 / 32;
		m_vecVertex[25].t.x = (float)28 / 64;	m_vecVertex[25].t.y = (float)20 / 32;
		m_vecVertex[26].t.x = (float)20 / 64;	m_vecVertex[26].t.y = (float)20 / 32;

		m_vecVertex[27].t.x = (float)28 / 64;	m_vecVertex[27].t.y = (float)16 / 32;
		m_vecVertex[28].t.x = (float)20 / 64;	m_vecVertex[28].t.y = (float)20 / 32;
		m_vecVertex[29].t.x = (float)20 / 64;	m_vecVertex[29].t.y = (float)16 / 32;

		m_vecVertex[30].t.x = (float)28 / 64;	m_vecVertex[30].t.y = (float)16 / 32;
		m_vecVertex[31].t.x = (float)28 / 64;	m_vecVertex[31].t.y = (float)20 / 32;
		m_vecVertex[32].t.x = (float)36 / 64;	m_vecVertex[32].t.y = (float)20 / 32;

		m_vecVertex[33].t.x = (float)28 / 64;	m_vecVertex[33].t.y = (float)16 / 32;
		m_vecVertex[34].t.x = (float)36 / 64;	m_vecVertex[34].t.y = (float)20 / 32;
		m_vecVertex[35].t.x = (float)36 / 64;	m_vecVertex[35].t.y = (float)20 / 32;
	}
	

	D3DXMATRIXA16 matS, matT, mat; 
	D3DXMatrixScaling(&matS, 0.4f, 0.6f, 0.2f); 
	D3DXMatrixTranslation(&matT, 0.0f, 0.0f, 0.0f);
	mat = matS*matT;

	for (size_t i = 0; i < m_vecVertex.size(); i++)
	{
		D3DXVec3TransformCoord(&m_vecVertex[i].p,
			&m_vecVertex[i].p,
			&mat);
	}
}