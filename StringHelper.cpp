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

#include "StringHelper.h"

StringSplitter::StringSplitter(void){
	m_bIgnoringCase = true;
}

StringSplitter::~StringSplitter(void){
	Clear();
}

void StringSplitter::Clear(void){
	//clear m_results
	std::vector<_tstring*>::iterator itor1;
	for(itor1 = m_results.begin();
		itor1 != m_results.end(); itor1++){
			delete *itor1;
	}
	m_results.clear();
}

 const _tstring &StringSplitter::GetContent(void) const{
	return m_content;
}

void StringSplitter::SetContent(const _tstring & content){
	if(content != m_content){
		m_content = content;
		match();
	}
}

const _tstring &StringSplitter::GetPattern(void) const{
	return m_pattern;
}

void StringSplitter::SetPattern(const _tstring & pattern){
	if(pattern != m_pattern){
		m_pattern = pattern;
		match();
	}
}

void StringSplitter::Set(const _tstring & content, const _tstring & pattern, bool bIgnoringCase){
	if(content == m_content && pattern == m_pattern && bIgnoringCase){
		return;
	}
	else{
		if(content != m_content){
			m_content = content;
		}
		if(pattern != m_pattern){
			m_pattern = pattern;
		}
		if(bIgnoringCase != m_bIgnoringCase){
			m_bIgnoringCase = bIgnoringCase;
		}
		match();
	}
}

bool StringSplitter::isIgnoringCase(void) const{
	return m_bIgnoringCase;
}

void StringSplitter::SetIgnoringCase(bool bIgnoringCase){
	if(bIgnoringCase != m_bIgnoringCase){
		m_bIgnoringCase = bIgnoringCase;
		match();
	}
}

size_t StringSplitter::GetSplittingSize() const {
	return m_results.size();
}

const _tstring & StringSplitter::GetSplittingAt(size_t index) const {
	if(index < 0 || index >= m_results.size()){
		return StringSplitter::m_emptyString;
	}
	else{
		return *m_results.at(index);
	}
}

void StringSplitter::match(void){
	if(m_content.empty() || m_pattern.empty()){
		return;
	}
	//
	Clear();
	//
	_tregex *pReg;
	//
	pReg=m_bIgnoringCase ? new _tregex(m_pattern, boost::regex::normal | boost::regex::perl) :
		new _tregex(m_pattern, boost::regex::normal | boost::regbase::icase | boost::regex::perl);
	_tregex_token_iterator i(m_content.begin(), m_content.end(), *pReg, -1);
	_tregex_token_iterator j;
	while(i != j){
		m_results.push_back(new _tstring(*i++));
	}
	//
	delete pReg;
}

_tstring StringReplacer::Replace(const _tstring & content, const _tstring & pattern, const _tstring & replacement, bool bIgnoringCase){
	//
	_tregex *pReg;
	pReg=!bIgnoringCase ? new _tregex(pattern, boost::regex::normal | boost::regex::perl) :
		new _tregex(pattern, boost::regex::normal | boost::regbase::icase | boost::regex::perl);
	//
	_tstring replacedContent = boost::regex_replace(content,*pReg, replacement);
	//
	delete pReg;
	//
	return replacedContent;
}

std::vector<_tstring> StringSearcher::Search(const _tstring & content, const _tstring & pattern,
	size_t nMatchIndex, bool bIgnoringCase){
	std::vector<_tstring> results;
	//
	if(content.length() < 1 || pattern.length() < 1){
		return results;
	}
	//std::cout << content.c_str() << _T(",") << pattern.c_str() << std::endl;
	_tregex *pReg;
	pReg=!bIgnoringCase ? new _tregex(pattern, boost::regex::normal | boost::regex::perl) :
		new _tregex(pattern, boost::regex::normal | boost::regbase::icase | boost::regex::perl);
	//
	////////METHOD I
	_tsmatch matches;
	//
	_tstring::const_iterator it = content.begin();
	while (boost::regex_search(it, content.end(), matches, *pReg))
    {
		_tstring rs(matches[nMatchIndex].first, matches[nMatchIndex].second);
		//std::cout << rs.c_str()<<"\n";
        results.push_back(rs);
        // Update the beginning of the range to the character
        // following the match
        it = matches[nMatchIndex].second;
    }

	////////METHOD II
	//boost::match_results<std::string::const_iterator> what;
	//
	//_tstring::const_iterator it = content.begin();
	//while (boost::regex_search(it, content.end(), what, *pReg))
    //{
	//	//std::cout << what.str()<<"\n";
    //    results.push_back(what.str());
    //    // Update the beginning of the range to the character
    //    // following the match
	//	it = what[0].second;
    //}

	////////METHOD III
    //boost::sregex_token_iterator p(content.begin(), content.end(), *pReg, 0);
    //boost::sregex_token_iterator end;

    //for (;p != end; ++p)
    //{
	//	_tstring rs(p->first, p->second);
	//	//std::cout << rs << std::endl;
	//	results.push_back(rs);
    //}


	//
	delete pReg;
	//
	return results;
}

