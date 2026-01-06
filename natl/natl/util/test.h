#pragma once

//own
#include "sourceLocation.h"
#include "basicTypes.h"
#include "main.h"
#include "enumUtils.h"
#include "executionSession.h"
#include "option.h"
#include "../container/dynArray.h"
#include "../container/flatHashMap.h"
#include "../container/flatHashSet.h"
#include "../container/functional.h"
#include "../system/printFormatted.h"
#include "../system/printColor.h"
#include "../system/filesystem.h"
#include "../system/timer.h"

//@export
namespace natl {
	struct TestTypeStdOut {
	public:
		//constructor
		TestTypeStdOut() noexcept { print("default constructed"); }
		TestTypeStdOut(TestTypeStdOut&) noexcept { println("ref constructed"); }
		TestTypeStdOut(const TestTypeStdOut&) noexcept { println("const ref constructed"); }
		TestTypeStdOut(TestTypeStdOut&&) noexcept { println("moved constructed"); }
		TestTypeStdOut(const TestTypeStdOut&&) noexcept { println("const moved constructed"); }

		//destructor 
		~TestTypeStdOut() noexcept { println("destructed\n"); }

		//util
		constexpr TestTypeStdOut& self() { return *this; }

		//assignment 
		TestTypeStdOut& operator=(TestTypeStdOut&) { println("ref assigned"); return self(); }
		TestTypeStdOut& operator=(const TestTypeStdOut&) { println("const ref assigned"); return self(); }
		TestTypeStdOut& operator=(TestTypeStdOut&&) noexcept { println("moved assigned"); return self(); }
		TestTypeStdOut& operator=(const TestTypeStdOut&&) noexcept { println("const moved assigned"); return self(); }
	};
	//custom 
	struct TestTypeTrivialStdOut {
		ui8 addressableData;
	public:
		//constructor
		TestTypeTrivialStdOut() noexcept : addressableData(0) { println("default constructed"); }
		TestTypeTrivialStdOut(TestTypeTrivialStdOut&) noexcept : addressableData(0) { println("ref constructed"); }
		TestTypeTrivialStdOut(const TestTypeTrivialStdOut&) noexcept : addressableData(0) { println("const ref constructed"); }
		TestTypeTrivialStdOut(TestTypeTrivialStdOut&&) noexcept : addressableData(0) { println("moved constructed"); }
		TestTypeTrivialStdOut(const TestTypeTrivialStdOut&&) noexcept : addressableData(0) { println("const moved constructed"); }

		//destructor 
		~TestTypeTrivialStdOut() noexcept { println("destructed"); }

		//util
		constexpr TestTypeTrivialStdOut& self() { return *this; }

		//assignment 
		TestTypeTrivialStdOut& operator=(TestTypeTrivialStdOut&) { println("ref assigned"); return self(); }
		TestTypeTrivialStdOut& operator=(const TestTypeTrivialStdOut&) { println("const ref assigned"); return self(); }
		TestTypeTrivialStdOut& operator=(TestTypeTrivialStdOut&&) noexcept { println("moved assigned"); return self(); }
		TestTypeTrivialStdOut& operator=(const TestTypeTrivialStdOut&&) noexcept { println("const moved assigned"); return self(); }

		//element access 
		const ui8* getAddress() const noexcept { return &addressableData; }
	};

	template<> struct IsTriviallyCompareableV<TestTypeTrivialStdOut> : TrueType {};
	template<> struct IsTriviallyRelocatableV<TestTypeTrivialStdOut> : TrueType {};
	template<> struct IsTriviallyConstructibleV<TestTypeTrivialStdOut> : TrueType {};
	template<> struct IsTriviallyDestructibleV<TestTypeTrivialStdOut> : TrueType {};
	template<> struct IsTriviallyConstRefConstructibleV<TestTypeTrivialStdOut> : TrueType {};
	template<> struct IsTriviallyMoveConstructibleV<TestTypeTrivialStdOut> : TrueType {};
	template<> struct IsTriviallyConstRefAssignableV<TestTypeTrivialStdOut> : TrueType {};
	template<> struct IsTriviallyMoveAssignableV<TestTypeTrivialStdOut> : TrueType {};

	struct TestSuiteBase;

	namespace impl {
		struct TestInfo {
		public:
			natl::ConstAsciiStringView name;
			natl::Function<void(TestSuiteBase*)> runFunc;
		};

