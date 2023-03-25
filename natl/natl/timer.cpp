//header
#pragma once
//interface
#include "timer.h"
//own
#include "pch.h"
//endHeader

/*module
module;
module natl.timer;

//std
import std;

endModule*/

//implementation
namespace natl {
	//Time Save
	TimeSave::TimeSave() = default;
	TimeSave::TimeSave(const std::string_view& inputString) : name(inputString) {}
	std::string_view TimeSave::getName() { return name; }
	std::uint64_t  TimeSave::getNanoseconds() { return nanoseconds; }
	std::uint64_t  TimeSave::getMicroseconds() { return microseconds; }
	long double TimeSave::getMiliseconds() { return miliseconds; }
	long double TimeSave::getSeconds() { return seconds; }
	void TimeSave::print() {
		std::cout << name << "\n";
		std::cout << "nanoseconds: " << nanoseconds << "\n";
		std::cout << "microseconds: " << microseconds << "\n";
		std::cout << "milliseconds: " << std::fixed << miliseconds << "\n";
		std::cout << "seconds: " << std::fixed << seconds << "\n";
	}
	void TimeSave::condensedPrint() {
		std::cout << name << "\n";
		std::cout << "nanoseconds: " << nanoseconds << "   ";
		std::cout << "microseconds: " << microseconds << "   ";
		std::cout << "milliseconds: " << std::fixed << miliseconds << "   ";
		std::cout << "seconds: " << std::fixed << seconds << "\n";
	}
	void TimeSave::changeName(const std::string_view&& input) {
		name = input;
	}
	std::string TimeSave::string() {
		std::string outputString;
		outputString += name;
		outputString += "\n";
		outputString += " microseconds: ";
		outputString += std::to_string(microseconds);
		outputString += "\n";
		outputString += " milliseconds: ";
		outputString += std::to_string(miliseconds);
		outputString += "\n";
		outputString += " seconds: ";
		outputString += std::to_string(seconds);
		outputString += "\n";
		return outputString;
	}
	std::string TimeSave::condensedString() {
		std::string outputString;
		outputString += name;
		outputString += "\n";
		outputString += " microseconds: ";
		outputString += std::to_string(microseconds);
		outputString += "   ";
		outputString += " milliseconds: ";
		outputString += std::to_string(miliseconds);
		outputString += "   ";
		outputString += " seconds: ";
		outputString += std::to_string(seconds);
		outputString += "\n";
		return outputString;
	}
	void TimeSave::inputTimeValues(std::uint64_t  inputMicroseconds, long double inputMiliseconds, long double inputSeconds) {
		microseconds = inputMicroseconds;
		miliseconds = inputMiliseconds;
		seconds = inputSeconds;
	}
	void TimeSave::inputTimeValues(TimeSave inputTimeSave) {
		name = inputTimeSave.getName();
		microseconds = inputTimeSave.getMicroseconds();
		miliseconds = inputTimeSave.getMiliseconds();
		seconds = inputTimeSave.getSeconds();
	}
	TimeSave TimeSave::operator= (TimeSave& inputTime) {
		microseconds = inputTime.microseconds;
		miliseconds = inputTime.miliseconds;
		seconds = inputTime.seconds;
		return *this;
	}
	TimeSave TimeSave::operator- (TimeSave inputTime) {
		TimeSave outputTime;
		outputTime.nanoseconds = nanoseconds - inputTime.nanoseconds;
		outputTime.microseconds = microseconds - inputTime.microseconds;
		outputTime.miliseconds = miliseconds - inputTime.miliseconds;
		outputTime.seconds = seconds - inputTime.seconds;
		return outputTime;
	}

