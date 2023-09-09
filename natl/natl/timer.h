#pragma once

//std
#include <cstdint>
#include <chrono>

//own
#include "dynamicArray.h"
#include "string.h"
#include "stringView.h"

//interface
namespace natl {
	struct TimeSave {
		String name = "not defined";
		std::uint64_t nanoseconds = 0;
		std::uint64_t microseconds = 0;
		long double miliseconds = 0;
		long double seconds = 0;
	public:
		TimeSave();
		TimeSave(const StringView& inputString);
	public:
		StringView getName();
		std::uint64_t getNanoseconds();
		std::uint64_t getMicroseconds();
		long double getMiliseconds();
		long double getSeconds();
		void print();
		void condensedPrint();
		void changeName(const StringView&& input);
		String string();
		String condensedString();
		void inputTimeValues(std::uint64_t inputMicroseconds, long double inputMiliseconds, long double inputSeconds);
		void inputTimeValues(TimeSave inputTimeSave);
		TimeSave operator= (TimeSave& inputTime);
		TimeSave operator- (TimeSave inputTime);
	};
	class TimeSaveCollection {
		String name;
		DynamicArray<TimeSave> timeSavesVector;
	public:
		TimeSaveCollection();
		TimeSaveCollection(const StringView& inputName);
		TimeSaveCollection(const String& inputName);
	public:
		void print();
		void condensedPrint();
		String string();
		String condensedString();
		TimeSave average();
		void changeName(const StringView& nameInput);
		void changeName(const String& nameInput);
		void inputTimeSave(TimeSave inputValue);
		TimeSaveCollection operator+= (TimeSave inputValue);
		TimeSaveCollection operator+= (TimeSaveCollection inputValue);
	};

	class Timer {
		String name = "not defined";
		std::chrono::time_point<std::chrono::high_resolution_clock> startpoint;
	public:
		Timer();
		Timer(const StringView& nameIn);
	public:
		void start();
		void start(const StringView& nameIn);
		TimeSave getTimeSave();
		void getTimeSave(TimeSave& outputtime);
		void changeName(const StringView& nameIn);
		std::uint64_t getNanoseconds();
		std::uint64_t getMicroseconds();
		long double getMiliseconds();
		long double getSeconds();
	};

	class SecondsTimePointTimer {
	protected:
		std::uint64_t timePoint;
		std::chrono::time_point<std::chrono::high_resolution_clock> startpoint;
		long double internalgetSeconds();
	private:
		void updateTimePoint();
	public:
		void resetTimePoint();
		std::uint64_t queryTimePostion();
		bool isNewTimePoint();
		std::uint64_t returnUpdatedTimePoint();
		std::uint64_t returnTimePoint();
	};

	class FunctionProfileTimer {
		Timer timer;
	public:
		FunctionProfileTimer(const StringView& name) : timer(name) { timer.start(); };
		~FunctionProfileTimer();
	};
}