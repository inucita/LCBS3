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

//[ttos_2009_6_5]:������ ���� ���� �߰�
#define MAX_SEALTYPE	5
#define MAX_SEALSUBTYPE	23
#define SEALTYPE_START_INDEX 656	 

enum eSealType
{
	SEAL_1 = 0,		//������ ����
	SEAL_2,			//ȭ���� ����
	SEAL_3,			//��ī�ο� ����
	SEAL_4,			//�ܴ��� ����
	SEAL_5,			//�Ҹ�ǰ ����
	SEAL_MEMBER,	//ȸ�� ���
};

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
{
	Clear ();
}

// ----------------------------------------------------------------------------
// Name : ~CUIProduct()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIProduct::~CUIProduct()
{
	Destroy();
	Clear();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIProduct::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	int diff = SKILLLEARN_HEIGHT - PRODUCT_HEIGHT;

	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	_iMaxMsgStringChar = PRODUCT_STRING_CHAR_WIDTH / ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 216, 22 );
	m_rcIcons.SetRect( 17, 55, 51, 200 );
	m_rcTab.SetRect( 21, 31, 204, 50 );
	m_rcItem.SetRect ( 13, 33, 218, 203 );
	m_rcDesc.SetRect ( 13, 203, 218, 331 );

	// Product Texture ���� - SkillLearn Texture ���
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\SkillLearn.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	m_rtBackgroundTop.SetUV( 0, 0, 216, PRODUCT_TOP_HEIGHT, fTexWidth, fTexHeight );
	m_rtBackgroundBtm.SetUV( 0, PRODUCT_TOP_HEIGHT + diff, 216, SKILLLEARN_HEIGHT, fTexWidth, fTexHeight );

	m_rtSelOutline.SetUV( 218, 80, 250, 112, fTexWidth, fTexHeight );

	// Close button
	m_btnClose.Create( this, CTString( "" ), 184, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 219, 0, 233, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 219, 15, 233, 29, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Product button
	m_btnOK.Create( this, _S( 191, "Ȯ��" ), 70, 372-diff, 63, 21 );			
	m_btnOK.SetUV( UBS_IDLE, 0, 403, 63, 424, fTexWidth, fTexHeight );
	m_btnOK.SetUV( UBS_CLICK, 66, 403, 129, 424, fTexWidth, fTexHeight );
	m_btnOK.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOK.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Cancel button
	m_btnCancel.Create( this, _S( 139, "���" ), 141, 372-diff, 63, 21 );
	m_btnCancel.SetUV( UBS_IDLE, 0, 403, 63, 424, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 66, 403, 129, 424, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Scroll bar of Product Item
	m_sbProductItem.Create( this, 193, 51, 9, 153 );
	m_sbProductItem.CreateButtons( TRUE, 9, 7, 0, 0, 10 );
	m_sbProductItem.SetScrollPos( 0 );
	m_sbProductItem.SetScrollRange( PRODUCT_SLOT_ROW_TOTAL );
	m_sbProductItem.SetCurItemCount( 0 );
	m_sbProductItem.SetItemsPerPage( PRODUCT_SLOT_ROW );	
	// Up button
	m_sbProductItem.SetUpUV( UBS_IDLE, 219, 62, 228, 69, fTexWidth, fTexHeight );
	m_sbProductItem.SetUpUV( UBS_CLICK, 230, 62, 239, 69, fTexWidth, fTexHeight );
	m_sbProductItem.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbProductItem.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_sbProductItem.SetBarTopUV( 219, 32, 228, 42, fTexWidth, fTexHeight );
	m_sbProductItem.SetBarMiddleUV( 219, 43, 228, 45, fTexWidth, fTexHeight );
	m_sbProductItem.SetBarBottomUV( 219, 46, 228, 56, fTexWidth, fTexHeight );
	// Down button
	m_sbProductItem.SetDownUV( UBS_IDLE, 219, 71, 228, 78, fTexWidth, fTexHeight );
	m_sbProductItem.SetDownUV( UBS_CLICK, 230, 71, 239, 78, fTexWidth, fTexHeight );
	m_sbProductItem.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbProductItem.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	// Wheel region
	m_sbProductItem.SetWheelRect( -181, -1, 180, 154 );


	// List box of Precondition description
	m_lbPreconditionDesc.Create( this, 13, 207, 180, 124, _pUIFontTexMgr->GetLineHeight(), 7, 6, 1, FALSE );
	m_lbPreconditionDesc.CreateScroll( TRUE, 0, 0, 9, 124, 9, 7, 0, 0, 10 );
	
	// Up button
	m_lbPreconditionDesc.SetScrollUpUV( UBS_IDLE, 219, 62, 228, 69, fTexWidth, fTexHeight );
	m_lbPreconditionDesc.SetScrollUpUV( UBS_CLICK, 230, 62, 239, 69, fTexWidth, fTexHeight );
	m_lbPreconditionDesc.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbPreconditionDesc.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbPreconditionDesc.SetScrollBarTopUV( 219, 32, 228, 42, fTexWidth, fTexHeight );
	m_lbPreconditionDesc.SetScrollBarMiddleUV( 219, 43, 228, 45, fTexWidth, fTexHeight );
	m_lbPreconditionDesc.SetScrollBarBottomUV( 219, 46, 228, 56, fTexWidth, fTexHeight );
	// Down button
	m_lbPreconditionDesc.SetScrollDownUV( UBS_IDLE, 219, 71, 228, 78, fTexWidth, fTexHeight );
	m_lbPreconditionDesc.SetScrollDownUV( UBS_CLICK, 230, 71, 239, 78, fTexWidth, fTexHeight );
	m_lbPreconditionDesc.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbPreconditionDesc.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );

	m_bSatisfied = FALSE;
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIProduct::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIProduct::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
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
	m_nMakeItemCount		= 0;

	memset ( m_NeedItems, 0, sizeof ( CNeedItems ) * MAX_MAKE_ITEM_MATERIAL );

	m_StrProductType.Clear ();

	m_bWaitProductResult = FALSE;
	m_lbPreconditionDesc.ResetAllStrings();

	int		i, nMax = m_vecIcons.size();

	for (i = 0; i < nMax; ++i)
	{
		SAFE_DELETE(m_vecIcons[i]);
	}

	m_vecIcons.clear ();
}

