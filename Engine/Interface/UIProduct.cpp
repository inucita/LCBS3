#include "stdh.h"

// ��� ���� [12/2/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIProduct.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UISkillLearn.h>
#include <Engine/Help/ItemHelp.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Contents/Base/UICharacterInfoNew.h>
#include <Common/Packet/ptype_old_do_skill.h>
#include <Engine/Info/MyInfo.h>
#include <Engine/Contents/function/ProductNPCUI.h>

//SealType�� ����Ÿ�԰����� ������
int SealTypeCount[MAX_SEALTYPE+1] = 
{
	0,	//3,				//������ ���� : ����, ����, �尩
	3,	//4,				//ȭ���� ���� : ����, �Ź�, ����, ����
	7,  //7,				//��ī�ο� ���� : ����, ������, ����, �̵���, �ϵ�, ����, ȥ
	14,	//6,				//�ܴ��� ���� : ��絵, ���, �� ������, Ȱ, �ܰ�, ���̵�
	20,	//3,				//�Ҹ�ǰ ���� : ��ȸ��, ����, ��Ÿ
	23,	//End
};

int SealSubType[MAX_SEALSUBTYPE] = 
						{ 0, 1, 3,						//������ ���� : ����, ����, �尩
						 2, 4, 5, 6,					//ȭ���� ���� : ����, �Ź�, ����, ����
						 1, 2, 4, 11, 12, 14, 15,		//��ī�ο� ���� : ����, ������, ����, �̵���, �ϵ�, ����, ȥ
						 0, 3, 5, 6, 7, 13, 			//�ܴ��� ���� : ��絵, ���, �� ������, Ȱ, �ܰ�, ���̵�
						 2, 6, 4,						//�Ҹ�ǰ ���� : ��ȸ��, ����, ��Ÿ
						};

// Date : 2005-03-07,   By Lee Ki-hwan
static int	_iMaxMsgStringChar = 0;

// ----------------------------------------------------------------------------
// Name : CUIProduct()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIProduct::CUIProduct()
	: m_pTxtSubject(NULL)
	, m_pListItems(NULL)
	, m_pListDesc(NULL)
	, m_pBtnOK(NULL)
{
	setInherit(false);
}

// ----------------------------------------------------------------------------
// Name : ~CUIProduct()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIProduct::~CUIProduct()
{
	Clear();
	Destroy();	
}

// ----------------------------------------------------------------------------
// Name : Clear()
// Desc :
// ----------------------------------------------------------------------------
void CUIProduct::Clear()
{
	m_nSelectProductItem	= -1;
	m_bWaitProductResult	= TRUE;
	m_bSatisfied			= FALSE;

	m_nProductText			= -1;
	m_nTab					= -1;
	m_nInvenIdx				= -1;
	
	m_nNeedItemCount		= 0;

	memset(m_NeedItems, 0, sizeof(CNeedItems) * MAX_MAKE_ITEM_MATERIAL);

	m_bWaitProductResult = FALSE;

	if (m_pListItems != NULL)
		m_pListItems->DeleteAllListItem();

	if (m_pListDesc != NULL)
		m_pListDesc->DeleteAllListItem();

	clearItems();
}

