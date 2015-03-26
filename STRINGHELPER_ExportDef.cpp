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

#ifdef _MSC_VER
#pragma once
#endif

#ifndef _EXPORT_STRINGHELPER_CPP_
#define _EXPORT_STRINGHELPER_CPP_

#ifndef STRINGHELPER_LIBRARY_STATIC

#ifdef WINDOWS
	//The following will ensure that we are exporting our C++ classes when 
	//building the DLL and importing the classes when build an application 
	//using this DLL.
	#define _STRINGHELPER_DLLAPI_
	#define _STRINGHELPER_NOAUTOLIB_
#endif

#endif //!STRINGHELPER_LIBRARY_STATIC

#endif //!_EXPORT_STRINGHELPER_CPP_