// ----------------------------------------------------------------------------
// Name : InitProduct()
// Desc :
// ----------------------------------------------------------------------------
void CUIProduct::InitProduct()
{	
	CItemData* pTextItemData = _pNetwork->GetItemData ( m_nProductText );
	const char* szItemName = _pNetwork->GetItemName( m_nProductText );
	
	// �ӽ� ó�� 
	int nProductType = pTextItemData->GetProcessType();
	int nProductSubType = pTextItemData->GetProcessSubType();

	m_StrProductType = szItemName;
	
	m_vecIcons.clear ();

	int SubTypeBit;

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
					CUIIcon* pIcon = new CUIIcon;

					pIcon->Create(this, 0, 0, BTN_SIZE, BTN_SIZE, UI_PRODUCT, UBET_ITEM);
					pIcon->setData(UBET_ITEM, pItemData->GetItemIndex());				
					m_vecIcons.push_back ( pIcon );
				}
			}
		}
	}		

	m_nMakeItemCount = pTextItemData->GetMakePotionCount();
		 
	AddString ( _S( 572, "�������� ������ �ֽʽÿ�." ), COLOR_TEXT );		

	m_sbProductItem.SetScrollPos( 0 );
	m_sbProductItem.SetCurItemCount( m_vecIcons.size()  );

	m_btnOK.SetEnable ( m_bSatisfied );
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
	
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIProduct::Render()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set skill learn texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Add render regions -----------------------------------------------------------------------------------------------
	// Background up
	pDrawPort->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, m_nPosY + PRODUCT_TOP_HEIGHT,
										m_rtBackgroundTop.U0, m_rtBackgroundTop.V0, m_rtBackgroundTop.U1, m_rtBackgroundTop.V1,
										0xFFFFFFFF );

	// Background down 
	pDrawPort->AddTexture( m_nPosX, m_nPosY+PRODUCT_TOP_HEIGHT, m_nPosX + m_nWidth, m_nPosY + m_nHeight,
										m_rtBackgroundBtm.U0, m_rtBackgroundBtm.V0, m_rtBackgroundBtm.U1, m_rtBackgroundBtm.V1,
										0xFFFFFFFF );

	// Render Title Text
	pDrawPort->PutTextEx( _S( 571, "����" ), m_nPosX + PRODUCT_TITLE_TEXT_OFFSETX,				
										m_nPosY + PRODUCT_TITLE_TEXT_OFFSETY, COLOR_TEXT );

	// Render Product Type
	pDrawPort->PutTextExCX( m_StrProductType, m_nPosX + 106, m_nPosY + 35, COLOR_TITLE );
	

	// Product Item Scroll bar
	m_sbProductItem.Render();

	// List box of Need Item Desc
	m_lbPreconditionDesc.Render();

	// Close button
	m_btnClose.Render();
	
	// Product button
	m_btnOK.Render();

	// Cancel button
	m_btnCancel.Render();

	pDrawPort->FlushRenderingQueue();
	
	// Render Product Item
	CTString m_strShortDesc;
	
	
	int	nX = SLEARN_SLOT_SX;
	int nY = SLEARN_SLOT_SY;

	int	iEnd = m_sbProductItem.GetScrollPos() + PRODUCT_SLOT_ROW;
	if( iEnd > m_vecIcons.size() )
		iEnd = m_vecIcons.size();
	
	
	CTString strMakePotionCount;

	strMakePotionCount.PrintF ( _S( 766, "%d ��" ), m_nMakeItemCount );	
	
	int OffSetY = 8;

	if ( m_nMakeItemCount > 0 )
	{
		OffSetY -= 5;
	}

	for( int i = m_sbProductItem.GetScrollPos(); i < iEnd; i++ )
	{
		if( !m_vecIcons[i]->IsEmpty() )
		{
			// render Item
			m_vecIcons[i]->SetPos ( nX, nY );
			m_vecIcons[i]->Render(pDrawPort);
			
			// render Item desc 
			CItemData* pItemData = _pNetwork->GetItemData ( m_vecIcons[i]->getIndex() );
			const char* szItemName = _pNetwork->GetItemName( m_vecIcons[i]->getIndex() );
			
			pDrawPort->PutTextExCX( szItemName, m_nPosX + 122, m_nPosY + nY + OffSetY, 0xC3C3C3ff );
			
			if ( m_nMakeItemCount > 0 )
				pDrawPort->PutTextExCX( strMakePotionCount, m_nPosX + 172, m_nPosY + nY + OffSetY + 18, 0xC3C3C3ff );
			
			nY += SLEARN_SLOT_OFFSETY;
		}
	}

	// Flush all render text queue
	pDrawPort->EndTextEx();

	nX = SLEARN_SLOT_SX;
	nY = SLEARN_SLOT_SY;

	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// render sel outline
	if ( m_nSelectProductItem != -1 )
	{
		int BoxX, BoxY;
		
		int nOffset = m_nSelectProductItem - m_sbProductItem.GetScrollPos();

		if ( nOffset >= 0 && nOffset < PRODUCT_SLOT_ROW )
		{
			BoxX = m_nPosX + nX - 1 ; 
			BoxY = m_nPosY + nY + ( SLEARN_SLOT_OFFSETY *  nOffset ) - 1 ;


			pDrawPort->AddTexture( BoxX, BoxY,	BoxX + 34, BoxY + 34,
											m_rtSelOutline.U0, m_rtSelOutline.V0, m_rtSelOutline.U1, m_rtSelOutline.V1,
											0xffffffff );
		}
	}
	
	pDrawPort->FlushRenderingQueue();
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIProduct::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL bTitleBarClick = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if (IsInside(nX, nY) == FALSE)
				return WMSG_FAIL;
				
			CUIManager::getSingleton()->SetMouseCursorInsideUIs();

			int	iRowS = m_sbProductItem.GetScrollPos();
			int	iRowE = iRowS + PRODUCT_SLOT_ROW;

			if ( m_vecIcons.size() < iRowE )
			{
				iRowE = m_vecIcons.size();
			}
				
			if ( IsInsideRect ( nX, nY, m_rcItem ) )
			{
				for( int iRow = iRowS; iRow < iRowE; iRow++ )
				{
					m_vecIcons[iRow]->MouseMessage( pMsg );
				}
			}

			// Move refine
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
			// Close button
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// OK button
			else if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Cancel button
			else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
				// List box of skill desc
			else if( m_sbProductItem.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_lbPreconditionDesc.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				nOldX = nX;		nOldY = nY;

				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Title bar
				else if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					bTitleBarClick = TRUE;
				}
				// OK button
				else if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Cancel button
				else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( m_sbProductItem.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					return WMSG_SUCCESS;
				}
				else if ( m_lbPreconditionDesc.MouseMessage ( pMsg ) != WMSG_FAIL )
				{
					return WMSG_SUCCESS;	
				}

				CUIManager::getSingleton()->RearrangeOrder( UI_PRODUCT, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			// If holding button doesn't exist
			if (UIMGR()->GetDragIcon() == NULL)
			{
				// Title bar
				bTitleBarClick = FALSE;

				// If refine isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CloseProduct();

					return WMSG_SUCCESS;
				}
				// OK button
				else if( ( wmsgResult = m_btnOK.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						SendProductReq();

					return WMSG_SUCCESS;
				}
				// Cancel button
				else if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CloseProduct();

					return WMSG_SUCCESS;
				}
				else if( m_sbProductItem.MouseMessage( pMsg ) != WMSG_FAIL )
				{
						return WMSG_SUCCESS;
				}
				else if ( m_lbPreconditionDesc.MouseMessage ( pMsg ) != WMSG_FAIL )
				{
					return WMSG_SUCCESS;	
				}
				else if( IsInsideRect( nX, nY, m_rcIcons ) )
				{
					int	iRowS = m_sbProductItem.GetScrollPos();
					int	iRowE = iRowS + PRODUCT_SLOT_ROW;

					if ( m_vecIcons.size() < iRowE )
					{
						iRowE = m_vecIcons.size();
					}
		
					for( int iRow = iRowS; iRow < iRowE; iRow++ )
					{
						if( m_vecIcons[iRow]->MouseMessage( pMsg ) != WMSG_FAIL )
						{
							m_nSelectProductItem = iRow;

							SelectItem ( m_nSelectProductItem );
												
							return WMSG_SUCCESS;
						}
					}
				}
			}
		
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
				return WMSG_SUCCESS;
		}
		break;
	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				if ( IsInsideRect ( nX, nY, m_rcItem ) )
				{ 
					return m_sbProductItem.MouseMessage( pMsg );
				}
				else if ( IsInsideRect ( nX, nY, m_rcDesc ) )
				{
					return m_lbPreconditionDesc.MouseMessage ( pMsg );
				}
		
				return WMSG_SUCCESS;
			}
		}
		break;
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : SelectItem()
// Desc :!!
// ----------------------------------------------------------------------------
void CUIProduct::SelectItem ( int _nIndex )
{
 
	/*  cpp2angel
		����â�� ��� ���¿��� �������� ���� �Ͽ��� �� SelectItem()�� ȣ�� �Ͽ��� 
		���� ���õ� ���������� �ʿ� ������ �ٽ� ���� �ϱ� ���� ��ƾ
	*/
	if ( _nIndex == -1 )
	{
		_nIndex = m_nSelectProductItem;	// ���� ���õ� ���������� ����

		if ( _nIndex == -1 ) return;	// �׷��� -1�̸� Return...
		
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
	m_lbPreconditionDesc.ResetAllStrings();
	
	// ���� �ϰ��� �ϴ� ������ ���
	if ( m_vecIcons[_nIndex]->getIndex() == -1 ) return;
	CItemData* pProductItemData	= _pNetwork->GetItemData ( m_vecIcons[_nIndex]->getIndex() );
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
		strTitle.PrintF ( _S( 576, "%s : %d��" ), szItemName, m_NeedItems[nIndex].llCount );
		AddString ( strTitle, bNeedItemCount[nIndex]? COLOR_TEXT : COLOR_NONE );
	}

	m_btnOK.SetEnable ( m_bSatisfied );
}