// ----------------------------------------------------------------------------
// Name : InitProduct()
// Desc :
// ----------------------------------------------------------------------------
void CUIProduct::InitProduct()
{
	clearItems();

	CItemData* pTextItemData = _pNetwork->GetItemData ( m_nProductText );
	const char* szItemName = _pNetwork->GetItemName( m_nProductText );
	
	// �ӽ� ó�� 
	int nProductType = pTextItemData->GetProcessType();
	int nProductSubType = pTextItemData->GetProcessSubType();

	if (m_pTxtSubject != NULL)
		m_pTxtSubject->SetText(CTString((char*)szItemName));

	m_pListItems->DeleteAllListItem();
	CUIListItem* pTemp = m_pListItems->GetListItemTemplate();
	CUIListItem* pClone = NULL;
	CUIIcon* pIcon = NULL;
	CUIText* pText = NULL;

	int SubTypeBit;
	int nCnt = 0;
	CTString tmpStr, strCount;

	// ���� ������ ���� �Ҽ� �ִ� ������ �˻� 
	CItemData::_map::iterator	iter = CItemData::_mapdata.begin();
	CItemData::_map::iterator	eiter = CItemData::_mapdata.end();

	for (;iter != eiter; ++iter)
	{
		CItemData* pItemData = (*iter).second;

		if (pItemData == NULL)
			continue;

		if ( pItemData->GetFlag() & ITEM_FLAG_MADE )
		{
			SubTypeBit = 0x00000001 << pItemData->GetSubType();		

			if ( ( pItemData->GetType () == nProductType ) && ( SubTypeBit & nProductSubType ) )
			{
				// Date : 2005-01-07,   By Lee Ki-hwan 
				// ������ ��쿡�� ����� üũ �ϰ�, �ٸ� �������� ��쿡�� ������ üũ �Ѵ�. 
				if ( pItemData->GetType () == CItemData::ITEM_POTION )
				{
					if ( pItemData->GetGrade() != pTextItemData->GetGrade() ) continue;
				}
				else
				{
					if ( pItemData->GetLevel() != pTextItemData->GetLevel() ) continue;
				}
				
				{
					nCnt = pTextItemData->GetMakePotionCount();

					CItems* pItems = new CItems(pItemData->GetItemIndex());
					m_vecItems.push_back(pItems);

					pClone = (CUIListItem*)pTemp->Clone();
					m_pListItems->AddListItem(pClone);

					pIcon = (CUIIcon*)pClone->findUI("icon");

					if (pIcon != NULL)
					{
						pIcon->setData(pItems);
					}

					pText = (CUIText*)pClone->findUI("text");

					if (pText != NULL)
					{
						pText->SetText(CTString(pItemData->GetName()));

						if (nCnt > 0)
						{
							pText->SetPosY(0);							
						}
					}

					if (nCnt > 0)
					{
						pText = (CUIText*)pClone->findUI("txt_count");

						if (pText != NULL)
						{
							strCount = UIMGR()->IntegerToCommaString(nCnt);
							tmpStr.PrintF(_S(766, "%s ��"), strCount);
							pText->SetText(tmpStr);
						}
					}
				}
			}
		}
	}

	m_pListItems->UpdateScroll(m_vecItems.size());
	m_pListItems->UpdateList();
		 
	AddString ( _S( 572, "�������� ������ �ֽʽÿ�." ), COLOR_TEXT );

	if (m_pBtnOK != NULL)
		m_pBtnOK->SetEnable(m_bSatisfied);
}

// ----------------------------------------------------------------------------
// Name : OpenProduct()
// Desc :
// ----------------------------------------------------------------------------
void CUIProduct::OpenProduct( int nItemIndex, SWORD nTab, SWORD inven_idx )
{
	// If this is already exist
	if( IsVisible() == TRUE )
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();

	// If inventory is already locked
	if( pUIManager->GetInventory()->IsLocked() == TRUE ||
		pUIManager->GetInventory()->IsLockedArrange() == TRUE)
	{
		pUIManager->GetInventory()->ShowLockErrorMessage();
		return;
	}

	Clear();

	if( pUIManager->GetInventory()->IsVisible() == FALSE )
		pUIManager->GetInventory()->ToggleVisible();

	m_nProductText	= nItemIndex;
	m_nTab			= nTab;
	m_nInvenIdx		= inven_idx;

	pUIManager->RearrangeOrder( UI_PRODUCT, TRUE );
	
	InitProduct ();

	pUIManager->GetInventory()->Lock( TRUE, TRUE, LOCK_PRODUCT );

	Hide(FALSE);
}

// ----------------------------------------------------------------------------
// Name : CloseProduct()
// Desc :
// ----------------------------------------------------------------------------
void CUIProduct::CloseProduct()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// Close refine
	pUIManager->RearrangeOrder( UI_PRODUCT, FALSE );

	// Unlock inventory
	pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_PRODUCT );

	Clear();

	Hide(TRUE);	
}

