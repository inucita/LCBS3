#include "stdh.h"

// ��� ����. [12/2/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UIProcess.h>
#include <Engine/Interface/UISkillLearn.h>
#include <Engine/Help/ItemHelp.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UIPlayerInfo.h>
#include <Engine/Interface/UIProduct.h>
#include <Engine/Contents/Base/UICharacterInfoNew.h>

// Date : 2005-03-07,   By Lee Ki-hwan
static int	_iMaxMsgStringChar = 0;

// ----------------------------------------------------------------------------
// Name : CUIProcess()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIProcess::CUIProcess()
	: m_pTxtSubject(NULL)
	, m_pListItems(NULL)
	, m_pListDesc(NULL)
	, m_pBtnOK(NULL)

{
	setInherit(false);
	Clear();
}

// ----------------------------------------------------------------------------
// Name : ~CUIProcess()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIProcess::~CUIProcess()
{
	clearItems();
	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Clear()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcess::Clear()
{
	m_nSelectProcessItem	= -1;
	m_bWaitProcessResult	= TRUE;
	m_bSatisfied			= FALSE;

	m_nProcessText			= -1;				
	m_nTab					= -1;
	m_nInvenIdx				= -1;

	memset ( m_NeedItems, 0, sizeof ( CNeedItems ) * MAX_MAKE_ITEM_MATERIAL );

	m_bWaitProcessResult = FALSE;	

	if (m_pListItems != NULL)
		m_pListItems->DeleteAllListItem();
	
	if (m_pListDesc != NULL)
		m_pListDesc->DeleteAllListItem();

	clearItems();
}

// ----------------------------------------------------------------------------
// Name : InitProcess()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcess::InitProcess()
{	
	clearItems();

	CItemData* pTextItemData = _pNetwork->GetItemData ( m_nProcessText );
	const char* szItemName	= _pNetwork->GetItemName( m_nProcessText );
	
	// �ӽ� ó�� 
	int nProcessType = pTextItemData->GetProcessType();
	int nProcessSubType = pTextItemData->GetProcessSubType();
	
	if (m_pTxtSubject != NULL)
	{
		m_pTxtSubject->SetText(CTString((char*)szItemName));
		m_pTxtSubject->setFontColor(DEF_UI_FONT_TITLE);
	}

	m_pListItems->DeleteAllListItem();
	CUIListItem* pTemp = m_pListItems->GetListItemTemplate();
	CUIListItem* pClone = NULL;
	CUIIcon* pIcon = NULL;
	CUIText* pText = NULL;
	
	int bit = 0x00000001;	
	CItemData::_map::iterator	iter = CItemData::_mapdata.begin();
	CItemData::_map::iterator	eiter = CItemData::_mapdata.end();

	for (;iter != eiter; ++iter)
	{
		CItemData* pItemData = (*iter).second;

		if (pItemData == NULL)
			continue;

		if ( pItemData->GetFlag() & ITEM_FLAG_MADE )
		{
			if ( pItemData->GetType () == CItemData::ITEM_ETC && 
				pItemData->GetSubType () == CItemData::ITEM_ETC_PROCESS && // ����ǰ �̶�� ���
				pItemData->GetProcessType () == nProcessType 
				&& pItemData->GetProcessSubType () == nProcessSubType 
				)			
			{
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
					pText->SetText(CTString(pItemData->GetName()));
			}
		}
	}

	m_pListItems->UpdateScroll(m_vecItems.size());
	m_pListItems->UpdateList();

	AddString(_S(561, "����ǰ�� ������ �ֽʽÿ�."), COLOR_TEXT);

	if (m_pBtnOK != NULL)
		m_pBtnOK->SetEnable(m_bSatisfied);
}

// ----------------------------------------------------------------------------
// Name : OpenProcess()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcess::OpenProcess( int nItemIndex, int nTab, int inven_idx )
{
	// If this is already exist
	if( IsVisible() == TRUE )
		return;

	// If inventory is already locked

	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->GetInventory()->IsLocked() )
	{
		pUIManager->GetInventory()->ShowLockErrorMessage();
		return;
	}

	Clear ();

	if( !pUIManager->GetInventory()->IsVisible() )
		pUIManager->GetInventory()->ToggleVisible();

	m_nProcessText	= nItemIndex;
	m_nTab			= nTab;
	m_nInvenIdx		= inven_idx;	

	Hide(FALSE);
	pUIManager->RearrangeOrder( UI_PROCESS, TRUE );
	
	InitProcess ();

	pUIManager->GetInventory()->Lock( TRUE, FALSE, LOCK_PROCESS );
}