std::vector<_tstring> StringSearcher::SearchWithReturningPrefixSuffix(const _tstring & content, const _tstring & pattern,
	size_t nMatchIndex, std::vector<std::pair<_tstring, _tstring> > & vectorPrefixSuffix, bool bIgnoringCase){
	std::vector<_tstring> results;
	//
	if(content.length() < 1 || pattern.length() < 1){
		return results;
	}
	//std::cout << content.c_str() << _T(",") << pattern.c_str() << std::endl;
	_tregex *pReg;
	pReg=!bIgnoringCase ? new _tregex(pattern, boost::regex::normal | boost::regex::perl) :
		new _tregex(pattern, boost::regex::normal | boost::regbase::icase | boost::regex::perl);
	//
	////////METHOD I
	_tsmatch matches;
	//
	_tstring::const_iterator it = content.begin();
	while (boost::regex_search(it, content.end(), matches, *pReg))
    {
		_tstring rs(matches[nMatchIndex].first, matches[nMatchIndex].second);
		//std::cout << rs.c_str()<<"\n";
        results.push_back(rs);
		//
		std::pair<_tstring, _tstring> pairPrefixSuffix(matches.prefix(), matches.suffix());
		vectorPrefixSuffix.push_back(pairPrefixSuffix);
		//
		if(vectorPrefixSuffix.size() > 1){
			size_t i=vectorPrefixSuffix.size()-2;
			size_t nPos = vectorPrefixSuffix[i].second.find(results[i+1]);
			if(nPos != _tstring::npos){
				vectorPrefixSuffix[i].second = vectorPrefixSuffix[i].second.substr(0, nPos);
			}
		}
		// Update the beginning of the range to the character
        // following the match
        it = matches[nMatchIndex].second;
    }

	////////METHOD II
	//boost::match_results<std::string::const_iterator> what;
	//
	//_tstring::const_iterator it = content.begin();
	//while (boost::regex_search(it, content.end(), what, *pReg))
    //{
	//	//std::cout << what.str()<<"\n";
    //    results.push_back(what.str());
    //    // Update the beginning of the range to the character
    //    // following the match
	//	it = what[0].second;
    //}

	////////METHOD III
    //boost::sregex_token_iterator p(content.begin(), content.end(), *pReg, 0);
    //boost::sregex_token_iterator end;

    //for (;p != end; ++p)
    //{
	//	_tstring rs(p->first, p->second);
	//	//std::cout << rs << std::endl;
	//	results.push_back(rs);
    //}

	//for(size_t i=0; results.size() > 0 && vectorPrefixSuffix.size() > 0 && i<vectorPrefixSuffix.size()-1 && i<results.size()-1; ++i){
	//	size_t nPos = vectorPrefixSuffix[i].second.find(results[i+1]);
	//	if(nPos != _tstring::npos){
	//		vectorPrefixSuffix[i].second = vectorPrefixSuffix[i].second.substr(0, nPos);
	//	}
	//}
	//
	delete pReg;
	//
	return results;
}

