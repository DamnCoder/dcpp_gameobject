/*
The MIT License (MIT)

Copyright (c) 2018 Jorge López González

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

//
//  vectorhelp.h
//  DCPP
//
//  Created by Jorge López on 24/3/16.
//
//
#pragma once

#include <vector>

namespace dc
{
	/**
	* Template for deleting pointers from vectors
	* Calls clear at the end.
	*/
	template<typename T>
	inline
	void SafeDelete(std::vector<T*>& v)
	{
		typename std::vector<T*>::iterator it, end;
		it = v.begin();
		end = v.end();

		for (; it != end; ++it)
		{
			delete (*it);
		}

		v.clear();
	}

	template<typename T>
	inline
	void Remove(std::vector<T>& list, T element)
	{
		typename std::vector<T>::iterator end = list.end();
		typename std::vector<T>::iterator it = std::find(list.begin(), end, element);

		if (it != end)
		{
			list.erase(it);
		}
	}

	template<typename T>
	inline
	const bool Exists(std::vector<T>& list, T element)
	{
		typename std::vector<T>::iterator end = list.end();
		typename std::vector<T>::iterator it = std::find(list.begin(), end, element);

		return (it != end);
	}
}