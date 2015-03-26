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

#ifndef _HASHSTRINGVECTOR_H_
#define _HASHSTRINGVECTOR_H_

#include <string>
#include <vector>
#ifdef WINDOWS
    #include <hash_map>
#else
    #ifdef __GNUC__
        #include <ext/hash_map>
    #else
        #include <hash_map>
    #endif
#endif
#include <iostream>

//using namespace std;

///////////////////////////////////////////////////////
#ifdef WINDOWS
#include "STRINGHELPER_Export.h"
template <class T>
class HashStringVector {
private:
	//typedef typename stdext::hash_map<_tstring, std::vector<T *> *, StringHasher> HASH_STRING_VECTOR;
	//HASH_STRING_VECTOR m_hashContent;
	//typename HASH_STRING_VECTOR::iterator m_itor;
	//typename HASH_STRING_VECTOR::const_iterator m_constItor;
	stdext::hash_map<_tstring, std::vector<T *> *, StringHasher> m_hashContent;
	typename typedef stdext::hash_map<_tstring, std::vector<T *> *, StringHasher>::iterator m_itor_type;
	typename typedef stdext::hash_map<_tstring, std::vector<T *> *, StringHasher>::const_iterator m_constItor_type;
	//
	const T m_emptyT;

private:
	//static struct cmp : public binary_function<T*, T*, bool>{
	static struct comparer{
		bool operator()( T*  pLeft, T* pRight){
			return *pLeft < *pRight;
		}
	};

public:
	bool Insert(const _tstring & key, const T & t, int & nErrorCode, bool bForceValueUnique = true){
		if(key.length() < 1){
			nErrorCode = -1;
			return false;
		}
		//
		m_constItor_type it = m_hashContent.find(key);
		if(it != m_hashContent.end()){
			std::vector<T *> * pValue = it->second;
			if(pValue == NULL){
				nErrorCode = -3;
				return false;
			}
			//
			if(bForceValueUnique){
				typename std::vector<T *>::const_iterator it_t = pValue->begin();
				for(; it_t != pValue->end(); ++it_t){
					if(**it_t == t){
						nErrorCode = -4;
						return true;
					}
				}
			}
			//
			T * pT = new T();
			*pT = t;
			pValue->push_back(pT);
		}
		else{
			T * pT = new T();
			*pT = t;
			std::vector<T *> * pValue = new std::vector<T *>();
			pValue->push_back(pT);
			m_hashContent[key] = pValue;
		}
		//
		nErrorCode = 0;
		return true;
	}
	//
	size_t GetKeySize() const {
		return m_hashContent.size();
	}
	//
	std::vector<_tstring> GetKeys() const {
		m_constItor_type hash_itor = m_hashContent.begin();
		std::vector<string> keys;
		for(; hash_itor != m_hashContent.end(); ++hash_itor){
			keys.push_back(hash_itor->first);
		}
		//
		return keys;
	}
	//
	void Sort(const _tstring & key, int & nErrorCode){
		nErrorCode = 0;
		m_constItor_type it =
			m_hashContent.find(key);
		//
		if(it == m_hashContent.end()){
			nErrorCode = -1;
			return;
		}
		//
		std::vector<T *> * pValue = it->second;
		if(pValue == NULL){
			nErrorCode = -2;
			return;
		}
		//
		std::sort(pValue->begin(), pValue->end(), HashStringVector::comparer());
	}
	//
	size_t GetVectorSize (const _tstring & key, int & nErrorCode) const{
		m_constItor_type it =
			m_hashContent.find(key);
		//
		if(it == m_hashContent.end()){
			nErrorCode = -1;
			return 0;
		}
		//
		std::vector<T *> * pValue = it->second;
		if(pValue == NULL){
			nErrorCode = -2;
			return 0;
		}
		//
		nErrorCode = 0;
		return pValue->size();
	}
	//
	T * const GetElementPtr(const _tstring & key, const size_t & nIndex, int & nErrorCode) const{
		size_t nSize = GetVectorSize(key, nErrorCode);
		if(nSize < 1){
			nErrorCode = -1;
			return NULL;
		}
		//
		if(nIndex < 0){
			nErrorCode = -2;
			return NULL;
		}
		//
		if(nIndex >= nSize){
			nErrorCode = -3;
			return NULL;
		}
		//
		m_constItor_type it =
			m_hashContent.find(key);
		if(it == m_hashContent.end()){
			nErrorCode = -4;
			return NULL;
		}
		//
		std::vector<T *> * pValue = it->second;
		if(pValue == NULL){
			nErrorCode = -5;
			return NULL;
		}
		//
		nErrorCode = 0;
		return (*pValue)[nIndex];
	}
	//
	const T & GetElement (const _tstring & key, const size_t & nIndex, int & nErrorCode) const{
		size_t nSize = GetVectorSize(key, nErrorCode);
		if(nSize < 1){
			nErrorCode = -1;
			return m_emptyT;
		}
		//
		if(nIndex < 0){
			nErrorCode = -2;
			return m_emptyT;
		}
		//
		if(nIndex >= nSize){
			nErrorCode = -3;
			return m_emptyT;
		}
		//
		m_constItor_type it =
			m_hashContent.find(key);
		if(it == m_hashContent.end()){
			nErrorCode = -4;
			return m_emptyT;
		}
		//
		std::vector<T *> * pValue = it->second;
		if(pValue == NULL){
			nErrorCode = -5;
			return m_emptyT;
		}
		//
		nErrorCode = 0;
		return *(*pValue)[nIndex];
	}
	//
	void Clear()
	{
		if(m_hashContent.empty())
			return;
		//
		m_itor_type hash_itor = m_hashContent.begin();
		for(; hash_itor != m_hashContent.end(); ++hash_itor){
			std::vector<T*>* pValue = hash_itor->second;
			if(pValue == NULL)
				continue;
			//release memory of vector pointer
			if(!pValue->empty()){
				typename typedef std::vector<T*>::iterator it_type;
				it_type it = pValue->begin();
				for(; it != pValue->end(); ++it){
					delete *it;
				}
			}
			//
			delete pValue;
		}
		//
		m_hashContent.clear();
	}

public:
	HashStringVector(void){
	}
	//
	~HashStringVector(void){
		Clear();
	}
};
#include "STRINGHELPER_Deport.h"
#endif
///////////////////////////////////////////////////////



#endif //!_HASHSTRINGVECTOR_H_
