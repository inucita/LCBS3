// ----------------------------------------------------------------------------
//  File : TransformInfo.h
//  Desc : Created by Dongmin
// ----------------------------------------------------------------------------
#ifndef	TRANSFORMINFO_H_
#define	TRANSFORMINFO_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Base/CTString.h>
#include <Engine/Base/FileName.h>
#include <Engine/JobInfo.h>

class ENGINE_API CTransformInfo
{
private:
	typedef struct _tagTransformInfo
	{
		_tagTransformInfo()
		{
			iAttackSpeed				= 0;
		};

		~_tagTransformInfo()
		{
		};

		CTString		strName;									// �̸� ����.		
		CTString		strFileName;								// ���ϸ�.
		CTString		aStrAnimationName[ANIM_TOTAL];				// �ִϸ��̼� �̸�.
		int				iAttackSpeed;								// �⺻ ����		
		float			afImpactTimeTable[MAX_ATTACK];				// ����Ʈ�� ���� �� �������� ����Ǵ� Ÿ�̹�.
		int				nSkillIndex;								// ��ų �ε���.
		int				nType;
		
	}sTransformInfo;

public:

	enum eTransformType
	{
		TYPE_1 = 0,
		TYPE_2,
		TOTAL_TRANSFORM,
	};

	CTransformInfo();
	~CTransformInfo();

	static CTransformInfo &Instance()			{ return m_instance;	}

	// Set Properties
	void		SetName( int iTransform, const CTString& strName );	

	// Get Properties
	CTString	GetName( int iTransform );	
	CTString	GetFileName( int iTransform );
	
	float		GetImpactTime( int iTransform, int iAttackType )	const;
	CTString	GetAnimationName( int iTransform, int iAnimType );

	void		SetInfo( int iTransform, int nIndex );
	int			GetSkillIndex( int iTransform );
	int			GetType( int nSkillIndex );

protected:
	static CTransformInfo m_instance;
	sTransformInfo		*m_pTransformInfo;
};

inline CTransformInfo &TransformInfo()				{ return CTransformInfo::Instance();	}

#endif // TRANSFORMINFO_H_