// ----------------------------------------------------------------------------
// Name : CloseProcess()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcess::CloseProcess()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// Close refine
	Hide(TRUE);
	pUIManager->RearrangeOrder( UI_PROCESS, FALSE );

	// Unlock inventory
	pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_PROCESS );

	Clear();
}

// ----------------------------------------------------------------------------
// Name : SelectItem()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcess::SelectItem ( int _nIndex )
{
	if (_nIndex < 0)
	{
		_nIndex = m_nSelectProcessItem;
		
		if (_nIndex == -1) 
			return;
	}

	if (m_vecItems.size() <= _nIndex)
		return;

	if (m_pListDesc == NULL)
		return;

	CUIManager* pUIMgr = CUIManager::getSingleton();

	BOOL bNeedItem			= FALSE;	
	BOOL bNeedItemCount		= FALSE;
	BOOL bNeedSSkill		= FALSE;
	BOOL bNeedSSkillLevel	= FALSE;

	m_pListDesc->DeleteAllListItem();

	// ���� �ϰ��� �ϴ� ������ ���
	if (m_vecItems[_nIndex]->Item_Index == -1)
		return;

	CItemData*		pProcessItemData	= _pNetwork-> GetItemData(m_vecItems[_nIndex]->Item_Index);

	if (pProcessItemData->GetNeedSSkillIndex() == -1)
		return;

	CItemData*		pTextItemData	= _pNetwork->GetItemData(m_nProcessText);

	CSpecialSkill*	pNeedSSkill		= CSpecialSkill::getData(pProcessItemData->GetNeedSSkillIndex());
	int				nSSkillLevel	= pProcessItemData->GetNeedSSkillCount();

	if (pNeedSSkill == NULL)
		return;

	// ���� �˻�
	// �ʿ� ��ų
	if (pNeedSSkill->GetIndex() == -1)
		return;

	if (nSSkillLevel == -1)
		return;

	bNeedSSkill = pUIMgr->GetCharacterInfo()->CheckSSkillByIndex(pNeedSSkill->GetIndex(), nSSkillLevel, &bNeedSSkillLevel);

	// �ʿ� ������
	int nIndexTemp = 0;

	// �ʿ� ������ ������ ���� ���
	nIndexTemp = pProcessItemData->GetNeedItemIndex(0);

	if (nIndexTemp == -1)
		return;

	m_NeedItems[0].ItemData	= _pNetwork->GetItemData(nIndexTemp);
	m_NeedItems[0].llCount	= pProcessItemData->GetNeedItemCount(0);


	// �κ����� �ʿ� ������ �˻� ( nUniIndex ó�� ������ )
	SWORD		nTab, nIndex;

	if (ItemHelp::HaveItem(m_NeedItems[0].ItemData->GetItemIndex(), &nTab, &nIndex, m_NeedItems[0].llCount))
	{
		bNeedItem = TRUE;
		bNeedItemCount = TRUE;

		m_NeedItems[0].MatTab = nTab;
		m_NeedItems[0].inven_idx = nIndex;
	}

	// ���� ���
	CTString strTitle;
	CTString strSpace = "";

	// �ʿ� ��ų ���
	AddString(_S(562, "�ʿ� ��ų"), COLOR_SUB_TITLE);
	AddString(strSpace + pNeedSSkill->GetName(), bNeedSSkill ? COLOR_TEXT : COLOR_NONE);
	m_bSatisfied = bNeedSSkill;

	AddString(CTString(" "));

	// �ʿ� ��ǰ
	switch (pTextItemData->GetProcessType())
	{
	case CItemData::PROCESS_DOC_STONE :
		strTitle = _S(563, "�ʿ� ����");
		break;

	case CItemData::PROCESS_DOC_PLANT :
		strTitle = _S(564, "�ʿ� ũ��");
		break;

	case CItemData::PROCESS_DOC_ELEMENT :
		strTitle = _S(565, "�ʿ� ����");
		break;
	}

	AddString(strTitle, COLOR_SUB_TITLE);

	bNeedItem &= bNeedItemCount;
	const char* szItemName = NULL;

	if (m_NeedItems[0].ItemData != NULL)
	{
		szItemName = _pNetwork->GetItemName(m_NeedItems[0].ItemData->GetItemIndex());
		CTString strCount = UIMGR()->IntegerToCommaString(m_NeedItems[0].llCount);
		strTitle.PrintF(_S(576, "%s : %s�� "), strSpace + szItemName, strCount);
	}

	AddString(strTitle, bNeedItem ? COLOR_TEXT : COLOR_NONE);
	m_bSatisfied &= bNeedItem;

	if (m_pBtnOK != NULL)
		m_pBtnOK->SetEnable(m_bSatisfied);
}