// ----------------------------------------------------------------------------
// Name : AddString()
// Desc : 
// ----------------------------------------------------------------------------
void CUIProduct::AddString ( CTString& strText, COLOR colText )
{
	int nLength =  strText.Length();

	if( nLength >= _iMaxMsgStringChar )
	{
		nLength -= nLength - _iMaxMsgStringChar;
		
		do
		{
			if ( strText[nLength] == ' ' )
			{
				break;
			}

		} while ( nLength-- );

		CTString strTemp2, strTemp;

		strText.Split( nLength, strTemp2, strTemp );
		m_lbPreconditionDesc.AddString ( 0, strTemp2, colText );

		strTemp2.PrintF ( "%s", strTemp );
		
		m_lbPreconditionDesc.AddString ( 0, strTemp2, colText );

	}
	else 
	{
		m_lbPreconditionDesc.AddString ( 0, strText, colText );
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

	_pNetwork->ProductReq ( m_nTab, m_nInvenIdx, m_vecIcons[m_nSelectProductItem]->getIndex(), m_nNeedItemCount, 
						m_NeedItems );

	m_bWaitProductResult = TRUE;
	
	m_btnOK.SetEnable ( !m_bWaitProductResult );
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


// ----------------------------------------------------------------------------
// Name : CUIProduct2()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIProduct2::CUIProduct2()
{
	m_pSubTypeString = new CTString[MAX_SEALSUBTYPE];
	m_pSubTypeString[0] = _S(2544, "����");
	m_pSubTypeString[1] = _S(4273, "����");
	m_pSubTypeString[2]	= _S(4275, "�尩");
	m_pSubTypeString[3] = _S(4274, "����");
	m_pSubTypeString[4] = _S(4276, "�Ź�");
	m_pSubTypeString[5] = _S(4277, "����");
	m_pSubTypeString[6] = _S(4613, "����");			//"����"
	m_pSubTypeString[7] = _S( 1039, "����");
	m_pSubTypeString[8] = _S( 1040, "������");
	m_pSubTypeString[9] = _S( 1042, "����");
	m_pSubTypeString[10] = _S( 1046, "�̵���");
	m_pSubTypeString[11] = _S( 1047, "�����ϵ�");
	m_pSubTypeString[12] = _S( 2307, "����");
	m_pSubTypeString[13] = _S(4614, "ȥ");				//"ȥ"
	m_pSubTypeString[14] = _S( 1038, "��絵");
	m_pSubTypeString[15] = _S( 1041, "���");
	m_pSubTypeString[16] = _S( 1043, "��������");
	m_pSubTypeString[17] = _S( 1044, "Ȱ");
	m_pSubTypeString[18] = _S( 1045, "�ܰ�");
	m_pSubTypeString[19] = _S( 2306, "���̵�");
	m_pSubTypeString[20] = _S( 4266, "��ȸ��");
	m_pSubTypeString[21] = _S( 4268, "����");
	m_pSubTypeString[22] = _S( 4267, "��Ÿ");

	m_pstrSealType = new CTString[MAX_SEALTYPE];
	m_pstrSealType[SEAL_1] = _S(4615, "������ ����");
	m_pstrSealType[SEAL_2] = _S(4616, "ȭ���� ����");
	m_pstrSealType[SEAL_3] = _S(4617, "��ī�ο� ����");
	m_pstrSealType[SEAL_4] = _S(4618, "�ܴ��� ����");
	m_pstrSealType[SEAL_5] = _S(4619, "�Ҹ�ǰ ����");

	pSelItemData = NULL;
	m_lbMakeList.Reset();
	m_ptdaddTexture = NULL;
}

// ----------------------------------------------------------------------------
// Name : ~CUIProduct2()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIProduct2::~CUIProduct2()
{
	m_vctMakeLearn.clear();
	m_lbMakeList.Reset();

	Destroy();

	STOCK_RELEASE(m_ptdaddTexture);

	SAFE_ARRAY_DELETE(m_pSubTypeString);
	SAFE_ARRAY_DELETE(m_pstrSealType);

	for (int i = 0; i < 5; ++i)
		SAFE_DELETE(m_pIconsStuff[i]);

	SAFE_DELETE(m_pIconsMake);
}

static int	_nMsgBoxLineHeight = 0;
// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIProduct2::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\NewItemProduct.tex" ) );
	m_ptdaddTexture = CreateTexture( CTString( "Data\\Interface\\CommonBtn.tex" ) );

	FLOAT	fTexWidth = m_ptdaddTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdaddTexture->GetPixHeight();
	_nMsgBoxLineHeight = _pUIFontTexMgr->GetFontHeight() + 4;

	m_bxBackground.SetBoxPos(WRect(0,0,457,303));
	m_bxBackground.SetBoxUV(m_ptdBaseTexture, 10, WRect(0,0,457,303));

	// Close button
	m_btnClose.Create( this, CTString( "" ), 434, 4, 16, 16 );
	m_btnClose.SetUV( UBS_IDLE, 211, 33, 226, 48, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 228, 33, 244, 48, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Make Item button
	m_btnMake.Create( this, _S(4620, "�����"), 348, 45, 94, 34 );
	m_btnMake.SetUV( UBS_IDLE, 259, 0, 352, 33, fTexWidth, fTexHeight );
	m_btnMake.SetUV( UBS_CLICK, 357, 0, 450, 33, fTexWidth, fTexHeight );
	m_btnMake.CopyUV( UBS_IDLE, UBS_ON );
	m_btnMake.CopyUV( UBS_IDLE, UBS_DISABLE );

	// List box
	m_lbMakeList.Create( this, 33, 72, 158, 210, _nMsgBoxLineHeight, 12, 8, 2, TRUE );
	m_lbMakeList.CreateScroll( TRUE, 0, 0, 11, 210, 10, 10, 0, 0, 10 );
	m_lbMakeList.SetSelBar( 160, _pUIFontTexMgr->GetLineHeight(), 145, 138, 239, 171, fTexWidth, fTexHeight );
	m_lbMakeList.SetOverColor( 0xF8E1B5FF );
	m_lbMakeList.SetSelectColor( 0xF8E1B5FF );
	m_lbMakeList.SetColumnPosX( 1, 10 );
	// Up button
	m_lbMakeList.SetScrollUpUV( UBS_IDLE, 156, 33, 165, 42, fTexWidth, fTexHeight );
	m_lbMakeList.SetScrollUpUV( UBS_CLICK, 168, 33, 177, 42, fTexWidth, fTexHeight );
	m_lbMakeList.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbMakeList.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_lbMakeList.SetScrollDownUV( UBS_IDLE, 156, 45, 165, 54, fTexWidth, fTexHeight );
	m_lbMakeList.SetScrollDownUV( UBS_CLICK, 168, 45, 177, 54, fTexWidth, fTexHeight );
	m_lbMakeList.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbMakeList.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button 185, 32, 194, 70,
	m_lbMakeList.SetScrollBarTopUV( 185, 32, 194, 40, fTexWidth, fTexHeight );
	m_lbMakeList.SetScrollBarMiddleUV( 185, 40, 194, 60, fTexWidth, fTexHeight );
	m_lbMakeList.SetScrollBarBottomUV( 185, 62, 194, 70, fTexWidth, fTexHeight );
	// pop button
	m_lbMakeList.SetPopBtnSpace(1, 0);
	m_lbMakeList.SetPopBtnSize(11, 11);
	m_lbMakeList.SetPopBtnUV( UCBS_NONE, 243, 59, 253, 69, fTexWidth, fTexHeight);
	m_lbMakeList.SetPopBtnUV( UCBS_CHECK, 229, 59, 239, 69, fTexWidth, fTexHeight);
// 	m_lbMakeList.SetPopBtnUV( UCBS_NONE_DISABLE, 230, 48, 230+14, 48+14, fTexWidth, fTexHeight);
//	m_lbMakeList.SetPopBtnUV( UCBS_CHECK_DISABLE, 230, 132, 230+14, 132+14, fTexWidth, fTexHeight);

	for (int i = 0; i < 5; i++)
	{
		m_pIconsStuff[i] = new CUIIcon;
		m_pIconsStuff[i]->Create(this, 224, 101+(37 * i), 30, 30, UI_PRODUCT2, UBET_ITEM);
	}

	m_pIconsMake = new CUIIcon;
	m_pIconsMake->Create(this, 224, 47, 30, 30, UI_PRODUCT2, UBET_ITEM);
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIProduct2::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIProduct2::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

void CUIProduct2::OpenProduct2(INDEX sealtype)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if (pUIManager->GetProductNPC()->IsVisible())
	{
		pUIManager->GetProductNPC()->CloseProductNPC();
	}
	 
	if (pUIManager->GetInventory()->IsLocked() == TRUE ||
		pUIManager->GetInventory()->IsLockedArrange() == TRUE)
	{
		pUIManager->GetInventory()->ShowLockErrorMessage();
		return;
	}

	m_lbMakeList.Reset();
	m_nSealType = sealtype;
	pSelItemData = NULL;
	m_pIconsMake->clearIconData();
	pUIManager->GetProduct2()->SetList(&m_lbMakeList, sealtype);

	// If this is already exist
	if( IsVisible() ) return;

	pUIManager->RearrangeOrder( UI_PRODUCT2, TRUE );
	pUIManager->GetInventory()->Lock(TRUE, TRUE, LOCK_PRODUCT_ITEM);

}

void CUIProduct2::CloseProduct2()
{
	m_lbMakeList.Reset();
	CUIManager* pUIManager = CUIManager::getSingleton();
	pUIManager->RearrangeOrder( UI_PRODUCT2, FALSE );
	pUIManager->GetInventory()->Lock(FALSE, FALSE, LOCK_PRODUCT_ITEM);
}

void CUIProduct2::ListReset()
{
	pSelItemData = NULL;
	m_vctMakeLearn.clear();
	//m_vctList.clear();
	m_pIconsMake->clearIconData();
}

void CUIProduct2::SetList(CUIListBoxEx* lbList, INDEX nSealtype)
{
	//SealType�� ��ų �ε����� �Ǿ� �����Ƿ� Skill Index 656 ���� �����̴�..
	int nSealtypenum = nSealtype-SEALTYPE_START_INDEX;
	//nSealSubCount�� List ������ ���� subtype ������ ������
	int nSealSubCount = SealTypeCount[nSealtypenum+1] - SealTypeCount[nSealtypenum];

	std::list<sResultItem> m_vctList;

	int		i;
	//Seal type�� ���� SubŸ�Կ� �°� ���� ��Ŵ
	for( i = 0; i < _pNetwork->wo_iNomOfMakeItemData; ++i )
	{
		CMakeItemData* temMakeItem =  &_pNetwork->wo_aMakeItemData[i];

		if (temMakeItem != NULL && temMakeItem->GetMakeItemType() == nSealtype)
		{

			if (lbList == &m_lbMakeList)
			{
				// ������ ��ų ����Ʈ ����
				if (IsLearn(temMakeItem->GetFactoryIndex()))
				{	
					sResultItem sTemdata;
					sTemdata.nitemIndex = temMakeItem->GetFactoryIndex();
					sTemdata.nSubType	= temMakeItem->GetMakeItemSubType();
					sTemdata.lExp		= temMakeItem->GetMakeExp();
					sTemdata.pItemData = temMakeItem;

					m_vctList.push_back(sTemdata);
				}
				
				
			}else
			{
				// ����� ���� ��ų ����Ʈ ����
				if (!IsLearn(temMakeItem->GetFactoryIndex()))
				{
					sResultItem sTemdata;
					sTemdata.nitemIndex = temMakeItem->GetFactoryIndex();
					sTemdata.nSubType	= temMakeItem->GetMakeItemSubType();
					sTemdata.lExp		= temMakeItem->GetMakeExp();
					sTemdata.pItemData = temMakeItem;

					m_vctList.push_back(sTemdata);
				}

			}
			
		}
	}

	m_vctList.sort();

	lbList->ResetAllStrings();
	lbList->Reset();
	
	int temStartpos = SealTypeCount[nSealtypenum];
	CTString strTemp;
	UQUAD nSealExp = MY_INFO()->GetSkillExp(nSealtype);

	std::list<sResultItem>::iterator endIter = m_vctList.end();
	std::list<sResultItem>::iterator startIter;
	// List�� SubType��� ������
	for( i = 0; i < nSealSubCount; i++)
	{
		strTemp = m_pSubTypeString[temStartpos+i];

		lbList->ChangeCurrentState(CUIListBoxEx::PS_EXTEND);
		lbList->AddString( 0, CTString( "" ), 0xFFFFFFFF, FALSE );
		lbList->AddString(1,strTemp,0xffb72dff);	
		startIter = m_vctList.begin();
		for(; startIter != endIter; ++startIter )
		{
			sResultItem tempdata = (*startIter);
			COLOR colTemp = 0xC0C0C0FF;
			if (SealSubType[temStartpos+i] == tempdata.nSubType)
			{
				lbList->ChangeCurrentState(CUIListBoxEx::PS_CHILD);
				lbList->AddString( 0, CTString( "" ), 0xF2F2F2FF );
				strTemp = tempdata.pItemData->GetName();
				SQUAD temResult = nSealExp - tempdata.pItemData->GetNeedExp();
				if (temResult < 10)
				{
					colTemp = 0xFF8000FF;
				}else if (temResult < 20)
				{
					colTemp = 0xFFFF00FF;
				}else if (temResult < 30)
				{
					colTemp = 0x008000FF;
				}
				lbList->AddString( 1, strTemp, colTemp, TRUE, tempdata.pItemData );
			}
			
		}

	}

	m_vctList.clear();

}

void CUIProduct2::Render()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Get position
	int	nX, nY;
	GetAbsPos( nX, nY );

	m_bxBackground.Render( nX, nY );

	pDrawPort->FlushRenderingQueue();

	pDrawPort->InitTextureData( m_ptdaddTexture );

	m_btnClose.Render();
	m_btnMake.Render();
	m_lbMakeList.Render();

	CTString strTemp;

	pDrawPort->PutTextEx( m_pstrSealType[m_nSealType-SEALTYPE_START_INDEX], m_nPosX + 20, m_nPosY + 42, 0xC3C3C3ff );

	UQUAD nSealExp = MY_INFO()->GetSkillExp(m_nSealType);

	strTemp.PrintF("%I64d", nSealExp);

	pDrawPort->PutTextExCX( strTemp , m_nPosX + 178, m_nPosY + 42, 0xC3C3C3ff );

	if (pSelItemData != NULL)
	{
		for (int i = 0; i < 5; i++)
		{
			if (m_pIconsStuff[i]->getIndex() != -1)
			{
				m_pIconsStuff[i]->Render(pDrawPort);
				CItemData* ptemItem = _pNetwork->GetItemData(m_pIconsStuff[i]->getIndex());
				strTemp.PrintF("%s", ptemItem->GetName());
				pDrawPort->PutTextEx( strTemp, m_nPosX + 264, m_nPosY + 102 + (37*i), 0xC3C3C3ff );
				strTemp.PrintF(_S( 4809, "%I64d ��"), m_pIconsStuff[i]->getItems()->Item_Sum);
				pDrawPort->PutTextEx( strTemp, m_nPosX + 264, m_nPosY + 117 + (37*i), 0xC3C3C3ff );
				
			}
			
		}
	}	

	if (m_pIconsMake->IsEmpty() == false)
	{
		m_pIconsMake->Render(pDrawPort);
		CItemData* ptemItem = _pNetwork->GetItemData(m_pIconsMake->getIndex());
		strTemp.PrintF("%s", ptemItem->GetName());
		pDrawPort->PutTextEx( strTemp, m_nPosX + 258, m_nPosY + 55, 0xC3C3C3ff );
		
	}

	pDrawPort->EndTextEx();
}

void CUIProduct2::AddMakeItemList(INDEX nListIndex)
{
	m_vctMakeLearn.push_back(nListIndex);
}

BOOL CUIProduct2::IsLearn(INDEX nIndex)
{
	std::vector<INDEX>::const_iterator list_end = m_vctMakeLearn.end();
	std::vector<INDEX>::iterator it = m_vctMakeLearn.begin();
	for(; it != list_end; ++it)
	{	
		if ((*it) == nIndex)
		{
			return TRUE;
		}
	}
	return FALSE;
	
}

void CUIProduct2::SetStuffItem()
{
	if (pSelItemData == NULL)
	{
		return;
	}

	m_pIconsMake->setData(UBET_ITEM, pSelItemData->GetItemButtonIndex());

	for (int i = 0; i < 5; i++)
	{
		int temstuffindex = pSelItemData->GetStuffItemIndex(i);
		m_pIconsStuff[i]->setData(UBET_ITEM, temstuffindex);
		m_pIconsStuff[i]->setCount(pSelItemData->GetStuffItemCount(i));
	}

}

WMSG_RESULT	CUIProduct2::MouseMessage( MSG *pMsg )
{
	//WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL bTitleBarClick = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();

			// Move refine
			if( IsFocused() && bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
			// Close button
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// OK button
			else if( m_btnMake.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_lbMakeList.MouseMessage(pMsg) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else
			{
				m_pIconsMake->MouseMessage(pMsg);
				
				for(int i = 0; i < 5; i++)
				{
					m_pIconsStuff[i]->MouseMessage(pMsg);
				}
				return WMSG_SUCCESS;
			}

		}break;
	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				nOldX = nX;		nOldY = nY;

				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Title bar
				else if( IsInsideRect( nX, nY,UIRect(0,0,457,22)) )
				{
					bTitleBarClick = TRUE;
				}
				// OK button
				else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Cancel button
				else if( m_btnMake.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if ( m_lbMakeList.MouseMessage ( pMsg ) != WMSG_FAIL )
				{
					int nSelList = m_lbMakeList.GetCurSel();
					if (nSelList != -1)
					{
						int groupItemcont = 0,groupNamecont = 0;
						for(int i=0; i<m_lbMakeList.GetCurItemCount(0); i++)
						{
							if (m_lbMakeList.GetState(i) == CUIListBoxEx::PS_CLOSE)
									groupNamecont++;
							
							if(m_lbMakeList.GetState(i) == CUIListBoxEx::PS_EXTEND)
								break;
							
							groupItemcont++;
						}
						int nRealIndex = (nSelList+groupItemcont-groupNamecont)*2+1;

						pSelItemData = (CMakeItemData*)m_lbMakeList.GetSelData(nRealIndex);
						SetStuffItem();
					}
					else
					{
						pSelItemData = NULL;
						m_pIconsMake->clearIconData();
					}
					return WMSG_SUCCESS;
				}

				CUIManager::getSingleton()->RearrangeOrder( UI_PRODUCT2, TRUE );
				return WMSG_SUCCESS;
			}

		}break;
	case WM_LBUTTONUP:
		{
			// Title bar
			bTitleBarClick = FALSE;
			if (IsInside( nX, nY))
			{
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					CloseProduct2();
				}
				
				if (m_btnMake.MouseMessage( pMsg) != WMSG_FAIL )
				{
					if (pSelItemData != NULL)
					{
						m_nSelFactoryIndex = pSelItemData->GetFactoryIndex();
						SendMakeItem(MSG_FACTORY_ITEM_MAKE_REQ, m_nSelFactoryIndex);
					}
				}
				return WMSG_SUCCESS;
			}

		}break;
	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{				
				return m_lbMakeList.MouseMessage(pMsg);
			}
		}
		break;
	}
	return WMSG_FAIL;
}

