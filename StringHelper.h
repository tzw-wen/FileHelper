/*
 * Copyright 2015 ZHIHUI WEN (tzw.wen@gmail.com)

 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#ifndef __STRINGHELPER_H__
#define __STRINGHELPER_H__

#ifdef _WINDOWS
#define WINDOWS _WINDOWS
#endif

#ifdef WINDOWS
	#pragma warning (disable: 4251)
#endif

#ifdef WINDOWS
	#include "afx.h"
	#include <atlbase.h>
	#include <afxdisp.h>
#else
	#include <assert.h>
	#define ASSERT(X) assert(X)
#endif

#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#ifdef WINDOWS
    #include <hash_set>
    #include <hash_map>
#else
    #ifdef __GNUC__
        #include <ext/hash_set>
        #include <ext/hash_map>
    #else
        #include <hash_set>
        #include <hash_map>
    #endif
#endif
#include <iostream>
#include <sstream>
#include <fstream>

#include <boost/regex.hpp>

#ifdef WINDOWS
    #include <atlbase.h>
    #include <atlconv.h>
#else
    #define abs(x) fabs(x)
    #define byte uint8_t
    #define _T(x) TEXT(x)
    #define  TEXT(s)  __T(s)
    #ifdef _UNICODE
        #define TCHAR wchar_t
        #define  __T(s)  L##s    /* platform is Unicode */
    #else
        #define TCHAR char
        #define  __T(s)  s         /* platform is ASCII   */
    #endif
    #define stdext std
    /////////
    // TEMPLATE CLASS hash_compare
    #define _STD std::
template<class _Kty,
	class _Pr = _STD less<_Kty> >
	class hash_compare
	{	// traits class for hash containers
public:
	enum
		{	// parameters for hash table
		bucket_size = 4,	// 0 < bucket_size
		min_buckets = 8};	// min_buckets = 2 ^^ N, 0 < N

	hash_compare()
		: comp()
		{	// construct with default comparator
		}

	hash_compare(_Pr _Pred)
		: comp(_Pred)
		{	// construct with _Pred comparator
		}

	size_t operator()(const _Kty& _Keyval) const
		{	// hash _Keyval to size_t value by pseudorandomizing transform
		ldiv_t _Qrem = ldiv((long)hash_value(_Keyval), 127773);
		_Qrem.rem = 16807 * _Qrem.rem - 2836 * _Qrem.quot;
		if (_Qrem.rem < 0)
			_Qrem.rem += 2147483647;
		return ((size_t)_Qrem.rem);
		}

	bool operator()(const _Kty& _Keyval1, const _Kty& _Keyval2) const
		{	// test if _Keyval1 ordered before _Keyval2
		return (comp(_Keyval1, _Keyval2));
		}

protected:
	_Pr comp;	// the comparator object
	};
#endif

#ifdef _WIN32
    #include <windows.h>
#else
    #include <sys/time.h>
#endif

#ifdef _WIN32
    //
#else
    #ifdef _UNICODE
        #define LPTSTR LPWSTR
        #define LPCTSTR LPCWSTR
    #else
        #define LPTSTR LPSTR
        #define LPCTSTR LPCSTR
    #endif
    #define LPSTR char *
    #define LPCSTR const char *
    #define LPWSTR wchar *
    #define LPCWSTR const wchar *

#endif

#ifdef WINDOWS
	typedef unsigned __int64 __uint64; //Portable signed long integer 8 bytes
#else
	typedef unsigned long DWORD;
	typedef long long __int64; //Portable signed long integer 8 bytes
	typedef unsigned long long __uint64; //Portable signed long integer 8 bytes
#endif

#include <stdio.h>


#define MAX_FILE_PATH_LENGTH    5000





#ifdef _UNICODE
	typedef std::wstring _tstring;
	typedef boost::wregex _tregex;
	typedef boost::wsmatch _tsmatch;
	typedef boost::wsregex_token_iterator _tregex_token_iterator;
#else
	typedef std::string _tstring;
	typedef boost::regex _tregex;
	typedef boost::smatch _tsmatch;
	typedef boost::sregex_token_iterator _tregex_token_iterator;
#endif

