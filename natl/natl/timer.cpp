//interface
#include "timer.h"

//own
#include "stringView.h"
#include "stringConvert.h"

//implementation
namespace natl {
	//Time Save
	TimeSave::TimeSave() = default;
	TimeSave::TimeSave(const StringView& inputString) : name(inputString) {}
	StringView TimeSave::getName() { return name; }
	std::uint64_t  TimeSave::getNanoseconds() { return nanoseconds; }
	std::uint64_t  TimeSave::getMicroseconds() { return microseconds; }
	long double TimeSave::getMiliseconds() { return miliseconds; }
	long double TimeSave::getSeconds() { return seconds; }
	void TimeSave::print() {
		std::cout << name.c_str() << "\n";
		std::cout << "nanoseconds: " << nanoseconds << "\n";
		std::cout << "microseconds: " << microseconds << "\n";
		std::cout << "milliseconds: " << std::fixed << miliseconds << "\n";
		std::cout << "seconds: " << std::fixed << seconds << "\n";
	}
	void TimeSave::condensedPrint() {
		std::cout << name.c_str() << "\n";
		std::cout << "nanoseconds: " << nanoseconds << "   ";
		std::cout << "microseconds: " << microseconds << "   ";
		std::cout << "milliseconds: " << std::fixed << miliseconds << "   ";
		std::cout << "seconds: " << std::fixed << seconds << "\n";
	}
	void TimeSave::changeName(const StringView&& input) {
		name = input;
	}
	String TimeSave::string() {
		String outputString;
		outputString += name;
		outputString += "\n";
		outputString += " microseconds: ";
		outputString += intToStringDecimal<std::uint64_t>(microseconds);
		outputString += "\n";
		outputString += " milliseconds: ";
		outputString += floatToString<long double>(miliseconds);
		outputString += "\n";
		outputString += " seconds: ";
		outputString += floatToString<long double>(seconds);
		outputString += "\n";
		return outputString;
	}
	String TimeSave::condensedString() {
		String outputString;
		outputString += name;
		outputString += "\n";
		outputString += " microseconds: ";
		outputString += intToStringDecimal<std::uint64_t>(microseconds);
		outputString += "   ";
		outputString += " milliseconds: ";
		outputString += floatToString<long double>(miliseconds);
		outputString += "   ";
		outputString += " seconds: ";
		outputString += floatToString<long double>(seconds);
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

	//TimeSaveCollection
	TimeSaveCollection::TimeSaveCollection() = default;
	TimeSaveCollection::TimeSaveCollection(const StringView& inputName) : name(inputName) {}
	TimeSaveCollection::TimeSaveCollection(const String& inputName) : name(inputName) {}
	void TimeSaveCollection::print() {
		std::cout << name.toStringView() << " collection\n";
		for (unsigned int i = 0; i < timeSavesVector.size(); i++)
		{
			timeSavesVector[i].print();
		}
	}
	void TimeSaveCollection::condensedPrint() {
		std::cout << name.toStringView() << " collection\n";
		for (unsigned int i = 0; i < timeSavesVector.size(); i++)
		{
			timeSavesVector[i].condensedPrint();
		}
	}
	String TimeSaveCollection::string() {
		String outputvalue;
		outputvalue += name;
		outputvalue += " collection\n";
		for (unsigned int i = 0; i < timeSavesVector.size(); i++)
		{
			outputvalue += timeSavesVector[i].string();
		}
		return outputvalue;
	}
	String TimeSaveCollection::condensedString() {
		String outputvalue;
		outputvalue += name;
		outputvalue += " collection\n";
		for (unsigned int i = 0; i < timeSavesVector.size(); i++)
		{
			outputvalue += timeSavesVector[i].condensedString();
		}
		return outputvalue;
	}
	TimeSave TimeSaveCollection::average() {
		TimeSave average;
		average = timeSavesVector[0];
		for (int i = 1; i < timeSavesVector.size(); i++)
		{
			average.inputTimeValues((average.getMicroseconds() + timeSavesVector[i].getMicroseconds()) / 2,
				(average.getMiliseconds() + timeSavesVector[i].getMiliseconds()) / 2,
				(average.getSeconds() + timeSavesVector[i].getSeconds()) / 2);
		}
		String tempName;
		tempName = name;
		tempName += " collection average";
		average.changeName(tempName);
		return average;
	}
	void TimeSaveCollection::changeName(const StringView& nameInput) {
		name = nameInput;
	}
	void TimeSaveCollection::changeName(const String& nameInput) {
		name = nameInput;
	}
	void TimeSaveCollection::inputTimeSave(TimeSave inputValue) {
		timeSavesVector.push_back(inputValue);
	}
	TimeSaveCollection TimeSaveCollection::operator+= (TimeSave inputValue) {
		timeSavesVector.push_back(inputValue);
		return *this;
	}
	TimeSaveCollection TimeSaveCollection::operator+= (TimeSaveCollection inputValue) {
		for (unsigned int i = 0; i < inputValue.timeSavesVector.size(); i++)
		{
			timeSavesVector.push_back(inputValue.timeSavesVector[i]);
		}
		return *this;
	}

	// Timer
	Timer::Timer() { start(); };
	Timer::Timer(const StringView& nameIn) : name(nameIn) { start(); }
	void Timer::start() {
		startpoint = std::chrono::high_resolution_clock::now();
	}
	void Timer::start(const StringView& nameIn) {
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
	void Timer::changeName(const StringView& nameIn) {
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