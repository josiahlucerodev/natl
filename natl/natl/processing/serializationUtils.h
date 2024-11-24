#pragma once 

#include "serialization.h"
#include "../container/smallDynArray.h"

//interface 
namespace natl {
	template<Size SmallBufferSize>
	struct FullDeserializeErrorHandler {
		struct SourceInfo {
			String256 source;
			String256 elementName;
		};

		String256 errorMessage;
		String256 locationDetails;
		DeserializeErrorLocation errorLocation;
		DeserializeErrorFlag errorFlag;
		SmallDynArray<SourceInfo, SmallBufferSize> sourceInfos;

		constexpr FullDeserializeErrorHandler(
			const ConstAsciiStringView& errorMessageIn,
			const ConstAsciiStringView& locationDetails,
			const DeserializeErrorLocation errorLocationIn,
			const DeserializeErrorFlag& errorFlagIn) noexcept
			: errorMessage(errorMessageIn),
			locationDetails(locationDetails),
			errorLocation(errorLocationIn),
			errorFlag(errorFlagIn) {}
		constexpr Bool hasMessage() noexcept { return errorMessage.isEmpty(); }
		constexpr FullDeserializeErrorHandler& addSource(
			const ConstAsciiStringView& source,
			const ConstAsciiStringView& elementName) noexcept {
			sourceInfos.pushBack(SourceInfo{source, elementName});
			return *this; 
		}

		template<typename StringDstType>
		constexpr StringDstType toMessage() noexcept {
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
				natl::formatToBack(output, FormatNewLine{}, sourceInfo.source,  " named ", sourceInfo.elementName);
			}
			return output;
		}

		template<typename DynStringType>
		constexpr void getErrorMessage(DynStringType& dst) {
			dst = errorMessage.toStringView();
		}
	};
}