void CUIProduct2::SetItemMaking(BOOL bMaking)
{
	m_bItemMaking = bMaking;
	SendMakeItem(MSG_FACTORY_ITEM_MAKE, m_nSelFactoryIndex);
}

void CUIProduct2::ProgressBarRender()
{

}


void CUIProduct2::SendMakeItem(UBYTE ubType, ULONG lindex)
{
	CNetworkMessage nmMessage(MSG_FACTORY);

	nmMessage << ubType;
	nmMessage << lindex;

	_pNetwork->SendToServerNew(nmMessage);
}

//*****************************
// CUIProductNPC
//*******************************
CUIProductNPC::CUIProductNPC()
{
	Reset();
	m_ptdaddTexture = NULL;
	m_nNpcVirIdx = -1;
}

CUIProductNPC::~CUIProductNPC()
{
	Reset();

	Destroy();

	STOCK_RELEASE(m_ptdaddTexture);
}

void CUIProductNPC::Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{

	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\ProductNPC.tex" ) );

	m_bxBackground.SetBoxPos(WRect(0,0,256,394));
	m_bxBackground.SetBoxUV(m_ptdBaseTexture, 10, WRect(0,0,256,394));

	m_ptdaddTexture = CreateTexture( CTString( "Data\\Interface\\CommonBtn.tex" ) );

	FLOAT	fTexWidth = m_ptdaddTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdaddTexture->GetPixHeight();
	_nMsgBoxLineHeight = _pUIFontTexMgr->GetFontHeight() + 4;

	// Close button
	m_btnClose.Create( this, CTString( "" ), 233, 4, 16, 16 );
	m_btnClose.SetUV( UBS_IDLE, 211, 33, 226, 48, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 228, 33, 244, 48, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	m_btnLearn.Create( this, _S(269,"����"), 11, 366, 90, 22 );
	m_btnLearn.SetUV( UBS_IDLE, 112, 0, 112+71, 23, fTexWidth, fTexHeight );
	m_btnLearn.SetUV( UBS_CLICK, 185, 0, 185+71, 23, fTexWidth, fTexHeight );
	m_btnLearn.CopyUV( UBS_IDLE, UBS_ON );
	m_btnLearn.CopyUV( UBS_IDLE, UBS_DISABLE );

	m_btnCancel.Create( this, _S(139,"���"), 156, 366, 90, 22 );
	m_btnCancel.SetUV( UBS_IDLE, 112, 0, 112+71, 23, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 185, 0, 185+71, 23, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );

	// List box
	m_lbLearnList.Create( this, 26, 69, 206, 242, _nMsgBoxLineHeight, 12, 8, 2, TRUE );
	m_lbLearnList.CreateScroll( TRUE, 0, 0, 10, 242, 10, 10, 0, 0, 10 );
	m_lbLearnList.SetSelBar( 160, _pUIFontTexMgr->GetLineHeight(), 145, 138, 239, 171, fTexWidth, fTexHeight );
	m_lbLearnList.SetOverColor( 0xF8E1B5FF );
	m_lbLearnList.SetSelectColor( 0xF8E1B5FF );
	m_lbLearnList.SetColumnPosX( 1, 10 );
	// Up button
	m_lbLearnList.SetScrollUpUV( UBS_IDLE, 156, 33, 166, 43, fTexWidth, fTexHeight );
	m_lbLearnList.SetScrollUpUV( UBS_CLICK, 168, 33, 178, 43, fTexWidth, fTexHeight );
	m_lbLearnList.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbLearnList.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_lbLearnList.SetScrollDownUV( UBS_IDLE, 156, 45, 166, 55, fTexWidth, fTexHeight );
	m_lbLearnList.SetScrollDownUV( UBS_CLICK, 168, 45, 178, 55, fTexWidth, fTexHeight );
	m_lbLearnList.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbLearnList.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button 185, 32, 194, 70,
	m_lbLearnList.SetScrollBarTopUV( 185, 32, 195, 40, fTexWidth, fTexHeight );
	m_lbLearnList.SetScrollBarMiddleUV( 185, 40, 195, 60, fTexWidth, fTexHeight );
	m_lbLearnList.SetScrollBarBottomUV( 185, 62, 195, 70, fTexWidth, fTexHeight );
	// pop button
	m_lbLearnList.SetPopBtnSpace(1, 0);
	m_lbLearnList.SetPopBtnSize(11, 11);
	m_lbLearnList.SetPopBtnUV( UCBS_NONE, 243, 59, 253, 69, fTexWidth, fTexHeight);
	m_lbLearnList.SetPopBtnUV( UCBS_CHECK, 229, 59, 239, 69, fTexWidth, fTexHeight);

}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIProductNPC::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIProductNPC::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

void CUIProductNPC::OpenProductNPC(int iMobIndex, int iMobVirIdx, FLOAT npcx, FLOAT npcz)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	m_fNpcX = npcx;
	m_fNpcZ = npcz;
	m_nNpcVirIdx = iMobVirIdx;

	if (iMobIndex == 1069)//���ۿ��� ������ �Ӹ�
	{
		pUIManager->CloseMessageBoxL( MSGLCMD_PRODUCETNPC_VOUCHER );
		
		pUIManager->CreateMessageBoxL( _S( 4621, "���ۿ��� ������"  ) , UI_PRODUCTNPC, MSGLCMD_PRODUCETNPC_VOUCHER );		
		
		pUIManager->AddMessageBoxLString( MSGLCMD_PRODUCETNPC_VOUCHER, TRUE, _S( 4622, "�̹����̿� � ���ʽÿ� ���� ���ۿ����� ����� �� ���� ����� ���ݱ��� ���� ���� ��Ű�� �θ� ��ô�� �� �־����ϴ�." ), -1, 0xA3A1A3FF );		
		pUIManager->AddMessageBoxLString( MSGLCMD_PRODUCETNPC_VOUCHER, TRUE, _S( 4623, "�� ��� �ٸ� ���� �������κ��� �ο� ����� �븮�� ħ���� ���Ӿ��� �̷�������ϴ�. " ), -1, 0xA3A1A3FF );		
		pUIManager->AddMessageBoxLString( MSGLCMD_PRODUCETNPC_VOUCHER, TRUE, _S( 4624, "�׷��� ���� ���ۿ��տ����� ����� ���ϴ� �ڿ��� ����ȸ������ �޾Ƶ鿩 ���� ����� �ܺη� �����ϱ�� �����Ͽ����ϴ�. " ), -1, 0xA3A1A3FF );
		pUIManager->AddMessageBoxLString( MSGLCMD_PRODUCETNPC_VOUCHER, TRUE, _S( 4625, "������ �� �̻��� ħ�����κ��� ��� ����� �� ������ �ƴ� ��Ȱ�� �ʿ��� ���� ����� �����ϱ� �����Դϴ�. " ), -1, 0xA3A1A3FF );
		pUIManager->AddMessageBoxLString( MSGLCMD_PRODUCETNPC_VOUCHER, TRUE, _S( 4626, "�̹����̿� ���ۿ���ȸ���� ������ �����Ű���?" ), -1, 0xA3A1A3FF );
		pUIManager->AddMessageBoxLString( MSGLCMD_PRODUCETNPC_VOUCHER, FALSE, _S( 4627, "���� ���� ȸ�� ���" ), SEAL_MEMBER );	
		pUIManager->AddMessageBoxLString( MSGLCMD_PRODUCETNPC_VOUCHER, FALSE, _S( 4628, "������ �� ������ ��ǥ" ), SEAL_1 );	
		pUIManager->AddMessageBoxLString( MSGLCMD_PRODUCETNPC_VOUCHER, FALSE, _S( 4629, "ȭ���� �� ������ ��ǥ" ), SEAL_2 );
		pUIManager->AddMessageBoxLString( MSGLCMD_PRODUCETNPC_VOUCHER, FALSE, _S( 4630, "��ī�ο� ���� ������ ��ǥ" ), SEAL_3 );
		pUIManager->AddMessageBoxLString( MSGLCMD_PRODUCETNPC_VOUCHER, FALSE, _S( 4631, "�ܴ��� ���� ������ ��ǥ" ), SEAL_4 );
		pUIManager->AddMessageBoxLString( MSGLCMD_PRODUCETNPC_VOUCHER, FALSE, _S( 4632, "�Ҹ�ǰ ������ ��ǥ" ), SEAL_5 );

	}else
	{
		pUIManager->CloseMessageBoxL( MSGLCMD_PRODUCETNPC_LEARN );
	
		pUIManager->CreateMessageBoxL( _S( 4633, "���� ��� NPC"  ) , UI_PRODUCTNPC, MSGLCMD_PRODUCETNPC_LEARN );		
		
		pUIManager->AddMessageBoxLString( MSGLCMD_PRODUCETNPC_LEARN, TRUE, _S( 4634, "���� �Ϸ� ���� ã�ƿ��̽��ϱ�?" ), -1, 0xA3A1A3FF );		
		pUIManager->AddMessageBoxLString( MSGLCMD_PRODUCETNPC_LEARN, TRUE, _S( 4635, "���۱���� ���� �ޱ� ������ ���̴ٰ��? ���� ���� ����� �ƹ��� ���� ���� �� �ִ� ���� �ƴմϴ�. " ), -1, 0xA3A1A3FF );		
		pUIManager->AddMessageBoxLString( MSGLCMD_PRODUCETNPC_LEARN, TRUE, _S( 4636, "���� ���� ������ �Ӹ����� ���Ͽ� ȸ������ ��ϵ� �ڵ鸸 ���� ���� �� �ֽ��ϴ�. " ), -1, 0xA3A1A3FF );		
		pUIManager->AddMessageBoxLString( MSGLCMD_PRODUCETNPC_LEARN, TRUE, _S( 4637, "������Բ� ���� ��ǥ�� �޾ƿ��ø� ���� ����� ������ �帮�ڽ��ϴ�. ��ǥ�� �޾ƿ��̴ٸ� ��ǥ�� �����ֽʽÿ�." ), -1, 0xA3A1A3FF );
		pUIManager->AddMessageBoxLString( MSGLCMD_PRODUCETNPC_LEARN, FALSE, _S( 4638, "������ �� ���۱��" ), SEAL_1 );	
		pUIManager->AddMessageBoxLString( MSGLCMD_PRODUCETNPC_LEARN, FALSE, _S( 4639, "ȭ���� �� ���۱��" ), SEAL_2 );
		pUIManager->AddMessageBoxLString( MSGLCMD_PRODUCETNPC_LEARN, FALSE, _S( 4640, "��ī�ο� ���� ���۱��" ), SEAL_3 );
		pUIManager->AddMessageBoxLString( MSGLCMD_PRODUCETNPC_LEARN, FALSE, _S( 4641, "�ܴ��� ���� ���۱��" ), SEAL_4 );
		pUIManager->AddMessageBoxLString( MSGLCMD_PRODUCETNPC_LEARN, FALSE, _S( 4642, "�Ҹ�ǰ ���۱��" ), SEAL_5 );

	}

	pUIManager->AddMessageBoxLString( MSGLCMD_PROCESSNPC_REQ, FALSE, _S( 1220, "����Ѵ�." ) );
}

