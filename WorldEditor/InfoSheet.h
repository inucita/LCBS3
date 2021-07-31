// InfoSheet.h : header file
//
#ifndef INFOSHEET_H
#define INFOSHEET_H 1

/////////////////////////////////////////////////////////////////////////////
// CInfoSheet

class CInfoFrame;

enum ModeID {
  INFO_MODE_ALL = 0,
  INFO_MODE_GLOBAL,
  INFO_MODE_POSITION,
  INFO_MODE_PRIMITIVE,
  INFO_MODE_POLYGON,
  INFO_MODE_SECTOR,
  INFO_MODE_TERRAIN,
};

class CInfoSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CInfoSheet)

// Construction
public:
	CInfoSheet(CWnd* pWndParent);
	BOOL OnIdle(LONG lCount);
  // functions for dealing with pages
  void DeleteAllPages();
  void SetInfoModeGlobal(void);
  void SetInfoModePosition(void);
  void SetInfoModePrimitive(void);
  void SetInfoModePolygon(void);
  void SetInfoModeSector(void);
  void SetInfoModeTerrain(void);
  void SoftSetActivePage( INDEX iActivePage);
// Attributes
public:
	ModeID m_ModeID;
  CDlgPgGlobal m_PgGlobal;
  CDlgPgTerrain m_PgTerrain;
  CDlgPgPosition m_PgPosition;
  CDlgPgPrimitive m_PgPrimitive;
  CDlgPgRenderingStatistics m_PgRenderingStatistics;
  CDlgPgPolygon m_PgPolygon;
  CDlgPgShadow m_PgShadow;
  CDlgPgSector m_PgSector;
  CDlgPgTexture m_PgTexture;
  CDlgPgAttribute	m_PgAttribute;		// yjpark

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInfoSheet)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CInfoSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CInfoSheet)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif // INFOSHEET_H