/* use the new safe string functions introduced in Visual Studio 2005 */
#if defined(_MSC_VER) && _MSC_VER >= 1400
	#ifdef _UNICODE
		# define _tstrncpy_s(dest,src,size) wcsncpy_s((dest),(size),(src),_TRUNCATE)
		# define _tstrncat_s(dest,src,size) wcsncat_s((dest),(size),(src))
	#else
		# define _tstrncpy_s(dest,src,size) strncpy_s((dest),(size),(src),_TRUNCATE)
		# define _tstrncat_s(dest,src,size) wcsncat_s((dest),(src))
	#endif
#else
# define _tstrncpy_s(dest,src,size) strncpy((dest),(src),(size))
#endif

#ifdef _UNICODE
	#define _tstrcmpi(x,y)    _wcsicmp(x, y)
	#define _tstrcmp(x,y)     wcscmp(x, y)
	#define _tstrchr(x,y)     wcschr(x, y)
	#define _tstrrchr(x,y)    wcsrchr(x, y)
	#define _tstrcat(x,y)     wcscat(x,y)
	#define _tstrcpy(x,y)     wcscpy(x,y)
	#define _tstrstr(x,y)     wcsstr(x,y)
	#define _tstrtok(x,y)     wcstok(x,y)
	#define _tstrcspn(x, y)   wcscspn(x,y)
	#define _tstrspn(x, y)    wcsspn(x,y)

    // for these, be aware that 'n' is the # of CHARACTERS, not bytes!!
    #define _tstrlen(x) wcslen(x)
    #define _tstrnicmp(x,y,n) _wcsnicmp(x, y, n)
    #define _tstrncat(x,y,n)  wcsncat(x, y, n)
    #define _tstrncpy(x,y,n)  wcsncpy(x, y, n)
	#define _tstrnlen(x, y)   wcsnlen(x, y)

    #define _tsprintf         swprintf
    #define _tsscanf          swscanf
    //#define _tfopen(x,y)           _wfopen(x,y)
    #define _tfclose(x)          fclose(x)

	#define _tprintf wprintf
#else
	#define _tstrcmpi(x,y)    _stricmp(x, y)
	#define _tstrcmp(x,y)     strcmp(x, y)
	#define _tstrchr(x,y)     strchr(x, y)
	#define _tstrrchr(x,y)    strrchr(x, y)
	#define _tstrcat(x,y)     strcat(x,y)
	#define _tstrcpy(x,y)     strcpy(x,y)
	#define _tstrstr(x,y)     strstr(x,y)
	#define _tstrtok(x,y)     strtok(x,y)
	#define _tstrcspn(x, y)   strcspn(x,y)
	#define _tstrspn(x, y)    strspn(x,y)

    // for these, be aware that 'n' is the # of CHARACTERS, not bytes!!
    #define _tstrlen(x) strlen(x)
    #define _tstrnicmp(x,y,n) _strnicmp(x, y, n)
    #define _tstrncat(x,y,n)  strncat(x, y, n)
    #define _tstrncpy(x,y,n)  strncpy(x, y, n)
	#define _tstrnlen(x, y)   strnlen(x, y)

    #define _tsprintf         sprintf
    #define _tsscanf          sscanf
    //#define _tfopen(x,y)           fopen(x,y)
    #define _tfclose(x)          fclose(x)

	#define _tprintf printf
#endif


///////////////////////////////////////////////////////
#include "STRINGHELPER_Export.h"
/////////
//utility classes
class StringSplitter {
private:
	_tstring m_content;
	_tstring m_pattern;
	bool m_bIgnoringCase;
public:
	const _tstring m_emptyString;
private:
	std::vector<_tstring *> m_results;
private:
	void match(void);
public:
	const _tstring & GetContent(void) const;
	const _tstring & GetPattern(void) const;
	bool isIgnoringCase(void) const;
	//
	void SetContent(const _tstring & content);
	void SetPattern(const _tstring & pattern);
	void SetIgnoringCase(bool bIgnoringCase);
	void Set(const _tstring & content, const _tstring & pattern, bool bIgnoringCase = true);
	//
	size_t GetSplittingSize() const;
	const _tstring &GetSplittingAt(size_t index) const;
	//
private:
	void Clear();
public:
	StringSplitter(void);
	~StringSplitter(void);
};
#include "STRINGHELPER_Deport.h"
///////////////////////////////////////////////////////