void CUIProductNPC::OpenProductList(INDEX sealtype)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if (pUIManager->GetProduct2()->IsVisible())
	{
		pUIManager->GetProduct2()->CloseProduct2();
	}

	Reset();
	m_nSealType = sealtype;
	pSelItemData = NULL;
	pUIManager->GetProduct2()->SetList(&m_lbLearnList, sealtype);

	// If this is already exist
	if( IsVisible() == TRUE )
		return;

	pUIManager->RearrangeOrder( UI_PRODUCTNPC, TRUE );
}

void CUIProductNPC::CloseProductNPC()
{
	Reset();
	m_nNpcVirIdx = -1;
	CUIManager::getSingleton()->RearrangeOrder( UI_PRODUCTNPC, FALSE );
}

void CUIProductNPC::Render()
{
	// Check distance
	FLOAT	fDiffX = _pNetwork->MyCharacterInfo.x - m_fNpcX;
	FLOAT	fDiffZ = _pNetwork->MyCharacterInfo.z - m_fNpcZ;

	if( fDiffX * fDiffX + fDiffZ * fDiffZ > UI_VALID_SQRDIST )
	{
		if (IsVisible())
		{
			CloseProductNPC();
		}
		return;
	}

	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Get position
	int	nX, nY;
	GetAbsPos( nX, nY );

	m_bxBackground.Render( nX, nY );

	// Render Title Text
	pDrawPort->PutTextExCX( pUIManager->GetProduct2()->GetSealTypeName(m_nSealType), m_nPosX + 126,				
										m_nPosY + 20, COLOR_TEXT );

	pDrawPort->FlushRenderingQueue();

	pDrawPort->InitTextureData( m_ptdaddTexture );

	m_btnClose.Render();
	m_btnLearn.Render();
	m_btnCancel.Render();
	m_lbLearnList.Render(); // list class ���ο��� Flush ȣ��

	pDrawPort->PutTextEx( _S(1058, "�ʿ� ����") , nX+23, nY+326, 0x80875BFF);
	pDrawPort->PutTextEx( _S(4643, "�ʿ� ���õ�") , nX+23, nY+345, 0x80875BFF);

	if (pSelItemData != NULL)
	{
		CTString strTemp;
		strTemp.PrintF("%I64d",pSelItemData->GetNeedNas());
		pDrawPort->PutTextExCX( strTemp , nX+192, nY+326, 0x80875BFF);
		strTemp.PrintF("%I64d",pSelItemData->GetNeedExp());
		pDrawPort->PutTextExCX( strTemp , nX+192, nY+345, 0x80875BFF);
	}

	pDrawPort->EndTextEx();
}

