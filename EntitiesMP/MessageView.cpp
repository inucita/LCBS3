/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#line 2 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"

#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Contents/function/UIPortalNew.h>
#include <Engine/Interface/UIAutoHelp.h>

#include <MessageView.h>
#include <MessageView_tables.h>
void CMessageView::SetDefaultProperties(void) {
	m_strName = "MessageView";
	m_strDescription = "";
	m_fnmMessage = CTString("");
	m_fDistance = 2.0f;
	m_bActive = TRUE ;
	m_penNext = NULL;
	eViewType = MSGVIEW_CUSTOM ;
	m_bShowOnce = FALSE ;
	m_iStringIDContent1 = -1;
	m_iStringIDContent2 = -1;
	m_iStringIDContent3 = -1;
	m_iStringIDContent4 = -1;
	m_iStringIDContent5 = -1;
	m_iStringIDChoice1 = -1;
	m_iStringIDChoice2 = -1;
	m_iStringIDChoice3 = -1;
	m_iStringIDChoice4 = -1;
	m_iStringIDChoice5 = -1;
	m_strTitle = "Custom Message Box";
	m_iStringTitleID = -1;
	m_strStringTitle = "";
	m_strStringIDContent1 = "";
	m_strStringIDContent2 = "";
	m_strStringIDContent3 = "";
	m_strStringIDContent4 = "";
	m_strStringIDContent5 = "";
	m_strStringIDChoice1 = "";
	m_strStringIDChoice2 = "";
	m_strStringIDChoice3 = "";
	m_strStringIDChoice4 = "";
	m_strStringIDChoice5 = "";
	m_iZone = -1;
	m_iExtra = -1;
	CRationalEntity::SetDefaultProperties();
}