///////////////////////////////////////////////////////
#include "STRINGHELPER_Export.h"
class StringReplacer {
public:
	static _tstring Replace(const _tstring & content, const _tstring & pattern, const _tstring & replacement, bool bIgnoringCase = true);
};

class StringSearcher {
public:
	static std::vector<_tstring> Search(const _tstring & content, const _tstring & pattern, size_t nMatchIndex = 0, bool bIgnoringCase = true);
	static std::vector<_tstring> SearchWithReturningPrefixSuffix(const _tstring & content, const _tstring & pattern, size_t nMatchIndex, std::vector<std::pair<_tstring, _tstring> > & vectorPrefixSuffix, bool bIgnoringCase = true);
	static void SearchWithReturningSuffix(const _tstring & content, const _tstring & pattern, size_t nMatchIndex, std::vector<std::pair<_tstring, _tstring> > & vectorMatchSuffix, bool bIgnoringCase = true);
	static _tstring Match(const _tstring & content, const _tstring & pattern, unsigned int nMatchUnitID = 0, bool bIgnoringCase = true);
};

class StringConverter {
public:
	static _tstring ToUpper(const _tstring & sSrc);
	static _tstring ToLower(const _tstring & sSrc);
	static _tstring StringToASCII(const TCHAR * p);
	#ifdef WINDOWS
	static char* BSTRToLPSTR (BSTR bstrIn);
	static char* LPWSTRToLPSTR (LPCWSTR lpwszStrIn);
	static HRESULT __fastcall AnsiToUnicode(LPCSTR pszA, LPOLESTR* ppszW);
	static  int AnsiToUnicode(LPCSTR s,std::wstring & sResults);
	static HRESULT __fastcall UnicodeToAnsi(LPCOLESTR pszW, LPSTR* ppszA);
	static  int UnicodeToAnsi(LPCWSTR s, std::string & sResults);
	#endif
};
#include "STRINGHELPER_Deport.h"
///////////////////////////////////////////////////////




///////////////////////////////////////////////////////
#include "STRINGHELPER_Export.h"
class StringToTCHARPtr {
private:
    int m_nAlgorithm;
private:
    TCHAR * m_pTCHAR;//used for METHOD-I in Reset()
    std::vector<TCHAR> m_vectorTCHAR;//used for METHOD-II and METHOD-III in Reset()
public:
    //
    StringToTCHARPtr();
    //
    StringToTCHARPtr(const _tstring & str, int nAlgorithm = 1);
    //
    ~StringToTCHARPtr();
    //
    void Reset(const _tstring & str, int nAlgorithm = 1);
    //
    operator TCHAR*();
};
#include "STRINGHELPER_Deport.h"
///////////////////////////////////////////////////////




///////////////////////////////////////////////////////
#include "STRINGHELPER_Export.h"
#ifdef WINDOWS
class StringHasher : public stdext::hash_compare <_tstring>
#else
class StringHasher : hash_compare <_tstring>
#endif
{
public:
  size_t operator() (const _tstring & s) const;
  bool operator() (const _tstring & s1, const _tstring & s2) const;
};
#include "STRINGHELPER_Deport.h"
///////////////////////////////////////////////////////






///////////////////////////////////////////////////////
#include "STRINGHELPER_Export.h"
class StringComparer{
private:
	static bool CharLess(TCHAR c1, TCHAR c2);
	static bool CharLessIgnoreCase(TCHAR c1, TCHAR c2);
public:
	static bool Comparision(const _tstring & s1, const _tstring & s2);
	static bool ComparisionByIgnoringCase(const _tstring & s1, const _tstring & s2);
};

class CStrTok
{
public:
	CStrTok();
	virtual ~CStrTok();

	// Attributes
public:
	bool m_bDelimiterAsToken;
	bool m_bOneByOneDelimiter;
	bool m_bDelimitersInSequence;

