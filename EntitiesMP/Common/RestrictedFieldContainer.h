#ifndef __RESTRICTEDCONTAINER_H
#define __RESTRICTEDCONTAINER_H

#include<Engine/Math/ExtensionMath.h>
#include<EntitiesMP/RestrictedField.h>
#include<EntitiesMP/Player.h>

//////////////////////////////////////////////////////////////////////////
//				���� ���� �����̳�.
// Desc :	�ش� ���� ���ο� �÷��̾ ������ Ư�� �ൿ�� ���ϵ��� ��.
//////////////////////////////////////////////////////////////////////////

// container functions
// insert and remove fields
void InsertRestrictedField(CRestrictedField* field);
void RemoveRestrictedField(CRestrictedField* field);

// lookup functions
// find the field the player is in
CRestrictedField* FindPlayerInRestrictedField(const CPlayer* player);
bool IsRestrictedSystemActive(void);

#endif