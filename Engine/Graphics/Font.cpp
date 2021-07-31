#include "stdh.h"

#include <Engine/Graphics/Font.h>
#include <Engine/Base/Stream.h>
#include <Engine/Graphics/Texture.h>
#include <Engine/Graphics/Color.h>
#include <Engine/Templates/Stock_CTextureData.h>

#define FONT_VERSION 2

// some default fonts
CFontData *_pfdDisplayFont;
CFontData *_pfdConsoleFont;
CFontData *_pfdGameFont;
CFontData *_pfdDefaultFont;


// constructor deletes letter data
CFontCharData::CFontCharData(void)
{
  fcd_pixXOffset = 0;
  fcd_pixYOffset = 0;
  fcd_pixStart = 0;
  fcd_pixEnd   = 0;
  fcd_code = 0;
}

// simple stream functions
void CFontCharData::Read_t(  CTStream *inFile, BOOL bNew)
{
  *inFile>>fcd_pixXOffset;
  *inFile>>fcd_pixYOffset;
  *inFile>>fcd_pixStart;
  *inFile>>fcd_pixEnd;
  if (bNew)
  {
	  *inFile>>fcd_code;
  }
}

void CFontCharData::Write_t( CTStream *outFile)
{
  *outFile<<fcd_pixXOffset;
  *outFile<<fcd_pixYOffset;
  *outFile<<fcd_pixStart;
  *outFile<<fcd_pixEnd;
  *outFile<<fcd_code;
}

CFontData::CFontData()
{
  fd_fcdFontCharData = NULL;
  fd_ptdTextureData = NULL;
  fd_fnTexture = CTString("");
  fd_pixShadowSpacing = 0;
  fd_ulFlags = FNF_CELLBASED; // old font by default
  fd_bSmallCaps=FALSE;
  fd_fSmallCapsStretch=0.6f;
  fd_Version = 0;
  fd_CharTotal = 0;
}

CFontData::~CFontData()
{
  Clear();
}


void CFontData::Clear()
{
	if (fd_fcdFontCharData != NULL)
	{
		delete [] fd_fcdFontCharData;
		fd_fcdFontCharData = NULL;
	}

  if( fd_ptdTextureData != NULL) {
    fd_fnTexture = CTString("");
    _pTextureStock->Release(fd_ptdTextureData);
    fd_ptdTextureData = NULL;
  }
}

void CFontData::SetFontCharData(INDEX iCount)
{
	if (iCount>0)
	{
		fd_fcdFontCharData = new CFontCharData[iCount];
	}
	else
	{
		fd_fcdFontCharData = NULL;
	}
}

void CFontData::Read_t( CTStream *inFile) // throw char *
{
  // clear current font data (if needed)
  Clear();

  INDEX iFontVersion=-1;
  BOOL bNew = TRUE;
  // if old format
  if(inFile->PeekID_t()==CChunkID("FTTF")) {
    // read the filename of the corresponding texture file.
    inFile->ExpectID_t( CChunkID("FTTF"));
  // else read new format
  } else {
    // read the filename of the corresponding texture file.
    inFile->ExpectID_t( CChunkID("FONN"));
    *inFile >> iFontVersion;
    // if version 2
    if(iFontVersion==2) {
      // read shadow spacing and flags
      *inFile >> fd_pixShadowSpacing;
      *inFile >> fd_ulFlags;
    } else {
		  ThrowF_t(TRANS("File '%s'.\nInvalid Font file version.\nExpected Ver \"%d\" but found \"%d\"\n"),
        (const char*)inFile->GetDescription(),FONT_VERSION,iFontVersion);
    }
  }
  *inFile >> fd_fnTexture;
  // read maximum width and height of all letters
  *inFile >> fd_pixCharWidth;
  *inFile >> fd_pixCharHeight;
  if (strcmp(inFile->strm_strStreamDescription, "Fonts\\Russia.fnt") == 0 || 
	  strcmp(inFile->strm_strStreamDescription, "Fonts\\Korean.fnt") == 0)
  {
	*inFile >> fd_CharTotal;
  }

  fd_Version = 1;

  // read entire letter data table
  if (fd_CharTotal == 0)
  {
	  fd_CharTotal = 256;
	if (strcmp(inFile->strm_strStreamDescription, "Fonts\\Russia.fnt") != 0)
			bNew = FALSE;
	  fd_Version = 0;
  }

  SetFontCharData(fd_CharTotal);
  for( INDEX iLetterData=0; iLetterData<fd_CharTotal; ++iLetterData) {
    fd_fcdFontCharData[iLetterData].Read_t( inFile, bNew);
  }

  // load corresponding texture file (don't allow any filters, mipmap removal and stuff)
  fd_ptdTextureData = _pTextureStock->Obtain_t(fd_fnTexture);
  fd_ptdTextureData->Force(TEX_CONSTANT);

  // initialize default font variables
  SetVariableWidth();
  if(iFontVersion==-1)
  {
    SetCharSpacing(+1);
    SetLineSpacing(+1);
    SetSpaceWidth(0.5f);
//    fd_fcdFontCharData[' '].fcd_pixStart = 0;
	
  } else {
    SetCharSpacing(0);
    SetLineSpacing(0);
  }
}