	//VectorTimeSave
	VectorTimeSave::VectorTimeSave() = default;
	VectorTimeSave::VectorTimeSave(const std::string_view& inputName) : name(inputName) {}
	VectorTimeSave::VectorTimeSave(const std::string& inputName) : name(inputName) {}
	void VectorTimeSave::print() {
		std::cout << name << " collection\n";
		for (unsigned int i = 0; i < timeSavesVector.size(); i++)
		{
			timeSavesVector[i].print();
		}
	}
	void VectorTimeSave::condensedPrint() {
		std::cout << name << " collection\n";
		for (unsigned int i = 0; i < timeSavesVector.size(); i++)
		{
			timeSavesVector[i].condensedPrint();
		}
	}
	std::string VectorTimeSave::string() {
		std::string outputvalue;
		outputvalue += name;
		outputvalue += " collection\n";
		for (unsigned int i = 0; i < timeSavesVector.size(); i++)
		{
			outputvalue += timeSavesVector[i].string();
		}
		return outputvalue;
	}
	std::string VectorTimeSave::condensedString() {
		std::string outputvalue;
		outputvalue += name;
		outputvalue += " collection\n";
		for (unsigned int i = 0; i < timeSavesVector.size(); i++)
		{
			outputvalue += timeSavesVector[i].condensedString();
		}
		return outputvalue;
	}
	TimeSave VectorTimeSave::average() {
		TimeSave average;
		average = timeSavesVector[0];
		for (int i = 1; i < timeSavesVector.size(); i++)
		{
			average.inputTimeValues((average.getMicroseconds() + timeSavesVector[i].getMicroseconds()) / 2,
				(average.getMiliseconds() + timeSavesVector[i].getMiliseconds()) / 2,
				(average.getSeconds() + timeSavesVector[i].getSeconds()) / 2);
		}
		std::string tempName;
		tempName = name;
		tempName += " collection average";
		average.changeName(tempName);
		return average;
	}
	void VectorTimeSave::changeName(const std::string_view& nameInput) {
		name = nameInput;
	}
	void VectorTimeSave::changeName(const std::string& nameInput) {
		name = nameInput;
	}
	void VectorTimeSave::inputTimeSave(TimeSave inputValue) {
		timeSavesVector.push_back(inputValue);
	}
	VectorTimeSave VectorTimeSave::operator+= (TimeSave inputValue) {
		timeSavesVector.push_back(inputValue);
		return *this;
	}
	VectorTimeSave VectorTimeSave::operator+= (VectorTimeSave inputValue) {
		for (unsigned int i = 0; i < inputValue.timeSavesVector.size(); i++)
		{
			timeSavesVector.push_back(inputValue.timeSavesVector[i]);
		}
		return *this;
	}

	// Timer
	Timer::Timer() { start(); };
	Timer::Timer(const std::string_view& nameIn) : name(nameIn) { start(); }
	void Timer::start() {
		startpoint = std::chrono::high_resolution_clock::now();
	}
	void Timer::start(const std::string_view& nameIn) {
		changeName(nameIn);
		start();
	}
	void Timer::getTimeSave(TimeSave& timeSaveInput) {
		std::uint64_t microseconds = getMicroseconds();
		timeSaveInput.changeName(name);
		timeSaveInput.nanoseconds = getNanoseconds();
		timeSaveInput.microseconds = microseconds;
		timeSaveInput.miliseconds = microseconds * 0.001L;
		timeSaveInput.seconds = microseconds * 0.000001L;
	}
	TimeSave Timer::getTimeSave() {
		TimeSave outputTimeSave;
		getTimeSave(outputTimeSave);
		return outputTimeSave;
	}
	void Timer::changeName(const std::string_view& nameIn) {
		name = nameIn;
	}
	std::uint64_t Timer::getNanoseconds() {
		std::chrono::steady_clock::time_point endTimpoint = std::chrono::high_resolution_clock::now();
		auto start = std::chrono::time_point_cast<std::chrono::nanoseconds>(startpoint).time_since_epoch().count();
		auto end = std::chrono::time_point_cast<std::chrono::nanoseconds>(endTimpoint).time_since_epoch().count();
		return end - start;
	}
	std::uint64_t  Timer::getMicroseconds() {
		std::chrono::steady_clock::time_point endTimpoint = std::chrono::high_resolution_clock::now();
		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(startpoint).time_since_epoch().count();
		auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimpoint).time_since_epoch().count();
		return end - start;
	}
	long double Timer::getMiliseconds() {
		return getMicroseconds() * 0.001L;
	}
	long double Timer::getSeconds() {
		return getMicroseconds() * 0.000001L;
	}

	//SecondsTimePointTimer
	long double SecondsTimePointTimer::internalgetSeconds() {
		std::chrono::steady_clock::time_point endTimpoint = std::chrono::high_resolution_clock::now();
		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(startpoint).time_since_epoch().count();
		auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimpoint).time_since_epoch().count();
		return (end - start) * 0.000001L;
	}
	void SecondsTimePointTimer::updateTimePoint() {
		timePoint = (std::uint64_t)std::floor(internalgetSeconds());
	}
	void SecondsTimePointTimer::resetTimePoint() {
		timePoint = 0;
		startpoint = std::chrono::high_resolution_clock::now();
	}
	std::uint64_t  SecondsTimePointTimer::queryTimePostion() {
		updateTimePoint();
		return timePoint;
	}
	bool SecondsTimePointTimer::isNewTimePoint() {
		std::uint64_t  currentTimePoint = timePoint;
		updateTimePoint();
		if (timePoint == currentTimePoint)
		{
			return false;
		}
		return true;
	}
	std::uint64_t  SecondsTimePointTimer::returnUpdatedTimePoint() {
		return (std::uint64_t)std::floor(internalgetSeconds());
	}
	std::uint64_t  SecondsTimePointTimer::returnTimePoint() {
		return timePoint;
	}

	//FunctionProfileTimer
	FunctionProfileTimer::~FunctionProfileTimer() {
		timer.getTimeSave().condensedPrint();
	}
}