		struct TestSuiteInfo {
		public:
			natl::ConstAsciiStringView name;
			natl::Function<TestSuiteBase*(void)> createFunc;
			natl::Function<void(TestSuiteBase*)> destoryFunc;
		};

		using tests_container_type = natl::FlatHashMap<natl::ConstAsciiStringView, natl::DynArray<TestInfo>>;
		using test_suites_container_type = natl::FlatHashMap<natl::ConstAsciiStringView, TestSuiteInfo>;
		struct TestEnvironment {
		public:
			tests_container_type unitTests;
			test_suites_container_type unitTestSuites;
			Size testSeed = 0;
			Bool breakOnFailure = false;
			String256 testResourcePath;
		};
		extern TestEnvironment globalTestEnvironment;

		inline void setTestResourcePathToFile(const ConstStringView& filePath) noexcept {
			if (not globalTestEnvironment.testResourcePath.isEmpty()) {
				return;
			}
			globalTestEnvironment.testResourcePath = ConstPathView(filePath).parentPath();
		}

		template<typename TestSuiteType, void (*func)(TestSuiteType&)>
			requires(IsStructC<TestSuiteType>)
		struct RegisterTest {
		public:
			using void_member_func = void(*)(TestSuiteType&);
			RegisterTest(const natl::ConstAsciiStringView testName) {
				constexpr natl::ConstAsciiStringView suiteName = natl::getNameOfStruct<TestSuiteType>();
				TestInfo test;
				test.name = testName;
				test.runFunc = [](TestSuiteBase* testSuiteVoid) -> void {
					TestSuiteType* testSuite = static_cast<TestSuiteType*>(testSuiteVoid);
					(func)(*testSuite);
				};

				auto testSuiteTests = globalTestEnvironment.unitTests.find(suiteName);
				if (testSuiteTests.hasValue()) {
					testSuiteTests.value().value().pushBack(test);
				} else {
					globalTestEnvironment.unitTests.insert(suiteName, { test });
				}
			}
		};

		template<typename TestSuiteType, auto func>
			requires(IsStructC<TestSuiteType>)
		constexpr Bool runTestConstexpr() noexcept {
			TestSuiteType testSuite;
			if (!testSuite.suiteSetup()) {
				return false;
			}

			if (!testSuite.setup()) {
				return false;
			}

			(func)(testSuite);

			if (!testSuite.teardown()) {
				return false;
			}

			if (!testSuite.suiteTeardown()) {
				return false;
			}

			return true;
		}

		template<typename TestSuiteType, auto TestFunc, auto GenerateFunc>
		struct RegisterParameterizedTest {
		public:
			RegisterParameterizedTest(const natl::ConstAsciiStringView testName) {
				const natl::ConstAsciiStringView suiteName = natl::getNameOfStruct<TestSuiteType>();
				TestInfo test;
				test.name = testName;
				test.runFunc = [](TestSuiteBase* testSuiteVoid) -> void {
					TestSuiteType* testSuite = static_cast<TestSuiteType*>(testSuiteVoid);
					auto values = (GenerateFunc)(*testSuite);
					for (auto value : values) {
						using values_tuple_type = decltype(value);
						[&] <Size... Indices> (IndexSequence<Indices...>) -> void {
							(TestFunc)(*testSuite, value.template get<Indices>()...);
						}(MakeIndexSequence<natl::TupleSize<values_tuple_type>>{});
					}
				};

				auto testSuiteTests = globalTestEnvironment.unitTests.find(suiteName);
				if (testSuiteTests.hasValue()) {
					testSuiteTests.value().value().pushBack(test);
				} else {
					globalTestEnvironment.unitTests.insert(suiteName, { test });
				}
			}
		};

		template<typename TestSuiteType, auto TestFunc, auto GenerateFunc>
			requires(IsStructC<TestSuiteType>)
		constexpr Bool runParameterizedTestConstexpr() noexcept {
			TestSuiteType testSuite;
			if (!testSuite.suiteSetup()) {
				return false;
			}

			if (!testSuite.setup()) {
				return false;
			}

			auto values = (GenerateFunc)(testSuite);
			for (auto value : values) {
				using values_tuple_type = decltype(value);
				[&] <Size... Indices> (IndexSequence<Indices...>) -> void {
					(TestFunc)(testSuite, value.template get<Indices>()...);
				}(MakeIndexSequence<natl::TupleSize<values_tuple_type>>{});
			}

			if (!testSuite.teardown()) {
				return false;
			}

			if (!testSuite.suiteTeardown()) {
				return false;
			}

			return true;
		}

