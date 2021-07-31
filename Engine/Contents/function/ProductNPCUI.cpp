#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "ProductNPCUI.h"
#include <Engine/Interface/UIProduct.h>
#include <Common/Packet/ptype_old_do_skill.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Info/MyInfo.h>
#include "ProductNPCCmd.h"
#include <Engine/Contents/function/Product2UI.h>

static int	_nMsgBoxLineHeight = 0;
//*****************************
// CUIProductNPC
//*******************************
CUIProductNPC::CUIProductNPC()
	: m_pTreeTemp(NULL)
	, m_pTree(NULL)
	, m_pTreePos(NULL)
	, m_pTreeItemTemp(NULL)
	, m_pScroll(NULL)
	, m_pTxtNas(NULL)
	, m_pTxtSP(NULL)
	, m_nfactory_idx(-1)
	, m_nsel_sub(-1)
	, m_nsel_idx(-1)
	, m_colPrev(DEF_UI_COLOR_WHITE)
{
	setInherit(false);

	Reset();
	m_nNpcVirIdx = -1;
}

CUIProductNPC::~CUIProductNPC()
{
	Reset();

	Destroy();
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

	set_data();

	// If this is already exist
	Hide(FALSE);
	if( IsVisible() == TRUE )
		return;

	pUIManager->RearrangeOrder( UI_PRODUCTNPC, TRUE );
}

void CUIProductNPC::CloseProductNPC()
{
	Reset();
	m_nNpcVirIdx = -1;
	CUIManager::getSingleton()->RearrangeOrder( UI_PRODUCTNPC, FALSE );
	Hide(TRUE);
}

void CUIProductNPC::Reset()
{
	m_nSealNum = -1;
	m_nfactory_idx = -1;
	m_nsel_sub = -1;
	m_nsel_idx = -1;

	if (m_pTxtNas != NULL)
		m_pTxtNas->SetText(CTString(""));

	if (m_pTxtSP != NULL)
		m_pTxtSP->SetText(CTString(""));
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
					CTString strTemp, strCount;
					strCount.PrintF("%I64d", nNeedCount);
					pUIManager->InsertCommaToString(strCount);
					strTemp.PrintF(_S(904, "�ʿ� ���� : %s" ), strCount);

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
					CTString strTemp, strCount;
					strCount.PrintF("%I64d", nNeedCount);
					pUIManager->InsertCommaToString(strCount);
					strTemp.PrintF(_S(904, "�ʿ� ���� : %s" ), strCount);

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

void CUIProductNPC::initialize()
{
#ifndef		WORLD_EDITOR

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

	m_pTreeTemp = (CUITree*)findUI("tree_list");

	m_pTreePos = findUI("ui_pos");
	m_pTreeItemTemp = (CUITreeItem*)findUI("ti_sub");
	m_pScroll = (CUIScrollBar*)findUI("scroll");
	m_pTxtNas = (CUIText*)findUI("txt_nas");
	m_pTxtSP = (CUIText*)findUI("txt_sp");

	CUIButton* pBtn;

	pBtn = (CUIButton*)findUI("btn_close");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&CUIProductNPC::CloseProductNPC, this));

	pBtn = (CUIButton*)findUI("btn_cancel");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&CUIProductNPC::CloseProductNPC, this));

	pBtn = (CUIButton*)findUI("btn_ok");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&CUIProductNPC::_callback_ok, this));


#endif		// WORLD_EDITOR
}

bool comp(CMakeItemData* a, CMakeItemData* b)
{
	if (a == NULL || b == NULL)
		return false;

	if (a->GetFactoryIndex() < b->GetFactoryIndex())
		return true;

	return false;
}

const int sub_type_string_idx[] = {
	2544, // "����"
	4273, // "����"
	4275, // "�尩"
	4274, // "����"
	4276, // "�Ź�"
	4277, // "����"
	4613, // "����"
	1039, // "����"
	1040, // "������"
	1042, // "����"
	1046, // "�̵���"
	1047, // "�����ϵ�"
	2307, // "����"
	4614, // "ȥ"
	1038, // "��絵"
	1041, // "���"
	1043, // "��������"
	1044, // "Ȱ"
	1045, // "�ܰ�"
	2306, // "���̵�"
	4266, // "��ȸ��"
	4268, // "����"
	4267, // "��Ÿ"
};

