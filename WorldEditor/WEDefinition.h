// ----------------------------------------------------------------------------
//  File : WE_DEFINITION.h
//  Desc : Created by Dongmin
// ----------------------------------------------------------------------------

#ifndef	WE_DEFINITION_H_
#define	WE_DEFINITION_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

enum eMobType
{
	TYPE_WHOLE		= 0,	// ��ü
	TYPE_ENEMY,				// ��	
	TYPE_ZONEMOVING,		// �� �̵�
	TYPE_REFINER,			// ���ݼ���
	TYPE_SHOPPER,			// ����
	TYPE_QUEST,				// ����Ʈ.
	TYPE_SKILLMASTER,		// ��ų ������.
	TYPE_SSKILLMASTER,		// Ư�� ��ų ������.
	TYPE_PRODUCE,			// ����ǰ
	TYPE_REMISSION,			// ���˺�
	TYPE_EVENT,				// �̺�Ʈ
	TYPE_GUARD,				// ���.
	TYPE_WAREHOUSE,			// â��
	TYPE_GUILD,				// ���
	TYPE_RESETSTAT,			// ���� �ʱ�ȭ NPC
	TYPE_CHANGEWEAPON,		// ���� ��ȯ NPC
	TYPE_WARCASTLE,			// ���� NPC
	TYPE_TOTAL,
};

static CString _aStrMobType[] = 
{ _T("��ü"), _T("����"), _T("���̵�"), _T("���ݼ���"), _T("����"), _T("����Ʈ"), _T("��ų������"), _T("Ư�� ��ų������"), 
_T("����ǰ"), _T("���˺�"), _T("�̺�Ʈ"), _T("���"), _T("â��"), _T("���"), _T("�����ʱ�ȭ"), _T("���� ��ȯ"), _T("����") };
#endif