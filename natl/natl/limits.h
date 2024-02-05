#pragma once 

//std
#include <Limits>

//interface
namespace natl {
	//I just dont like the name 
	template<typename BasicType>
	using Limits = std::numeric_limits<BasicType>;
}