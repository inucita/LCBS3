// WSS_MINIGAME 070422 --------------------------------------------->>
// ������ �̺�Ʈ �̴ϰ��� UI
// -----------------------------------------------------------------<<

#include "stdh.h"

// ��� ����. [12/2/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIMinigame.h>

#define MINIGAME_BUTTON_SIZE 47
#define MINIGAME_BUTTON_POS_Y 125

// ----------------------------------------------------------------------------
// Name : CUIMinigame()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIMinigame::CUIMinigame()
{
	// wooss 060515 
	m_npcIdx =-1;
	m_npcChoice =0; // �⺻ ���� ����
	m_userChoice =-1;
	m_iWinNum =0;
	m_iWinNumOld =0;
	m_whowin =-1;
	m_bTalkOnce =FALSE;
	m_npcState = MINIGAME_NPC_NOTHING;
	m_ptdAddTexture = NULL;
	m_dwTimePass = 0;
}

// ----------------------------------------------------------------------------
// Name : ~CUIMinigame()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIMinigame::~CUIMinigame()
{
	if (m_ptdAddTexture)
	{
		_pTextureStock->Release(m_ptdAddTexture);
		m_ptdAddTexture = NULL;
	}
}

// ----------------------------------------------------------------------------
// Name : InitData()
// Desc :
// ----------------------------------------------------------------------------
void CUIMinigame::InitData()
{
	m_npcIdx =-1;
	m_npcChoice =0; // �⺻ ���� ����
	m_userChoice =-1;
	m_iWinNum =0;
	m_iWinNumOld =0;
	m_whowin =-1;
	m_bTalkOnce =FALSE;
	m_npcState = MINIGAME_NPC_NOTHING;

	m_btnReceiveItem.SetEnable(FALSE);
	m_btnStartGame.SetEnable(TRUE);
	m_btnKeepGoing.SetEnable(FALSE);

	m_btnScissors.SetEnable(FALSE);
	m_btnStone.SetEnable(FALSE);
	m_btnPaper.SetEnable(FALSE);
}

