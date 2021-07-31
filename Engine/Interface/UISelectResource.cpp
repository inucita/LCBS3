#include "stdh.h"

// ��� ����. [12/2/2009 rumist]
#include <vector>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UISelectResource.h>
#include <Engine/Entities/EntityProperties.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Ska/Render.h>

#define BUTTON_HEIGHT	21
#define BUTTON_WIDTH	131
#define BUTTON_SPAN		4
#define TITLEBAR_HEIGHT 26

#define PRODUCT_TYPE_RANDOM		0	// ������ ����
#define PRODUCT_TYPE_SELECT		1	// ���� ����


int g_iProductItemDBIndex[3][5] = {
	{156, 155, 154, 153, 152}
	, {199, 198, 197, 163, 162}
	, {161, 160, 159, 158, 157}
};

// ----------------------------------------------------------------------------
// Name : CUISelectResource()
// Desc : Constructorreturn WMSG_FAIL;
// ----------------------------------------------------------------------------
CUISelectResource::CUISelectResource()
{	
}

// ----------------------------------------------------------------------------
// Name : ~CUISelectResource()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUISelectResource::~CUISelectResource()
{
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUISelectResource::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);
	
	// Region of each part
	m_rcTitle.SetRect( 0, 0, 236, 22 );
	
	// Create skill learn texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\MessageBox.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();
	
	// UV Coordinate of each part
	// Background
	m_rtBackTop.SetUV( 0, 0, 216, 26, fTexWidth, fTexHeight );
	m_rtBackMiddle1.SetUV( 0, 31, 216, 33, fTexWidth, fTexHeight );
	m_rtBackMiddle2.SetUV( 0, 35, 216, 37, fTexWidth, fTexHeight );
	m_rtBackBottom.SetUV( 0, 38, 216, 45, fTexWidth, fTexHeight );

	// Close button
	m_btnClose.Create( this, CTString( "" ), 147, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 219, 0, 233, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 234, 0, 248, 14, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	// ���� ä�� ä�� �� ��� Ű���� �̵��� ���� ���� �÷��� [3/26/2013 Ranma]
	m_bKeyMove = true;
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUISelectResource::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}
// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUISelectResource::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : OpenSelectResource()
// Desc : iType = 1 ä��, iType = 2 ä��, iType = 3 ����
// ----------------------------------------------------------------------------
void CUISelectResource::OpenSelectResource( int npcIndex, int iType )
{	
	if(iType == -1)
		return;
	if( IsVisible() )
	{
		ResetSelectResource();
		OpenSelectResource(npcIndex, iType);
		return;
	}

	CEntityProperty *pEP = CEntity::GetPlayerEntity(0)->PropertyForTypeAndID(CEntityProperty::EPT_BOOL, 221);
	if(pEP)
	{
		ENTITYPROPERTY( &*CEntity::GetPlayerEntity(0), pEP->ep_slOffset, BOOL) = FALSE;
	}
	pEP = CEntity::GetPlayerEntity(0)->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 222);
	if(pEP)
	{
		ENTITYPROPERTY( &*CEntity::GetPlayerEntity(0), pEP->ep_slOffset, INDEX) = -1;
	}
	ResetSelectResource();

	m_iResourceType = iType;
	m_iClientNPCIndex = npcIndex;

	if(iType == 4 ) //�����̸� �н�
	{
		StartProduct();
		ResetSelectResource();
	}
	else 
	{
		FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
		FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

		CUIButton TempUIButton;
		TempUIButton.SetUV( UBS_IDLE, 0, 117, 131, 138, fTexWidth, fTexHeight );
		TempUIButton.SetUV( UBS_CLICK, 0, 140, 131, 161, fTexWidth, fTexHeight );
		TempUIButton.CopyUV( UBS_IDLE, UBS_ON );
		TempUIButton.CopyUV( UBS_IDLE, UBS_DISABLE );

		const int iStartX = 20;
		const int iStartY = 30;	
		int iCurPosX = iStartX;
		int iCurPosY = iStartY;

		CTString strName;
		for(INDEX i=0; i<2; ++i)
		{
			strName = i?_S( 1972, "���� ����" ):_S( 1973, "������ ����" );
			TempUIButton.Create(this, strName, iCurPosX, iCurPosY, BUTTON_WIDTH, BUTTON_HEIGHT );
			m_vectorResourceList.push_back(TempUIButton);
			iCurPosY += BUTTON_HEIGHT + BUTTON_SPAN;
		}
		int nHeight = m_vectorResourceList.size() * (BUTTON_HEIGHT + BUTTON_SPAN) + TITLEBAR_HEIGHT + 10;
		SetSize( m_nWidth, nHeight );
		CUIManager::getSingleton()->RearrangeOrder( UI_SELECTRESOURCE, TRUE );
	}

	// ���� �Ǹ� Ű���� �̵� ���� [3/26/2013 Ranma]
	m_bKeyMove = false;
	return;
}

