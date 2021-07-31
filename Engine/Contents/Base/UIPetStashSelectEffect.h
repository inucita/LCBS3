#ifndef UI_PETSTASH_SELECT_EFFECT_H_
#define UI_PETSTASH_SELECT_EFFECT_H_

enum eSoundType
{
	SOUND_TYPE_MOVE,
	SOUND_TYPE_SELECT
};

class ENGINE_API CUIPetStashSelectEffect : public CUIWindow
{
public:
	CUIPetStashSelectEffect();
	~CUIPetStashSelectEffect();

	// Ani
	void	StartAni();
	void	UpdateAni();
	void	EndAni();

	void	ClosePetStashSelectEffect();
	void	OpenPetStashSelectEffect();

	void	initialize();

	void	OnUpdate( float fDeltaTime, ULONG ElapsedTime );

	BOOL	CloseWindowByEsc()	{ ClosePetStashSelectEffect(); return TRUE; }

	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
private:
	// �� ����
	void	SelectModel();
	// �ܺο��� �о�鿩 ��� �ʱ�ȭ
	void	InitPSAniTIme();
	void	_ResetAni();
	void	_PlaySound(eSoundType type);

	UIRect			m_rcOriginPos;
	CUIButton*		m_pbtnOk;
	CUIImage*		m_imgSelect;
	CUISpriteAni*	m_aniLeft;
	CUISpriteAni*	m_aniRight;
	CUISpriteAni*	m_aniMid;

	DWORD			m_dwAniStartTime;
	DWORD			m_dwAniMaitionTime;
	DWORD			m_dwDelayTime;
	DWORD			m_dwDelay;
	DWORD			m_dwSlowTime;
	BOOL			m_bAni;
	BOOL			m_bSelect;
	int				m_nMaxFrame;
	int				m_nCurFrame;
};
#endif // UI_PETSTASH_SELECT_EFFECT_H_