void CFontData::Write_t( CTStream *outFile) // throw char *
{
  ASSERT( fd_ptdTextureData != NULL);
  // write the filename of the corresponding texture file
  outFile->WriteID_t( CChunkID("FONN"));
  *outFile << FONT_VERSION;
  *outFile << fd_pixShadowSpacing;
  *outFile << fd_ulFlags;

  *outFile << fd_fnTexture;
  // write max letter width and height of all letters
  *outFile << fd_pixCharWidth;
  *outFile << fd_pixCharHeight;
  *outFile << fd_CharTotal;
  INDEX tmpCount = fd_CharTotal;
	if (fd_CharTotal == 0)
		tmpCount = 256;

  // write entire letter data table
  for( INDEX iLetterData=0; iLetterData<tmpCount; ++iLetterData) {
    fd_fcdFontCharData[iLetterData].Write_t(outFile);
  }
}

int CFontData::SearchIndex(UWORD uCode) const
{
	if (fd_Version == 0) return (int)uCode;

	INDEX i1=0, i2 = fd_CharTotal-1, c;

	do 
	{
		c = (i1 + i2) >> 1;

		if (uCode > fd_fcdFontCharData[c].fcd_code)
			i1 = c+1;
		else
			i2 = c;

	} while(i1 < i2);

	return i1;
}

/*
 * Function used for creating font data object
 */
