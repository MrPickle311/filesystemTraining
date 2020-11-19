#pragma once

#include <mutex>
#include <string>
#include <vector>
#include <algorithm>
#include <istream>
#include <ostream>

namespace num
{

	struct MatrixError
	{
		std::string msg_;
		MatrixError(std::string msg) : msg_{msg}
		{}
	};


	inline void error(std::string msg)
	{
		throw MatrixError{msg};
	}

	using Idx = long;

	template<class Type = double ,size_t Dimension = 1>
	class Matrix
	{
		Matrix() = delete;
	};

	template<class Type >
	class MatrixBase
	{

	};







}