// ----------------------------------------------------------------------------
// Name : SelectItem()
// Desc :!!
// ----------------------------------------------------------------------------
void CUIProduct::SelectItem( int _nIndex )
{
 
	/*  cpp2angel
		����â�� ��� ���¿��� �������� ���� �Ͽ��� �� SelectItem()�� ȣ�� �Ͽ��� 
		���� ���õ� ���������� �ʿ� ������ �ٽ� ���� �ϱ� ���� ��ƾ
	*/
	if ( _nIndex == -1 )
	{
		_nIndex = m_nSelectProductItem;	// ���� ���õ� ���������� ����

		if (_nIndex == -1)   // �׷��� -1�̸� Return...
			return;	
		
	}
	
	BOOL bNeedItem[MAX_MAKE_ITEM_MATERIAL];
	BOOL bNeedItemCount[MAX_MAKE_ITEM_MATERIAL];
	
	BOOL bNeedSSkill		= FALSE;
	BOOL bNeedSSkillLevel	= FALSE;

	/*
	BOOL bNeedSSkill2		= FALSE;
	BOOL bNeedSSkillLevel2	= FALSE;
	*/

	// ������ �ʱ�ȭ
	memset ( bNeedItem, FALSE, sizeof (BOOL) * MAX_MAKE_ITEM_MATERIAL );
	memset ( bNeedItemCount, FALSE, sizeof (BOOL) * MAX_MAKE_ITEM_MATERIAL );

	m_nNeedItemCount = 0;
	m_pListDesc->DeleteAllListItem();
	
	// ���� �ϰ��� �ϴ� ������ ���
	if ( m_vecItems[_nIndex]->Item_Index == -1 ) return;
	CItemData* pProductItemData	= _pNetwork->GetItemData ( m_vecItems[_nIndex]->Item_Index );
	CItemData* pTextItemData	= _pNetwork->GetItemData ( m_nProductText );
	
	if ( pProductItemData->GetNeedSSkillIndex () == -1 ) return;
	CSpecialSkill* pNeedSSkill	= CSpecialSkill::getData( pProductItemData->GetNeedSSkillIndex () );

	if (pNeedSSkill == NULL)
		return;

	if ( pProductItemData->GetNeedSSkillCount () == -1 ) return;
	int			nSSkillLevel	= pProductItemData->GetNeedSSkillCount ();

	// �ʿ� ��ų ( Preference äũ �ٶ� )
	bNeedSSkill =  CUIManager::getSingleton()->GetCharacterInfo()->CheckSSkillByIndex ( pNeedSSkill->GetIndex(), nSSkillLevel, &bNeedSSkillLevel );

	// �ʿ� ������
	int nIndexTemp = 0;
	CItemData* pInvenItem = NULL;

	int		nIndex;
	// �ʿ� ������ ������ ���� ��� 
	for ( nIndex = 0; nIndex < MAX_MAKE_ITEM_MATERIAL; nIndex++ )
	{	
		nIndexTemp = pProductItemData->GetNeedItemIndex( nIndex );

		if( nIndexTemp == -1 )	continue;
		
		m_NeedItems[m_nNeedItemCount].ItemData	= _pNetwork->GetItemData ( nIndexTemp );
		m_NeedItems[m_nNeedItemCount].llCount	= pProductItemData->GetNeedItemCount( nIndex ); 
	
		m_nNeedItemCount++;	
	}

	if ( m_nNeedItemCount == 0 ) return;

	CUIInventory* pInven = CUIManager::getSingleton()->GetInventory();
	int		dbIndex = 0;

	// �κ����� �ʿ� ������ �˻� ( nUniIndex ó�� ������ )
	for ( nIndex = 0; nIndex < m_nNeedItemCount; nIndex++ )
	{
		SWORD		nTab, nIdx;
		dbIndex = m_NeedItems[nIndex].ItemData->GetItemIndex();
		if (ItemHelp::HaveItem( dbIndex, &nTab, &nIdx ) == true)
		{
			bNeedItem[nIndex] = TRUE;

			if (pInven->GetItemCount(dbIndex) >= m_NeedItems[nIndex].llCount)
			{
				bNeedItemCount[nIndex] = TRUE;

				m_NeedItems[nIndex].MatTab = nTab;
				m_NeedItems[nIndex].inven_idx = nIdx;
			}
		}
	}

// ���� ��� 
	CTString strTitle;
	m_bSatisfied = TRUE;
	
	// �ʿ佺ų -------------------------------------------------------------------------------	
	AddString ( _S( 562, "�ʿ� ��ų" ), COLOR_SUB_TITLE );		
	AddString ( CTString ( pNeedSSkill->GetName() ), bNeedSSkill? COLOR_TEXT : COLOR_NONE );

	strTitle.PrintF ( _S( 573, "%s : %d " ), _S( 574, "����" ), nSSkillLevel ); 
	AddString ( strTitle, bNeedSSkillLevel? COLOR_TEXT : COLOR_NONE );


	{
		m_bSatisfied &= bNeedSSkill;		
	}


	// �ʿ� ��ǰ -------------------------------------------------------------------------------
	AddString ( CTString ( "" ) );		
	AddString ( _S( 575, "�ʿ� ��ǰ" ), COLOR_SUB_TITLE );		
		
	for ( nIndex = 0; nIndex < m_nNeedItemCount; nIndex++ )
	{
		m_bSatisfied &= bNeedItem[nIndex] && bNeedItemCount[nIndex];
	}
	
	
	for ( nIndex = 0; nIndex < m_nNeedItemCount; nIndex++ )
	{
		const char* szItemName;
		if (m_NeedItems[nIndex].ItemData != NULL)
			szItemName = _pNetwork->GetItemName( m_NeedItems[nIndex].ItemData->GetItemIndex() );

		CTString strCount = UIMGR()->IntegerToCommaString(m_NeedItems[nIndex].llCount);
		strTitle.PrintF ( _S( 576, "%s : %s��" ), szItemName, strCount );
		AddString ( strTitle, bNeedItemCount[nIndex]? COLOR_TEXT : COLOR_NONE );
	}

	if (m_pBtnOK != NULL)
		m_pBtnOK->SetEnable(m_bSatisfied);
}