// ----------------------------------------------------------------------------
// Name : SetButtonState()
// Desc :
// ----------------------------------------------------------------------------
void CUIMinigame::SetButtonState(UBYTE iState)
{
	// 0 :	��� ��Ȱ��
	// 1 :	m_btnScissors, m_btnStone, m_btnPaper, m_btnKeepGoing   - disable   â ���½�
	//		m_btnReceiveItem, m_btnStartGame						- enable
	// 2 :  m_btnScissors, m_btnStone, m_btnPaper, m_btnStartGame   - disable   �¸�/��� �� 
	//		m_btnReceiveItem, m_btnKeepGoing						- enable
	// 3 :  m_btnReceiveItem, m_btnKeepGoing, m_btnStartGame		- disable   ���ӽ���/����ϱ�  ���� ��
	//		m_btnScissors, m_btnStone, m_btnPaper					- enable 
	// 4 :	m_btnScissors, m_btnStone, m_btnPaper, m_btnKeepGoing, m_btnStartGame   - disable   �й� ��
	//		m_btnReceiveItem				 										- enable

	// ���������� Ŭ���ô� bit flag�� ó��

	// 0x80 -> bit check
	// m_btnReceiveItem m_btnStartGame m_btnKeepGoing m_btnScissors m_btnStone m_btnPaper
	// 0x01				0x02			0x04			0x08		0x10		0x20
	UBYTE tState = iState;
	if(!(iState&0x80))
	{
		if(iState == BTN_STATE_DISABLE_ALL) tState = 0x00;
			else if(iState == BTN_STATE_OPEN ) tState = 0x03;
			else if(iState == BTN_STATE_WIN_DRAW ) tState = 0x05;	
			else if(iState == BTN_STATE_START_CONTINUE ) tState = 0x38;	
			else if(iState == BTN_STATE_DEFEAT ) tState = 0x01;	
			else if(iState == BTN_STATE_ENABEL_ALL ) tState = 0x3F;
	}
	
	m_btnReceiveItem.SetEnable(tState&0x01);
	m_btnStartGame.SetEnable(tState&0x02);
	m_btnKeepGoing.SetEnable(tState&0x04);

	m_btnScissors.SetEnable(tState&0x08);
	m_btnStone.SetEnable(tState&0x10);
	m_btnPaper.SetEnable(tState&0x20);


	// �ӽ÷� 
	m_btnReceiveItem.SetVisible(tState&0x01);
	m_btnStartGame.SetVisible(tState&0x02);
	m_btnKeepGoing.SetVisible(tState&0x04);

	m_btnScissors.SetVisible(tState&0x08);
	m_btnStone.SetVisible(tState&0x10);
	m_btnPaper.SetVisible(tState&0x20);

}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIMinigame::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	// Create Minigame texture ----------------------------------->><<
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\MessageBox.tex" ) );
	m_ptdAddTexture = CreateTexture( CTString( "Data\\Interface\\Gift.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();
	FLOAT	fTexWidth2 = m_ptdAddTexture->GetPixWidth();
	FLOAT	fTexHeight2 = m_ptdAddTexture->GetPixHeight();

	// Set Boxies ------------------------------------------------>><<
	m_bxTitle.SetBoxUV( m_ptdBaseTexture, 22, WRect(0,0,215,22) );
	m_bxTitle.SetBoxPos(WRect(0,0,MINIGAME_WIDTH,21));
	
	m_bxBack1.SetBoxUV( m_ptdBaseTexture, 10, WRect(0,22,215,44) );
	m_bxBack1.SetBoxPos(WRect(0,22,MINIGAME_WIDTH,MINIGAME_HEIGHT - 22));

	m_bxBack1_1.SetBoxUV( m_ptdBaseTexture, 4, WRect(146,46,162,62) );
	m_bxBack1_1.SetBoxPos(WRect(15,110,MINIGAME_WIDTH-15,MINIGAME_HEIGHT - 90));

	m_bxBack2.SetBoxUV( m_ptdBaseTexture, 2, WRect(328,177,338,187) );
	m_bxBack2.SetBoxPos(WRect(20,115,MINIGAME_WIDTH-20,MINIGAME_HEIGHT - 95));
	
	m_bxNpcTalk.SetBoxUV( m_ptdBaseTexture, 4, WRect(146,46,162,62) );
	m_bxNpcTalk.SetBoxPos(WRect(15,42,330,105));

	m_bxWinNum.CopyUV(&m_bxNpcTalk);
	m_bxWinNum.SetBoxPos(WRect(346,42,413,88));

	// Set Buttones ---------------------------------------------->><<
	
	// m_ptdBaseTexture ==>

	// Receive Button
	m_btnReceiveItem.Create( this, _S(3453, "�׸��ϰ� ��ǰ�ޱ�" ), 280, 300, 125, 21 );
	m_btnReceiveItem.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnReceiveItem.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnReceiveItem.CopyUV( UBS_IDLE, UBS_ON );
	m_btnReceiveItem.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Start Button
	m_btnStartGame.Create( this, _S(239, "���� ����" ), 30, 300, 95, 21 );
	m_btnStartGame.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnStartGame.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnStartGame.CopyUV( UBS_IDLE, UBS_ON );
	m_btnStartGame.CopyUV( UBS_IDLE, UBS_DISABLE );

	// KeepGoing Button
	m_btnKeepGoing.Create( this, _S(3454, "��� �����ϱ�" ), 130, 300, 95, 21 );
	m_btnKeepGoing.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnKeepGoing.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnKeepGoing.CopyUV( UBS_IDLE, UBS_ON );
	m_btnKeepGoing.CopyUV( UBS_IDLE, UBS_DISABLE );

	// FIX ME : ���� �̹��� ����...
	
	// m_ptdAddTexture ==>

	// m_btnNpcChoice Button
	for(int i=0; i<3 ; i++)
	{
		int tWidth = MINIGAME_BUTTON_SIZE;
		m_btnNpcChoice[i].Create( this, CTString( "" ), 186, MINIGAME_BUTTON_POS_Y, MINIGAME_BUTTON_SIZE, MINIGAME_BUTTON_SIZE );
		m_btnNpcChoice[i].SetUV( UBS_IDLE, i*tWidth, 137, (i+1)*tWidth, 184, fTexWidth2, fTexHeight2 );
		m_btnNpcChoice[i].CopyUV( UBS_IDLE, UBS_CLICK );
		m_btnNpcChoice[i].CopyUV( UBS_IDLE, UBS_ON );
		m_btnNpcChoice[i].CopyUV( UBS_IDLE, UBS_DISABLE );
	}
	// m_btnNpcChoiceMove Button
	m_btnNpcChoiceMove.Create( this, CTString( "" ), 186, MINIGAME_BUTTON_POS_Y, MINIGAME_BUTTON_SIZE, MINIGAME_BUTTON_SIZE );
	m_btnNpcChoiceMove.SetUV( UBS_IDLE, 0, 137, MINIGAME_BUTTON_SIZE, 184, fTexWidth2, fTexHeight2 );
	m_btnNpcChoiceMove.CopyUV( UBS_IDLE, UBS_CLICK );
	m_btnNpcChoiceMove.CopyUV( UBS_IDLE, UBS_ON );
	m_btnNpcChoiceMove.CopyUV( UBS_IDLE, UBS_DISABLE );
	

	// m_btnScissors Button
	m_btnScissors.Create( this, CTString( "" ), 97, MINIGAME_BUTTON_POS_Y + 94, MINIGAME_BUTTON_SIZE, MINIGAME_BUTTON_SIZE );
	m_btnScissors.SetUV( UBS_IDLE, 0, 89, MINIGAME_BUTTON_SIZE, 136, fTexWidth2, fTexHeight2 );
	m_btnScissors.CopyUV( UBS_IDLE, UBS_CLICK );
	m_btnScissors.CopyUV( UBS_IDLE, UBS_ON );
	m_btnScissors.CopyUV( UBS_IDLE, UBS_DISABLE );

	// m_btnStone Button
	m_btnStone.Create( this, CTString( "" ), 186, MINIGAME_BUTTON_POS_Y + 94, MINIGAME_BUTTON_SIZE, MINIGAME_BUTTON_SIZE );
	m_btnStone.SetUV( UBS_IDLE, MINIGAME_BUTTON_SIZE, 89, MINIGAME_BUTTON_SIZE*2, 136, fTexWidth2, fTexHeight2 );
	m_btnStone.CopyUV( UBS_IDLE, UBS_CLICK );
	m_btnStone.CopyUV( UBS_IDLE, UBS_ON );
	m_btnStone.CopyUV( UBS_IDLE, UBS_DISABLE );

	// m_btnPaper Button
	m_btnPaper.Create( this, CTString( "" ), 275, MINIGAME_BUTTON_POS_Y + 94, MINIGAME_BUTTON_SIZE, MINIGAME_BUTTON_SIZE );
	m_btnPaper.SetUV( UBS_IDLE, MINIGAME_BUTTON_SIZE*2, 89, MINIGAME_BUTTON_SIZE*3, 136, fTexWidth2, fTexHeight2 );
	m_btnPaper.CopyUV( UBS_IDLE, UBS_CLICK );
	m_btnPaper.CopyUV( UBS_IDLE, UBS_ON );
	m_btnPaper.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Set SelectBtn
	m_rtSelBtn.SetRect(186, MINIGAME_BUTTON_POS_Y, 186 + MINIGAME_BUTTON_SIZE,MINIGAME_BUTTON_POS_Y+MINIGAME_BUTTON_SIZE);
	m_rcSelBtn.SetUV(141,89,188,136,fTexWidth2,fTexHeight2);
	m_rtSelBtn2.SetRect(185, MINIGAME_BUTTON_POS_Y-1, 187 + MINIGAME_BUTTON_SIZE,MINIGAME_BUTTON_POS_Y+MINIGAME_BUTTON_SIZE+1);
	m_rcSelBtn2.SetUV(190,89,235,136,fTexWidth2,fTexHeight2);
	

	// Set VS 
	m_rtVS.SetRect(187,MINIGAME_BUTTON_POS_Y + 48,234,MINIGAME_BUTTON_POS_Y + 95);
	m_rcVS.SetUV(141,137,188,184,fTexWidth2,fTexHeight2);


	// Set NPC talk list
	// List box of guild description
	m_listNpcTalk.Create( this, 20, 45, 300 , 55, _pUIFontTexMgr->GetLineHeight(), 0, 0, 1, FALSE );
	m_listNpcTalk.CreateScroll( TRUE, 0, 0, 9, 58, 9, 7, 0, 0, 10 );	
	m_listNpcTalk.SetOverColor( 0x66B5FFFF );	
	// Scroll bar of list box
	// Up button
	m_listNpcTalk.SetScrollUpUV( UBS_IDLE, 230, 16, 238, 22, fTexWidth, fTexHeight );
	m_listNpcTalk.SetScrollUpUV( UBS_CLICK, 240, 16, 248, 22, fTexWidth, fTexHeight );
	m_listNpcTalk.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_listNpcTalk.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_listNpcTalk.SetScrollBarTopUV( 219, 16, 227, 21, fTexWidth, fTexHeight );
	m_listNpcTalk.SetScrollBarMiddleUV( 219, 21, 227, 35, fTexWidth, fTexHeight );
	m_listNpcTalk.SetScrollBarBottomUV( 219, 35, 227, 39, fTexWidth, fTexHeight );
	// Down button
	m_listNpcTalk.SetScrollDownUV( UBS_IDLE, 230, 24, 240, 31, fTexWidth, fTexHeight );
	m_listNpcTalk.SetScrollDownUV( UBS_CLICK,  240, 24, 250, 31, fTexWidth, fTexHeight );
	m_listNpcTalk.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_listNpcTalk.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );	
	// Adjust column position of list box
	m_listNpcTalk.SetColumnPosX( 0, 1 , TEXT_LEFT);

		
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIMinigame::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIMinigame::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : OpenMinigame()
// Desc :
// ----------------------------------------------------------------------------
void CUIMinigame::OpenMinigame(UBYTE eventType)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	CDrawPort* pDrawPort = pUIManager->GetDrawPort();
	int	nX = ( pDrawPort->dp_MinI + pDrawPort->dp_MaxI ) / 2 - m_nWidth / 2;
	int	nY = ( pDrawPort->dp_MinJ + pDrawPort->dp_MaxJ ) / 2 - m_nHeight / 2;

	SetPos( nX, nY );	
	SetButtonState(BTN_STATE_OPEN);
	// ���� ��� �� ���� 

	
	// set random seed 
	srand((unsigned int)time((time_t *)NULL));
	int iBack = rand()%9;
	WRect rtBack;
	rtBack.Left		= 328+(iBack%3)*10;
	rtBack.Top		= 177+(iBack/3)*10;
	rtBack.Right	= rtBack.Left+10;
	rtBack.Bottom	= rtBack.Top+10;
	
	m_bxBack2.SetBoxUV( m_ptdBaseTexture, 2, rtBack );

	m_ubEventType   = eventType;

	// set npc talk
	if (m_ubEventType == MSG_EVENT_GOMDORI_2007)
	{
		m_strNpcTalk[MINIGAME_START] = _S(3409, "9���� ������ ���� ������ ��� ã���̱���. �׷��ٸ� ������������ �����ؿ�, �غ� ������ ~��������~����!����!��!");
		m_strNpcTalk[MINIGAME_LOSE]	 = _S(3410, "���� �¸��׿�~~! ��������! ��, �ƽ����� ��������Դϴ�~ ���� �����̸� ��� ���� �絵���� �޾� �ְڽ��ϴ�. ��, ��. �׷��� ���ݱ��� ����� �� �������� �ʾƿ�. ���ϴ� �Ϳ� �� ��ġ������, �׷��� �̰Ŷ� �޾��ּ���.");
	}
	else if (m_ubEventType == MSG_EVENT_XMAS_2007)
	{
		m_strNpcTalk[MINIGAME_START] = _S(5799, "9���� ��ŸŬ�ν� ���� ������ ��� ã���̱���. �׷��ٸ� ������������ �����ؿ�. �غ� ������ ~��������~����! ����! ��!");
		m_strNpcTalk[MINIGAME_LOSE]	 = _S(5800, "���� �¸��׿�~~! ��������! ��, �ƽ����� ��������Դϴ�~ ���� ��ŸŬ�ν� ������ ��� ���� �絵���� �޾� �ְڽ��ϴ�. ��, ��. �׷��� ���ݱ��� ����� �� �������� �ʾƿ�. ���ϴ� �Ϳ� �� ��ġ������, �׷��� �̰Ŷ� �޾��ּ���.");
	}
	m_strNpcTalk[MINIGAME_DRAW] = _S(3411, "�̷�~ ���׿�! ��, �ٽ� �ѹ�, ���� ����~  ����!    ����!   ��!");
	m_strNpcTalk[MINIGAME_WIN_1] = _S(3412, "�¸� �ϼ̳׿�! ���⼭ �׸� �ΰ� ������ �����ðھ��?  �ƴϸ� �ѹ� �� �ؼ� �� ū ��ǰ�� ������ðھ��?");
	m_strNpcTalk[MINIGAME_WIN_2] = _S(3413, "�̹����� �̱�̳׿�. ���⼭ �׸� �νðھ��? �ƴϸ� �ѹ� �� �ؼ� �� ū ��ǰ�� ������ðھ��? ���� �ǿ��� �¸��Ѵٸ�, 3�� ��ǰ�� ���� �� �־��!");
	m_strNpcTalk[MINIGAME_WIN_3] = _S(3414, "�¸��ϼ̱���! ��ξ��� �� ���̳� ���� ���� ���Ҿ��! ���� 3�����̳׿�. ���⼭ �׸� �ΰ� �� �޾ư��ðھ��? �ƴϸ� 4���� ������ðھ��?");
	m_strNpcTalk[MINIGAME_WIN_4] = _S(3415, "�̹����� �¸��� �ϼ̱���! �ٺ����� ��ξ�! �� ���̳� ���� ���ҳ׿�. ���� 4���Դϴ�. ���⼭ �׸� �νðھ��? �ƴϸ� ������� �Դµ� ��¥ ��� ��ǰ���� ������ڳĿ�?  ���� �ǵ� �̱�� 5���̿���!");
	m_strNpcTalk[MINIGAME_WIN_5] = _S(3416, "�� ���׿�! �ִ� �� ���� �� �� �о� ���ǰ���? ���� 5���̿���! �����뿡�� �׸� �νðھ��? �ƴϸ� �ѹ� �� �غ��ðھ��?");
	m_strNpcTalk[MINIGAME_WIN_6] = _S(3417, "�������� ���� ��ξ�, �ϰ� ���� �������� �� �Ǵµ� ��� �Ͻðھ��? ");
	m_strNpcTalk[MINIGAME_WIN_7] = _S(3418, "7���г׿�. �¸��ϼ̽��ϴ�. �̴�� ���ư��� �������� ū�ϳ��ڳ׿�. �ƹ����� �̱�� ���� �� ���׿�. �ٸ� ��������� ���� ���� �̰ܾ� �� �� ���ƿ�. ����, �� �ϰڴٴ� �̾߱⸦ ���� ���� ������? �׸��ΰ� 7�� ��ǰ�� �����ðھ��?");
	m_strNpcTalk[MINIGAME_WIN_8] = _S(3419, "�̹����� ��ξ��� ���� ���ҳ׿�! ������������ ���̽ó׿�!  ��ξ��� �� �� �ִ� ���� ū �����̿���. �����մϴ�.");

	SetNpcTalk(MINIGAME_START);

	// Setup minigame network messages
	if (m_ubEventType == MSG_EVENT_GOMDORI_2007)
	{
		m_ubMsgStart    = MSG_EVENT_GOMDORI_2007_START;
		m_ubMsgSelect   = MSG_EVENT_GOMDORI_2007_SELECT;
		m_ubMsgContinue = MSG_EVENT_GOMDORI_2007_CONTINUE;
		m_ubMsgEnd      = MSG_EVENT_GOMDORI_2007_END;
	}
	else if (m_ubEventType == MSG_EVENT_XMAS_2007)
	{
		m_ubMsgStart    = MSG_EVENT_XMAS_PUZZLE_START_REQ;
		m_ubMsgSelect   = MSG_EVENT_XMAS_PUZZLE_SELECT_REQ;
		m_ubMsgContinue = MSG_EVENT_XMAS_PUZZLE_CONTINUE_REQ;
		m_ubMsgEnd      = MSG_EVENT_XMAS_PUZZLE_END_REQ;
	}
	else
	{
		m_ubEventType   = 0;
		m_ubMsgStart    = 0;
		m_ubMsgSelect   = 0;
		m_ubMsgContinue = 0;
		m_ubMsgEnd      = 0;
	}
	
	pUIManager->RearrangeOrder( UI_MINIGAME, TRUE );
	
	InitData();

}

