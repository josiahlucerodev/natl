/*header
#pragma once
//own
#include "pch.h"
endHeader*/

//module
export module natl.timer;
//std
import std;
//endModule

//interface
export namespace natl {
	struct TimeSave {
		std::string name = "not defined";
		std::uint64_t nanoseconds = 0;
		std::uint64_t microseconds = 0;
		long double miliseconds = 0;
		long double seconds = 0;
	public:
		TimeSave();
		TimeSave(const std::string_view& inputString);
	public:
		std::string_view getName();
		std::uint64_t getNanoseconds();
		std::uint64_t getMicroseconds();
		long double getMiliseconds();
		long double getSeconds();
		void print();
		void condensedPrint();
		void changeName(const std::string_view&& input);
		std::string string();
		std::string condensedString();
		void inputTimeValues(std::uint64_t inputMicroseconds, long double inputMiliseconds, long double inputSeconds);
		void inputTimeValues(TimeSave inputTimeSave);
		TimeSave operator= (TimeSave& inputTime);
		TimeSave operator- (TimeSave inputTime);
	};
	class VectorTimeSave {
		std::string name;
		std::vector<TimeSave> timeSavesVector;
	public:
		VectorTimeSave();
		VectorTimeSave(const std::string_view& inputName);
		VectorTimeSave(const std::string& inputName);
	public:
		void print();
		void condensedPrint();
		std::string string();
		std::string condensedString();
		TimeSave average();
		void changeName(const std::string_view& nameInput);
		void changeName(const std::string& nameInput);
		void inputTimeSave(TimeSave inputValue);
		VectorTimeSave operator+= (TimeSave inputValue);
		VectorTimeSave operator+= (VectorTimeSave inputValue);
	};

	class Timer {
		std::string name = "not defined";
		std::chrono::time_point<std::chrono::high_resolution_clock> startpoint;
	public:
		Timer();
		Timer(const std::string_view& nameIn);
	public:
		void start();
		void start(const std::string_view& nameIn);
		TimeSave getTimeSave();
		void getTimeSave(TimeSave& outputtime);
		void changeName(const std::string_view& nameIn);
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
		FunctionProfileTimer(const std::string_view& name) : timer(name) { timer.start(); };
		~FunctionProfileTimer();
	};
}