// ----------------------------------------------------------------------------
// Name : AddString()
// Desc : 
// ----------------------------------------------------------------------------
void CUIProduct::AddString ( CTString& strText, COLOR colText )
{
	int nLength = UTIL_HELP()->GetFontWidth(strText.str_String);

	// ���� �Ѿ�� ���
	if (nLength >= _iMaxMsgStringChar)
	{
		CUITextBox	_tb;
		_tb.SetSplitMode(SPLIT_SPACE);
		_tb.SetWidth(_iMaxMsgStringChar);
		_tb.SetText(strText);

		int max = _tb.GetLineCount();

		for (int i = 0; i < max; ++i)
		{
			pushback_string(_tb.GetLineString(i), colText);
		}
	}
	else 
	{
		pushback_string(strText, colText);
	}

	if (m_pListDesc != NULL)
	{
		m_pListDesc->UpdateScroll(m_pListDesc->getListItemCount());
		m_pListDesc->UpdateList();
	}
}


// ========================================================================= //
//                           Send message functions                          //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : SendProductReq()
// Desc :
// ----------------------------------------------------------------------------
void CUIProduct::SendProductReq()
{
	if( m_bWaitProductResult == TRUE )
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();

	if( ( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->IsSkilling() )
	{
		pUIManager->GetChattingUI()->AddSysMessage(  _S( 943, "��ų ����߿��� ������ �� �� �����ϴ�." ) , SYSMSG_ERROR );		
		return;
	}

	if( pUIManager->IsCSFlagOn( CSF_TELEPORT ) )
	{
		pUIManager->GetChattingUI()->AddSysMessage(  _S( 944, "���� �̵��߿��� ������ �� �� �����ϴ�." ) , SYSMSG_ERROR );	
		return;
	}

	_pNetwork->ProductReq ( m_nTab, m_nInvenIdx, m_vecItems[m_nSelectProductItem]->Item_Index, m_nNeedItemCount, 
						m_NeedItems );

	m_bWaitProductResult = TRUE;
	
	if (m_pBtnOK != NULL)
		m_pBtnOK->SetEnable(!m_bWaitProductResult);
	//SetEnable ( !m_bWaitProductResult );
}

// ========================================================================= //
//                         Receive message functions                         // 
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : ProductRep()
// Desc :
// ----------------------------------------------------------------------------
void CUIProduct::ProductRep( SBYTE sbResult )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// Close message box
	pUIManager->CloseMessageBox( MSGCMD_PRODUCT_REP );

	// Show result
	CTString	strMessage;

	switch( sbResult )
	{
	case MSG_MAKE_FAIL_SYSTEM:				// �־�� �ȵǴ� ������ ����
		strMessage = _S( 577, "���ۿ� ���� �Ͽ����ϴ�." );										
		break;
	case MSG_MAKE_FAIL_MATERIAL:			// ��� ���� ����
		strMessage = _S( 577, "���ۿ� ���� �Ͽ����ϴ�." );										
		break;
	case MSG_MAKE_FAIL_NO_SSKILL:			// ���� ��ų ����
		strMessage = _S( 578, "���ۿ� �ʿ��� ���� ��ų�� �����ϴ�." );							
		break;
	case MSG_MAKE_FAIL_ITEM_LEVEL :			// ���� ��ų�� ���� �Ұ��� ������ (����)
		strMessage = _S( 579, "���ۿ� �ʿ��� ��ų ������ �����մϴ�." );						
		break;
	case MSG_MAKE_FAIL_PROB:				// Ȯ�� ����
		strMessage = _S( 577, "���ۿ� ���� �Ͽ����ϴ�." );										
		break;
	case MSG_MAKE_SUCCESS:
		strMessage = _S( 580, "���ۿ� ���� �Ͽ����ϴ�." );										
		break;
	}

	CloseProduct ();

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 571, "����" ), UMBS_OK, UI_PRODUCT, MSGCMD_PRODUCT_REP );		
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
	
	m_bWaitProductResult = FALSE;	
}