void StringSearcher::SearchWithReturningSuffix(const _tstring & content, const _tstring & pattern,
	size_t nMatchIndex, std::vector<std::pair<_tstring, _tstring> > & vectorMatchSuffix, bool bIgnoringCase){
	//
	if(content.length() < 1 || pattern.length() < 1){
		return;
	}
	//std::cout << content.c_str() << _T(",") << pattern.c_str() << std::endl;
	_tregex *pReg;
	pReg=!bIgnoringCase ? new _tregex(pattern, boost::regex::normal | boost::regex::perl) :
		new _tregex(pattern, boost::regex::normal | boost::regbase::icase | boost::regex::perl);
	//
	////////METHOD I
	_tsmatch matches;
	//
	_tstring::const_iterator it = content.begin();
	while (boost::regex_search(it, content.end(), matches, *pReg))
    {
		_tstring rs(matches[nMatchIndex].first, matches[nMatchIndex].second);
		//std::cout << rs.c_str()<<"\n";
		//
		std::pair<_tstring, _tstring> pairMatchSuffix(rs, matches.suffix());
		vectorMatchSuffix.push_back(pairMatchSuffix);
		//
		if(vectorMatchSuffix.size() > 1){
			size_t i =  vectorMatchSuffix.size() - 2;
			size_t nPos = vectorMatchSuffix[i].second.find(vectorMatchSuffix[i+1].first);
			if(nPos != _tstring::npos){
				vectorMatchSuffix[i].second = vectorMatchSuffix[i].second.substr(0, nPos);
			}
		}
		// Update the beginning of the range to the character
        // following the match
        it = matches[nMatchIndex].second;
    }

	////////METHOD II
	//boost::match_results<std::string::const_iterator> what;
	//
	//_tstring::const_iterator it = content.begin();
	//while (boost::regex_search(it, content.end(), what, *pReg))
    //{
	//	//std::cout << what.str()<<"\n";
    //    results.push_back(what.str());
    //    // Update the beginning of the range to the character
    //    // following the match
	//	it = what[0].second;
    //}

	////////METHOD III
    //boost::sregex_token_iterator p(content.begin(), content.end(), *pReg, 0);
    //boost::sregex_token_iterator end;

    //for (;p != end; ++p)
    //{
	//	_tstring rs(p->first, p->second);
	//	//std::cout << rs << std::endl;
	//	results.push_back(rs);
    //}

	//for(size_t i=0; vectorMatchSuffix.size() > 0 && i<vectorMatchSuffix.size()-1; ++i){
	//	size_t nPos = vectorMatchSuffix[i].second.find(vectorMatchSuffix[i+1].first);
	//	if(nPos != _tstring::npos){
	//		vectorMatchSuffix[i].second = vectorMatchSuffix[i].second.substr(0, nPos);
	//	}
	//}
	//
	delete pReg;
	//
}


_tstring StringSearcher::Match(
	const _tstring & content,
	const _tstring & pattern,
	unsigned int nMatchUnitID,
	bool bIgnoringCase){
	_tstring results;
	//
	if(content.length() < 1 || pattern.length() < 1){
		return results;
	}
	_tregex *pReg;
	pReg=!bIgnoringCase ? new _tregex(pattern, boost::regex::normal | boost::regex::perl) :
		new _tregex(pattern, boost::regex::normal | boost::regbase::icase | boost::regex::perl);
	//
	////////METHOD I
	_tsmatch matches;
	//
	if(boost::regex_match(content, matches, *pReg))
    {
		_tstring rs(matches[nMatchUnitID].first, matches[nMatchUnitID].second);
		//std::cout << rs.c_str()<<"\n";
        results = rs;
    }

	////////METHOD II
	//boost::match_results<std::string::const_iterator> what;
	//
	//if (boost::regex_search(content, what, *pReg))
    //{
	//	//std::cout << what.str()<<"\n";
    //    results = what.str();
    //}

	////////METHOD III
    //boost::sregex_token_iterator p(content, *pReg, 0);
    //boost::sregex_token_iterator end;

    //if (p != end)
    //{
	//	_tstring rs(p->first, p->second);
	//	//std::cout << rs << std::endl;
	//	results = rs;
    //}


	//
	delete pReg;
	//
	return results;
}

_tstring StringConverter::ToLower(const _tstring & sSrc){
	_tstring sDest = sSrc;;
	for(size_t i = 0; i< sSrc.length(); ++i){
		if(!std::islower(sSrc[i]))
			sDest[i] = std::tolower(sSrc[i]);
	}
	return sDest;
}

_tstring StringConverter::ToUpper(const _tstring & sSrc){
	_tstring sDest = sSrc;;
	for(size_t i = 0; i< sSrc.length(); ++i){
		if(!std::isupper(sSrc[i]))
			sDest[i] = std::toupper(sSrc[i]);
	}
	return sDest;
}