// ----------------------------------------------------------------------------
// Name : RenderMinigameWinMark()
// Desc :
// ----------------------------------------------------------------------------
void CUIMinigame::SetMinigameWinMark()
{
	// ����� ���
	if( m_whowin == 1 || m_whowin ==-1) return ;

	// ���� ���
	if( m_whowin == 2)
	{
		m_rtSelBtn.SetRect(186, MINIGAME_BUTTON_POS_Y,186+MINIGAME_BUTTON_SIZE, MINIGAME_BUTTON_POS_Y+MINIGAME_BUTTON_SIZE); 
	}
	// �̰��� ���
	else if( m_whowin == 0)
	{
		if(m_userChoice==0)
			m_rtSelBtn.SetRect( 97, MINIGAME_BUTTON_POS_Y + 94, 97+MINIGAME_BUTTON_SIZE, MINIGAME_BUTTON_POS_Y + MINIGAME_BUTTON_SIZE +94);
		else if(m_userChoice==1)
			m_rtSelBtn.SetRect(186, MINIGAME_BUTTON_POS_Y + 94, 186+MINIGAME_BUTTON_SIZE, MINIGAME_BUTTON_POS_Y + MINIGAME_BUTTON_SIZE +94);
		else if(m_userChoice==2)
			m_rtSelBtn.SetRect(275, MINIGAME_BUTTON_POS_Y + 94, 275+MINIGAME_BUTTON_SIZE, MINIGAME_BUTTON_POS_Y + MINIGAME_BUTTON_SIZE +94);
	}
}