		template<typename TestSuiteType>
		struct RegisterTestSuite {
		public:
			RegisterTestSuite(const natl::ConstAsciiStringView suiteName) {
				TestSuiteInfo testSuite;
				testSuite.name = suiteName;

				testSuite.createFunc = []() -> TestSuiteBase* {
					TestSuiteType* testSuite = natl::StandardAllocator<TestSuiteType>::allocate(1);
					construct(testSuite);
					return static_cast<TestSuiteBase*>(testSuite);
				};

				testSuite.destoryFunc = [](TestSuiteBase* testSuiteVoid) -> void {
					TestSuiteType* testSuite = static_cast<TestSuiteType*>(testSuiteVoid);
					deconstruct(testSuite);
					natl::StandardAllocator<TestSuiteType>::deallocate(testSuite, 1);
				};

				globalTestEnvironment.unitTestSuites.insert(suiteName, testSuite);
			}
		};

		template<typename TestSuiteType>
			requires(IsStructC<TestSuiteType>)
		constexpr Bool testIsSuiteConstexpr() noexcept {
			TestSuiteType testSuite;
			if (!testSuite.suiteSetup()) {
				return false;
			}

			if (!testSuite.setup()) {
				return false;
			}

			if (!testSuite.teardown()) {
				return false;
			}

			if (!testSuite.suiteTeardown()) {
				return false;
			}

			return true;
		}

		constexpr String formatLocation(const SourceLocation location, const Bool functionName = true) noexcept {
			if (functionName) {
				return sformat(
					ConstPathView(location.fileName().c_str()).filename().toStringView().substr(1),
					":", location.line(), " \'", location.functionName(), "\'");
			} else {
				return sformat(
					ConstPathView(location.fileName().c_str()).filename().toStringView().substr(1),
					":", location.line());
			}
		}
	}

	inline void setTestResourcePath(const natl::ConstStringView& resourcePath) noexcept {
		if (!isConstantEvaluated()) {
			impl::globalTestEnvironment.testResourcePath = resourcePath;
		}
	}

	template<typename... Types>
	using ParameterizedValues = natl::DynArray<natl::Tuple<Types...>>;

	template<typename TestSuiteType>
	concept IsValidTestSuiteC = requires(TestSuiteType* ts) {
		{ static_cast<TestSuiteBase*>(ts) };
	};

	struct TestAssertData {
		ConstAsciiStringView testName;
		SourceLocation location;
		Bool failed;
		Size assertFailCount;
		Size assertRunCount;

	public:
		//constructors
		constexpr TestAssertData() noexcept = default;
		constexpr TestAssertData(ConstAsciiStringView testNameIn, SourceLocation locationIn) noexcept {
			testName = testNameIn;
			location = locationIn;
			failed = true;
			assertFailCount = 0;
			assertRunCount = 0;
		}

		//destructor
		constexpr ~TestAssertData() noexcept = default;

		//utils
		constexpr void signalAssert(Bool success) noexcept {
			if (success) {
				assertRunCount++;
			} else {
				failed = true;
				assertRunCount++;
				assertFailCount++;
			}
		};

		template<typename Hasher>
			requires(IsHasherC<Hasher>)
		constexpr void hashAppend(Hasher& hasher) const noexcept {
			natl::hashAppend(hasher, location.fileName());
			natl::hashAppend(hasher, location.line());
		}

		friend constexpr Bool operator==(const TestAssertData& lhs, const TestAssertData& rhs) noexcept {
			if (lhs.location.line() != rhs.location.line()) {
				return false;
			}
			return lhs.location.fileName() == rhs.location.fileName();
		}
	};

	struct TestFailData {
	public:
		Bool failed = false;
		Size assertRunCount = 0;
		Size assertFailCount = 0;
		ConstAsciiStringView testName;

	public:
		//constructor
		constexpr TestFailData(ConstAsciiStringView testNameIn) noexcept : testName(testNameIn) {}

		//destructor
		constexpr ~TestFailData() noexcept = default;

	public:
		constexpr void signalAssert(Bool success) noexcept {
			if (success) {
				assertRunCount++;
			} else {
				assertRunCount++;
				assertFailCount++;
				failed = true;
			}
		};

