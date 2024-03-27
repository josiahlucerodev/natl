#pragma once

//std
#include <cstdint>
#include <chrono>

//own
#include "basicTypes.h"
#include "string.h"
#include "stringView.h"
#include "stringConvert.h"
#include "dynArray.h"

//interface
namespace natl {
	class NamedTimeInfo {
	private:
		String name;
		i64 nanoseconds;
		i64 microseconds;
		f64 miliseconds;
		f64 seconds;
	public:
		constexpr NamedTimeInfo() noexcept : name("not defined"), nanoseconds(0), microseconds(0), miliseconds(0), seconds(0) {};
		constexpr NamedTimeInfo(const ConstStringView& inputString) noexcept : name(inputString), nanoseconds(0), microseconds(0), miliseconds(0), seconds(0) {};

		constexpr ConstStringView getName() const noexcept { return name.toStringView(); }
		constexpr i64 getNanoseconds() const noexcept { return nanoseconds; }
		constexpr i64 getMicroseconds() const noexcept { return microseconds; }
		constexpr f64 getMiliseconds() const noexcept { return miliseconds; }
		constexpr f64 getSeconds() const noexcept { return seconds; }

		constexpr void inputTimeValues(const i64 inputNanoseconds, const i64 inputMicroseconds, const f64 inputMiliseconds, const f64 inputSeconds) noexcept {
			nanoseconds = inputNanoseconds;
			microseconds = inputMicroseconds;
			miliseconds = inputMiliseconds;
			seconds = inputSeconds;
		}
		constexpr void inputTimeValues(const NamedTimeInfo& inputNamedTimeInfo) noexcept {
			name = inputNamedTimeInfo.getName();
			microseconds = inputNamedTimeInfo.getMicroseconds();
			miliseconds = inputNamedTimeInfo.getMiliseconds();
			seconds = inputNamedTimeInfo.getSeconds();
		}
		constexpr void changeName(const ConstStringView& input) { name = input; }

		constexpr String string() const noexcept {
			String outputString;
			outputString += name;
			outputString += "\n";
			outputString += " microseconds: ";
			outputString += intToStringDecimalType<i64>(microseconds);
			outputString += "\n";
			outputString += " milliseconds: ";
			outputString += floatToStringType<f64>(miliseconds);
			outputString += "\n";
			outputString += " seconds: ";
			outputString += floatToStringType<f64>(seconds);
			outputString += "\n";
			return outputString;
		}
		constexpr String condensedString() const noexcept {
			String outputString;
			outputString += name;
			outputString += "\n";
			outputString += " microseconds: ";
			outputString += intToStringDecimalType<i64>(microseconds);
			outputString += "   ";
			outputString += " milliseconds: ";
			outputString += floatToStringType<f64>(miliseconds);
			outputString += "   ";
			outputString += " seconds: ";
			outputString += floatToStringType<f64>(seconds);
			outputString += "\n";
			return outputString;
		}

		void print() const noexcept {
			std::cout << name.c_str() << "\n";
			std::cout << "nanoseconds: " << nanoseconds << "\n";
			std::cout << "microseconds: " << microseconds << "\n";
			std::cout << "milliseconds: " << std::fixed << miliseconds << "\n";
			std::cout << "seconds: " << std::fixed << seconds << "\n";
		}
		void condensedPrint() const noexcept {
			std::cout << name.c_str() << "\n";
			std::cout << "nanoseconds: " << nanoseconds << "   ";
			std::cout << "microseconds: " << microseconds << "   ";
			std::cout << "milliseconds: " << std::fixed << miliseconds << "   ";
			std::cout << "seconds: " << std::fixed << seconds << "\n";
		}
	};

	class NamedTimeInfoCollection {
	private:
		String name;
		DynArray<NamedTimeInfo> timeSavesVector;
	public:
		NamedTimeInfoCollection() noexcept = default;
		NamedTimeInfoCollection(const StringView& inputName) noexcept : name(inputName), timeSavesVector() {}
		NamedTimeInfoCollection(const String& inputName) noexcept : name(inputName), timeSavesVector() {}
		
		constexpr NamedTimeInfoCollection& self() noexcept { return *this; }
		constexpr const NamedTimeInfoCollection& self() const noexcept { return *this; }

		constexpr ConstStringView getName() const noexcept { return name.toStringView(); }

		constexpr void changeName(const StringView& nameInput) noexcept {
			name = nameInput;
		}
		constexpr void changeName(const String& nameInput) noexcept {
			name = nameInput;
		}
		constexpr void inputNamedTimeInfo(const NamedTimeInfo& inputValue) noexcept {
			timeSavesVector.push_back(inputValue);
		}