void CFontData::Make_t( const CTFileName &fnTexture, PIX pixCharWidth, PIX pixCharHeight,
                        CTFileName &fnOrderFile, BOOL bUseAlpha)
{
  // do it only if font has not been created already
  ASSERT( fd_ptdTextureData == NULL);

  // remember texture name
  fd_fnTexture = fnTexture;
  // load texture and cache width
  fd_ptdTextureData = _pTextureStock->Obtain_t(fd_fnTexture);
  fd_ptdTextureData->Force(TEX_STATIC|TEX_CONSTANT);
  PIX pixTexWidth = fd_ptdTextureData->GetPixWidth();

  // load ascii order file (no application path necessary)
  CTString strLettersOrder;
  IgnoreApplicationPath();
  strLettersOrder.Load_t( fnOrderFile);
  UseApplicationPath();

  // remember letter width and height
  fd_pixCharWidth  = pixCharWidth;
  fd_pixCharHeight = pixCharHeight;
  // determine address in memory where font definition begins in its larger mip-map
  ULONG *pulFont = fd_ptdTextureData->td_pulFrames;
  ASSERT( pulFont!=NULL);

  // find number of letters in line (assuming that the 1st line represents the width of every line)
  INDEX iLettersInLine=0;
  while( (strLettersOrder[iLettersInLine]!='\n') && iLettersInLine<strlen(strLettersOrder)) iLettersInLine++;
  if( iLettersInLine<=0) FatalError( "Invalid font definition ASCII file.");

  // determine pixelcheck mast depending of alpha channel usage
  COLOR colPixMask = 0xFFFFFF00;  // FC is because of small tolerance for black 
  if( bUseAlpha) colPixMask = 0xFFFFFFFF;

  // how much we must add to jump to character down
  PIX pixFontCharModulo = pixTexWidth * fd_pixCharHeight;

  // for all letters in font (ranging from space to last letter that user defined)
  INDEX iLetter=0;
  INDEX iCurrentLetterLine = 0;
  while( iLetter<strlen(strLettersOrder))
  { // for letters in one line
    for( INDEX iCurrentLetterColumn=0; iCurrentLetterColumn<iLettersInLine; iCurrentLetterColumn++)
    { // test if we at the end of whole array
      if( iLetter>=strlen(strLettersOrder)) break;
      // get char params
      unsigned char chrLetter = strLettersOrder[iLetter++];
      // reset current letter's width
      PIX pixCurrentStart = fd_pixCharWidth;
      PIX pixCurrentEnd   = 0;
      // for all of the letter's lines
      for( INDEX iPixelLine=0; iPixelLine<fd_pixCharHeight; iPixelLine++)
      { // for all of the letter's pixels
        for( INDEX iPixelColumn=0; iPixelColumn<fd_pixCharWidth; iPixelColumn++)
        { // calculate current pixel's adress in font's texture
          ULONG *puwPixel = (ULONG*)( pulFont + pixFontCharModulo * iCurrentLetterLine +  // calc right letter line
                                      fd_pixCharWidth * iCurrentLetterColumn +            // move to right letter column
                                      pixTexWidth * iPixelLine +   // move trough pixel lines of one letter
                                      iPixelColumn);               // move trough pixel columns of one letter
          // if we test alpha channel and alpha value is not 0
          if( ByteSwap(*puwPixel) & colPixMask) {
            // if examined pixel is narrower in letter than last opaque pixel found, remember it as left-most pixel
            if( iPixelColumn < pixCurrentStart) pixCurrentStart = iPixelColumn;
            // if examined pixel is wider in letter than last opaque pixel found, remember it as right-most pixel
            if( iPixelColumn > pixCurrentEnd) pixCurrentEnd = iPixelColumn;
          }
        }
      }
      // letter's data is stored into table on appropriate place
      fd_fcdFontCharData[chrLetter].fcd_pixXOffset = iCurrentLetterColumn * fd_pixCharWidth;
      fd_fcdFontCharData[chrLetter].fcd_pixYOffset = iCurrentLetterLine   * fd_pixCharHeight;
      fd_fcdFontCharData[chrLetter].fcd_pixStart   = pixCurrentStart;
      fd_fcdFontCharData[chrLetter].fcd_pixEnd     = pixCurrentEnd +1;
    }
    // advance to next line in text file
    iCurrentLetterLine++;
    iLetter++;  // skip carriage return
  }
  // set default space width
  fd_fcdFontCharData[' '].fcd_pixStart = 0;
  SetSpaceWidth(0.5f);

  // all done
  SetVariableWidth();
  _pTextureStock->Release( fd_ptdTextureData);
}
/*
//////////////////////////////////////////////////////////////////////////
// ���ο� ��Ʈ Ŭ���� ����
//////////////////////////////////////////////////////////////////////////
CFontEx::CFontEx()
{
	m_codes = NULL;
	m_chinfo = NULL;
	m_image = NULL;
	m_maxheight = 0;
	m_CharGap = 0;
}

CFontEx::~CFontEx()
{
	if (m_codes)
		delete [] m_codes;
	if (m_chinfo)
		delete [] m_chinfo;
	if (m_image)
		delete [] m_image;
}

//////////////////////////////////////////////////////////////////////////
// CFontEx::Load() ��Ʈ �����͸� �д´�.
//////////////////////////////////////////////////////////////////////////
bool CFontEx::Load(const char* fname)
{
	FILE *fp;
	int i, size, len, offset;

	fp = fopen(fname, "rb");

	if (fp)
	{
		fread(&m_cnt, 1, 2, fp);
		fread(&size, 1, 4, fp);

		m_codes = new unsigned short[m_cnt];
		m_chinfo = new CHINFO [m_cnt];
		m_image = new unsigned char[size];

		for (i=0, offset=0; i<m_cnt; i++)
		{
			fread(&m_codes[i], 1, 2, fp);
			fread(&m_chinfo[i].w, 1, 1, fp);
			fread(&m_chinfo[i].h, 1, 1, fp);
			m_chinfo[i].offset = offset;

			if (m_chinfo[i].h > m_maxheight)
				m_maxheight = m_chinfo[i].h;

			len = (m_chinfo[i].w * m_chinfo[i].h + 3) / 4;

			fread(&m_image[offset], 1, len, fp);
			offset += len;
		}
		fclose(fp);

		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
// ������ �ε����� ã�´�.
//////////////////////////////////////////////////////////////////////////
int CFontEx::SearchIndex(unsigned short code) const
{
	int i1 = 0, i2 =m_cnt-1, c;

	do {
		c = (i1 + i2) >> 1;

		if (code > m_codes[c])
			i1 = c+1;
		else
			i2 = c;
	} while(i1 < i2);

#ifdef _DEBUG
	if (m_codes[i1] != code)
		_ASSERT(!"Is not font set data");
#endif
	return i1;
}

//////////////////////////////////////////////////////////////////////////
// ������ ���� ���̸� ���Ѵ�.
//////////////////////////////////////////////////////////////////////////
int CFontEx::GetWidth(unsigned short code) const
{
	return m_chinfo[SearchIndex(code)].w + m_CharGap;
}

//////////////////////////////////////////////////////////////////////////
// ������ ���� ���̸� ����� ����.
//////////////////////////////////////////////////////////////////////////
int CFontEx::CalcWidth(const char* text) const
{
	int i, code, w;

	for (i=0, w=0; text[i] != '\0';)
	{
		code = (unsigned char)text[i++];

		if (code&0x80)
			code = (code<<8) | (unsigned char)text[i++];

		w += GetWidth(code);
	}

	return w;
}

//////////////////////////////////////////////////////////////////////////
// �ؽ�ó�� �� �޸𸮸� �����Ѵ�.
//////////////////////////////////////////////////////////////////////////
int CFontEx::Draw(unsigned char *ptr, int pitch, unsigned short code)
{
	CHINFO *info = &m_chinfo[SearchIndex(code)];
	unsigned char *img, byte;
	int x, y, i;

	img = &m_image[info->offset];

	for (y=0, i=8; y<info->h; y++, ptr+=pitch) 
	{
		for (x=0; x<info->w; x++, i+=2, byte>>=2)
		{
			if (i >= 8)
			{
				byte = *img++;
				i = 0;
			}
			ptr[x] = byte & 0x03;
		}

		if (m_CharGap>0)
		{
			for (; x<info->w+m_CharGap; x++)
				ptr[x] = 0;
		}
	}

	return info->w + m_CharGap;
}

//-----------------------------------------------------------------------------------------
// CFontExWorkspace
//-----------------------------------------------------------------------------------------
#define TEX_W	128
#define TEX_H	128

//////////////////////////////////////////////////////////////////////////
// CFontExWorkspace
//////////////////////////////////////////////////////////////////////////
CFontExWorkspace::CFontExWorkspace()
{
	m_curtex = m_pivottex = new _TEXTURE;
	m_curtex->handle = 0;
	m_curtex->next = m_curtex;

	m_image = new unsigned char [TEX_W * TEX_H];
	memset(m_image, 0, TEX_W * TEX_H);

	m_piece = NULL;

	m_u = m_v = 0;
	m_vtop = m_vbottom = 0;
	m_fheight = 0;

	m_updatecnt = 1;
}

//////////////////////////////////////////////////////////////////////////
// ~CFontWorkspace
// �ؽ�ó ������ ��ϵ� ���� ����
//////////////////////////////////////////////////////////////////////////
CFontExWorkspace::~CFontExWorkspace()
{
	_TEXTURE *t = m_curtex , *pt;
	do {
		pt = t;
		t = t->next;
		if (pt->handle)
		{ // ���⼭ �ؽ�ó �ڵ��� ������ Release�� ȣ��

		}
		delete pt;
	} while (t != m_curtex);

	_TEXTPIECE *next, *p;
	for (p=m_piece; p; p=next)
	{
		next = p->next;
		delete p;
	}

	delete [] m_image;
}

//////////////////////////////////////////////////////////////////////////
// CFontExWorkspace::Flush()
// �ؽ�ó ������Ʈ �� ���� �ʱ�ȭ
//////////////////////////////////////////////////////////////////////////
void CFontExWorkspace::Flush()
{
	UpdateTexture(m_curtex, m_vtop, m_vbottom);

	m_vtop = m_vbottom = m_v;
	m_pivottex = m_curtex;
	m_updatecnt++;
}

//////////////////////////////////////////////////////////////////////////
// CFontExWorkspace::GetWidth()
// �ؽ�ó ���� ����
//////////////////////////////////////////////////////////////////////////
int CFontExWorkspace::GetWidth(void* p)
{
	return ((_TEXTPIECE *)p)->w;
}

//////////////////////////////////////////////////////////////////////////
// CFontExWorkspace::GetTexPiece()
// �ؽ�ó ������ �����ش�. ������ �̹� �׷������� ������ ���� �����Ͽ�
// �����ش�.(Text���� ��ŭ)
//////////////////////////////////////////////////////////////////////////
void* CFontExWorkspace::GetTexPiece(const char* text, CFontEx* font, int* width)
{
	_TEXTPIECE *p;

// 1. �̹� �÷��� ���� ��Ʈ�� ����, ������ ���� ��
	for (p=m_piece; p; p=p->next)
		if (p->font == font && !strcmp(p->text, text))
			break;
// 2. ã�� ���Ͽ��� �� ���� ���� �� ����Ʈ �߰�
	if (p == NULL)
	{
		p = (_TEXTPIECE*) (new char [sizeof(_TEXTPIECE)+strlen(text)]);
		strcpy(p->text, text);
		p->font = font;
		p->updatecnt = 0;
		
		m_piece = AddLink(m_piece, p);
		WriteTexture(p);
	}
// 3. �̹� �����ӿ� �������� ǥ���Ѵ�.
	p->updatecnt = m_updatecnt;

	// ������ ���� ���̸� �����ش�.
	if (width)
		*width = p->w;
	
	return (void*) p;
}

//////////////////////////////////////////////////////////////////////////
// CFontExWorkspace::RemovePage()
// �ؽ�ó�� ���� �����߿��� �ش� �ؽ�ó �������� �Ҵ�� �͵��� �����Ѵ�.
// ��, �� �����ӿ� ������ �Ǿ�� �ϴ� �༮���� m_reserve ����Ʈ�� �־�д�.
//////////////////////////////////////////////////////////////////////////
CFontExWorkspace::_TEXTPIECE* CFontExWorkspace::RemovePage(_TEXTURE *tex)
{
	_TEXTPIECE *p, *next;
	_TEXTPIECE *list = NULL;

	for (p=m_piece; p;)
	{
		next = p->next;

		if (p->tex == tex)
		{
			m_piece = RemoveLink(m_piece, p);

			if (p->updatecnt == m_updatecnt)
			{
				list = AddLink(list, p);
			} else {
				delete p;
			}
		}

		p = next;
	}
	
	return list;
}

//////////////////////////////////////////////////////////////////////////
// CFontExWorkspace::WriteTexture()
// �ؽ�ó�� ���� �̹����� �׸���.
//////////////////////////////////////////////////////////////////////////
void CFontExWorkspace::WriteTexture(_TEXTPIECE *p)
{
	int i, code, w, chpos = 0, line = 0;
	CFontEx *font = p->font;
	_TEXTPIECE *rest = NULL;

	if (m_fheight < font->GetHeight())
		m_fheight = font->GetHeight();

//	1. �ؽ�ó�� ù��° ��ġ�� ����صд�.
	p->u = m_u;
	p->v = m_v;
	p->h = font->GetHeight();
	p->fh = m_fheight;
	p->w = 0;
	p->tex = m_curtex;
	p->updatecnt = m_updatecnt;

	for (i=0, chpos=0; p->text[i] != '\0';)
	{
		chpos = i;

//	2. ���ڿ��� �� ���ڸ� �д´�.
		code = (unsigned char)p->text[i];

		if (code&0x80)
			code = (code<<8) | (unsigned char)p->text[i++];

//	3. ������ ���� ���̸� ��´�.
		w = font->GetWidth(code);

//	4. �ؽ�ó ����ũ�⸦ �Ѵ� ��쿡�� �����ٷ� �Ѱܼ� ��� �׸����� �Ѵ�.
//	   �������� ��� ��Ʈ�� ũ�⿡ ���� �����ٷ� �Ѱܾ� �� ���� �ִ�.

		if (m_u + w > TEX_W || m_fheight + m_v >= TEX_H)
		{
//	5. ���� �ٷ� �ѱ�ϴ�.
			m_u = 0;
			m_v += m_fheight + 1;
			m_fheight = font->GetHeight();

//	6. ���� ���� �������� �Ѿ ��� ���� �������� �׸���.
			if (m_v + font->GetHeight() >= TEX_H)
			{
//	7. ���� ���� �������� ������Ʈ �Ѵ�.
				UpdateTexture(m_curtex, m_vtop, m_v);

//	8. ���������� �Ѿ��.
//	   ���� ���� ���� ù��(������ �����Ҷ�) �̸� ���� �� �Ҵ��ؾ� �Ѵ�.( ��ȯ�ǰ� �����Ǿ� �ִ�.)
				if (m_curtex->next == m_pivottex)
				{
					_TEXTURE *t = new _TEXTURE;
					t->handle = 0;
					t->next = m_curtex->next;
					m_curtex->next = t;
				} else{
//	9. ���Ӱ� ����� �ؽ�ó�� ����ؾ� �� ������ ������ �����, �ƴ� ���� �����Ѵ�.
					rest = RemovePage(m_curtex->next);
				}
				
				m_curtex = m_curtex->next;

				m_u = m_v = 0;
				m_vbottom = 0;
				m_vtop = 0;

				memset(m_image, 0, TEX_W * TEX_H);
			}
//	10. ���ۺ��� �Ѱܾ� �ϴ� ��쿡�� �� ���� ���������� �Ѵ�.
//	    �׸��� Ư���� �� ������ �ƴ� ��쿡 ���� ���ϴ� ���ϸ� ���̱� ���� ������ �����Ѵ�.
			if (chpos == 0)
			{
				p->u = m_u;
				p->v = m_v;
				p->fh = m_fheight;
				p->tex = m_curtex;
			} else {
				if (line < sizeof(p->chw)/sizeof(*p->chw))
				{
					p->chw[line][0] = chpos;
					p->chw[line][1] = p->w;
					line++;
				}
			}

//	11. �ؽ�ó ��ġ�� ���ڸ� �׸���.
			font->Draw(m_image + m_u + m_v * TEX_W, TEX_W, code);

			m_u += w;
			p->w += w;
		}

//	12. �ٴ� ��ġ�� �����Ѵ�.
		if (m_vbottom < m_v + font->GetHeight() + 1)
			m_vbottom = m_v + font->GetHeight() + 1;

//	13. ���Ӱ� ������ �Ҵ��ϸ鼭 �и� ������ �ٽ� �����Ѵ�.
		if (rest)
		{
			_TEXTPIECE *next, *p = rest;
			for (; p; p=next)
			{
				next = p->next;

				m_piece = AddLink(m_piece, p);
				WriteTexture(p);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// CFontExWorkspace::DrawPrimitive()
// ���յ� �����̹����� �׸���.(������ ó��)
//////////////////////////////////////////////////////////////////////////
void CFontExWorkspace::DrawPrimitive(float x, float y, float scale, unsigned long type, unsigned long color, void* ptr)
{
	int u1, u2, v, remain;
	int line = 0;
	_TEXTPIECE *p = (_TEXTPIECE*)ptr;
	_TEXTURE *tex;
	int i=0, chwide;

//	1. �ؽ�ó�� ���� ��ġ�� ��´�.
	tex = p->tex;
	u1 = u2 = p->u;
	v = p->v;

	remain = p->w;

	do {
//	2. ���� ���ο��� ������ �����ϴ� ���� ���� ���̸� ���Ѵ�.
		if (remain + u1 <= TEX_W)
		{
			u2 = u1 + remain;
		}
		else
		{
			if (line < sizeof(p->chw)/sizeof(*p->chw))
			{
				i = p->chw[line][0];
				u2 += line == 0 ? p->chw[line][1] : p->chw[line][1] - p->chw[line-1][1];
			}
			else
			{
				for (; p->text[i] != '\0';)
				{
					int code = (unsigned char)p->text[i++];

					if (code&0x80)
						code = (code<<8) | (unsigned char)p->text[i++];

					chwide = p->font->GetWidth(code);

					if (u2 + chwide > TEX_W)
						break;

					u2 += chwide;
				}
			}
		}
//	3. �׸���. (���� �������� �׸���.) 
//			Render(tex->handle, x, y, (u2 - u1) * scale, p->h *scale,
//				(float)u1 / TEX_W, (float)v / TEX_H, (float)(u2 - u1) / TEX_W, (float)p->h /TEX_H, type, color);

		remain -= u2 - u1;

		if (remain > 0) 
		{
//	4. ���� �� �׷Ⱦ��. x ��ġ ����.
			x += (u2 - u1) * scale;

//	5. ���� ���� �̹�����.(chw���� ��� ���� �ƴ� ��� �ѱ�� Ÿ�̹��� ���� ù���ڱ��� üũ�� �Ĵ�.)
			v += (line == 0 ? p->fh : p->h) + 1;
			u1 = 0;
			u2 = line < sizeof(p->chw)/sizeof(*p->chw) ? 0 : chwide;

//	6. ���� �������� �Ѱܾ� �� ��� ó���Ѵ�.
			if (v + p->h + 1 > TEX_H)
			{
				tex = tex->next;
				v = 0;
			}

			line++;
		}
	} while (remain > 0);
}

//////////////////////////////////////////////////////////////////////////
// CFontExWorkspace::UpdateTexture()
//
//////////////////////////////////////////////////////////////////////////
void CFontExWorkspace::UpdateTexture(_TEXTURE *tex, int top, int bottom)
{
	static unsigned char palette[4][3] = {
		0, 0, 0,
		128, 128, 128,
		196, 196, 196,
		255, 255, 255,
	};

	if (bottom >= TEX_W)
	{
		UpdateTexture(tex, top, TEX_W-1);
	}
	else
	{
		if (top < bottom)
		{
			if (tex->handle == 0)
			{
				// ���⼭ �ؽ�ó�� �����ϰ� �ڵ��� tex->handle�� �޴´�.
			}
			
			// ���⼭ ������ �ؽ�ó �Ǵ� �̹� ������ �ؽ�ó�� �ȼ�(�̹�������) ������ Copy�Ѵ�.

		}
	}
}

//////////////////////////////////////////////////////////////////////////
// CFontExManager
//////////////////////////////////////////////////////////////////////////
CFontExManager::CFontExManager()
{
	m_workspace = new CFontExWorkspace;
	m_font = NULL;
	m_fontlist = NULL;
	
	m_color = 0xffffffff;
	m_polytype =0;
	m_polylistn = 0;
}

CFontExManager::~CFontExManager()
{
	_FONTSET *f, *next;

	delete m_workspace;

	for (f=m_fontlist; f; f=next)
	{
		next = f->next;
		delete f->font;
		delete f;
	}
}

//////////////////////////////////////////////////////////////////////////
// CFontExManager::Reset()
//////////////////////////////////////////////////////////////////////////
void CFontExManager::Reset()
{
	delete m_workspace;
	m_workspace = new CFontExWorkspace;
}

//////////////////////////////////////////////////////////////////////////
// CFontExManager::SetFont()
// ����� ��Ʈ�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////////
bool CFontExManager::SetFont(const char *fname)
{
	// 1. ������ �о��� �� �߿� �ִ��� üũ �Ѵ�.
	_FONTSET *f;

	for (f=m_fontlist; f; f=f->next)
	{
		if (!strcmp(f->fname, fname))
		{
			m_font = f->font;
			return true;
		}
	}

	// 2. ����. �߰��Ѵ�.
	m_font = new CFontEx;

	if (m_font->Load(fname) == false)
	{
		delete m_font;
		m_font = NULL;
		return false;
	}

	f = (_FONTSET*) (new char [sizeof(_FONTSET) + strlen(fname)]);
	f->font = m_font;
	strcpy(f->fname, fname);
	f->next = m_fontlist;
	m_fontlist = f;
	return true;
}

//////////////////////////////////////////////////////////////////////////
// CFontExManager::Flush()
//
//////////////////////////////////////////////////////////////////////////
void CFontExManager::Flush()
{
	m_workspace->Flush();

	for (int i=0; i<m_polylistn; ++i)
	{
		m_workspace->DrawPrimitive(m_polylist[i].x, m_polylist[i].y, m_polylist[i].scale, m_polylist[i].type, m_polylist[i].color, m_polylist[i].p);
	}

	m_polylistn = 0;
}

//////////////////////////////////////////////////////////////////////////
// CFontExManager::Print()
// ������ �����Ѵ�.���� ������ ���� ���̸� �����ش�.
//////////////////////////////////////////////////////////////////////////
int CFontExManager::Print(float x, float y, const char *text, float scale)
{
	void *p;
	int w;

	if (m_polylistn >= sizeof(m_polylist) / sizeof(*m_polylist))
		Flush();

	m_polylist[m_polylistn].p = p = m_workspace->GetTexPiece(text, m_font, &w);
	m_polylist[m_polylistn].color = m_color;
	m_polylist[m_polylistn].x = x;
	m_polylist[m_polylistn].y = y;
	m_polylist[m_polylistn].scale = scale;
	m_polylist[m_polylistn].type = m_polytype;
	m_polylistn++;

	return w;
}
*/