		template<typename Hasher>
			requires(IsHasherC<Hasher>)
		constexpr void hashAppend(Hasher& hasher) const noexcept {
			natl::hashAppend(hasher, testName);
		}

		friend constexpr Bool operator==(const TestFailData& lhs, const TestFailData& rhs) noexcept {
			return lhs.testName == rhs.testName;
		}
	};

	struct TestSuiteTestingData {
	public:
		Bool failed = false;
		Size testCount = 0;
		Size testFailCount = 0;
		Size testRunCount = 0;
		Size assertFailCount = 0;
		Size assertCount = 0;
		ConstAsciiStringView suiteName = "";
		natl::SmallFlatHashSet<TestAssertData, 128> assertDataSet{};
		natl::SmallFlatHashSet<TestFailData, 16> failDataSet{};
		natl::Nanoseconds<natl::Size> time;
	};

	struct TestSuiteBase {
	public:
		constexpr virtual Bool suiteSetup() noexcept { return true; }
		constexpr virtual Bool suiteTeardown() noexcept { return true; }
		constexpr virtual Bool setup() noexcept { return true; }
		constexpr virtual Bool teardown() noexcept { return true; }

	private:
		ObserverPtr<TestSuiteTestingData> data;
		ConstAsciiStringView activeTestName;
		Bool failedActiveTest;
		StandardRNG rng;

	public:
		constexpr TestSuiteBase() noexcept : data(nullptr), activeTestName("unknown"), failedActiveTest(false), rng() {}
		constexpr ~TestSuiteBase() noexcept = default;
	
	public:
		constexpr void setTestingConfig(ObserverPtr<TestSuiteTestingData> dataDst, ConstAsciiStringView suiteName) noexcept {
			data = dataDst;
			data->suiteName = suiteName;
		}
		constexpr void setTestName(ConstAsciiStringView activeTestNameIn) noexcept {
			activeTestName = activeTestNameIn;
			failedActiveTest = false;
			data->testRunCount++;
			rng.seed(seed());
		}
		template<typename Type>
		constexpr Type compileOrRun(const Type& compileTime, const Type& runTime) noexcept {
			if (isConstantEvaluated()) {
				return compileTime;
			} else {
				return runTime;
			}
		}

		constexpr Bool failedTest() noexcept {
			return failedActiveTest;
		}

		constexpr TestAssertData& getAssertData(const SourceLocation location) noexcept {
			TestAssertData testAssertData(activeTestName, location);
			auto testAssertDataOption = data->assertDataSet.find(testAssertData);
			if (testAssertDataOption.hasValue()) {
				return *testAssertDataOption.getPtr();
			} else {
				return data->assertDataSet.insert(testAssertData).iter().deref();
			}
		}
		constexpr TestFailData& getFailData() noexcept {
			TestFailData testFailData(activeTestName);
			auto testFailDataOption = data->failDataSet.find(testFailData);
			if (testFailDataOption.hasValue()) {
				return *testFailDataOption.getPtr();
			} else {
				return data->failDataSet.insert(testFailData).iter().deref();
			}
		}

		void signalAssertSuccess(const SourceLocation location) noexcept {
			signalAssert(true, location);
		};

		void signalAssertFailure(const SourceLocation location) noexcept {
			signalAssert(false, location);
		};
		void signalAssert(Bool success, const SourceLocation location) noexcept {
			getAssertData(location).signalAssert(success);
			getFailData().signalAssert(success);
			
			if (success) {
				data->assertCount++;
			} else {
				if (data->failed == false) {
					data->testFailCount++;
				}
				failedActiveTest = true;
				data->failed = true;
				data->assertCount++;
				data->assertFailCount++;
				if (natl::impl::globalTestEnvironment.breakOnFailure) {
					natl::debugBreak();
					exit(ExitCode::failure);
				}
			}
		};

		template<typename... ArgTypes>
			requires(IsFormattableC<Decay<ArgTypes>, Ascii> && ...)
		constexpr void traceImpl(const SourceLocation location, ArgTypes&&... args) noexcept {
			if (isConstantEvaluated()) {
				return;
			}
			printlnf("trace: ", impl::formatLocation(location), ": ", natl::forward<ArgTypes>(args)...);
		}

