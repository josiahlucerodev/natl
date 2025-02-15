#pragma once 

//own
#include "base.h"
#include "../container/string.h"

//interface 
namespace natl {
	template<typename IdNumberType>
		requires(IsBuiltInUnsignedIntegerC<IdNumberType>)
	struct SerializeJumpId {
		IdNumberType number;
		String name;

		//constructor
		constexpr SerializeJumpId() noexcept = default;
		constexpr SerializeJumpId(const ConstAsciiStringView& nameIn) noexcept
			requires(IsSameC<Decay<IdNumberType>, natl::Size>) : name(nameIn) {
			number = hashValue(nameIn);
		}
		constexpr SerializeJumpId(const ConstAsciiStringView& nameIn, const IdNumberType& numberIn) noexcept
			: name(nameIn), number(numberIn) {
		}

		//destructor
		constexpr ~SerializeJumpId() noexcept = default;
	};

	namespace impl {
		struct SerializeJumpSaveIdTag {};
		struct SerializeJumpLocationTag {};
	}
	using SerializeJumpSaveId = StrongType<Size, impl::SerializeJumpSaveIdTag>;
	using SerializeJumpLocation = StrongType<Size, impl::SerializeJumpLocationTag>;

	template<typename IdNumberType>
	struct SerializeJumpInfo {
		SerializeJumpId<IdNumberType> jumpId;
		SerializeJumpLocation jumpLocation;
	};

	template<typename IdNumberType>
	struct SerializeJumpTableInfo {
		SerializeJumpLocation tableLocation;
		natl::Size size;
	};

	template<Size SmallBufferSize, typename IdNumberType>
	struct SerializeJumpTableData {
		SerializeJumpTableInfo<IdNumberType> info;
		natl::SmallDynArray<SerializeJumpInfo<IdNumberType>, SmallBufferSize> jumps;
	};
}