/**
* Created by pk 2008.01.04
*/

#include "autostring.h"

#ifdef _WIN32


#include "global.h"
#include "windows.h"

#ifdef _MSC_VER
#	pragma warning(push)
#	pragma warning(disable:4267)
#endif

autostring::autostring() : std::string()
{
	init();
}

autostring::autostring(const autostring& _Right)
: std::string(_Right.c_str())
{
	init();
}

autostring::autostring(const autostring& _Right, size_type _Roff, size_type _Count)
: std::string(_Right.c_str(), _Roff, _Count)
{
	init();
}

autostring::autostring(const std::string& _Right)
: std::string(_Right)
{
	init();
}

autostring::autostring(const std::string& _Right, size_type _Roff, size_type _Count)
: std::string(_Right, _Roff, _Count)
{
	init();
}

autostring::autostring(const char* _Ptr)
: std::string(_Ptr)
{
	init();
}

autostring::autostring(const char* _Ptr, size_type _Count)
: std::string(_Ptr, _Count)
{
	init();
}

autostring::autostring(const char* _Ptr, size_type _Roff, size_type _Count)
: std::string(_Ptr, _Roff, _Count)
{
	init();
}

autostring::autostring(char _Ch)
: std::string(1, _Ch)
{
	init();
}

autostring::autostring(size_type _Count, char _Ch)
: std::string(_Count, _Ch)
{
	init();
}

autostring::~autostring()
{
	release();
}

/*
	autostring& autostring::operator=(const autostring& _Right)
	{	// assign _Right
	assign(_Right);
	return *this;
	}

	autostring& autostring::operator=(const std::string& _Right)
	{	// assign _Right
	assign(_Right);
	return *this;
	}

	autostring& autostring::operator=(const char* _Ptr)
	{	// assign [_Ptr, <null>)
	assign(_Ptr);
	return *this;
	}

	autostring& autostring::operator=(char _Ch)
	{	// assign 1 * _Ch
	assign(1, _Ch);
	return *this;
	}

	autostring& autostring::operator+=(const autostring& _Right)
	{	// append _Right
	append(_Right.c_str());
	return *this;
	}

	autostring& autostring::operator+=(const std::string& _Right)
	{	// append _Right
	append(_Right.c_str());
	return *this;
	}

	autostring& autostring::operator+=(const char *_Ptr)
	{	// append [_Ptr, <null>)
	append(_Ptr);
	return *this;
	}

	autostring& autostring::operator+=(char _Ch)
	{	// append 1 * _Ch
	append((size_type)1, _Ch);
	return *this;
	}
	*/

void autostring::init()
{
	m_pUnicode = NULL;
	m_pAnsi = NULL;
	m_pUtf8 = NULL;
}
// Çå³ý»º´æ
void autostring::release()
{
	safeDeleteArray(m_pUnicode);
	safeDeleteArray(m_pAnsi);
	safeDeleteArray(m_pUtf8);
}

wchar_t* autostring::ANSIToUnicode(const char *pszANSI)
{
	safeDeleteArray(m_pUnicode);

	int nLen = ::MultiByteToWideChar(CP_ACP, 0, pszANSI, -1, NULL, 0);
	m_pUnicode = new wchar_t[nLen + 1];
	memset(m_pUnicode, 0, (nLen + 1) * sizeof(wchar_t));
	::MultiByteToWideChar(CP_ACP, 0, pszANSI, -1, (LPWSTR)m_pUnicode, nLen);
	return m_pUnicode;
}

char* autostring::UnicodeToANSI(const wchar_t *pszUnicode)
{
	safeDeleteArray(m_pAnsi);

	int nLen = WideCharToMultiByte(CP_ACP, 0, pszUnicode, -1, NULL, 0, NULL, NULL);
	m_pAnsi = new char[nLen + 1];
	memset(m_pAnsi, 0, sizeof(char)* (nLen + 1));
	::WideCharToMultiByte(CP_ACP, 0, pszUnicode, -1, m_pAnsi, nLen, NULL, NULL);
	return m_pAnsi;
}

wchar_t* autostring::UTF8ToUnicode(const char *pszUTF8)
{
	safeDeleteArray(m_pUnicode);

	int nLen = ::MultiByteToWideChar(CP_UTF8, 0, pszUTF8, -1, NULL, 0);
	m_pUnicode = new wchar_t[nLen + 1];
	memset(m_pUnicode, 0, (nLen + 1) * sizeof(wchar_t));
	::MultiByteToWideChar(CP_UTF8, 0, pszUTF8, -1, (LPWSTR)m_pUnicode, nLen);
	return m_pUnicode;
}

char* autostring::UnicodeToUTF8(const wchar_t *pszUnicode)
{
	safeDeleteArray(m_pUtf8);

	int nLen = WideCharToMultiByte(CP_UTF8, 0, pszUnicode, -1, NULL, 0, NULL, NULL);
	m_pUtf8 = new char[nLen + 1];
	memset(m_pUtf8, 0, sizeof(char)* (nLen + 1));
	::WideCharToMultiByte(CP_UTF8, 0, pszUnicode, -1, m_pUtf8, nLen, NULL, NULL);
	return m_pUtf8;
}

char* autostring::ANSIToUTF8(const char *pszANSI)
{
	ANSIToUnicode(pszANSI);
	UnicodeToUTF8(m_pUnicode);
	return m_pUtf8;
}

char* autostring::UTF8ToANSI(const char *pszUTF8)
{
	UTF8ToUnicode(pszUTF8);
	UnicodeToANSI(m_pUnicode);
	return m_pAnsi;
}


#ifdef _MSC_VER
#	pragma warning(pop)
#endif

#endif // _WIN32
