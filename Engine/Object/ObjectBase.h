
#ifndef		_OBJECT_BASE_H_
#define		_OBJECT_BASE_H_

#include <string>
#include <Engine/Util.h>
#include "DefineObject.h"

//#pragma warning(disable:4150)

enum eATTRIBUTE_TYPE
{
	eATTR_NONE = 0,
	eATTR_ATT,
	eATTR_DEF,
};

struct ObjectBase
{
	eOBJ_TYPE	m_eType;			// ������Ʈ ����
	INDEX 		m_nIdxServer;		// ���� NetID
	INDEX 		m_nIdxClient;		// 
	INDEX		m_nType;			// ������Ʈ�� Ÿ��
	std::string m_strName;			// �̸�

	CEntity* 	m_pEntity;			// ��

	SBYTE		m_yLayer;
	SBYTE		m_sbPremiumType;	// �����̾� ĳ���� Ÿ��
	UBYTE		m_ubAttrAtt;		// �Ӽ� ����
	UBYTE		m_ubAttrDef;
	UBYTE		m_ubAttrAttLv;
	UBYTE		m_ubAttrDefLv;

	ObjectBase() 
		: m_nIdxServer(-1)
		, m_nIdxClient(-1)
		, m_pEntity(NULL)
		, m_sbPremiumType(0)
		, m_ubAttrAtt(0)
		, m_ubAttrDef(0)
		, m_ubAttrAttLv(1)
		, m_ubAttrDefLv(1)
	{}

	virtual ~ObjectBase()
	{
		//SAFE_DELETE(m_pEntity);
	}

	INDEX		GetSIndex()				{ return m_nIdxServer; }
	INDEX		GetCIndex()				{ return m_nIdxClient; }
	INDEX		GetType()				{ return m_nType; }
	CEntity*	GetEntity()				{ return m_pEntity; }
	void		SetyLayer(SBYTE cLayer)	{ m_yLayer = cLayer; }
	SBYTE		GetyLayer()				{ return m_yLayer; }
	void		SetPremiumType(SBYTE sbType) { m_sbPremiumType = sbType; }
	SBYTE		GetPremiumType()		{ return m_sbPremiumType; }
	// �Ӽ�
	void		SetAttribute(SBYTE sbType, UBYTE ubAttr, UBYTE ubAttrLv = 1)
	{
		if(sbType == eATTR_ATT)	
		{
			m_ubAttrAtt = ubAttr;
			m_ubAttrAttLv = ubAttrLv;
		}
		else if (sbType == eATTR_DEF)	
		{
			m_ubAttrDef = ubAttr;
			m_ubAttrDefLv = ubAttrLv;
		}
	}
	UBYTE		GetAttrAtt()			{ return m_ubAttrAtt; }
	UBYTE		GetAttrDef()			{ return m_ubAttrDef; }
	UBYTE		GetAttrAttLv()			{ return m_ubAttrAttLv; }
	UBYTE		GetAttrDefLv()			{ return m_ubAttrDefLv; }
};

#endif		// _OBJECT_BASE_H_