void CUIProduct::initialize()
{
	m_bSatisfied = FALSE;

	CUIText* pTxt = (CUIText*)findUI("txt_title");

	if (pTxt != NULL)
	{
		int l, t, r, b;
		l = pTxt->GetPosX();
		t = pTxt->GetPosY();
		r = l + pTxt->GetWidth();
		b = t + pTxt->GetHeight();
		setTitleRect(l, t, r, b);
	}

	m_pTxtSubject = (CUIText*)findUI("txt_subject");

	m_pListItems = (CUIList*)findUI("list_process");

	m_pListDesc = (CUIList*)findUI("list_desc");

	if (m_pListDesc != NULL)
	{
		CUIListItem* pItem = m_pListDesc->GetListItemTemplate();

		if (pItem != NULL)
			_iMaxMsgStringChar = pItem->GetWidth();
	}

	m_pBtnOK = (CUIButton*)findUI("btn_ok");	

	CUIButton* pBtn;

#ifndef		WORLD_EDITOR

	if (m_pListItems != NULL)
		m_pListItems->SetCommandFSelect(boost::bind(&CUIProduct::callback_select, this));

	if (m_pBtnOK != NULL)
		m_pBtnOK->SetCommandFUp(boost::bind(&CUIProduct::SendProductReq, this));

	pBtn = (CUIButton*)findUI("btn_close");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&CUIProduct::CloseProduct, this));

	pBtn = (CUIButton*)findUI("btn_cancel");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&CUIProduct::CloseProduct, this));

#endif		// WORLD_EDITOR
}

WMSG_RESULT CUIProduct::OnLButtonDown( UINT16 x, UINT16 y )
{
	CUIManager::getSingleton()->RearrangeOrder(UI_PRODUCT, TRUE);

	return CUIWindow::OnLButtonDown(x, y);
}

//-------------------------------------------------------------------
// PRIVATE

void CUIProduct::clearItems()
{
	int		i, nMax = m_vecItems.size();

	for (i = 0; i < nMax; ++i)
	{
		SAFE_DELETE(m_vecItems[i]);
	}

	m_vecItems.clear ();
}

void CUIProduct::pushback_string( CTString& strText, COLOR color )
{
	if (m_pListDesc == NULL)
		return;

	CUIListItem* pclone = NULL;
	CUIListItem* pTemp = m_pListDesc->GetListItemTemplate();
	CUIText* pText = NULL;

	if (pTemp == NULL)
		return;

	pclone = (CUIListItem*)pTemp->Clone();
	m_pListDesc->AddListItem(pclone);

	pText = (CUIText*)pclone->findUI("txt_line");

	if (pText != NULL)
	{
		pText->SetText(strText);
		pText->setFontColor(color);		
	}
}

void CUIProduct::callback_select()
{
	if (m_pListItems == NULL)
		return;

	int sel = m_pListItems->getCurSel();

	m_nSelectProductItem = sel;
	SelectItem(sel);
}