		constexpr NamedTimeInfoCollection operator+= (const NamedTimeInfo& inputValue) noexcept {
			timeSavesVector.push_back(inputValue);
			return self();
		}
		constexpr NamedTimeInfoCollection operator+= (const NamedTimeInfoCollection& inputValue) noexcept {
			for (Size i = 0; i < inputValue.timeSavesVector.size(); i++) {
				timeSavesVector.push_back(inputValue.timeSavesVector[i]);
			}
			return self();
		}

		constexpr NamedTimeInfo average() const noexcept {
			NamedTimeInfo average;
			average = timeSavesVector[0];
			for (Size i = 1; i < timeSavesVector.size(); i++) {
				average.inputTimeValues(
					(average.getNanoseconds() + timeSavesVector[i].getNanoseconds()) / 2,
					(average.getMicroseconds() + timeSavesVector[i].getMicroseconds()) / 2,
					(average.getMiliseconds() + timeSavesVector[i].getMiliseconds()) / 2,
					(average.getSeconds() + timeSavesVector[i].getSeconds()) / 2);
			}
			String tempName;
			tempName = name;
			tempName += " collection average";
			average.changeName(tempName);
			return average;
		}

		constexpr String string() const noexcept {
			String outputvalue;
			outputvalue += name;
			outputvalue += " collection\n";
			for (Size i = 0; i < timeSavesVector.size(); i++) {
				outputvalue += timeSavesVector[i].string();
			}
			return outputvalue;
		}
		constexpr String condensedString() const noexcept {
			String outputvalue;
			outputvalue += name;
			outputvalue += " collection\n";
			for (Size i = 0; i < timeSavesVector.size(); i++)
			{
				outputvalue += timeSavesVector[i].condensedString();
			}
			return outputvalue;
		}

		void print() const noexcept {
			std::cout << name.toStringView() << " collection\n";
			for (Size i = 0; i < timeSavesVector.size(); i++) {
				timeSavesVector[i].print();
			}
		}
		void condensedPrint() const noexcept {
			std::cout << name.toStringView() << " collection\n";
			for (Size i = 0; i < timeSavesVector.size(); i++) {
				timeSavesVector[i].condensedPrint();
			}
		}
	};

	class NamedTimer {
		String name;
		std::chrono::time_point<std::chrono::high_resolution_clock> startpoint;
	public:
		NamedTimer() noexcept : name("not defined"), startpoint() { start(); };
		NamedTimer(const ConstStringView& nameIn)  noexcept : name(nameIn) { start(); }
	public:
		void start() noexcept {
			startpoint = std::chrono::high_resolution_clock::now();
		}
		void start(const ConstStringView& nameIn) noexcept {
			changeName(nameIn);
			start();
		}

		void changeName(const ConstStringView& nameIn) noexcept { name = nameIn; }

		i64 getNanoseconds() const noexcept {
			std::chrono::high_resolution_clock::time_point endTimpoint = std::chrono::high_resolution_clock::now();
			auto start = std::chrono::time_point_cast<std::chrono::nanoseconds>(startpoint).time_since_epoch().count();
			auto end = std::chrono::time_point_cast<std::chrono::nanoseconds>(endTimpoint).time_since_epoch().count();
			return end - start;
		}
		i64  getMicroseconds() const noexcept {
			std::chrono::high_resolution_clock::time_point endTimpoint = std::chrono::high_resolution_clock::now();
			auto start = std::chrono::time_point_cast<std::chrono::microseconds>(startpoint).time_since_epoch().count();
			auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimpoint).time_since_epoch().count();
			return end - start;
		}
		f64 getMiliseconds() const noexcept {
			return static_cast<f64>(getMicroseconds()) * f64(0.001L);
		}
		f64 getSeconds() const noexcept {
			return static_cast<f64>(getMicroseconds()) * f64(0.000001L);
		}

		void getNamedTimeInfo(NamedTimeInfo& timeSaveInput) const noexcept {
			i64 microseconds = getMicroseconds();
			i64 nonoseconds = getNanoseconds();
			timeSaveInput.changeName(timeSaveInput.getName());
			timeSaveInput.inputTimeValues(
				nonoseconds, 
				microseconds, 
				static_cast<f64>(microseconds) * f64(0.001L), 
				static_cast<f64>(microseconds) * f64(0.000001L)
			);
		}
		NamedTimeInfo getNamedTimeInfo() const noexcept {
			NamedTimeInfo outputNamedTimeInfo;
			getNamedTimeInfo(outputNamedTimeInfo);
			return outputNamedTimeInfo;
		}
	};
}