	TCHAR* m_lpszNext;
	TCHAR m_chDelimiter;
	// Operations
public:
	TCHAR* GetFirst(TCHAR* lpsz, const TCHAR* lpcszDelimiters);
	TCHAR* GetNext(const TCHAR* lpcszDelimiters);
	void SetNext(const TCHAR* lpcszNext);
	bool IsEOB();
	void Break();
	void TrimLeft(TCHAR* &lpsz, const TCHAR* lpcszDelimiters = NULL);
	void TrimRight(const TCHAR* lpcszDelimiters = NULL);

	static bool IsDelimiter(TCHAR ch, const TCHAR* lpcszDelimiters);
};
#include "STRINGHELPER_Deport.h"
///////////////////////////////////////////////////////






///////////////////////////////////////////////////////
#ifdef WINDOWS
#include "STRINGHELPER_Export.h"
class CStringDPXSearcher{
	template <class _RandomAccessIter, class _Integer, class _Tp> inline
	_RandomAccessIter search_n(_RandomAccessIter __first, _RandomAccessIter __last,
						  _Integer __count, const _Tp& __val)
	{
		if (__count <= 0)
			return __first;
		if (__count == 1)
			return std::find(__first, __last, __val);

		typedef std::iterator_traits<_RandomAccessIter>::difference_type iter_diff;

		iter_diff __tailSize = __last - __first;
		const iter_diff __pattSize = __count;

		if (__tailSize >= __pattSize)
		{
			_RandomAccessIter __backTrack;

			iter_diff __remainder, __prevRemainder;
			const iter_diff __skipOffset = __pattSize - 1;

			_RandomAccessIter __lookAhead = __first + __skipOffset;

			for ( ; ; __lookAhead += __pattSize ) // the main loop...
			{
				//__lookAhead here is always pointing to the last element of next possible match.
				assert( __tailSize >= __pattSize );
				__tailSize -= __pattSize;

				for ( ; ; ) // the skip loop...
				{
					if (*__lookAhead == __val)
						break;
					if (__tailSize < __pattSize)
						return __last;

					__lookAhead += __pattSize;
					__tailSize -= __pattSize;
				}

				assert( __tailSize == (__last - __lookAhead) - 1 );
				__remainder = __skipOffset;

				for ( __backTrack = __lookAhead - 1; *__backTrack == __val; --__backTrack )
				{
					if (--__remainder == 0)
						return (__lookAhead - __skipOffset); //Success
				}

				for ( ; ; )
				{
					if (__remainder > __tailSize)
						return __last; //failure

					__lookAhead += __remainder;
					__tailSize -= __remainder;

					if (*__lookAhead == __val)
					{
						__prevRemainder = __remainder;
						__backTrack = __lookAhead;

						do
						{
							if (--__remainder == 0)
								return (__lookAhead - __skipOffset); //Success

						} while ( *--__backTrack == __val);

						//adjust remainder for next comparison
						__remainder += __pattSize - __prevRemainder;
					}
					else
						break;
				}

				//__lookAhead here is always pointing to the element of the last mismatch.
				if (__tailSize < __pattSize)
					return __last;
			}
		}

		return __last; //failure
	}
};
#include "STRINGHELPER_Deport.h"
#endif
///////////////////////////////////////////////////////




///////////////////////////////////////////////////////
#include "STRINGHELPER_Export.h"
class CStringBMSearcher{
public:
	static unsigned long BMSearchSimple(
		char *szContent,
		char *szPattern,
		unsigned long nLenSZContent,
		unsigned long nLenSZPattern,
		_tstring & sError,
		std::vector<unsigned long> & vectorOccurencePos,
		unsigned long nMaxOccurance=0L,
		unsigned long nStartSearchPos=0L,
		unsigned long nEndSearchPos=0L);
};
#include "STRINGHELPER_Deport.h"
///////////////////////////////////////////////////////






///////////////////////////////////////////////////////
#include "STRINGHELPER_Export.h"
class CStringRandomGenerator{
public:
	enum RandomStringType {
		RST_ALLSYMBOLS = 1,
		RST_ALPHANUMERIC,
		RST_ALPHA,
		RST_NUMERIC
	};
public:
	static _tstring RandomString(int iLength, int iType);
};
#include "STRINGHELPER_Deport.h"
///////////////////////////////////////////////////////





#endif//!__STRINGHELPER_H_
