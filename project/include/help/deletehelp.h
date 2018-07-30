/*
The MIT License (MIT)

Copyright (c) 2018 Jorge López González

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

//
//  deletehelp.h
//  DCPP
//
//  Created by Jorge López on 24/3/16.
//
//

#pragma once

#include "vectorhelp.h"
#include <vector>
#include <map>

namespace dc
{
	//-----------------------------------------------------------------------
	// COMMON FUNCTIONS
	//-----------------------------------------------------------------------

	/**
	* Template for safely deleting pointers
	* @param data
	*/
	template <typename T>
	inline
	void SafeDelete(T*& data)
	{
		if (data)
		{
			delete data;
			data = 0;
		}
	}

	/**
	* Template for deleting pointers from maps
	* Calls clear at the end.
	*/
	template <typename K, typename T>
	inline
	void SafeDelete(std::map<K, T*>& m)
	{
		typename std::map<K, T*>::iterator it, end;
		it = m.begin();
		end = m.end();

		for (; it != end; ++it)
		{
			T* element = it->second;

			delete element;
		}

		m.clear();
	}

	/**
	* Template for deleting vector of pointers from maps
	* Calls clear at the end.
	*/
	template <typename K, typename T>
	inline
	void SafeDelete(std::map<K, std::vector<T*>>& m)
	{
		typename std::map<K, std::vector<T*>>::iterator it, end;
		it = m.begin();
		end = m.end();

		for (; it != end; ++it)
		{
			std::vector<T*>& elementList = it->second;
			SafeDelete(elementList);
		}

		m.clear();
	}
}