// ----------------------------------------------------------------------------
// Name : SetMinigameResult()
// Desc :
// ----------------------------------------------------------------------------
void CUIMinigame::SetMinigameResult(int wincount,int usersel,int npcsel,int whowin)
{
	m_iWinNum		= wincount;
	m_userChoice	= usersel;
	m_npcChoice		= npcsel;
	m_whowin		= whowin;		
}

// ----------------------------------------------------------------------------
// Name : SetNpcTalk()
// Desc :
// ----------------------------------------------------------------------------
void CUIMinigame::SetNpcTalk(int iTalkIdx)
{
	ASSERT(iTalkIdx<MINIGAME_STATE_END);

	m_listNpcTalk.ResetAllStrings();
	CUIManager::getSingleton()->AddStringToList( &m_listNpcTalk ,m_strNpcTalk[iTalkIdx],48);
}

// ----------------------------------------------------------------------------
// Name : RenderNpcSlot()
// Desc :
// ----------------------------------------------------------------------------
void CUIMinigame::RenderNpcSlot(INDEX npcState)
{
	static int tRoll=0;
	static DWORD tTimeOld= (unsigned int(_pTimer->GetLerpedCurrentTick()*1000)); //timeGetTime();
	DWORD tTimeNew;

	switch(npcState)
	{
	case MINIGAME_NPC_NOTHING:
		m_btnNpcChoice[0].Render();
		break;
	case MINIGAME_NPC_READY:
		{
			m_btnNpcChoice[tRoll].Render();

			tTimeNew = (unsigned int(_pTimer->GetLerpedCurrentTick()*1000)); //timeGetTime();

			if( tTimeOld + 300 < tTimeNew)
			{
				tTimeOld = tTimeNew;
				tRoll++;
			}
			if(tRoll>2) tRoll = 0;
		}
		break;
	case MINIGAME_NPC_START:
		{
			FLOAT	fTexWidth = m_ptdAddTexture->GetPixWidth();
			FLOAT	fTexHeight = m_ptdAddTexture->GetPixHeight();
			
			//if(tTimePass==0) tTimePass=timeGetTime();
			tTimeNew = (unsigned int(_pTimer->GetLerpedCurrentTick()*1000)); //timeGetTime();
		
			if( tTimeOld +50< tTimeNew)
			{
				tTimeOld = tTimeNew;
				tRoll+=10;				
			}
			
			if(tRoll>MINIGAME_BUTTON_SIZE*2) tRoll = 0;
			
			m_btnNpcChoiceMove.SetUV( UBS_IDLE, tRoll, 137,tRoll+MINIGAME_BUTTON_SIZE, 184, fTexWidth, fTexHeight );
			m_btnNpcChoiceMove.Render();	
			
			// 2�ʰ� ��� ������ ��� ���
			if ((m_dwTimePass + 2000 < tTimeNew) && m_whowin >=0)
			{
				m_npcState = MINIGAME_NPC_RESULT;
				m_dwTimePass =0;
			}
		}
		break;
	case MINIGAME_NPC_RESULT:
		{

			m_btnNpcChoice[m_npcChoice].Render();

			if( !m_bTalkOnce )
			{
				if (m_whowin == 2) // ���� ��
				{			
					SetButtonState(BTN_STATE_DEFEAT);
					SetNpcTalk(MINIGAME_LOSE);
				}
				else if (m_whowin == 1) // ����� ��
				{
					SetButtonState(BTN_STATE_WIN_DRAW);	
					SetNpcTalk(MINIGAME_DRAW);
				}
				else if (m_whowin == 0) // �̰��� ��
				{
					if (m_iWinNum >= 8)
					{
						// 8�� �̻��� �� �׸��ϰ� ��ǰ�ޱ⸸ Ȱ��ȭ
						SetButtonState(0x81);	
					}
					else 
					{
						SetButtonState(BTN_STATE_WIN_DRAW);	
					}
					
					SetNpcTalk(m_iWinNum + 2);
				}	

				m_bTalkOnce = TRUE;
			}
				
		}

		tRoll =0;				
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIMinigame::Render()
{
	// Check distance
/*	FLOAT	fDiffX = _pNetwork->MyCharacterInfo.x - m_fNpcX;
	FLOAT	fDiffZ = _pNetwork->MyCharacterInfo.z - m_fNpcZ;
	if( fDiffX * fDiffX + fDiffZ * fDiffZ > UI_VALID_SQRDIST )
		Close();
*/
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();
	
	// Set Minigame texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );
	// Add render regions
	// Background
	m_bxTitle.Render(m_nPosX,m_nPosY);
	m_bxBack1.Render(m_nPosX,m_nPosY);
	m_bxBack1_1.Render(m_nPosX,m_nPosY);
//	m_bxBack2.Render(m_nPosX,m_nPosY,0xFFFFFF80);
	m_bxNpcTalk.Render(m_nPosX,m_nPosY);
	m_bxWinNum.Render(m_nPosX,m_nPosY);
	
	// Render buttons
	m_btnReceiveItem.Render();
	m_btnStartGame.Render();
	m_btnKeepGoing.Render();

	// Npc Talk
	m_listNpcTalk.Render();

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	pDrawPort->InitTextureData( m_ptdAddTexture );

	// Render Buttons
	m_btnScissors.Render();
	m_btnStone.Render();	
	m_btnPaper.Render();

	RenderNpcSlot(m_npcState);

//	m_btnNpcChoice[m_npcChoice].Render();
	// Render Select Mark 
//	if(m_npcState==MINIGAME_NPC_START)
	{
		pDrawPort->AddTexture(
		m_nPosX+m_rtSelBtn2.Left,m_nPosY+m_rtSelBtn2.Top,m_nPosX+m_rtSelBtn2.Right,m_nPosY+m_rtSelBtn2.Bottom,
		m_rcSelBtn2.U0,m_rcSelBtn2.V0,m_rcSelBtn2.U1,m_rcSelBtn2.V1,
		0xFFFFFFFF);
	}

	// Reder Win Mark
	if(m_whowin != 1 && m_npcState==MINIGAME_NPC_RESULT)
	{
		SetMinigameWinMark();

		pDrawPort->AddTexture(
		m_nPosX+m_rtSelBtn.Left,m_nPosY+m_rtSelBtn.Top,m_nPosX+m_rtSelBtn.Right,m_nPosY+m_rtSelBtn.Bottom,
		m_rcSelBtn.U0,m_rcSelBtn.V0,m_rcSelBtn.U1,m_rcSelBtn.V1,
		0xFFFFFFFF);
	}

	// Render VS mark
	pDrawPort->AddTexture(
		m_nPosX+m_rtVS.Left,m_nPosY+m_rtVS.Top,
		m_nPosX+m_rtVS.Right,m_nPosY+m_rtVS.Bottom,
		m_rcVS.U0,m_rcVS.V0,m_rcVS.U1,m_rcVS.V1,
		0xFFFFFFFF);

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	// Render Text
	CTString tStr;
	tStr=_S(3455, "�¼�" );
	pDrawPort->PutTextEx(tStr, m_nPosX + 351, m_nPosY + 48, 0xFFFFFFFF );
	
	if(m_npcState == MINIGAME_NPC_RESULT) 
		m_iWinNumOld = m_iWinNum;

	tStr.PrintF("%d", m_iWinNumOld);
	pDrawPort->PutTextEx( tStr, m_nPosX + 387, m_nPosY + 60, 0xFFFFFFFF );
	tStr=_S(3456, "���������� ����" );
	pDrawPort->PutTextEx(tStr, m_nPosX +25, m_nPosY + 5, 0xFFFFFFFF );

	// Flush all render text queue
	pDrawPort->EndTextEx();
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIMinigame::MouseMessage( MSG *pMsg )
{
	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Title bar
	static BOOL bTitleBarClick = FALSE;
	UIRect tTitle;
	tTitle.Left = m_bxTitle.m_rcBoxRT.Left;
	tTitle.Right = m_bxTitle.m_rcBoxRT.Right;
	tTitle.Top = m_bxTitle.m_rcBoxRT.Top;
	tTitle.Bottom = m_bxTitle.m_rcBoxRT.Bottom;

	// Mouse message
	switch(pMsg->message)
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();

			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}

			if( m_btnReceiveItem.MouseMessage( pMsg ) != WMSG_FAIL ||
				m_btnStartGame.MouseMessage( pMsg ) != WMSG_FAIL ||
				m_btnKeepGoing.MouseMessage( pMsg ) != WMSG_FAIL ||
				m_btnScissors.MouseMessage( pMsg ) != WMSG_FAIL ||
				m_btnStone.MouseMessage( pMsg ) != WMSG_FAIL ||
				m_btnPaper.MouseMessage( pMsg ) != WMSG_FAIL 
			//	m_btnNpcChoice[0].MouseMessage( pMsg ) != WMSG_FAIL ||	
			//	m_btnNpcChoice[1].MouseMessage( pMsg ) != WMSG_FAIL ||
			//	m_btnNpcChoice[2].MouseMessage( pMsg ) != WMSG_FAIL 
			)	return WMSG_SUCCESS;
			else if( m_listNpcTalk.MouseMessage( pMsg ) != WMSG_FAIL ) 
			{
				// TODO : NOTHING

			}			

		}
		break;

	case WM_LBUTTONDOWN:
		{
		/*	if( !IsInside( nX, nY ) )	//���� ����� ���,
			{
				Close();
				break;
			}*/

			nOldX = nX;		nOldY = nY;
			
			m_btnReceiveItem.MouseMessage( pMsg ); 
			m_btnStartGame.MouseMessage( pMsg );
			m_btnKeepGoing.MouseMessage( pMsg );
			if( m_btnScissors.MouseMessage( pMsg ) != WMSG_FAIL )				
			{
				m_btnScissors.SetSize(MINIGAME_BUTTON_SIZE*1.1,MINIGAME_BUTTON_SIZE*1.1);
			}
			else if( m_btnStone.MouseMessage( pMsg ) != WMSG_FAIL )				
			{
				m_btnStone.SetSize(MINIGAME_BUTTON_SIZE*1.1,MINIGAME_BUTTON_SIZE*1.1);				
			}
			else if( m_btnPaper.MouseMessage( pMsg ) != WMSG_FAIL )				
			{
				m_btnPaper.SetSize(MINIGAME_BUTTON_SIZE*1.1,MINIGAME_BUTTON_SIZE*1.1);			
			}			
			else if( m_listNpcTalk.MouseMessage( pMsg ) != WMSG_FAIL ) 
			{
				// TODO : NOTHING

			}			
			else if( IsInsideRect( nX, nY, tTitle ) )
			{
				bTitleBarClick = TRUE;
			}			

			CUIManager::getSingleton()->RearrangeOrder( UI_MINIGAME, TRUE );
			return WMSG_SUCCESS;
		}
		break;

	case WM_LBUTTONUP:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			// If holding button doesn't exist
			if (pUIManager->GetDragIcon() == NULL)
			{
				bTitleBarClick = FALSE;

				if( !IsFocused() )
					return WMSG_FAIL;

				if( m_btnReceiveItem.MouseMessage( pMsg ) != WMSG_FAIL)
				{
					// TODO : �׸��ϰ� ������ �ޱ�
					if (m_ubEventType)
						_pNetwork->SendMinigameDefaultMessage(m_ubEventType, m_ubMsgEnd);
				}
				if( m_btnStartGame.MouseMessage( pMsg ) != WMSG_FAIL)
				{
					// ���� �����ϱ� 
					if (m_ubEventType)
						_pNetwork->SendMinigameDefaultMessage(m_ubEventType, m_ubMsgStart);
					// �¼� �ʱ�ȭ
					m_iWinNum = 0;
					// Win Mark Disable
					m_whowin = -1;
					// set npc state
					m_npcState = MINIGAME_NPC_READY;
					m_bTalkOnce = FALSE;

				}
				if( m_btnKeepGoing.MouseMessage( pMsg ) != WMSG_FAIL)
				{
					// TODO : ��� �ϱ�
					if (m_ubEventType)
						_pNetwork->SendMinigameDefaultMessage(m_ubEventType, m_ubMsgContinue);
					// Win Mark Disable
					m_whowin = -1;
					// set npc state
					m_npcState = MINIGAME_NPC_READY;
					m_bTalkOnce = FALSE;
				}
				else if( m_btnScissors.MouseMessage( pMsg ) == WMSG_COMMAND )				
				{
					m_btnScissors.SetSize(MINIGAME_BUTTON_SIZE,MINIGAME_BUTTON_SIZE);
					// ��� ��Ȱ��ȭ
					SetButtonState(0x80);
					if (m_ubEventType)
						_pNetwork->SendMinigameSelect(m_ubEventType, m_ubMsgSelect, 0);
					// set npc state
					m_npcState = MINIGAME_NPC_START;
					m_dwTimePass = (unsigned int(_pTimer->GetLerpedCurrentTick()*1000));
					
				}
				else if( m_btnStone.MouseMessage( pMsg ) == WMSG_COMMAND )				
				{
					m_btnStone.SetSize(MINIGAME_BUTTON_SIZE,MINIGAME_BUTTON_SIZE);
					// ��� ��Ȱ��ȭ 
					SetButtonState(0x80);
					if (m_ubEventType)
						_pNetwork->SendMinigameSelect(m_ubEventType, m_ubMsgSelect, 1);
					// set npc state
					m_npcState = MINIGAME_NPC_START;
					m_dwTimePass = (unsigned int(_pTimer->GetLerpedCurrentTick()*1000));
				}
				else if( m_btnPaper.MouseMessage( pMsg ) == WMSG_COMMAND )
				{
					m_btnPaper.SetSize(MINIGAME_BUTTON_SIZE,MINIGAME_BUTTON_SIZE);
					// ��� ��Ȱ��ȭ
					SetButtonState(0x80);
					if (m_ubEventType)
						_pNetwork->SendMinigameSelect(m_ubEventType, m_ubMsgSelect, 2);
					// set npc state
					m_npcState = MINIGAME_NPC_START;
					m_dwTimePass = (unsigned int(_pTimer->GetLerpedCurrentTick()*1000));
				}
				else if( m_listNpcTalk.MouseMessage( pMsg ) != WMSG_FAIL ) 
				{
				// TODO : NOTHING

				}			
				return WMSG_SUCCESS;
			}
			// If holding button exists
			else
			{
				if( IsInside( nX, nY ) )
				{
					// Reset holding button
					pUIManager->ResetHoldBtn();

					return WMSG_SUCCESS;
				}
			}
		}
		break;
	case WM_MOUSEWHEEL:
		{
			if( m_listNpcTalk.MouseMessage( pMsg ) != WMSG_FAIL ) 
			{
			// TODO : NOTHING
			}			
			return WMSG_SUCCESS;
		}
		break;
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : Close()
// Desc :
// ----------------------------------------------------------------------------
void CUIMinigame::Close()
{
	InitData();
	CUIManager::getSingleton()->RearrangeOrder( UI_MINIGAME, FALSE );
}