#line 73 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
void CMessageView::OpenMessageView() 
#line 74 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
{
#line 75 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
CUIManager  * pUIManager  = SE_Get_UIManagerPtr  ();
#line 77 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
if(eViewType  == MSGVIEW_CUSTOM )
#line 78 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
{
#line 79 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
if(! pUIManager  -> DoesMessageBoxLExist  (MSGLCMD_STORY_INTRO ))
#line 80 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
{
#line 82 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
if(m_iStringTitleID  != - 1)
#line 83 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
{
#line 85 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
pUIManager  -> CreateMessageBoxL  (_S  (m_iStringTitleID  , "") , UI_NONE  , MSGLCMD_STORY_INTRO );
#line 88 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
if(m_iStringIDContent1  != - 1)
#line 89 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
{
#line 90 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
pUIManager  -> AddMessageBoxLString  (MSGLCMD_STORY_INTRO  , TRUE  , _S  (m_iStringIDContent1  , ""));
#line 91 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
}
#line 93 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
if(m_iStringIDContent2  != - 1)
#line 94 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
{
#line 95 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
pUIManager  -> AddMessageBoxLString  (MSGLCMD_STORY_INTRO  , TRUE  , _S  (m_iStringIDContent2  , ""));
#line 96 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
}
#line 98 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
if(m_iStringIDContent3  != - 1)
#line 99 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
{
#line 100 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
pUIManager  -> AddMessageBoxLString  (MSGLCMD_STORY_INTRO  , TRUE  , _S  (m_iStringIDContent3  , ""));
#line 101 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
}
#line 103 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
if(m_iStringIDContent4  != - 1)
#line 104 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
{
#line 105 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
pUIManager  -> AddMessageBoxLString  (MSGLCMD_STORY_INTRO  , TRUE  , _S  (m_iStringIDContent4  , ""));
#line 106 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
}
#line 108 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
if(m_iStringIDContent5  != - 1)
#line 109 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
{
#line 110 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
pUIManager  -> AddMessageBoxLString  (MSGLCMD_STORY_INTRO  , TRUE  , _S  (m_iStringIDContent5  , ""));
#line 111 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
}
#line 114 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
if(m_iStringIDChoice1  != - 1)
#line 115 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
{
#line 116 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
pUIManager  -> AddMessageBoxLString  (MSGLCMD_STORY_INTRO  , FALSE  , _S  (m_iStringIDChoice1  , ""));
#line 117 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
}
#line 119 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
if(m_iStringIDChoice2  != - 1)
#line 120 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
{
#line 121 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
pUIManager  -> AddMessageBoxLString  (MSGLCMD_STORY_INTRO  , FALSE  , _S  (m_iStringIDChoice2  , ""));
#line 122 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
}
#line 124 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
if(m_iStringIDChoice3  != - 1)
#line 125 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
{
#line 126 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
pUIManager  -> AddMessageBoxLString  (MSGLCMD_STORY_INTRO  , FALSE  , _S  (m_iStringIDChoice3  , ""));
#line 127 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
}
#line 129 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
if(m_iStringIDChoice4  != - 1)
#line 130 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
{
#line 131 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
pUIManager  -> AddMessageBoxLString  (MSGLCMD_STORY_INTRO  , FALSE  , _S  (m_iStringIDChoice4  , ""));
#line 132 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
}
#line 134 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
if(m_iStringIDChoice5  != - 1)
#line 135 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
{
#line 136 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
pUIManager  -> AddMessageBoxLString  (MSGLCMD_STORY_INTRO  , FALSE  , _S  (m_iStringIDChoice5  , ""));
#line 137 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
}
#line 139 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
if(m_bShowOnce )
#line 140 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
{
#line 142 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
if(m_bActive )
#line 143 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
{
#line 144 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
m_bActive  = FALSE ;
#line 145 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
}
#line 146 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
}
#line 147 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
}
#line 148 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
}
#line 149 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
}
#line 150 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
else if(eViewType  == MSGVIEW_STRING )
#line 151 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
{
#line 152 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
if(! pUIManager  -> DoesMessageBoxLExist  (MSGLCMD_STORY_INTRO ))
#line 153 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
{
#line 155 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
if(m_strStringTitle  . Length  () > 0)
#line 156 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
{
#line 158 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
pUIManager  -> CreateMessageBoxL  (m_strStringTitle  , UI_NONE  , MSGLCMD_STORY_INTRO );
#line 161 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
if(m_strStringIDContent1  . Length  () > 0)
#line 162 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
{
#line 163 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
pUIManager  -> AddMessageBoxLString  (MSGLCMD_STORY_INTRO  , TRUE  , m_strStringIDContent1 );
#line 164 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
}
#line 166 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
if(m_strStringIDContent2  . Length  () > 0)
#line 167 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
{
#line 168 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
pUIManager  -> AddMessageBoxLString  (MSGLCMD_STORY_INTRO  , TRUE  , m_strStringIDContent2 );
#line 169 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
}
#line 171 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
if(m_strStringIDContent3  . Length  () > 0)
#line 172 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
{
#line 173 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
pUIManager  -> AddMessageBoxLString  (MSGLCMD_STORY_INTRO  , TRUE  , m_strStringIDContent3 );
#line 174 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
}
#line 176 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
if(m_strStringIDContent4  . Length  () > 0)
#line 177 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
{
#line 178 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
pUIManager  -> AddMessageBoxLString  (MSGLCMD_STORY_INTRO  , TRUE  , m_strStringIDContent4 );
#line 179 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
}
#line 181 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
if(m_strStringIDContent5  . Length  () > 0)
#line 182 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
{
#line 183 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
pUIManager  -> AddMessageBoxLString  (MSGLCMD_STORY_INTRO  , TRUE  , m_strStringIDContent5 );
#line 184 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
}
#line 187 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
if(m_strStringIDChoice1  . Length  () > 0)
#line 188 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
{
#line 189 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
pUIManager  -> AddMessageBoxLString  (MSGLCMD_STORY_INTRO  , FALSE  , m_strStringIDChoice1 );
#line 190 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
}
#line 192 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
if(m_strStringIDChoice2  . Length  () > 0)
#line 193 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
{
#line 194 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
pUIManager  -> AddMessageBoxLString  (MSGLCMD_STORY_INTRO  , FALSE  , m_strStringIDChoice2 );
#line 195 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
}
#line 197 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
if(m_strStringIDChoice3  . Length  () > 0)
#line 198 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
{
#line 199 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
pUIManager  -> AddMessageBoxLString  (MSGLCMD_STORY_INTRO  , FALSE  , m_strStringIDChoice3 );
#line 200 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
}
#line 202 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
if(m_strStringIDChoice4  . Length  () > 0)
#line 203 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
{
#line 204 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
pUIManager  -> AddMessageBoxLString  (MSGLCMD_STORY_INTRO  , FALSE  , m_strStringIDChoice4 );
#line 205 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
}
#line 207 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
if(m_strStringIDChoice5  . Length  () > 0)
#line 208 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
{
#line 209 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
pUIManager  -> AddMessageBoxLString  (MSGLCMD_STORY_INTRO  , FALSE  , m_strStringIDChoice5 );
#line 210 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
}
#line 212 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
if(m_bShowOnce )
#line 213 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
{
#line 215 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
if(m_bActive )
#line 216 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
{
#line 217 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
m_bActive  = FALSE ;
#line 218 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
}
#line 219 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
}
#line 220 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
}
#line 221 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
}
#line 222 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
}
#line 223 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
else if(eViewType  == MSGVIEW_NOTICE )
#line 224 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
{
#line 225 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
if(m_strStringIDContent1  . Length  () > 0)
#line 226 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
{
#line 227 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
_UIAutoHelp  -> SetGMNotice  (_S  (m_iStringIDContent1  , ""));
#line 228 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
if(m_bShowOnce )
#line 229 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
{
#line 230 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
m_bActive  = FALSE ;
#line 231 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
}
#line 232 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
}
#line 233 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
}
#line 234 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
}
BOOL CMessageView::
#line 238 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
Main(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CMessageView_Main
	ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CMessageView::Main expects 'EVoid' as input!");	const EVoid &e = (const EVoid &)__eeInput;
#line 240 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
InitAsEditorModel  ();
#line 241 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
SetPhysicsFlags  (EPF_MODEL_IMMATERIAL );
#line 242 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
SetCollisionFlags  (ECF_IMMATERIAL );
#line 245 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
SetModel  (MODEL_MARKER );
#line 246 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
SetModelMainTexture  (TEXTURE_MARKER );
#line 247 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
SetTimerAt(THINKTIME_NEVER);
Jump(STATE_CURRENT, 0x03810000, FALSE, EBegin());return TRUE;}BOOL CMessageView::H0x03810000_Main_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x03810000
switch(__eeInput.ee_slEvent)
#line 248 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
{case(EVENTCODE_ETrigger):
#line 250 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
{const ETrigger&eTrigger= (ETrigger&)__eeInput;

#line 251 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
if(! m_bActive )
#line 252 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
{
#line 253 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
return TRUE;
#line 254 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
}
#line 256 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
OpenMessageView  ();
#line 258 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
return TRUE;
#line 259 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
}ASSERT(FALSE);break;case(EVENTCODE_EActivate):
#line 261 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
{const EActivate&e= (EActivate&)__eeInput;

#line 262 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
m_bActive  = TRUE ;
#line 263 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
return TRUE;
#line 264 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
}ASSERT(FALSE);break;case(EVENTCODE_EDeactivate):
#line 266 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
{const EDeactivate&e= (EDeactivate&)__eeInput;

#line 267 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
m_bActive  = FALSE ;
#line 268 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
return TRUE;
#line 269 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
}ASSERT(FALSE);break;default: return FALSE; break;
#line 270 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
}return TRUE;}BOOL CMessageView::H0x03810001_Main_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x03810001

#line 271 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
Return(STATE_CURRENT,EVoid());
#line 271 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageView.es"
return TRUE; ASSERT(FALSE); return TRUE;};