		template<typename... ArgTypes>
			requires(IsFormattableC<Decay<ArgTypes>, Ascii> && ...)
		constexpr void assertImpl(Bool c, ConstAsciiStringView type, const SourceLocation location, ArgTypes&&... args) noexcept {
			if (!c) {
				if (isConstantEvaluated()) {
					constantEvaluatedError();
				}
				printlnf(type, ": ", impl::formatLocation(location), ": ", natl::forward<ArgTypes>(args)...);
				signalAssertFailure(location);
			} else {
				if (!isConstantEvaluated()) {
					signalAssertSuccess(location);
				}
			}
		}

	public:
		constexpr Size seed() const noexcept {
			if (isConstantEvaluated()) {
				return natl::defaultRandomSeed;
			} else {
				return natl::impl::globalTestEnvironment.testSeed;
			}
		}
		constexpr StandardRNG& getRNG() noexcept {
			return rng;
		}

		constexpr inline const char* getResourcePath() noexcept {
			return natl::impl::globalTestEnvironment.testResourcePath.c_str();
		}

		constexpr void fail(const SourceLocation location = SourceLocation::current()) noexcept {
			assertImpl(false, "fail", location);
		}
		constexpr void assertTrue(Bool condition, const SourceLocation location = SourceLocation::current()) noexcept {
			assertImpl(condition == true, "assertTrue: ", location);
		}
		constexpr void assertFalse(Bool condition, const SourceLocation location = SourceLocation::current()) noexcept {
			assertImpl(condition == false, "assertFalse: ", location);
		}
		template<typename Type>
		constexpr void assertNull(Type* ptr, const SourceLocation location = SourceLocation::current()) noexcept {
			if (isConstantEvaluated()) {
				assertImpl(ptr == nullptr, "assertNull: ", location);
			} else {
				assertImpl(ptr == nullptr, "assertNull: ", location, formatArgText<"hex">(bitCast<ui64>(ptr)));
			}
		}
		template<typename Type>
		constexpr void assertNotNull(Type* ptr, const SourceLocation location = SourceLocation::current()) noexcept {
			if (isConstantEvaluated()) {
				assertImpl(ptr != nullptr, "assertNull: ", location);
			} else {
				assertImpl(ptr != nullptr, "assertNull: ", location, formatArgText<"hex">(bitCast<ui64>(ptr)));
			}
		}


		template<typename Compare, typename LhsType, typename RhsType>
		constexpr void assertCondition(const LhsType& lhs, const RhsType& rhs, const SourceLocation location,
			const ConstAsciiStringView name, const ConstAsciiStringView compare) noexcept {
			if (!Compare::compare(lhs, rhs)) {
				if (isConstantEvaluated()) {
					constantEvaluatedError();
				}

				if constexpr (IsFormattableAsciiC<LhsType> && IsFormattableAsciiC<RhsType>) {
					printlnf(name, ": ", impl::formatLocation(location), ": not {", lhs, "} ", compare, " {", rhs, "}");
				} else {
					printlnf(name, ": ", impl::formatLocation(location));
				}
				signalAssertFailure(location);
			} else {
				if (!isConstantEvaluated()) {
					signalAssertSuccess(location);
				}
			}
		}

		template<typename LhsType, typename RhsType = LhsType>
			requires(HasCompareEqualC<LhsType, RhsType>)
		constexpr void assertEqual(const LhsType& lhs, const RhsType& rhs,
			const SourceLocation location = SourceLocation::current()) noexcept {
			assertCondition<CompareEqual<LhsType, RhsType>, LhsType, RhsType>(lhs, rhs, location, "assertEqual", "==");
		}
		template<typename LhsType, typename RhsType = LhsType>
			requires(HasCompareNotEqualC<LhsType, RhsType>)
		constexpr void assertNotEqual(const LhsType& lhs, const RhsType& rhs,
			const SourceLocation location = SourceLocation::current()) noexcept {
			assertCondition<CompareNotEqual<LhsType, RhsType>, LhsType, RhsType>(lhs, rhs, location, "assertNotEqual", "!=");
		}
		template<typename LhsType, typename RhsType = LhsType>
			requires(HasCompareLessC<LhsType, RhsType>)
		constexpr void assertLessThan(const LhsType& lhs, const RhsType& rhs,
			const SourceLocation location = SourceLocation::current()) noexcept {
			assertCondition<CompareLess<LhsType, RhsType>, LhsType, RhsType>(lhs, rhs, location, "assertLessThan", "<");
		}
		template<typename LhsType, typename RhsType = LhsType>
			requires(HasCompareLessEqualC<LhsType, RhsType>)
		constexpr void assertLessThanEqual(const LhsType& lhs, const RhsType& rhs,
			const SourceLocation location = SourceLocation::current()) noexcept {
			assertCondition<CompareLessEqual<LhsType, RhsType>, LhsType, RhsType>(lhs, rhs, location, "assertLessThanEqual", "<=");
		}
		template<typename LhsType, typename RhsType = LhsType>
			requires(HasCompareGreaterC<LhsType, RhsType>)
		constexpr void assertGreaterThan(const LhsType& lhs, const RhsType& rhs,
			const SourceLocation location = SourceLocation::current()) noexcept {
			assertCondition<CompareGreater<LhsType, RhsType>, LhsType, RhsType>(lhs, rhs, location, "assertGreaterThan", ">");
		}
		template<typename LhsType, typename RhsType = LhsType>
			requires(HasCompareGreaterEqualC<LhsType, RhsType>)
		constexpr void assertGreaterThanEqual(const LhsType& lhs, const RhsType& rhs,
			const SourceLocation location = SourceLocation::current()) noexcept {
			assertCondition<CompareGreaterEqual<LhsType, RhsType>, LhsType, RhsType>(lhs, rhs, location, "assertGreaterThanEqual", ">=");
		}

