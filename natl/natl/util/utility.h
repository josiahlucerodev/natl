#pragma once 

//interface
namespace natl {
	[[nodiscard]] constexpr bool isConstantEvaluated() noexcept {
		return std::is_constant_evaluated();
	}
	template<typename ToType, typename FormType>
	ToType& punCast(FormType& from) noexcept {
		return *reinterpret_cast<ToType*>(&from);
	}
}