// ----------------------------------------------------------------------------
// Name : AddString()
// Desc : 
// ----------------------------------------------------------------------------
void CUIProcess::AddString ( CTString& strText, COLOR colText )
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
		m_pListDesc->UpdateList();
}

// ========================================================================= //
//                           Send message functions                          //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : SendProcessReq()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcess::SendProcessReq()
{
	if( m_bWaitProcessResult == TRUE )
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();

	if( ( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->IsSkilling() )
	{
		pUIManager->GetChattingUI()->AddSysMessage(  _S( 941, "��ų ����߿��� ������ �� �� �����ϴ�." ) , SYSMSG_ERROR );		
		return;
	}

	if( pUIManager->IsCSFlagOn( CSF_TELEPORT ) )
	{
		pUIManager->GetChattingUI()->AddSysMessage(  _S( 942, "���� �̵��߿��� ������ �� �� �����ϴ�." ) , SYSMSG_ERROR );	
		return;
	}

	_pNetwork->ProcessReq ( m_nTab, m_nInvenIdx, m_vecItems[m_nSelectProcessItem]->Item_Index, 1, 
						m_NeedItems );

	m_bWaitProcessResult = TRUE;
	
	if (m_pBtnOK != NULL)
		m_pBtnOK->SetEnable(!m_bWaitProcessResult);
}

// ========================================================================= //
//                         Receive message functions                         // 
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : ProcessRep()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcess::ProcessRep( SBYTE sbResult )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// Close message box
	pUIManager->CloseMessageBox( MSGCMD_PROCESS_REP );

	// Show result
	CTString	strMessage;
	
	switch( sbResult )
	{

	case MSG_PROCESS_FAIL_SYSTEM :			// �־�� �ȵǴ� ������ ����
		strMessage = _S( 566, "������ �����Ͽ����ϴ�." );						
		break;

	case MSG_PROCESS_FAIL_MATERIAL :		// ��� ���� ����
		strMessage = _S( 567, "������ �ʿ��� ��ᰡ �����մϴ�." );			
		break;

	case MSG_PROCESS_FAIL_NO_SSKILL :		// ���� ��ų ����
		strMessage = _S( 568, "������ �ʿ��� ���� ��ų�� �����ϴ�." );				
		break;
	//case MSG_PROCESS_FAIL_SSKILL_LEVEL :	// ���� ��ų ���� ����
	//	strMessage = _S( 569, "������ �ʿ��� ��ų ������ �����մϴ�." );			
	//	break;
	case MSG_PROCESS_FAIL_PROB :			// Ȯ�� ����
		strMessage = _S( 566, "������ �����Ͽ����ϴ�." );							
		break;
	case MSG_PROCESS_SUCCESS :				// ���� ����
		strMessage = _S( 570, "������ �����Ͽ����ϴ�." );							
		break;
	
	}

	CloseProcess();

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 560, "����" ), UMBS_OK, UI_PROCESS, MSGCMD_PROCESS_REP );		
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );

	m_bWaitProcessResult = FALSE;	
}

void CUIProcess::clearItems()
{
	for (int i = 0; i < m_vecItems.size(); ++i)
		SAFE_DELETE(m_vecItems[i]);

	m_vecItems.clear();
}

void CUIProcess::initialize()
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
		m_pListItems->SetCommandFSelect(boost::bind(&CUIProcess::callback_select, this));

	if (m_pBtnOK != NULL)
		m_pBtnOK->SetCommandFUp(boost::bind(&CUIProcess::SendProcessReq, this));

	pBtn = (CUIButton*)findUI("btn_close");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&CUIProcess::CloseProcess, this));

	pBtn = (CUIButton*)findUI("btn_cancel");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&CUIProcess::CloseProcess, this));

#endif		// WORLD_EDITOR
}

WMSG_RESULT CUIProcess::OnLButtonDown( UINT16 x, UINT16 y )
{
	CUIManager::getSingleton()->RearrangeOrder( UI_PROCESS, TRUE );

	return CUIWindow::OnLButtonDown(x, y);
}

void CUIProcess::pushback_string( CTString& strText, COLOR color )
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

void CUIProcess::callback_select()
{
	if (m_pListItems == NULL)
		return;

	int sel = m_pListItems->getCurSel();

	m_nSelectProcessItem = sel;
	SelectItem(sel);
}

