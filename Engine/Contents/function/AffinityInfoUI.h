#ifndef __AFFINITYINFO_UI_H__
#define __AFFINITYINFO_UI_H__

#define AFFINITY_REWARD_MAX 7 // ģȭ�� ����2 ���� ��� �ִ� ���� [2/13/2013 Ranma]

class CAffinityInfoUI : public CUIWindow
{
public:
	CAffinityInfoUI();
	~CAffinityInfoUI();

	enum __tagAffinityInfo_UI
	{
		// ģȭ�� ����2 ��� ũ�� ���� [2/5/2013 Ranma]
		NPCAFFINITYINFO_WIDTH		= 512,
		NPCAFFINITYINFO_HEIGHT		= 421,
		NPCAFFINITYINFO_ITEMCOUNT	= 6
	};

	// ģȭ�� ����2 ����/������/����Ʈ [2/12/2013 Ranma]
	enum _eAffinityTab_UI
	{
		UI_TAB_ITEM = 0,
		UI_TAB_QUEST = 1,
		UI_TAB_MONSTER = 2,
		UI_TAB_END
	};

	void initialize();
	void OpenAffinityInfo();
	void CloseAffinityInfo();
	// ģȭ�� ����2 ���� ���� �ޱ� [2/13/2013 Ranma]
	void RecvAffinity_RewardInfo(CNetworkMessage* istr);
	void ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void AffinityReset();

	void ChangeEnterColor(bool bEnter, int idx);
	void ChangeSelectColor(int idx);
	void ChangeRewardPage(bool bPrev);
	BOOL CloseWindowByEsc() { CloseAffinityInfo(); return TRUE;	}

protected:
	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);

private:
	typedef std::vector< std::pair<int,bool> >	AffintyList_vec;
	AffintyList_vec	m_AffinityList;

	CUIList* m_pListNpc;
	CUITab* m_pTabDonationList;
	CUITabPage* m_pTPDonation[UI_TAB_END];
	CUIList* m_pListDonation[UI_TAB_END];
	CUITextBox* m_pTBNpcDesc;
	CUIText* m_pTextNpcName;
	CUIText* m_pTextAffinityPt;
	CUITextBox* m_pTBAdminNpcName;
	CUIArray* m_pArrayReward;
	CUIImage* m_pImgMap;
	CUIText* m_pTextAdminPos;
	CUIBase* m_pDrag;
	CUIText* m_pTextPage;
	int m_nOldSelectIdx;

	bool m_bDrag;
	int m_nOldX;
	int m_nOldY;

	int m_nRewardCurPage;
	int m_nRewardNextPoint;
	int m_nRewardNpcIndex;
	SLONG m_slRewardCheckSum;

	// [2013/02/12] sykim70 �̴ϸ� �߰�
	int m_nAffinityIndex;
	int m_nAdminNPCIndex;
	int m_nPositionNPCIndex;
	int m_nPositionZoneNo;

	// [2013/02/13] sykim70 ������ NPC ���� ǥ��
	CTextureData *m_ptdSignalTexture;

	// not used func.
	void _toggleVisible();
	// return UI button vector pointer.
	const ULONG _getUIType() const;
	// if rendering type is affinity, setting data from affinity info.
	void _setAffinityData();
	void _SetAffinityListData();
	// ģȭ�� ����2 �� ���� [2/12/2013 Ranma]
	void _SetAffinityTab(int affinityIdx);

	// [2013/02/12] sykim70 �̴ϸ� �߰�
	void RenderMiniMap();
	void AddDonationList(_eAffinityTab_UI tab
		, eUIBTN_ETC_TYPE btn_type
		, int idx
		, int point
		, CTString& str
		, COLOR col = DEF_UI_COLOR_WHITE);
	void SetDonationMonsterList(CAffinityData::_AffinityMonsterInfo* pData);

	void ClearDonationList();
	void SetMiniMap();
	void OpenMap();
	void SetRewardList(int nPage = 0);
	void SetPage(int nCur, int nMax);
};

#endif // __AFFINITYINFO_UI_H__