bool CUIProductNPC::set_data()
{
	if (m_pTreeTemp == NULL || m_pTreePos == NULL ||
		m_pTreeItemTemp == NULL)
		return false;

	if (m_nSealType < 0)
		return false;

	int nSealtypenum = m_nSealType - SEALTYPE_START_INDEX;
	//nSealSubCount�� List ������ ���� subtype ������ ������
	int nSealSubCount = SealTypeCount[nSealtypenum+1] - SealTypeCount[nSealtypenum];	

	MyInfo* pMyinfo = MY_INFO();

	int		i;
	//Seal type�� ���� SubŸ�Կ� �°� ���� ��Ŵ
	for( i = 0; i < _pNetwork->wo_iNomOfMakeItemData; ++i )
	{
		CMakeItemData* temMakeItem =  &_pNetwork->wo_aMakeItemData[i];

		if (temMakeItem != NULL && temMakeItem->GetMakeItemType() == m_nSealType)
		{
			{
				// ����� ���� ��ų ����Ʈ ����
				if (pMyinfo->find_makeitem(temMakeItem->GetFactoryIndex()) == false)
				{
					m_vecList.push_back(temMakeItem);
				}

			}

		}
	}

	std::sort(m_vecList.begin(), m_vecList.end(), comp);

	// ���� Ʈ���� �����Ͱ� �ִٸ� �����Ѵ�.
	if (m_pTree != NULL)
	{
		m_pTreePos->deleteChild(m_pTree);
		m_pTree = NULL;
	}

	// tree root ó��
	{
		m_pTree = (CUITree*)m_pTreeTemp->Clone();
		m_pTree->setRoot();
		m_pTreePos->addChild(m_pTree);

		m_pScroll->Hide(FALSE);
		m_pTree->setScroll(m_pScroll);
	}

	CUITree*		ptree_clone = NULL;
	CUITree*		ptree_child = NULL;
	CUIBase*		pclone = NULL;
	CUIText*		pText = NULL;

	int temStartpos = SealTypeCount[nSealtypenum];
	CTString strTemp;
	UQUAD nSealExp = MY_INFO()->GetSkillExp(m_nSealType);

	vec_iter endIter = m_vecList.end();
	vec_iter startIter;

	int		idx;
	// List�� SubType��� ������
	for( i = 0; i < nSealSubCount; i++)
	{
		strTemp = _S(sub_type_string_idx[temStartpos + i], "");

		// ���� �޴� �߰�
		{
			ptree_clone = (CUITree*)m_pTreeTemp->Clone();

			pclone = m_pTreeItemTemp->Clone();
			pclone->Hide(FALSE);
			pText = (CUIText*)pclone->findUI("txt_");
			pText->SetText(strTemp);

			ptree_clone->addChild(pclone);		// Child �� ���� �� 			
		}

		startIter = m_vecList.begin();

		for(; startIter != endIter; ++startIter )
		{
			CMakeItemData* tempdata = (*startIter);
			COLOR colTemp = 0xC0C0C0FF;

			if (SealSubType[temStartpos+i] == tempdata->GetMakeItemSubType())
			{
				strTemp = tempdata->GetName();
				SQUAD temResult = nSealExp - tempdata->GetNeedExp();

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

				ptree_child = (CUITree*)m_pTreeTemp->Clone();
				ptree_child->SetHeight(20);			

				pclone = m_pTreeItemTemp->Clone();
				pclone->Hide(FALSE);
				pText = (CUIText*)pclone->findUI("txt_");
				pText->SetText(strTemp);
				pText->setFontColor(colTemp);

				ptree_child->addChild(pclone);

				ptree_clone->addTree(ptree_child, false);
				idx = ptree_clone->getChildTreeCount() - 1;

				// cmd ���
				CmdProductNPCSelect<CUIProductNPC>* pcmd = new CmdProductNPCSelect<CUIProductNPC>;
				pcmd->setData(this, i, idx, tempdata->GetFactoryIndex(), colTemp);
				ptree_child->SetCommandUp(pcmd);

				CmdProductNPCOver<CUIProductNPC>* pcmd_over = new CmdProductNPCOver<CUIProductNPC>;
				pcmd_over->setData(this, i, idx, true, DEF_UI_COLOR_OVER);
				ptree_child->SetCommandEnter(pcmd_over);

				CmdProductNPCOver<CUIProductNPC>* pcmd_leave = new CmdProductNPCOver<CUIProductNPC>;
				pcmd_leave->setData(this, i, idx, false, colTemp);
				ptree_child->SetCommandLeave(pcmd_leave);
			}
		}

		m_pTree->addTree(ptree_clone);		// Tree �� ����ؾ� ���̰� ������.
		ptree_clone->setUncollapse(TRUE);
		ptree_clone->updateTree();
	}

	m_vecList.clear();

	m_pTree->SetScrollPos(0);
	m_pTree->updateTree();

	return true;
}

