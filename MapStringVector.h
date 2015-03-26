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

#ifndef _MAPSTRINGVECTOR_H_
#define _MAPSTRINGVECTOR_H_

#include <string>
#include <vector>
#include <map>
#include <iostream>

//using namespace std;

///////////////////////////////////////////////////////
#include "STRINGHELPER_Export.h"
template <class T>
class MapStringVector {
private:
	//typedef typename std::map<_tstring, std::vector<T *> *, std::less<_tstring> > MAP_STRING_VECTOR;
	//MAP_STRING_VECTOR m_mapContent;
	//typename MAP_STRING_VECTOR::iterator m_itor;
	//typename MAP_STRING_VECTOR::const_iterator m_constItor;
	std::map<_tstring, std::vector<T *> *, std::less<_tstring> > m_mapContent;
	typename typedef std::map<_tstring, std::vector<T *> *, std::less<_tstring> >::iterator m_itor_type;
	typename typedef std::map<_tstring, std::vector<T *> *, std::less<_tstring> >::const_iterator m_constItor_type;
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
		m_constItor_type it = m_mapContent.find(key);
		if(it != m_mapContent.end()){
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
			m_mapContent[key] = pValue;
		}
		//
		nErrorCode = 0;
		return true;
	}
	//
	size_t GetKeySize() const {
		return m_mapContent.size();
	}
	//
	std::vector<_tstring> GetKeys() const {
		m_constItor_type map_itor = m_mapContent.begin();
		std::vector<string> keys;
		for(; map_itor != m_mapContent.end(); ++map_itor){
			keys.push_back(map_itor->first);	
		}
		//
		return keys;
	}
	//
	void Sort(const _tstring & key, int & nErrorCode){
		nErrorCode = 0;
		m_constItor_type it = 
			m_mapContent.find(key);
		//
		if(it == m_mapContent.end()){
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
		std::sort(pValue->begin(), pValue->end(), MapStringVector::comparer());
	}
	//
	size_t GetVectorSize (const _tstring & key, int & nErrorCode) const{
		m_constItor_type it = 
			m_mapContent.find(key);
		//
		if(it == m_mapContent.end()){
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
			m_mapContent.find(key);
		if(it == m_mapContent.end()){
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
			m_mapContent.find(key);
		if(it == m_mapContent.end()){
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
		if(m_mapContent.empty())
			return;
		//
		m_itor_type map_itor = m_mapContent.begin();
		for(; map_itor != m_mapContent.end(); ++map_itor){
			std::vector<T*>* pValue = map_itor->second;
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
		m_mapContent.clear();
	}
	
public:
	MapStringVector(void){
	}
	//
	~MapStringVector(void){
		Clear();
	}
};
#include "STRINGHELPER_Deport.h"
///////////////////////////////////////////////////////



#endif //!_MAPSTRINGVECTOR_H_