#pragma once 

//std
#include <limits>

//interface
namespace natl {
	//I just dont like the name 
	template<typename BasicType>
	using limits = std::numeric_limits<BasicType>;
}