void CUIProductNPC::_callback_ok()
{
	if (m_nfactory_idx < 0)
		return;

	SendFactoryLearn(m_nfactory_idx);
}

void CUIProductNPC::callback_select( int nsub, int idx, int fac_idx, COLOR col )
{
	CUITree* ptree;

	// ���� ���� �����ϸ� ����
	if (m_nsel_sub == nsub && m_nsel_idx == idx)
		return;

	// ���� ������ �ǵ���.
	if (m_nsel_sub >= 0 && m_nsel_idx >= 0)
	{
		ptree = getTree(m_nsel_sub, m_nsel_idx);

		if (ptree != NULL)
		{
			set_color(ptree, m_colPrev);
			CUIBase* pimg = ptree->findUI("img_select");

			if (pimg != NULL)
				pimg->Hide(TRUE);
		}
	}

	ptree = getTree(nsub, idx);

	if (ptree == NULL)
		return;

	// select image
	{
		CUIBase* pimg = ptree->findUI("img_select");

		if (pimg != NULL)
			pimg->Hide(FALSE);
	}

	m_nsel_sub = nsub;
	m_nsel_idx = idx;
	m_nfactory_idx = fac_idx;
	m_colPrev = col;

	CMakeItemData& Itemdata = _pNetwork->GetMakeItemData(m_nfactory_idx);

	if (Itemdata.GetFactoryIndex() > 0)
	{
		CTString str;
		CUIManager* pUIMgr = UIMGR();

		if (m_pTxtNas != NULL)
		{
			str.PrintF("%I64d", Itemdata.GetNeedNas());
			pUIMgr->InsertCommaToString(str);
			m_pTxtNas->SetText(str);
		}

		if (m_pTxtSP != NULL)
		{
			str.PrintF("%I64d", Itemdata.GetNeedExp());
			pUIMgr->InsertCommaToString(str);
			m_pTxtSP->SetText(str);
		}		
	}
}

void CUIProductNPC::callback_over( int nsub, int idx, bool bover, COLOR col )
{
	if (m_nsel_sub == nsub && m_nsel_idx == idx)
		return;

	CUITree* ptree = getTree(nsub, idx);

	if (bover == true)
	{
		set_color(ptree, DEF_UI_COLOR_OVER);
	}
	else
	{
		set_color(ptree, col);
	}
}

CUITree* CUIProductNPC::getTree( int nsub, int idx )
{
	CUITree* ret = NULL;

	if (m_pTree == NULL)
		return ret;

	CUITree* psub = m_pTree->getChildTree(nsub);

	if (psub != NULL)
	{
		ret = psub->getChildTree(idx);
	}

	return ret;
}

void CUIProductNPC::set_color( CUITree* pTree, COLOR col )
{
	if (pTree == NULL)
		return;

	CUIText* ptxt = (CUIText*)pTree->findUI("txt_");

	if (ptxt == NULL)
		return;

	ptxt->setFontColor(col);
}