///////////////////////////////////////////////////////
#ifdef WINDOWS
char* StringConverter::BSTRToLPSTR (BSTR bstrIn)
{
	//BSTR is usually allocated with SysAllocString
	//and freed with SysFreeString
    LPSTR pszOut = NULL;
    if (bstrIn != NULL)
    {
	    int nInputStrLen = SysStringLen (bstrIn);

		// Double NULL Termination
		int nOutputStrLen = WideCharToMultiByte(CP_ACP, 0, bstrIn, nInputStrLen, NULL, 0, 0, 0) + 2;
		pszOut = new char [nOutputStrLen];

		if (pszOut)
		{
		  memset (pszOut, 0x00, sizeof (char)*nOutputStrLen);
		  WideCharToMultiByte (CP_ACP, 0, bstrIn, nInputStrLen, pszOut, nOutputStrLen, 0, 0);
		}
    }
    return pszOut;
}
#endif
///////////////////////////////////////////////////////

///////////////////////////////////////////////////////
#ifdef WINDOWS
char* StringConverter::LPWSTRToLPSTR (LPCWSTR lpwszStrIn)
{
    LPSTR pszOut = NULL;
    if (lpwszStrIn != NULL)
    {
		int nInputStrLen = wcslen (lpwszStrIn);

		// Double NULL Termination
		int nOutputStrLen = WideCharToMultiByte (CP_ACP, 0, lpwszStrIn, nInputStrLen, NULL, 0, 0, 0) + 2;
		pszOut = new char [nOutputStrLen];

		if (pszOut)
		{
		  memset (pszOut, 0x00, nOutputStrLen);
		  WideCharToMultiByte(CP_ACP, 0, lpwszStrIn, nInputStrLen, pszOut, nOutputStrLen, 0, 0);
		}
    }
    //use delete [] to free the memory of pszOut
    return pszOut;
}
#endif
///////////////////////////////////////////////////////

///////////////////////////////////////////////////////
#ifdef WINDOWS
/*
 * AnsiToUnicode converts the ANSI string pszA to a Unicode string
 * and returns the Unicode string through ppszW. Space for the
 * the converted string is allocated by AnsiToUnicode.
 * This one should replace StringConverter::BSTRToLPSTR(BSTR bstrIn) for com+
 */
HRESULT __fastcall StringConverter::AnsiToUnicode(LPCSTR pszA, LPOLESTR* ppszW)
{
    ULONG cCharacters;
    DWORD dwError;

    // If input is null then just return the same.
    if (NULL == pszA)
    {
        *ppszW = NULL;
        return NOERROR;
    }

    // Determine number of wide characters to be allocated for the
    // Unicode string.
    cCharacters =  strlen(pszA)+1;

    // Use of the OLE allocator is required if the resultant Unicode
    // string will be passed to another COM component and if that
    // component will free it. Otherwise you can use your own allocator.
	//you use CoTaskMemFree to free the memory block.
    *ppszW = (LPOLESTR) CoTaskMemAlloc(cCharacters*2);
    if (NULL == *ppszW)
        return E_OUTOFMEMORY;

    // Covert to Unicode.
    if (0 == MultiByteToWideChar(CP_ACP, 0, pszA, cCharacters,
                  *ppszW, cCharacters))
    {
        dwError = GetLastError();
        CoTaskMemFree(*ppszW);
        *ppszW = NULL;
        return HRESULT_FROM_WIN32(dwError);
    }
    return NOERROR;
}
#endif
///////////////////////////////////////////////////////

///////////////////////////////////////////////////////
#ifdef WINDOWS
int StringConverter::AnsiToUnicode(LPCSTR s, std::wstring & sResults){
	if (s==NULL)
		return -1;
	//
	int nCS=lstrlen(s);
	if (nCS==0) {
		return -2;
	}
	//
	int nCharacters = (nCS+1)*sizeof(WCHAR);
	WCHAR *pszW=new WCHAR[nCharacters];
	if(pszW == NULL)
		return -4;
	//
	int nCC = MultiByteToWideChar(CP_ACP, 0, s, nCharacters,
                  pszW, nCharacters);
	if (0 == nCC)
        return -5;
	//
	if (nCC==0) {
		delete[] pszW;
		return -6;
	}
	pszW[nCC*sizeof(WCHAR)]=L'\0';
	//
	sResults = pszW;
	//
	delete[] pszW;
	return 1;
}
#endif
///////////////////////////////////////////////////////