void CUIProductNPC::Reset()
{
	pSelItemData = NULL;
	m_nSealNum = -1;
	m_lbLearnList.Reset();
}

WMSG_RESULT	CUIProductNPC::MouseMessage( MSG *pMsg )
{
//	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL bTitleBarClick = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();

			// Move refine
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
			// Close button
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// OK button
			else if( m_btnLearn.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Cancel button
			else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if (m_lbLearnList.MouseMessage(pMsg) != WMSG_FAIL)
				return WMSG_SUCCESS;

		}break;
	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				nOldX = nX;		nOldY = nY;				

				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Title bar
				else if( IsInsideRect( nX, nY,UIRect(0,0,256,22)) )
				{
					bTitleBarClick = TRUE;
				}
				// OK button
				else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Cancel button
				else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( m_btnLearn.MouseMessage( pMsg ) != WMSG_FAIL )
				{
				
				}
				else if ( m_lbLearnList.MouseMessage ( pMsg ) != WMSG_FAIL )
				{
					int nSelList = m_lbLearnList.GetCurSel();
					if (nSelList != -1)
					{
						int groupItemcont = 0,groupNamecont = 0;
						for(int i=0; i<m_lbLearnList.GetCurItemCount(0); i++)
						{
							if (m_lbLearnList.GetState(i) == CUIListBoxEx::PS_CLOSE)
									groupNamecont++;
							
							if(m_lbLearnList.GetState(i) == CUIListBoxEx::PS_EXTEND)
								break;
							
							groupItemcont++;
						}
						int nRealIndex = (nSelList+groupItemcont-groupNamecont)*2+1;

						pSelItemData = (CMakeItemData*)m_lbLearnList.GetSelData(nRealIndex);
					}
					else
					{
						pSelItemData = NULL;
					}
					return WMSG_SUCCESS;	
				}
				CUIManager::getSingleton()->RearrangeOrder( UI_PRODUCTNPC, TRUE );
				return WMSG_SUCCESS;
			}

		}break;
	case WM_LBUTTONUP:
		{
			// Title bar
			bTitleBarClick = FALSE;
			if( !IsFocused() )
					return WMSG_FAIL;
			
			if( IsInside( nX, nY ) )
			{
				if ( m_lbLearnList.MouseMessage ( pMsg ) != WMSG_FAIL )
				{					
					return WMSG_SUCCESS;	
				}
				else if (m_btnLearn.MouseMessage(pMsg) != WMSG_FAIL)
				{
					if (pSelItemData != NULL)
					{
						SendFactoryLearn(pSelItemData->GetFactoryIndex());
					}
				}else if (m_btnClose.MouseMessage(pMsg) != WMSG_FAIL)
				{
					CloseProductNPC();
					return WMSG_SUCCESS;
				}
				else if (m_btnCancel.MouseMessage(pMsg) != WMSG_FAIL)
				{
					CloseProductNPC();
					return WMSG_SUCCESS;
				}
			}

		}break;
	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{				
				return m_lbLearnList.MouseMessage(pMsg);
			}
		}
		break;
	}
	return WMSG_FAIL;
}