// ----------------------------------------------------------------------------
// Name : ResetSelectResource()
// Desc : 
// ----------------------------------------------------------------------------
void CUISelectResource::ResetSelectResource()
{
	m_iResourceType = -1;

	m_bSelectType = TRUE;

	if(!m_vectorResourceList.empty())
		m_vectorResourceList.clear();
	CUIManager::getSingleton()->RearrangeOrder( UI_SELECTRESOURCE, FALSE );
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUISelectResource::Render()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set skill learn texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );
	
	// Add render regions
	// Background
	int nX = m_nPosX;
	int nY = m_nPosY;
	pDrawPort->AddTexture( nX, m_nPosY, nX + m_nWidth, nY + TITLEBAR_HEIGHT,
		m_rtBackTop.U0, m_rtBackTop.V0, 
		m_rtBackTop.U1, m_rtBackTop.V1,
		0xFFFFFFFF );
	
	nY += TITLEBAR_HEIGHT;	
	pDrawPort->AddTexture( nX, nY, nX + m_nWidth, m_nPosY + m_nHeight - 7,
		m_rtBackMiddle1.U0, m_rtBackMiddle1.V0, 
		m_rtBackMiddle1.U1, m_rtBackMiddle1.V1,
		0xFFFFFFFF );
	
	nY = m_nPosY + m_nHeight - 7;

	pDrawPort->AddTexture( nX, nY, nX + m_nWidth, m_nPosY + m_nHeight,
		m_rtBackBottom.U0, m_rtBackBottom.V0, 
		m_rtBackBottom.U1, m_rtBackBottom.V1,
		0xFFFFFFFF );	

	m_btnClose.Render();

	std::vector<CUIButton>::iterator itend = m_vectorResourceList.end();
	std::vector<CUIButton>::iterator it = m_vectorResourceList.begin();

	for(; it != itend; ++it)
	{
		(*it).Render();
	}

	// Render all elements
	pDrawPort->FlushRenderingQueue();	
	
	CTString strTitle = _S( 1823, "�̳׶� ����" );
	
	// Text in skill learn	
	if( m_bSelectType )
	{
		strTitle =_S( 1974,  "������ ����" );
	}
		
	pDrawPort->PutTextEx( strTitle, m_nPosX + SELECTRESOURCE_TITLE_TEXT_OFFSETX,	
		m_nPosY + SELECTRESOURCE_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );
	
	// Flush all render text queue
	pDrawPort->EndTextEx();
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT	CUISelectResource::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;
	
	// Title bar
	static BOOL	bTitleBarClick = FALSE;
	
	// Item clicked
	static BOOL	bLButtonDownInItem = FALSE;
	
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
			
			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;
			
			// Move shop
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX;	nOldY = nY;
				
				Move( ndX, ndY );
				
				return WMSG_SUCCESS;
			}
			
			// Close Button
			if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;

			for(int i = 0; i < m_vectorResourceList.size(); ++i)
			{
				if( (wmsgResult = m_vectorResourceList[i].MouseMessage( pMsg ) )!= WMSG_FAIL )
				{
					return WMSG_SUCCESS;
				}
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				CUIManager* pUIManager = CUIManager::getSingleton();
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
				else
				{
					std::vector<CUIButton>::iterator end = m_vectorResourceList.end();
					std::vector<CUIButton>::iterator it = m_vectorResourceList.begin();
					
					for(; it != end; ++it)
					{
						if( (*it).MouseMessage( pMsg ) != WMSG_FAIL )//button ����
						{
							int dist = std::distance(m_vectorResourceList.begin(), it);

							if( m_bSelectType )
							{
								if( dist == PRODUCT_TYPE_RANDOM )
								{
									StartProduct();
									ResetSelectResource();
								}
								else if( dist == PRODUCT_TYPE_SELECT )
								{
									m_bSelectType = FALSE;
									CTString strName;
					
									CUIButton TempUIButton;
									TempUIButton.SetUV( UBS_IDLE, 0, 117, 131, 138, m_ptdBaseTexture->GetPixWidth(), m_ptdBaseTexture->GetPixHeight() );
									TempUIButton.SetUV( UBS_CLICK, 0, 140, 131, 161, m_ptdBaseTexture->GetPixWidth(), m_ptdBaseTexture->GetPixHeight() );
									TempUIButton.CopyUV( UBS_IDLE, UBS_ON );
									TempUIButton.CopyUV( UBS_IDLE, UBS_DISABLE );

									m_vectorResourceList.clear();

									int iCurPosY = 30;
									for(INDEX i=0; i<3; ++i)
									{
	
										strName = _pNetwork->GetItemData( g_iProductItemDBIndex[m_iResourceType-1][i] )->GetName();
										TempUIButton.Create(this, strName, 20, iCurPosY, BUTTON_WIDTH, BUTTON_HEIGHT );
										m_vectorResourceList.push_back(TempUIButton);
										iCurPosY += BUTTON_HEIGHT + BUTTON_SPAN;
									}
									int nHeight = m_vectorResourceList.size() * (BUTTON_HEIGHT + BUTTON_SPAN) + TITLEBAR_HEIGHT + 10;
									SetSize( m_nWidth, nHeight );
									pUIManager->RearrangeOrder( UI_SELECTRESOURCE, TRUE );
								}
							}
							else
							{
								StartProduct( dist );
								ResetSelectResource();	

							}
							return WMSG_SUCCESS;
						}
					}
				}
				
				pUIManager->RearrangeOrder( UI_SELECTRESOURCE, TRUE );
				return WMSG_SUCCESS;
			}
			else
			{
				ResetSelectResource();
				m_bKeyMove = true;
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			bTitleBarClick = FALSE;
			
			// Close button
			if( (wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if(wmsgResult == WMSG_COMMAND)
				{
					ResetSelectResource();
					m_bKeyMove = true;
				}
				return WMSG_SUCCESS;
			}
			else
			{	
				for(int i = 0; i < m_vectorResourceList.size(); ++i)
				{
					if( (wmsgResult = m_vectorResourceList[i].MouseMessage( pMsg ) )!= WMSG_FAIL )
					{
						if(wmsgResult  == WMSG_COMMAND)
						{
						}
						return WMSG_SUCCESS;
					}
				}
			}
		}
		break;
	}
	return WMSG_FAIL;
}