///////////////////////////////////////////////////////
#ifdef WINDOWS
/*
 * UnicodeToAnsi converts the Unicode string pszW to an ANSI string
 * and returns the ANSI string through ppszA. Space for the
 * the converted string is allocated by UnicodeToAnsi.
 */
HRESULT __fastcall StringConverter::UnicodeToAnsi(LPCOLESTR pszW, LPSTR* ppszA)
{

    ULONG cbAnsi, cCharacters;
    DWORD dwError;

    // If input is null then just return the same.
    if (pszW == NULL)
    {
        *ppszA = NULL;
        return NOERROR;
    }

    cCharacters = wcslen(pszW)+1;
    // Determine number of bytes to be allocated for ANSI string. An
    // ANSI string can have at most 2 bytes per character (for Double
    // Byte Character Strings.)
    cbAnsi = cCharacters*2;

    // Use of the OLE allocator is not required because the resultant
    // ANSI  string will never be passed to another COM component. You
    // can use your own allocator. If you use CoTaskMemAlloc, you use CoTaskMemFree
	// to free the memory block.
    *ppszA = (LPSTR) CoTaskMemAlloc(cbAnsi);
    if (NULL == *ppszA)
        return E_OUTOFMEMORY;

    // Convert to ANSI.
    if (0 == WideCharToMultiByte(CP_ACP, 0, pszW, cCharacters, *ppszA,
                  cbAnsi, NULL, NULL))
    {
        dwError = GetLastError();
        CoTaskMemFree(*ppszA);
        *ppszA = NULL;
        return HRESULT_FROM_WIN32(dwError);
    }
    return NOERROR;

}
#endif
///////////////////////////////////////////////////////

///////////////////////////////////////////////////////
#ifdef WINDOWS
int StringConverter::UnicodeToAnsi(LPCWSTR s, std::string & sResults){
	if (s==NULL)
		return -1;
	//
	int nCW=lstrlenW(s);
	if (nCW==0) {
		return -2;
	}
	//
	int nCC=WideCharToMultiByte(CP_ACP,0,s,nCW,NULL,0,NULL,NULL);
	if (nCC==0)
		return -3;
	CHAR *pSZ=new CHAR[nCC+1];
	if(pSZ == NULL)
		return -4;
	//
	nCC=WideCharToMultiByte(CP_ACP,0,s,nCW,pSZ,nCC,NULL,NULL);
	if (nCC==0) {
		delete[] pSZ;
		return -5;
	}
	pSZ[nCC]='\0';
	//
	sResults = pSZ;
	//
	delete[] pSZ;
	return 1;
}
#endif
///////////////////////////////////////////////////////

///////////////////////////////////////////////////////
#ifdef WINDOWS
_tstring StringConverter::StringToASCII(const TCHAR *p){
	_tstring res;
	TCHAR end = (TCHAR)'\0';
	for(; *p != end; p++){
		TCHAR asc[20];
		sprintf(asc, _T("%d"), int(*p));
		res.append(asc);
	}
	return res;
}
#endif
///////////////////////////////////////////////////////

StringToTCHARPtr::StringToTCHARPtr(){
    m_nAlgorithm = 0;
    m_pTCHAR = NULL;
}

StringToTCHARPtr::StringToTCHARPtr(const _tstring & str, int nAlgorithm){
    m_pTCHAR = NULL;
    Reset(str, nAlgorithm);
}

StringToTCHARPtr::~StringToTCHARPtr(){
    if(m_pTCHAR != NULL){
        delete []m_pTCHAR;
    }
    //
    if(m_vectorTCHAR.size() > 0){
        m_vectorTCHAR.clear();
    }
}