void CUIProductNPC::MsgBoxLCommand( int nCommandCode, int nResult )
{

	switch( nCommandCode )
	{
	case MSGLCMD_PRODUCETNPC_VOUCHER:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			switch( nResult )
			{
			case SEAL_MEMBER:	//���� ���
				{
					CUIMsgBox_Info	MsgBoxInfo;
					CSkill &rSkill = _pNetwork->GetSkillData(SEALTYPE_START_INDEX-1);
					SQUAD nNeedCount = rSkill.GetLearnItemCount(0,0);
					CTString strTemp;
					strTemp.PrintF(_S(904, "�ʿ� ���� : %I64d" ),nNeedCount);
					MsgBoxInfo.SetMsgBoxInfo( _S( 191, "Ȯ��" ), UMBS_OKCANCEL, UI_PRODUCTNPC, MSGCMD_SEAL_MEMBER );		
					MsgBoxInfo.AddString( _S( 4712, "���� ���� ȸ�� ����� �Ͻðڽ��ϱ�?") );
					MsgBoxInfo.AddString( strTemp );
					pUIManager->CreateMessageBox( MsgBoxInfo );

				}break;
			case SEAL_1:	// ���� ��ǥ ��û			
			case SEAL_2:				
			case SEAL_3:				
			case SEAL_4:
			case SEAL_5:
				{
					m_nSealNum = SEALTYPE_START_INDEX+nResult;

					CUIMsgBox_Info	MsgBoxInfo;
					CSkill &rSkill = _pNetwork->GetSkillData(m_nSealNum);
					SQUAD nNeedCount = rSkill.GetLearnItemCount(0,0);
					CTString strTemp;
					strTemp.PrintF(_S(904, "�ʿ� ���� : %I64d" ),nNeedCount);
					MsgBoxInfo.SetMsgBoxInfo( _S( 191, "Ȯ��" ), UMBS_OKCANCEL, UI_PRODUCTNPC, MSGCMD_LEARN_SEAL );		
					MsgBoxInfo.AddString( _S( 4713, "������ ��ǥ�� ȹ�� �Ͻðڽ��ϱ�?") );
					MsgBoxInfo.AddString( strTemp );
					pUIManager->CreateMessageBox( MsgBoxInfo );

				}break;
			default:
				{
					pUIManager->CloseMessageBoxL( MSGLCMD_PRODUCETNPC_VOUCHER );
				}
			}
		}break;
	case MSGLCMD_PRODUCETNPC_LEARN:
		{
			switch(nResult)
			{
			case SEAL_1:	// ���� ��ǥ ��û			
			case SEAL_2:				
			case SEAL_3:				
			case SEAL_4:
			case SEAL_5:
				{
					_pNetwork->SendMakeItemList(SEALTYPE_START_INDEX+nResult,FALSE );
					
				}break;
			default:
				{
					CUIManager::getSingleton()->CloseMessageBoxL( MSGLCMD_PRODUCETNPC_LEARN );
				};
				
			}
		}break;
	}

}