//------------------------------------------------------------------------------
// CUISelectResource::StartProduct
// Explain: nSelectResource�� -1�� ��쿡�� Random���� ó���Ѵ�. 
// Date : 2005-08-30,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISelectResource::StartProduct( int nSelectResource )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// ����� ������ �޼����� ���� ������ ���� 
	CTString strTemp;
	// BUG-FIX : ITS(#1727) �꿡 ž�½ÿ��� ����/������ �� �� ����. [5/20/2011 rumist]
	if( _pNetwork->MyCharacterInfo.bWildPetRide || _pNetwork->MyCharacterInfo.bPetRide )
	{
		if( m_iResourceType == 4 )
			strTemp = _S( 5490, "�꿡 ž���� ���¿����� ����Ʈ �������� ������ �� �����ϴ�." );
		else
			strTemp = _S( 5491, "�꿡 ž���� ���¿����� ������ �� �����ϴ�." );
		
		pUIManager->GetChattingUI()->AddSysMessage( strTemp, SYSMSG_ERROR );
		return;
	}

	// BUG-FIX : ���Ż��¿����� ���� / ä�� �Ұ�. [6/3/2011 rumist]
	CPlayerEntity* pPlayerEntity = (static_cast<CPlayerEntity*>(CEntity::GetPlayerEntity(0)));

	if( pPlayerEntity->IsTransforming())
	{
		strTemp = _S( 5494, "���� ���¿����� ����/ä��/������ �� �� �����ϴ�." );
		
		pUIManager->GetChattingUI()->AddSysMessage( strTemp, SYSMSG_ERROR );
		return;
	}

	// ���� �� ���� ������ ���� ���ϸ��̼� ���� 
	extern INDEX idPlayerWhole_Animation[ANIM_TOTAL];
	switch(m_iResourceType)
	{
	case 1://ä��
		{
			INDEX ani = idPlayerWhole_Animation[ANIM_MINE];
			//INDEX ani = ska_StringToID(JobInfo().GetAnimationName( _pNetwork->MyCharacterInfo.job, ANIM_MINE ));
			CEntity::GetPlayerEntity(0)->AddAnimation(ani, AN_LOOPING|AN_CLEAR, 1.0f, 0x03,
					ESKA_MASTER_MODEL_INSTANCE, 1.0f);
		}
		break;
	case 2://ä��
		{
			INDEX ani = idPlayerWhole_Animation[ANIM_COLLECT];
			//INDEX ani = ska_StringToID(JobInfo().GetAnimationName( _pNetwork->MyCharacterInfo.job, ANIM_GATHER ));
			CEntity::GetPlayerEntity(0)->AddAnimation(ani, AN_LOOPING|AN_CLEAR, 1.0f, 0x03,
					ESKA_MASTER_MODEL_INSTANCE, 1.0f);
		}
		break;
	case 3://����
		{
			INDEX ani = idPlayerWhole_Animation[ANIM_GATHER];
			//INDEX ani = ska_StringToID(JobInfo().GetAnimationName( _pNetwork->MyCharacterInfo.job, ANIM_COLLECT ));
			CEntity::GetPlayerEntity(0)->AddAnimation(ani, AN_LOOPING|AN_CLEAR, 1.0f, 0x03,
					ESKA_MASTER_MODEL_INSTANCE, 1.0f);
		}
		break;
	case 4://����
		{
			INDEX ani = idPlayerWhole_Animation[ANIM_COLLECT];
			//INDEX ani = ska_StringToID(JobInfo().GetAnimationName( _pNetwork->MyCharacterInfo.job, ANIM_GATHER ));
			CEntity::GetPlayerEntity(0)->AddAnimation(ani, AN_LOOPING|AN_CLEAR, 1.0f, 0x03,
					ESKA_MASTER_MODEL_INSTANCE, 1.0f);
		}
		break;
	}

	if( nSelectResource >= 0 ) 
	{
		m_iProduceItemDBIndex = g_iProductItemDBIndex[m_iResourceType-1][nSelectResource];
		strTemp = _pNetwork->GetItemData( g_iProductItemDBIndex[m_iResourceType-1][nSelectResource] )->GetName();
	}
	else 
	{
		m_iProduceItemDBIndex = -1;
	}

	CEntityProperty *pEP = CEntity::GetPlayerEntity(0)->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 239);
	if(pEP)
	{
		ENTITYPROPERTY( &*CEntity::GetPlayerEntity(0), pEP->ep_slOffset, INDEX) = m_iProduceItemDBIndex;
	}
	
	pEP = CEntity::GetPlayerEntity(0)->PropertyForTypeAndID(CEntityProperty::EPT_BOOL, 221);
	if(pEP)
	{
		ENTITYPROPERTY( &*CEntity::GetPlayerEntity(0), pEP->ep_slOffset, BOOL) = TRUE;
	}
	
	pEP = CEntity::GetPlayerEntity(0)->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 222);
	if(pEP)
	{
		ENTITYPROPERTY( &*CEntity::GetPlayerEntity(0), pEP->ep_slOffset, INDEX) = m_iResourceType;
	}
	
	// ���� ���� ���� �������� ȭ�鿡 ǥ�� 
	// �����ϰ�� 
	if(m_iResourceType == 4)
		strTemp += _S(3053, "����Ʈ ������ ������ �����մϴ�." );
	else strTemp += _S( 1824, "������ �����մϴ�." );

	pUIManager->GetChattingUI()->AddSysMessage(strTemp, SYSMSG_NORMAL);

}