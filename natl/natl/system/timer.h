#pragma once

//own
#include "../util/basicTypes.h"
#include "../util/stringConvert.h"
#include "../container/string.h"
#include "../container/stringView.h"
#include "../container/dynArray.h"
#include "../fundamental/strongType.h"
#include "../units/standard.h"

//interface
namespace natl {

	template<typename DataType, typename Tag>
	struct Timepoint {
	public:
		using tag_type = Tag;
		using value_type = DataType;
	private:
		value_type internalValueStorage;
	public:

		//constructor 
		constexpr Timepoint() noexcept = default;
		explicit constexpr Timepoint(const value_type valueIn) noexcept : internalValueStorage(valueIn) {};

		//destructor 
		constexpr ~Timepoint() noexcept = default;

		//util 
		constexpr Timepoint& self() noexcept { return *this; }
		constexpr const Timepoint& self() const noexcept { return *this; }

		//element access 
		constexpr value_type value() noexcept { return internalValueStorage; }
		constexpr value_type value() const noexcept { return internalValueStorage; }
		explicit constexpr operator value_type&() & noexcept { return internalValueStorage; }
		explicit constexpr operator const value_type&() const& noexcept { return internalValueStorage; }

		//operations 
		constexpr Timepoint& operator+=(const Timepoint& rhs) noexcept { internalValueStorage += rhs.value(); return self(); }
		constexpr Timepoint& operator-=(const Timepoint& rhs) noexcept { internalValueStorage -= rhs.value(); return self(); }
		constexpr Timepoint operator+(const Timepoint& rhs) const noexcept { return Timepoint(value() + rhs.value()); }
		constexpr Timepoint operator-(const Timepoint& rhs) const noexcept { return Timepoint(value() - rhs.value()); }
		constexpr Timepoint& operator++() noexcept { internalValueStorage++; return self(); }
		constexpr Timepoint operator++(int) noexcept { Timepoint temp = self(); ++self().internalValueStorage; return temp; }
		constexpr Timepoint& operator--() noexcept { internalValueStorage--; return self(); }
		constexpr Timepoint operator--(int) noexcept { Timepoint temp = self(); --self().internalValueStorage; return temp; }
		
		//compare 
		constexpr Bool operator==(const Timepoint& rhs) const noexcept { return value() == rhs.value(); }
		constexpr Bool operator!=(const Timepoint& rhs) const noexcept { return value() != rhs.value(); }
		constexpr Bool operator<(const Timepoint& rhs) const noexcept { return value() < rhs.value(); }
		constexpr Bool operator<=(const Timepoint& rhs) const noexcept { return value() <= rhs.value(); }
		constexpr Bool operator>(const Timepoint& rhs) const noexcept { return value() > rhs.value(); }
		constexpr Bool operator>=(const Timepoint& rhs) const noexcept { return value() >= rhs.value(); }
		constexpr CompareThreeWayResult<value_type> operator<=>(const Timepoint& rhs) const noexcept { return value() <=> rhs.value(); }

		//special 
		constexpr Size hash() const noexcept { return value(); }
	};


	namespace impl {
		struct HighResolutionTag {};
	}
	using HighResolutionTimePoint = Timepoint<i64, impl::HighResolutionTag>;
	HighResolutionTimePoint highResolutionNow() noexcept;
	Nanoseconds<i64> highResolutionTimePointToNanoseconds(const HighResolutionTimePoint timePoint) noexcept;

	struct Timer {
	private:
		HighResolutionTimePoint startTimePoint;
	public:
		//constructor
		constexpr Timer() noexcept {
			if (!isConstantEvaluated()) {
				start(); 
			}
		};

		//destructor 
		constexpr ~Timer() noexcept = default;

		//operaitons 
		void start() noexcept {
			startTimePoint = highResolutionNow();
		}

		natl::Nanoseconds<natl::i64,1> getNanosecondsInt() const noexcept {
			const HighResolutionTimePoint endTimePoint = highResolutionNow();
			const HighResolutionTimePoint elapsedTime = endTimePoint - startTimePoint;
			return highResolutionTimePointToNanoseconds(elapsedTime);
		}
		natl::Microseconds<natl::i64> getMicrosecondsInt() const noexcept {
			return getNanosecondsFloat().convertTo<natl::abbrt::mus>().asInt<natl::i64>();
		}
		natl::Milliseconds<natl::i64> getMillisecondsInt() const noexcept {
			return getNanosecondsFloat().convertTo<natl::abbrt::ms>().asInt<natl::i64>();
		}
		natl::Seconds<natl::i64> getSecondsInt() const noexcept {
			return getNanosecondsFloat().convertTo<natl::abbrt::s>().asInt<natl::i64>();
		}