void StringToTCHARPtr::Reset(const _tstring & str, int nAlgorithm){
    if(m_pTCHAR != NULL){
        delete []m_pTCHAR;
    }
    //
    if(m_vectorTCHAR.size() > 0){
        m_vectorTCHAR.clear();
    }
    //
    if(str.length() < 1)
        return;
    //METHOD-I
    if(nAlgorithm==0){
        m_pTCHAR = new TCHAR[str.length() + 1];
        std::memset(m_pTCHAR, NULL, str.length() + 1);
        _tstrcpy(m_pTCHAR,str.c_str());
    }
    //METHOD-II
    else if(nAlgorithm == 1){
        m_vectorTCHAR.reserve(str.length() + 1);
        m_vectorTCHAR = std::vector<TCHAR>(str.begin(), str.end());
        m_vectorTCHAR.push_back(NULL);
        //std::vector<TCHAR> buffer(str.c_str(), std.c_str()+str.length() + 1);
        //m_vectorTCHAR = buffer;
    }
    //METHOD-III
    else if(nAlgorithm == 2){
        m_vectorTCHAR.reserve(str.length() + 1);
        for(size_t i=0; i<str.length(); ++i){
            m_vectorTCHAR.push_back(str[i]);
        }
        m_vectorTCHAR.push_back(NULL);
    }
    //
    m_nAlgorithm = nAlgorithm;
}

StringToTCHARPtr::operator TCHAR*(){
    switch (m_nAlgorithm){
        case 0:
        {
            return m_pTCHAR;
        }
        case 1:
        {
            if(m_vectorTCHAR.size() > 0){
                return &*m_vectorTCHAR.begin();
            }
            else{
                return NULL;
            }
        }
        case 2:
        {
            if(m_vectorTCHAR.size() > 0){
                return &*m_vectorTCHAR.begin();
            }
            else{
                return NULL;
            }
        }
        default:
        {
            return NULL;
        }
    }
}

size_t StringHasher::operator() (const _tstring & s) const {
	size_t h = 0;
    _tstring::const_iterator p, p_end;
    for(p = s.begin(), p_end = s.end(); p != p_end; ++p)
    {
      h = 31 * h + (*p);
    }
    return h;
}

bool StringHasher::operator() (const _tstring & s1, const _tstring & s2) const {
    return s1 < s2;
}

bool StringComparer::CharLess(TCHAR c1, TCHAR c2){
	return c1 < c2;
}

bool StringComparer::CharLessIgnoreCase(TCHAR c1, TCHAR c2){
	return (std::tolower( static_cast<unsigned char>(c1)) < std::tolower( static_cast<unsigned char>(c2)));
}

bool StringComparer::Comparision(const _tstring & s1, const _tstring & s2){
	return std::lexicographical_compare( s1.begin(), s1.end(),
		s2.begin(), s2.end(),
		CharLess);
}

bool StringComparer::ComparisionByIgnoringCase(const _tstring & s1, const _tstring & s2){
	return std::lexicographical_compare( s1.begin(), s1.end(),
		s2.begin(), s2.end(), CharLessIgnoreCase);
}

CStrTok::CStrTok()
{
	m_bDelimiterAsToken = m_bOneByOneDelimiter = m_bDelimitersInSequence = false;
	m_chDelimiter = 0;
	m_lpszNext = NULL;
}

CStrTok::~CStrTok()
{

}

char* CStrTok::GetFirst(char* lpsz, const char* lpcszDelimiters)
{
	Break();

	m_lpszNext = lpsz;
	return GetNext(lpcszDelimiters);
}

char* CStrTok::GetNext(const char* lpcszDelimiters)
{
	if(m_lpszNext == NULL)
		return NULL;

	// return the saved delimiter
	if(m_chDelimiter != 0)
		*m_lpszNext = m_chDelimiter;

	char* lpsz = m_lpszNext;
	if(m_bDelimiterAsToken)
		// increment the next pointer to the next delimiter or token
		if(m_bDelimitersInSequence)
		{
			int nLength = _tstrlen(lpcszDelimiters);
			if(memcmp(lpcszDelimiters, m_lpszNext, nLength) == 0)
				m_lpszNext += nLength;
			else
				m_lpszNext = _tstrstr(m_lpszNext, lpcszDelimiters); // may be NULL
		}
		else
		{
			if(_tstrchr(lpcszDelimiters, *m_lpszNext))
				m_lpszNext++;
			else
				m_lpszNext += _tstrcspn(m_lpszNext, lpcszDelimiters);
		}
	else
		if(m_bDelimitersInSequence)
		{
			int nLength = _tstrlen(lpcszDelimiters);
			// increment the token pointer
			while(memcmp(lpcszDelimiters, lpsz, nLength) == 0)
			{
				lpsz += nLength;
				if(m_bOneByOneDelimiter)
					break;
			}
			// increment the next pointer after the end of the token
			m_lpszNext = _tstrstr(lpsz, lpcszDelimiters); // may be NULL
		}
		else
		{
			// increment the token pointer to the start of the token
			if(!m_bOneByOneDelimiter)
				lpsz += _tstrspn(lpsz, lpcszDelimiters);
			else
				if(_tstrchr(lpcszDelimiters, *lpsz) && m_chDelimiter)
					lpsz++;
			// increment the next pointer after the end of the token
			m_lpszNext = lpsz + _tstrcspn(lpsz, lpcszDelimiters);
		}

	if(m_lpszNext == NULL || *m_lpszNext == 0)
	{	// reach the end of the buffer
		m_chDelimiter = 0;
		m_lpszNext = NULL;
	}
	else
	{	// save the delimiter and terminate the token by null
		m_chDelimiter = *m_lpszNext;
		*m_lpszNext = 0;
	}
	if(*lpsz == 0 && !m_bOneByOneDelimiter)
		return NULL;
	return lpsz;
}