		template<typename LhsType, typename RhsType = LhsType>
			requires(HasCompareEqualC<LhsType, RhsType>)
		constexpr void assertEq(const LhsType& lhs, const RhsType& rhs,
			const SourceLocation location = SourceLocation::current()) noexcept {
			assertEqual<LhsType, RhsType>(lhs, rhs, location);
		}
		template<typename LhsType, typename RhsType = LhsType>
			requires(HasCompareNotEqualC<LhsType, RhsType>)
		constexpr void assertNe(const LhsType& lhs, const RhsType& rhs,
			const SourceLocation location = SourceLocation::current()) noexcept {
			assertNotEqual<LhsType, RhsType>(lhs, rhs, location);
		}
		template<typename LhsType, typename RhsType = LhsType>
			requires(HasCompareLessC<LhsType, RhsType>)
		constexpr void assertLt(const LhsType& lhs, const RhsType& rhs,
			const SourceLocation location = SourceLocation::current()) noexcept {
			assertLessThan<LhsType, RhsType>(lhs, rhs, location);
		}
		template<typename LhsType, typename RhsType = LhsType>
			requires(HasCompareLessEqualC<LhsType, RhsType>)
		constexpr void assertLe(const LhsType& lhs, const RhsType& rhs,
			const SourceLocation location = SourceLocation::current()) noexcept {
			assertLessThanEqual<LhsType, RhsType>(lhs, rhs, location);
		}
		template<typename LhsType, typename RhsType = LhsType>
			requires(HasCompareGreaterC<LhsType, RhsType>)
		constexpr void assertGt(const LhsType& lhs, const RhsType& rhs,
			const SourceLocation location = SourceLocation::current()) noexcept {
			assertGreaterThan<LhsType, RhsType>(lhs, rhs, location);
		}
		template<typename LhsType, typename RhsType = LhsType>
			requires(HasCompareGreaterEqualC<LhsType, RhsType>)
		constexpr void assertGe(const LhsType& lhs, const RhsType& rhs,
			const SourceLocation location = SourceLocation::current()) noexcept {
			assertGreaterThanEqual<LhsType, RhsType>(lhs, rhs, location);
		}