		natl::Nanoseconds<natl::f64> getNanosecondsFloat() const noexcept {
			return getNanosecondsInt().asF64();
		}
		natl::Microseconds<natl::f64> getMicrosecondsFloat() const noexcept {
			return getNanosecondsFloat().convertTo<natl::abbrt::mus>();
		}
		natl::Milliseconds<natl::f64> getMillisecondsFloat() const noexcept {
			return getNanosecondsFloat().convertTo<natl::abbrt::ms>();
		}
		natl::Seconds<natl::f64> getSecondsFloat() const noexcept {
			return getNanosecondsFloat().convertTo<natl::abbrt::s>();
		}
	};

	struct NamedTimeInfo {
	private:
		natl::Nanoseconds<natl::i64> elapsedTime;
		String timerName;
	public:
		constexpr NamedTimeInfo() noexcept = default;
		constexpr NamedTimeInfo(const natl::Nanoseconds<natl::i64> elapsedTimeIn, const String& timerNameIn) noexcept
			: elapsedTime(elapsedTimeIn), timerName(timerNameIn) {};
		constexpr NamedTimeInfo(const natl::Nanoseconds<natl::i64> elapsedTimeIn, String&& timerNameIn) noexcept
			: elapsedTime(elapsedTimeIn), timerName(natl::forward<String>(timerNameIn)) {};

		constexpr const String& name() const noexcept { 
			return timerName;
		}

		natl::Nanoseconds<natl::i64> getNanosecondsInt() const noexcept {
			return elapsedTime;
		}
		natl::Microseconds<natl::i64> getMicrosecondsInt() const noexcept {
			return getNanosecondsFloat().convertTo<natl::abbrt::mus>().asInt<natl::i64>();
		}
		natl::Milliseconds<natl::i64> getMillisecondsInt() const noexcept {
			return getNanosecondsFloat().convertTo<natl::abbrt::ms>().asInt<natl::i64>();
		}
		natl::Seconds<natl::i64> getSecondsInt() const noexcept {
			return getNanosecondsFloat().convertTo<natl::abbrt::s>().asInt<natl::i64>();
		}

		natl::Nanoseconds<natl::f64> getNanosecondsFloat() const noexcept {
			return getNanosecondsInt().asF64();
		}
		natl::Microseconds<natl::f64> getMicrosecondsFloat() const noexcept {
			return getNanosecondsFloat().convertTo<natl::abbrt::mus>();
		}
		natl::Milliseconds<natl::f64> getMillisecondsFloat() const noexcept {
			return getNanosecondsFloat().convertTo<natl::abbrt::ms>();
		}
		natl::Seconds<natl::f64> getSecondsFloat() const noexcept {
			return getNanosecondsFloat().convertTo<natl::abbrt::s>();
		}
	};

	struct NamedTimer {
	private:
		Timer timer;
		String timerName;
	public:
		NamedTimer() noexcept : timer(), timerName("not defined") {};
		NamedTimer(const ConstStringView& nameIn)  noexcept : timer(), timerName(nameIn) {}
	public:
		//operations
		void start() noexcept { 
			timer.start(); 
		}
		void start(const ConstStringView& nameIn) noexcept {
			changeName(nameIn);
			start();
		}
		
		void changeName(const ConstStringView& nameIn) noexcept { timerName = nameIn;  }
		natl::Nanoseconds<natl::i64> getNanosecondsInt() const noexcept { return timer.getNanosecondsInt(); }
		natl::Microseconds<natl::i64> getMicrosecondsInt() const noexcept { return timer.getMicrosecondsInt(); }
		natl::Milliseconds<natl::i64> getMillisecondsInt() const noexcept { return timer.getMillisecondsInt(); }
		natl::Seconds<natl::i64> getSecondsInt() const noexcept { return timer.getSecondsInt(); }

		natl::Nanoseconds<natl::f64> getNanosecondsFloat() const noexcept { return timer.getNanosecondsFloat(); }
		natl::Microseconds<natl::f64> getMicrosecondsFloat() const noexcept { return timer.getMicrosecondsFloat(); }
		natl::Milliseconds<natl::f64> getMillisecondsFloat() const noexcept { return timer.getMillisecondsFloat(); }
		natl::Seconds<natl::f64> getSecondsFloat() const noexcept { return timer.getSecondsFloat(); }
		NamedTimeInfo getTimeInfo() const noexcept { return NamedTimeInfo(getNanosecondsInt(), timerName); }
	};
}