void CStrTok::SetNext(const char* lpcszNext)
{
	if(m_chDelimiter != 0)
		*m_lpszNext = m_chDelimiter;
	m_chDelimiter = 0;
	m_lpszNext = (char*)lpcszNext;
}

bool CStrTok::IsEOB()
{
	return m_lpszNext == NULL;
}

bool CStrTok::IsDelimiter(char ch, const char* lpcszDelimiters)
{
	return _tstrchr(lpcszDelimiters, ch) != NULL;
}

void CStrTok::Break()
{
	SetNext(NULL);
}

void CStrTok::TrimLeft(char* &lpsz, const char* lpcszDelimiters /*= NULL*/)
{
	if(m_lpszNext == NULL)
		return;
	if(lpcszDelimiters == NULL)
		lpcszDelimiters = " \t\r\n";
	while(_tstrchr(lpcszDelimiters, *lpsz))
		lpsz++;
}

void CStrTok::TrimRight(const char* lpcszDelimiters /*= NULL*/)
{
	if(m_lpszNext == NULL)
		return;
	if(lpcszDelimiters == NULL)
		lpcszDelimiters = " \t\r\n";
	char* pNext = m_lpszNext-1;
	while(_tstrchr(lpcszDelimiters, *pNext))
		*pNext = 0, pNext--;
}