		/*
		# Python generator for trace functions
		for i in range(1, 10):
			print("template<", end='')
			types = []
			for j in range(i):
				types.append("typename T" + str(j + 1))
			print(", ".join(types), end='')
			print(">\n", end='')
    
			print("constexpr void trace(", end='')
			args = []
			for j in range(i):
				args.append("T" + str(j + 1) + "&& a" + str(j + 1))
			print(", ".join(args), end='')
			print(", const SourceLocation location = SourceLocation::current()) noexcept {\n", end='')
    
			print("traceImpl(location, ", end='');
			params = []
			for j in range(i):
				params.append("forward(a" + str(j + 1) + ")")
			print(", ".join(params), end='')
			print(");\n", end='')
    
			print("}\n", end='')
		*/
		constexpr void trace(const SourceLocation location = SourceLocation::current()) noexcept {
			traceImpl(location);
		}
		template<typename T1>
		constexpr void trace(T1&& a1, const SourceLocation location = SourceLocation::current()) noexcept {
			traceImpl(location, forward<T1>(a1));
		}
		template<typename T1, typename T2>
		constexpr void trace(T1&& a1, T2&& a2, const SourceLocation location = SourceLocation::current()) noexcept {
			traceImpl(location, forward<T1>(a1), forward<T2>(a2));
		}
		template<typename T1, typename T2, typename T3>
		constexpr void trace(T1&& a1, T2&& a2, T3&& a3, const SourceLocation location = SourceLocation::current()) noexcept {
			traceImpl(location, forward<T1>(a1), forward<T2>(a2), forward<T3>(a3));
		}
		template<typename T1, typename T2, typename T3, typename T4>
		constexpr void trace(T1&& a1, T2&& a2, T3&& a3, T4&& a4, const SourceLocation location = SourceLocation::current()) noexcept {
			traceImpl(location, forward<T1>(a1), forward<T2>(a2), forward<T3>(a3), forward<T4>(a4));
		}
		template<typename T1, typename T2, typename T3, typename T4, typename T5>
		constexpr void trace(T1&& a1, T2&& a2, T3&& a3, T4&& a4, T5&& a5, const SourceLocation location = SourceLocation::current()) noexcept {
			traceImpl(location, forward<T1>(a1), forward<T2>(a2), forward<T3>(a3), forward<T4>(a4), forward<T5>(a5));
		}
		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
		constexpr void trace(T1&& a1, T2&& a2, T3&& a3, T4&& a4, T5&& a5, T6&& a6, const SourceLocation location = SourceLocation::current()) noexcept {
			traceImpl(location, forward<T1>(a1), forward<T2>(a2), forward<T3>(a3), forward<T4>(a4), forward<T5>(a5), forward<T6>(a6));
		}
		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
		constexpr void trace(T1&& a1, T2&& a2, T3&& a3, T4&& a4, T5&& a5, T6&& a6, T7&& a7, const SourceLocation location = SourceLocation::current()) noexcept {
			traceImpl(location, forward<T1>(a1), forward<T2>(a2), forward<T3>(a3), forward<T4>(a4), forward<T5>(a5), forward<T6>(a6), forward<T7>(a7));
		}
		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
		constexpr void trace(T1&& a1, T2&& a2, T3&& a3, T4&& a4, T5&& a5, T6&& a6, T7&& a7, T8&& a8, const SourceLocation location = SourceLocation::current()) noexcept {
			traceImpl(location, forward<T1>(a1), forward<T2>(a2), forward<T3>(a3), forward<T4>(a4), forward<T5>(a5), forward<T6>(a6), forward<T7>(a7), forward<T8>(a8));
		}
		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
		constexpr void trace(T1&& a1, T2&& a2, T3&& a3, T4&& a4, T5&& a5, T6&& a6, T7&& a7, T8&& a8, T9&& a9, const SourceLocation location = SourceLocation::current()) noexcept {
			traceImpl(location, forward<T1>(a1), forward<T2>(a2), forward<T3>(a3), forward<T4>(a4), forward<T5>(a5), forward<T6>(a6), forward<T7>(a7), forward<T8>(a8), forward<T9>(a9));
		}