// ----------------------------------------------------------------------------
// Name : MsgBoxCommand()
// Desc : [090824 sora]
// ----------------------------------------------------------------------------
void CUIProductNPC::MsgBoxCommand(int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode )
	{
		case MSGCMD_SEAL_MEMBER:
			{
				if(bOK)
				{
					CNetworkMessage nmMessage;
					RequestClient::sealSkillLearn* packet = reinterpret_cast<RequestClient::sealSkillLearn*>(nmMessage.nm_pubMessage);
					packet->type = MSG_SKILL;
					packet->subType = MSG_SKILL_LEARN_SEAL;
					packet->npcIndex = m_nNpcVirIdx;
					packet->skillIndex = SEALTYPE_START_INDEX-1; //���յ�� ��ų �ε��� 655
					nmMessage.setSize( sizeof(*packet) );

					_pNetwork->SendToServerNew(nmMessage);
				}
			}
			break;

		case MSGCMD_LEARN_SEAL:
			{
				if(bOK)
				{
					CNetworkMessage nmMessage;
					RequestClient::sealSkillLearn* packet = reinterpret_cast<RequestClient::sealSkillLearn*>(nmMessage.nm_pubMessage);
					packet->type = MSG_SKILL;
					packet->subType = MSG_SKILL_LEARN_SEAL;
					packet->npcIndex = m_nNpcVirIdx;
					packet->skillIndex = m_nSealNum;
					nmMessage.setSize( sizeof(*packet) );

					_pNetwork->SendToServerNew(nmMessage);
				}
				else
				{
					m_nSealNum = -1;
				}
			}
			break;

		default:
			CUIManager::getSingleton()->GetChattingUI()->AddSysMessage(_S(16, "�˼� ���� ������ �߻��߽��ϴ�."));
			break;
	}
}


void CUIProductNPC::SendFactoryLearn(int nIndex)
{
	CNetworkMessage nmMessage(MSG_FACTORY);

	nmMessage << (UBYTE)MSG_FACTORY_LEARN;
	nmMessage << (ULONG) nIndex;

	_pNetwork->SendToServerNew(nmMessage);
}