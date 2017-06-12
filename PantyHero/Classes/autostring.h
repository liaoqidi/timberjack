/**
* Created by pk 2008.01.04
*/

#ifndef MINI_AUTOSTRING_H
#define MINI_AUTOSTRING_H

#ifdef _WIN32

#include <string>

#ifdef _MSC_VER
#	pragma warning(push)
#	pragma warning(disable:4267 4275 )
#endif



template class std::allocator<char>;
template class std::basic_string<char, std::char_traits<char>, std::allocator<char> >;
/*
*	功能描述:
*  1.继承自std::string，拥有父类的所有功能；
*  2.增加c_wstr()函数，使之自动转换和存储转换后的结果；
*  3.推荐在转换前完成了字符串的编辑工作，否则需要显式调用 clear_wstr 清除已经转换的wchar缓存
*  4.为了简单好用，没有把wstr跟原始字符串进行同步更新，一般都是在绘制的时候转换一下即可
*/
class autostring : public std::string
{
public:
	typedef std::string::size_type size_type;

private:
	wchar_t *m_pUnicode;	/// 用于记录编码转换后的地址
	char	*m_pAnsi;
	char	*m_pUtf8;


	// 构造（析构）
public:
	autostring();
	autostring(const autostring& _Right);
	autostring(const autostring& _Right, size_type _Roff, size_type _Count = std::string::npos);
	autostring(const std::string& _Right);
	autostring(const std::string& _Right, size_type _Roff, size_type _Count = std::string::npos);
	autostring(const char* _Ptr);
	autostring(const char* _Ptr, size_type _Count);
	autostring(const char* _Ptr, size_type _Roff, size_type _Count);
	autostring(char _Ch);
	autostring(size_type _Count, char _Ch);

	~autostring();

	/*
			// 操作
			public:
			autostring& operator=(const autostring& _Right);
			autostring& operator=(const std::string& _Right);
			autostring& operator=(const char* _Ptr);
			autostring& operator=(char _Ch);
			autostring& operator+=(const autostring& _Right);
			autostring& operator+=(const std::string& _Right);
			autostring& operator+=(const char *_Ptr);
			autostring& operator+=(char _Ch);*/

	// 转换
public:
	void init();
	void release();

	wchar_t*	ANSIToUnicode(const char *pszANSI);
	char*		UnicodeToANSI(const wchar_t *pszUnicode);

	wchar_t*	UTF8ToUnicode(const char *pszUTF8);
	char*		UnicodeToUTF8(const wchar_t *pszUnicode);

	char*		ANSIToUTF8(const char *pszANSI);
	char*		UTF8ToANSI(const char *pszUTF8);
};


/********************** 常用转换宏 ***********************************/
/*
////////////////////////////////////////////////////////////////////////////////
// 字符串哈希函数,你可以改写
#if defined(MINI_COMPILER_MSVC) && MINI_COMPILER_VER >= 1300
namespace stdext
{
	inline size_t hash_value(const mini::autostring& _Str)
	{
		#define _HASH_SEED	(size_t)0xdeadbeef

		size_t _Val = _HASH_SEED;
		size_t _Size = _Str.size();
		if (0 < _Size)
		{	// add one or more elements
			size_t _Stride = (_Size / 16) + 1;
			_Size -= _Stride;	// protect against _Size near _Str.max_size()
			for(size_t _Idx = 0; _Idx <= _Size; _Idx += _Stride)
				_Val += (size_t)_Str[_Idx];
		}
		return (_Val);
	};
}
#elif defined(MINI_COMPILER_GNUC) && MINI_COMPILER_VER >= 310
#include <ext/hash_map>
namespace __gnu_cxx
//namespace std
{
	template <> struct hash< const mini::autostring& >
	{
		size_t operator()( const mini::autostring& _Str ) const
		{
			#define _HASH_SEED	(size_t)0xdeadbeef

			size_t _Val = _HASH_SEED;
			size_t _Size = _Str.size();
			if (0 < _Size)
			{	// add one or more elements
				size_t _Stride = (_Size / 16) + 1;
				_Size -= _Stride;	// protect against _Size near _Str.max_size()
				for(size_t _Idx = 0; _Idx <= _Size; _Idx += _Stride)
					_Val += (size_t)_Str[_Idx];
			}
			return (_Val);
		}
	};

	template<>
	struct hash<std::string>
	{
		size_t operator()(std::string __s) const
		{ return __stl_hash_string(__s.c_str()); }
	};	
}
#endif
*/
#ifdef _MSC_VER
#	pragma warning(pop)
#endif

#else
class autostring
{
public:
	autostring()	{};
	~autostring()	{};
	char*		ANSIToUTF8(const char *pszANSI) { return (char*)pszANSI; }
	char*		UTF8ToANSI(const char *pszUTF8)	{ return (char*)pszUTF8; }

};

#endif // _WIN32

#endif
