#pragma once 

//@begin_non_modules
//natl
#include "../util/compilerDependent.h"
//@end_non_modules

//own
#include "base.h"
#include "../util/strongType.h"
#include "../container/string.h"
#include "../container/smallDynArray.h"

//@export
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