		/*
		# Python generator for trace functions
		for i in range(1, 10):
			print("template<", end='')
			types = []
			for j in range(i):
				types.append("typename T" + str(j + 1))
			print(", ".join(types), end='')
			print(">\n", end='')
    
			print("constexpr void tassert(natl::Bool c,", end='')
			args = []
			for j in range(i):
				args.append("T" + str(j + 1) + "&& a" + str(j + 1))
			print(", ".join(args), end='')
			print(", const SourceLocation location = SourceLocation::current()) noexcept {\n", end='')
    
			print("assertImpl(c, \"tassert\", location, ", end='');
			params = []
			for j in range(i):
				params.append("forward<T" + str(j + 1) + ">(a" + str(j + 1) + ")")
			print(", ".join(params), end='')
			print(");\n", end='')
    
			print("}\n", end='')
		*/
		constexpr void tassert(Bool condition, const SourceLocation location = SourceLocation::current()) noexcept {
			assertImpl(condition == true, "tassert", location);
		}
		template<typename T1>
		constexpr void tassert(natl::Bool c, T1&& a1, const SourceLocation location = SourceLocation::current()) noexcept {
			assertImpl(c, "tassert", location, forward<T1>(a1));
		}
		template<typename T1, typename T2>
		constexpr void tassert(natl::Bool c, T1&& a1, T2&& a2, const SourceLocation location = SourceLocation::current()) noexcept {
			assertImpl(c, "tassert", location, forward<T1>(a1), forward<T2>(a2));
		}
		template<typename T1, typename T2, typename T3>
		constexpr void tassert(natl::Bool c, T1&& a1, T2&& a2, T3&& a3, const SourceLocation location = SourceLocation::current()) noexcept {
			assertImpl(c, "tassert", location, forward<T1>(a1), forward<T2>(a2), forward<T3>(a3));
		}
		template<typename T1, typename T2, typename T3, typename T4>
		constexpr void tassert(natl::Bool c, T1&& a1, T2&& a2, T3&& a3, T4&& a4, const SourceLocation location = SourceLocation::current()) noexcept {
			assertImpl(c, "tassert", location, forward<T1>(a1), forward<T2>(a2), forward<T3>(a3), forward<T4>(a4));
		}
		template<typename T1, typename T2, typename T3, typename T4, typename T5>
		constexpr void tassert(natl::Bool c, T1&& a1, T2&& a2, T3&& a3, T4&& a4, T5&& a5, const SourceLocation location = SourceLocation::current()) noexcept {
			assertImpl(c, "tassert", location, forward<T1>(a1), forward<T2>(a2), forward<T3>(a3), forward<T4>(a4), forward<T5>(a5));
		}
		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
		constexpr void tassert(natl::Bool c, T1&& a1, T2&& a2, T3&& a3, T4&& a4, T5&& a5, T6&& a6, const SourceLocation location = SourceLocation::current()) noexcept {
			assertImpl(c, "tassert", location, forward<T1>(a1), forward<T2>(a2), forward<T3>(a3), forward<T4>(a4), forward<T5>(a5), forward<T6>(a6));
		}
		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
		constexpr void tassert(natl::Bool c, T1&& a1, T2&& a2, T3&& a3, T4&& a4, T5&& a5, T6&& a6, T7&& a7, const SourceLocation location = SourceLocation::current()) noexcept {
			assertImpl(c, "tassert", location, forward<T1>(a1), forward<T2>(a2), forward<T3>(a3), forward<T4>(a4), forward<T5>(a5), forward<T6>(a6), forward<T7>(a7));
		}
		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
		constexpr void tassert(natl::Bool c, T1&& a1, T2&& a2, T3&& a3, T4&& a4, T5&& a5, T6&& a6, T7&& a7, T8&& a8, const SourceLocation location = SourceLocation::current()) noexcept {
			assertImpl(c, "tassert", location, forward<T1>(a1), forward<T2>(a2), forward<T3>(a3), forward<T4>(a4), forward<T5>(a5), forward<T6>(a6), forward<T7>(a7), forward<T8>(a8));
		}
		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
		constexpr void tassert(natl::Bool c, T1&& a1, T2&& a2, T3&& a3, T4&& a4, T5&& a5, T6&& a6, T7&& a7, T8&& a8, T9&& a9, const SourceLocation location = SourceLocation::current()) noexcept {
			assertImpl(c, "tassert", location, forward<T1>(a1), forward<T2>(a2), forward<T3>(a3), forward<T4>(a4), forward<T5>(a5), forward<T6>(a6), forward<T7>(a7), forward<T8>(a8), forward<T9>(a9));
		}
	};

	template<typename TestSuiteType>
	struct TestSuite : public TestSuiteBase {
	public:
		using test_suite_type = TestSuiteType;
		constexpr TestSuite() noexcept = default;
		constexpr TestSuite(const TestSuite&) noexcept = delete;
		constexpr TestSuite(TestSuite&&) noexcept = delete;
	
		constexpr ~TestSuite() noexcept = default;

		TestSuite& operator=(const TestSuite&) noexcept = delete;
		TestSuite& operator=(TestSuite&&) noexcept = delete;
	};

	namespace impl {
		enum struct TestCommand {
			help,
			noArgs,
			listTests,
			listSuites,
			breakOnFailure,
			runAll,
			shuffleTests,
			shuffleSuites,
			repeat,
			test,
			timeout,
			seed,
			resourcePath,
			reportTests,
			reportAsserts,
			reportTestsFailed,
			reportAssertsFailed,
		};

		void testCommandHelp(const TestCommand command) noexcept;
	}

	ExitCode test_main(const MainArgsView args) noexcept;
}