unsigned long CStringBMSearcher::BMSearchSimple(
	char *szContent,
	char *szPattern,
	unsigned long nLenSZContent,
	unsigned long nLenSZPattern,
	_tstring & sError,
	std::vector<unsigned long> & vectorOccurencePos,
	unsigned long nMaxOccurance,
	unsigned long nStartSearchPos,
	unsigned long nEndSearchPos){
	//
	vectorOccurencePos.clear();
	//
	unsigned int i,nTable[256];
	unsigned long nPatternLen=nLenSZPattern;
	//
	unsigned long nComparisonNeeded=0;
	//
	//if(strlen(szContent) != nLenSZContent){
	//	std::stringstream ss;
	//	ss <<_T("size of szContent != nLenSZContent:") << strlen(szContent) <<_T(",") << nLenSZContent;
	//	sError.append(ss.str().c_str());
	//	return nComparisonNeeded;
	//}
	//
	//if(strlen(szPattern) != nLenSZPattern){
	//	std::stringstream ss;
	//	ss <<_T("size of szPattern != nLenSZPattern:") << strlen(szPattern) <<_T(",") << nLenSZPattern;
	//	sError.append(ss.str().c_str());
	//	return nComparisonNeeded;
	//}
	//
	//
	if(nStartSearchPos > 0L && nStartSearchPos > nLenSZContent-1){
		std::stringstream ss;
		ss <<_T("nStartSearchPos > nLenSZContent-1:") << nStartSearchPos <<_T(",") << nLenSZContent-1;
		sError.append(ss.str().c_str());
		return nComparisonNeeded;
	}
	if(nEndSearchPos > 0L && nEndSearchPos > nLenSZContent-1){
		std::stringstream ss;
		ss <<_T("nEndSearchPos > nLenSZContent-1:") << nEndSearchPos <<_T(",") << nLenSZContent-1;
		sError.append(ss.str().c_str());
		return nComparisonNeeded;
	}
	if(nStartSearchPos > 0L &&  nEndSearchPos > 0L && nStartSearchPos > nEndSearchPos){
		std::stringstream ss;
		ss <<_T("nStartSearchPos > nEndSearchPos:") << nStartSearchPos <<_T(",") << nEndSearchPos;
		sError.append(ss.str().c_str());
		return nComparisonNeeded;
	}
	//
	for(i=0;i<256;i++)
		nTable[i]=nPatternLen;
	for(i=0;i<nPatternLen;i++)
		nTable[szPattern[i]]=nPatternLen-(i+1);
	//unsigned long ptct=nPatternLen-1;
	unsigned long ptct=nStartSearchPos>0?(nStartSearchPos+nPatternLen-1):nPatternLen-1;
	//while(ptct<nLenSZContent){
	//while(nEndSearchPos>0?ptct<=nEndSearchPos:ptct<nLenSZContent){
	//because the last char could still be pattern, we change to:
	unsigned long nEndPTCT = nEndSearchPos>0?nEndSearchPos:nLenSZContent-1;
	while(ptct<=nEndPTCT){
		unsigned int nCount=0;
		while(nCount<nPatternLen){
			ASSERT((ptct-nCount) < nLenSZContent
				&&
				(nPatternLen-1-nCount) < nPatternLen
				);
			//
			if(szContent[ptct-nCount]!=szPattern[nPatternLen-1-nCount]){
				nComparisonNeeded++;
				break;
			}
			else nCount++;
		}
		//
		if(nCount==nPatternLen){
			unsigned long nExactMatchPos = ptct-nCount+1;
			//vectorOccurencePos.push_back(ptct-nCount+1);
			vectorOccurencePos.push_back(nExactMatchPos);
			//
			if(nMaxOccurance > 0L && vectorOccurencePos.size() >= nMaxOccurance){
				return nComparisonNeeded;
			}
			//
			ptct+=nPatternLen;
		}
		else{
			//ptct+=(nTable[szContent[ptct-nCount]]-nCount);
			//when (nTable[szContent[ptct-nCount]]-nCount) <=0, a dead loop will appear,
			//so we need to change to:
			//ptct+=(nTable[szContent[ptct-nCount]]-nCount)>0?(nTable[szContent[ptct-nCount]]-nCount):1;
			//because nTable[szContent[ptct-nCount]]-nCount could overflow, change to:
			ASSERT((ptct-nCount) < nLenSZContent);
			ptct+=nTable[szContent[ptct-nCount]]>nCount?(nTable[szContent[ptct-nCount]]-nCount):1;
		}

		//
		if(ptct>nEndPTCT){
			break;
		}
		//std::cout << "(" << nCount << "," << ptct << "," << nEndPTCT << "," << (ptct>nEndPTCT ? "true":"false") << "," << nLenSZContent << ") ";
	}
	return nComparisonNeeded;
}

_tstring CStringRandomGenerator::RandomString(int iLength, int iType){
	_tstring strReturn;
    _tstring strLocal;

    for( int i = 0 ; i < iLength ; ++i )
    {
        int iNumber;

        // Seed the random-number generator with TickCount so that
        // the numbers will be different every time we run.
        #ifdef WINDOWS
        srand( (unsigned int)( (i+1)*iLength*GetTickCount() ) );
        #else
        struct timeval time_now;
        gettimeofday(&time_now, NULL);
        long dt=1e6*time_now.tv_sec +time_now.tv_usec;
        srand( (unsigned int)( (i+1)*iLength*dt) );
        #endif

        switch( iType )
        {
            case 1:
				{
                iNumber = rand()%122;
                if( 48 > iNumber )
                    iNumber += 48;
                if( ( 57 < iNumber ) &&
                    ( 65 > iNumber ) )
                    iNumber += 7;
                if( ( 90 < iNumber ) &&
                    ( 97 > iNumber ) )
                    iNumber += 6;
                strReturn += (char)iNumber;
				}
                break;
            case 2:
				{
                iNumber = rand()%122;
                if( 65 > iNumber )
                    iNumber = 65 + iNumber%56;
                if( ( 90 < iNumber ) &&
                    ( 97 > iNumber ) )
                    iNumber += 6;
                strReturn += (char)iNumber;
				}
                break;
            case 3:
				{
				std::stringstream ss;
				ss << rand()%9;
				strLocal = ss.str();
                strReturn += strLocal;
				}
                break;
            default:
				{
                strReturn += (char)rand();
				}
                break;
        }
    }

    return strReturn;
}

