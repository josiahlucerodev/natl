#pragma once 

//own
#include "base.h"
#include "jump.h"
#include "../container/smallDynArray.h"

//interface 
namespace natl {
	template<Size SmallBufferSize>
	struct FullDeserializeErrorHandler {
		String256 errorMessage;
		String256 locationDetails;
		DeserializeErrorLocation errorLocation;
		DeserializeErrorFlag errorFlag;
		SmallDynArray<String256, SmallBufferSize> sourceInfos;

		constexpr FullDeserializeErrorHandler(
			const ConstAsciiStringView& errorMessageIn,
			const ConstAsciiStringView& locationDetails,
			const DeserializeErrorLocation errorLocationIn,
			const DeserializeErrorFlag& errorFlagIn) noexcept
			: errorMessage(errorMessageIn),
			locationDetails(locationDetails),
			errorLocation(errorLocationIn),
			errorFlag(errorFlagIn) {}

		constexpr Bool hasMessage() const noexcept { return errorMessage.isEmpty(); }
		constexpr FullDeserializeErrorHandler& addSource(const ConstAsciiStringView& source) noexcept {
			sourceInfos.pushBack(source);
			return *this; 
		}
		constexpr FullDeserializeErrorHandler& addSource(const ConstAsciiStringView& source, 
			const ConstAsciiStringView& element) noexcept {
			sourceInfos.pushBack(source);
			return *this;
		}

		template<typename StringDstType>
		constexpr StringDstType getMessage() const noexcept {
			StringDstType output;
			
			natl::formatToBack(output, 
				deserializeErrorFlagToString(errorFlag), 
				" error at ", 
				deserializeErrorLocationToString(errorLocation),
				", ", errorMessage.toStringView(),
				" location details: ", locationDetails);
			if(sourceInfos.isEmpty()) {
				return output;
			}

			natl::formatToBack(output, FormatNewLine{}, "location: ");
			for(auto& sourceInfo : sourceInfos) {
				natl::formatToBack(output, FormatNewLine{}, sourceInfo);
			}
			return output;
		}

		template<typename DynStringType>
		constexpr void getMessageTo(DynStringType& dst) const noexcept {
			dst = errorMessage.toStringView();
		}
	};

	static_assert(IsDeserializeErrorHandlerC<FullDeserializeErrorHandler<8>>);

	template<Size SmallBufferSize>
	struct FullSerializeErrorHandler {
		String256 errorMessage;
		SerializeErrorLocation errorLocation;
		SerializeErrorFlag errorFlag;
		SmallDynArray<String256, SmallBufferSize> sourceInfos;

		constexpr FullSerializeErrorHandler(
			const ConstAsciiStringView& errorMessageIn,
			const SerializeErrorLocation errorLocationIn,
			const SerializeErrorFlag& errorFlagIn) noexcept
			: errorMessage(errorMessageIn),
			errorLocation(errorLocationIn),
			errorFlag(errorFlagIn) {
		}

		constexpr Bool hasMessage() noexcept { return errorMessage.isEmpty(); }
		constexpr FullSerializeErrorHandler& addSource(
			const ConstAsciiStringView& source) noexcept {
			sourceInfos.pushBack(source);
			return *this;
		}

		template<typename StringDstType>
		constexpr StringDstType toMessage() noexcept {
			StringDstType output;

			natl::formatToBack(output,
				serializeErrorFlagToString(errorFlag),
				" error at ",
				deserializeErrorLocationToString(errorLocation),
				", ", errorMessage.toStringView());
			if (sourceInfos.isEmpty()) {
				return output;
			}

			natl::formatToBack(output, FormatNewLine{}, "location: ");
			for (auto& sourceInfo : sourceInfos) {
				natl::formatToBack(output, FormatNewLine{}, sourceInfo);
			}
			return output;
		}

		template<typename DynStringType>
		constexpr void getErrorMessage(DynStringType& dst) {
			dst = errorMessage.toStringView();
		}
	};

	template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags,
		typename SerializeComponentType, natl::Size SmallBufferSize, typename IdNumberType, typename ParentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr natl::Expect<SerializeJumpTableData<SmallBufferSize, IdNumberType>, typename Deserializer::deserialize_error_handler>
		deserializeReadJumpTable(Deserializer& deserializer, typename Deserializer::template deserialize_info<ParentType>& info,
			const natl::ConstAsciiStringView& name) {
		SerializeJumpTableData<SmallBufferSize, IdNumberType> jumpData;

		auto beginJumpTableExpect = deserializer.template beginReadJumpTable<
			Flags, CustomFlags, SerializeComponentType, IdNumberType>(info, name);
		if (beginJumpTableExpect.hasError()) {
			return natl::unexpected(beginJumpTableExpect.error());
		}

		jumpData.info = beginJumpTableExpect.value();
		jumpData.jumps.resize(jumpData.info.size);

		for (Size i = 0; i < jumpData.info.size; i++) {
			auto readJumpTableElementExpect = deserializer.template readJumpTableElement<
				Flags, CustomFlags, SerializeComponentType, IdNumberType>(info);
			if (readJumpTableElementExpect.hasError()) {
				return natl::unexpected(readJumpTableElementExpect.error());
			}
			jumpData.jumps[i] = readJumpTableElementExpect.value();
		}

		auto endJumpTableError = deserializer.template endReadJumpTable<
			Flags, CustomFlags, SerializeComponentType, IdNumberType>(info, jumpData.info);
		if (endJumpTableError.hasValue()) {
			return natl::unexpected(endJumpTableError.value());
		}
		return jumpData;
	}
}