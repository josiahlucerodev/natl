//@interface
#include "test.h"

//@begin_non_modules
//natl
#include "executionSessionMacros.h"
//@end_non_modules

//own
#include "random.h"
#include "../container/flatHashSet.h"
#include "../units/standard.h"
#include "../sync/smartThread.h"
#include "../system/timer.h"

namespace natl {
	enum struct ReportType {
		none,
		tests,
		asserts,
		testsFailed,
		assertsFailed,
	};

	namespace impl {
		void timeoutThread(ThreadStopAlert stopAlert, Seconds<Size> timeout) noexcept {
			Timer timer;
			timer.start();

			while (!stopAlert.stopRequested() && timer.getSecondsInt().asInt<Size>() < timeout) {
				thisThreadSleep(Milliseconds(10).asInt<i64>());
			}

			if (timer.getSecondsInt().asInt<Size>() >= timeout) {
				printlnf("timeout failure: ", timer.getSecondsInt(), " or ", timer.getMillisecondsInt());
				if (impl::globalTestEnvironment.breakOnFailure) {
					debugBreak();
				}
				exit(ExitCode::failure);
			} else {
				printlnf("test time: ", timer.getSecondsInt(), " or ", timer.getMillisecondsInt());
			}
		}

		void testCommandHelp(const TestCommand command) noexcept {
			switch (command) {
			break; case TestCommand::help:
				println("\t--help or <command> -h");
				println("\tprint info about commands or command specified");
				println("\twill cause --run-all to be defaulted to false");
			break; case TestCommand::noArgs:
				println("\t--n or --no-args");
				println("\tprevent program args from being outputted");
			break; case TestCommand::listTests:
				println("\t-l or --list-tests");
				println("\tlists all tests");
				println("\twill cause --run-all to be defaulted to false");
			break; case TestCommand::listSuites:
				println("\t-ls or --list-suites");
				println("\tlists all test suites");
				println("\twill cause --run-all to be defaulted to false");
			break; case TestCommand::breakOnFailure:
				println("\t-b or --break-on-failure");
				println("\tbreaks during any test failure");
			break; case TestCommand::runAll:
				println("\t-a or --run-all");
				println("\trun all tests");
			break; case TestCommand::shuffleTests:
				println("\t-st or --shuffle-tests");
				println("\trun tests in random order based on seed");
			break; case TestCommand::shuffleSuites:
				println("\t-ss or --shuffle-suites");
				println("\trun suites in random order based on seed");
			break; case TestCommand::repeat:
				println("\t-r <count> or --repeat <count>");
				println("\trun tests <count> number of times");
			break; case TestCommand::test:
				println("\t-t <test_name> or --test <test_name>");
				println("\trun only the named test give by <test_name>");
				println("\ttest is ran with <test_name> formatted as <suite_name>::<test_name>");
				println("\ttest suite is ran with <test_name> formatted as <suite_name>");
			break; case TestCommand::timeout:
				println("\t-o <time_out> or --timeout <time_seconds>");
				println("\tonly run testing for <time_seconds> amount of seconds before stoping test as failure");
			break; case TestCommand::seed:
				println("\t-s <number> or --seed <number>");
				println("\trun tests with seed which is provided to tests and used for shuffling");
				println("\tby default the seed is {natl::defaultRandomSeed} for \"deterministic\" testing");
			break; case TestCommand::resourcePath:
				println("\t-rp <path> or --resource-path <path>");
				println("\truns tests with given resource-path");
			break; case TestCommand::reportTests:
				println("\t-rt or --report-tests");
				println("\treport all tests at end of each testing run");
			break; case TestCommand::reportAsserts:
				println("\t-ra or --report-asserts");
				println("\treport all tests and asserts at end of each testing run");
			break; case TestCommand::reportTestsFailed:
				println("\t-rtf or --report-tests-failed");
				println("\treport failed tests at end of each testing run");
			break; case TestCommand::reportAssertsFailed:
				println("\t-raf or --report-asserts-failed");
				println("\treport failed tests and asserts at end of each testing run");
			break; default:
				println("\t<unknown commands>");
			}
		};

		struct FilteredSuiteTests {
		public:
			ConstAsciiStringView name;
			natl::DynArray<natl::ConstAsciiStringView> tests;
		};

		natl::DynArray<FilteredSuiteTests> allTests() noexcept {
			natl::DynArray<FilteredSuiteTests> filteredSuiteTests;
			for (auto tests : impl::globalTestEnvironment.unitTests) {
				natl::DynArray<natl::ConstAsciiStringView> testsArray;
				for (auto test : tests.value()) {
					testsArray.pushBack(test.name);
				}

				FilteredSuiteTests filteredSuiteTest;
				filteredSuiteTest.name = tests.key();
				filteredSuiteTest.tests = testsArray;
				filteredSuiteTests.pushBack(filteredSuiteTest);
			}
			return filteredSuiteTests;
		}

		natl::DynArray<FilteredSuiteTests> filterTests(ArrayView<ConstAsciiStringView> testsArg) noexcept {
			natl::FlatHashMap<natl::ConstAsciiStringView, natl::FlatHashSet<natl::ConstAsciiStringView>> testsMap;
			for (const ConstAsciiStringView testArg : testsArg) {
				const natl::Size suiteIndex = testArg.find("::");
				if (suiteIndex == natl::String::npos) {
					const ConstAsciiStringView suiteName = testArg;
					if (not impl::globalTestEnvironment.unitTestSuites.find(suiteName).hasValue()) {
						printlnf("CLI Error: test suite {", suiteName, "} does not exist");
						exit(ExitCode::failure);
					}

					auto suiteTests = globalTestEnvironment.unitTests.find(suiteName);
					if (not suiteTests.hasValue()) {
						printlnf("CLI Error: test suite {", suiteName, "} does not have any tests");
						exit(ExitCode::failure);
					}

					if (testsMap.find(suiteName)) {
						printlnf("CLI Error: test suite {", suiteName, "} has tests already added");
						exit(ExitCode::failure);
					}

					natl::FlatHashSet<natl::ConstAsciiStringView> suiteTestsToRun;
					for (const auto test : suiteTests.value().value()) {
						suiteTestsToRun.insert(test.name);
					}
					testsMap.insert(suiteName, suiteTestsToRun);
				} else {
					const ConstAsciiStringView suiteName = testArg.substr(0, suiteIndex);
					const ConstAsciiStringView testName = testArg.substr(suiteIndex + 2);

					if (not globalTestEnvironment.unitTestSuites.find(suiteName).hasValue()) {
						printlnf("CLI Error: test suite {", suiteName, "} does not exist. Defined in ", testArg);
						exit(ExitCode::failure);
					} 

					auto suiteTests = globalTestEnvironment.unitTests.find(suiteName);
					if (not suiteTests.hasValue()) {
						printlnf("CLI Error: test suite {", suiteName, "} does not have any tests");
						exit(ExitCode::failure);
					}

					Bool found = false;
					for (const auto suiteTest : suiteTests.value().value()) {
						if (testArg == suiteTest.name) {
							found = true;
						}
					}

					if (not found) {
						printlnf("CLI Error: test {", testArg, "} does not exist");
						exit(ExitCode::failure);
					}

					auto suiteTestsToRun = testsMap.find(suiteName);
					if (suiteTestsToRun.hasValue() && suiteTestsToRun.value().value().contains(testName)) {
						printlnf("CLI Error: test {", testArg, "} already defined");
						exit(ExitCode::failure);
					}

					if (suiteTestsToRun.hasValue()) {
						suiteTestsToRun.value().value().insert(testName);
					} else {
						testsMap.insert(suiteName, {}).iter().keyValue().value().insert(testName);
					}
				}
			}

			natl::DynArray<FilteredSuiteTests> filteredSuiteTests;
			for (auto tests : testsMap) {
				natl::DynArray<natl::ConstAsciiStringView> testsArray;
				for (auto test : tests.value()) {
					testsArray.pushBack(test);
				}
				FilteredSuiteTests filteredSuiteTest;
				filteredSuiteTest.name = tests.key();
				filteredSuiteTest.tests = testsArray;
				filteredSuiteTests.pushBack(filteredSuiteTest);
			}
			return filteredSuiteTests;
		}
	}

	void reportTest(const TestSuiteTestingData& testingData, ReportType reportType) noexcept {
		natl::printlnf("natl_tests: test suite ", testingData.suiteName, " ", testingData.failed ? "failed" : "passed", ": ",
			testingData.testCount, " tests attempted, ", 
			testingData.testRunCount, " tests ran, ",
			"(", testingData.testCount - testingData.testFailCount, "/", testingData.testCount, ") tests passed, ",
			"(", testingData.assertCount - testingData.assertFailCount, "/", testingData.assertCount, ") asserts passed"
		);

		auto timeNS = testingData.time.asFloat<natl::f64>();
		auto timeS = timeNS.convertTo<natl::abbrt::s>();
		auto timeMs = timeNS.convertTo<natl::abbrt::ms>();
		auto timeMus = timeNS.convertTo<natl::abbrt::mus>();
		natl::printlnf("natl_tests: test time (", 
			natl::formatArgText<"p: 2">(timeS.value()), timeS.units(), " or ",
			natl::formatArgText<"p: 2">(timeMs.value()), timeMs.units(), " or ",
			natl::formatArgText<"p: 2">(timeMus.value()), timeMus.units(), " or ",
			natl::formatArgText<"p: 2">(timeNS.value()), timeNS.units(), ")");

		if (reportType == ReportType::none) {
			return;
		}
		
		using test_assert_dyn_array_type = natl::SmallDynArray<ObserverPtr<const TestAssertData>, 16>;
		natl::SmallFlatHashMap<natl::ConstAsciiStringView, test_assert_dyn_array_type, 16> testAssertDatasMap;
		if (reportType == ReportType::asserts || reportType == ReportType::assertsFailed) {
			for (const TestAssertData& assertData : testingData.assertDataSet) {
				auto testAssertDatasOption = testAssertDatasMap.find(assertData.testName);
				if (testAssertDatasOption.hasValue()) {
					testAssertDatasOption->value().pushBack(makeObserver(&assertData));
				} else {
					testAssertDatasMap.insert(assertData.testName, { makeObserver(&assertData) });
				}
			}
		}

		for (const TestFailData& failData : testingData.failDataSet) {
			if ((reportType == ReportType::testsFailed || reportType == ReportType::assertsFailed) && !failData.failed) {
				continue;
			}

			natl::printlnf("\t", testingData.suiteName, "::", failData.testName, " ", failData.failed ? "failed" : "passed",
				" (", failData.assertRunCount - failData.assertFailCount, "/", failData.assertRunCount, ") asserts passed");
			
			if (reportType == ReportType::asserts || reportType == ReportType::assertsFailed) {
				auto testAssertDatasOption = testAssertDatasMap.find(failData.testName);
				if (testAssertDatasOption.hasValue()) {
					test_assert_dyn_array_type& assertTestDatas = testAssertDatasOption->value();
					sort(assertTestDatas.begin(), assertTestDatas.end(),
						[](ObserverPtr<const TestAssertData> lhs, ObserverPtr<const TestAssertData> rhs) noexcept {
							return lhs->location.line() < rhs->location.line();
						}
					);

					for (const ObserverPtr<const TestAssertData> testAssertData : assertTestDatas) {
						if (reportType == ReportType::assertsFailed && !testAssertData->failed) {
							continue;
						}

						natl::printlnf("\t\t", impl::formatLocation(testAssertData->location, false),
							" (", testAssertData->assertRunCount - testAssertData->assertFailCount, "/", testAssertData->assertRunCount, ") asserts passed");
					}
				}
			}
		}
	}

	Bool runTestSuite(impl::FilteredSuiteTests suiteTests, TestSuiteTestingData& testingData) noexcept {
		auto suiteInfoOption = impl::globalTestEnvironment.unitTestSuites.find(suiteTests.name);
		if (not suiteInfoOption.hasValue()) {
			natl::printlnf("Testing Failed: failed to run tests: test suite ", suiteTests.name, " not found");
			return false;
		}

		auto testInfosOption = impl::globalTestEnvironment.unitTests.find(suiteTests.name);
		if (not testInfosOption.hasValue()) {
			natl::printlnf("Testing failed: failed to run tests: tests for ", suiteTests.name, " not found");
			return false;
		}

		auto& suiteInfo = suiteInfoOption.value().value();
		TestSuiteBase* testSuite = nullptr;

		constexpr natl::ConstAsciiStringView sessionName = "natl::RunTest";
		natl::ObserverPtr<natl::ExecutionSession> session = natl::pushExecutionSession(sessionName);
		
		auto failure = [&]() -> Bool {
			natl::popExecutionSession(sessionName);
			return false;
		};

		if (NATL_EXECUTION_SESSION_ERROR(session)) {
			auto errorInfoExpect = natl::getExecutionSessionError();
			if (errorInfoExpect.hasValue()) {
				natl::println("Test Failed:");
				natl::println(errorInfoExpect.value()->errorMessage.c_str());
			}

			natl::popExecutionSession(sessionName);
			natl::executionSessionErrorHandled();
			return false;
		}

		Timer timer;
		timer.start();

		testSuite = suiteInfo.createFunc();

		if (!testSuite->suiteSetup()) {
			natl::printlnf("Test Failed: failed to setup test suite", suiteTests.name);
			return failure();
		}

		auto testInfos = testInfosOption.value().value();
		testingData.testCount = testInfos.size();
		testSuite->setTestingConfig(makeObserver(&testingData), suiteInfo.name);
		natl::FlatHashMap<natl::String, natl::impl::TestInfo> testInfoMap;
		for (impl::TestInfo& testInfo : testInfos) {
			testInfoMap.insert(testInfo.name, testInfo);
		}

		for (auto test : suiteTests.tests) {
			natl::impl::TestInfo& testInfo = testInfoMap.find(test).value().value();
			testSuite->setTestName(testInfo.name);

			if (!testSuite->setup()) {
				natl::printlnf("natl_test: Test Failed: failed to setup for test ", suiteTests.name, "::", testInfo.name);
				return failure();
			}

			testInfo.runFunc(testSuite);
			if (testSuite->failedTest()) {
				natl::printlnf("natl_test: Test Failed: ", suiteTests.name, "::", testInfo.name, " failed one or more asserts");
			}

			if (!testSuite->teardown()) {
				natl::printlnf("natl_test: Test Failed: failed to teardown from test ", suiteTests.name, "::", testInfo.name);
				return failure();
			}
		}

		if (!testSuite->suiteTeardown()) {
			natl::printlnf("natl_test: Test Failed: failed to teardown test suite ", suiteTests.name);
			return failure();
		}

		suiteInfo.destoryFunc(testSuite);

		natl::popExecutionSession(sessionName);
		testingData.time = timer.getNanosecondsInt().asInt<natl::Size>();
		
		return true;
	}

	natl::Bool runTests(natl::DynArray<impl::FilteredSuiteTests> suiteTests, Option<Size> timeout, ReportType reportType) noexcept {
		SmartThread timeoutTread;
		if (timeout) {
			timeoutTread = SmartThread(impl::timeoutThread, Seconds<Size>(timeout.value()));
		}

		auto timeoutEnd = [&]() {
			if (timeout) {
				timeoutTread.requestStop();
			}
		};

		constexpr natl::ConstAsciiStringView sessionName = "natl::RunTests";
		natl::ObserverPtr<natl::ExecutionSession> session = natl::pushExecutionSession(sessionName);

		if (NATL_EXECUTION_SESSION_ERROR(session)) {
			auto errorInfoExpect = natl::getExecutionSessionError();
			if (errorInfoExpect.hasValue()) {
				natl::println(errorInfoExpect.value()->errorMessage.c_str());
			}

			natl::popExecutionSession(sessionName);
			natl::executionSessionErrorHandled();
			timeoutEnd();
			exit(ExitCode::failure);
		}

		Bool failed = false;
		for (impl::FilteredSuiteTests& filteredSuiteTests : suiteTests) {
			TestSuiteTestingData testingData;
			Bool s = runTestSuite(filteredSuiteTests, testingData);
			reportTest(testingData, reportType);
			if (s == false || testingData.failed) {
				failed = true;
			}
		}

		natl::popExecutionSession(sessionName);
		timeoutEnd();
		return !failed;
	}

	ExitCode test_main(const MainArgsView args) noexcept {
		natl::impl::globalTestEnvironment.breakOnFailure = false;
		natl::impl::globalTestEnvironment.testSeed = defaultRandomSeed;
		Bool help = false;
		Bool noArgs = false;
		Bool listTests = false;
		Bool listSuites = false;
		Bool runAll = false;
		Bool shuffleTests = false;
		Bool shuffleSuites = false;
		Bool reportTests = false;
		Bool reportAsserts = false;
		Bool reportTestsFailed = false;
		Bool reportAssertsFailed = false;
		ReportType reportType = ReportType::none;
		Bool defaultRunAll = true;

		Option<Size> repeat;
		Option<Size> timeout;
		DynArray<ConstAsciiStringView> tests;

		for (Size i = 1; i < args.size(); i++) {
			const ConstAsciiStringView arg = args[i];
			auto hasNextArg = [&]() {
				return isInRange(args, i + 1);
			};
			auto getNextArg = [&]() {
				i++;
				return args[i];
			};
			auto commandHelp = [&]() {
				return hasNextArg() && (args[i + 1] == "-h" || args[i + 1] == "--help");
			};
			auto outputCommandHelp = [&](const impl::TestCommand command) -> ExitCode {
				impl::testCommandHelp(command);
				return ExitCode::success;
			};

			if (arg == "-h" || arg == "--help") {
				if (commandHelp()) { return outputCommandHelp(impl::TestCommand::help); }
				help = true;
				defaultRunAll = false;
			} else if (arg == "-n" || arg == "--no-args") {
				if (commandHelp()) { return outputCommandHelp(impl::TestCommand::noArgs); }
				noArgs = true;
			} else if (arg == "-l" || arg == "--list-tests") {
				if (commandHelp()) { return outputCommandHelp(impl::TestCommand::listTests); }
				listTests = true;
				defaultRunAll = false;
			} else if (arg == "-ls" || arg == "--list-suites") {
				if (commandHelp()) { return outputCommandHelp(impl::TestCommand::listSuites); }
				listSuites = true;
				defaultRunAll = false;
			} else if (arg == "-b" || arg == "--break-on-failure") {
				if (commandHelp()) { return outputCommandHelp(impl::TestCommand::breakOnFailure); }
				natl::impl::globalTestEnvironment.breakOnFailure = true;
			} else if (arg == "-a" || arg == "--run-all") {
				if (commandHelp()) { return outputCommandHelp(impl::TestCommand::runAll); }
				runAll = true;
			} else if (arg == "-st" || arg == "--shuffle-tests") {
				if (commandHelp()) { return outputCommandHelp(impl::TestCommand::shuffleTests); }
				shuffleTests = true;
			} else if (arg == "-ss" || arg == "--shuffle-suites") {
				if (commandHelp()) { return outputCommandHelp(impl::TestCommand::shuffleSuites); }
				shuffleSuites = true;
			} else if (arg == "-rt" || arg == "--report-tests") {
				if (commandHelp()) { return outputCommandHelp(impl::TestCommand::reportTests); }
				reportTests = true;
				reportType = ReportType::tests;
			} else if (arg == "-ra" || arg == "--report-asserts") {
				if (commandHelp()) { return outputCommandHelp(impl::TestCommand::reportAsserts); }
				reportAsserts = true;
				reportType = ReportType::asserts;
			} else if (arg == "-rtf" || arg == "--report-tests-failed") {
				if (commandHelp()) { return outputCommandHelp(impl::TestCommand::reportTestsFailed); }
				reportTestsFailed = true;
				reportType = ReportType::testsFailed;
			} else if (arg == "-raf" || arg == "--report-asserts-failed") {
				if (commandHelp()) { return outputCommandHelp(impl::TestCommand::reportAssertsFailed); }
				reportType = ReportType::assertsFailed;
			} else if (arg == "-r" || arg == "--repeat") {
				if (commandHelp()) { return outputCommandHelp(impl::TestCommand::repeat); }
				if (!hasNextArg()) {
					println("CLI Error: expect repeat to have assocated repeat count");
					impl::testCommandHelp(impl::TestCommand::repeat);
					return ExitCode::failure;
				}
				ConstAsciiStringView repeatCountArg = getNextArg();
				Expect<Size, NumericConvertError> repeatCountExpect
					= stringDecimalToInt<Size>(repeatCountArg);
				if (repeatCountExpect.hasError()) {
					printlnf("CLI Error: repeat count parse error: ",
						convertErrorToString(repeatCountExpect.error().flag));
					impl::testCommandHelp(impl::TestCommand::repeat);
					return ExitCode::failure;
				}
				repeat = repeatCountExpect.value();
				if (repeat.value() == 0) {
					printlnf("CLI Error: repeat cannot be zero");
					impl::testCommandHelp(impl::TestCommand::repeat);
					return ExitCode::failure;
				}
			} else if (arg == "-t" || arg == "--test") {
				if (commandHelp()) { return outputCommandHelp(impl::TestCommand::test); }
				if (!hasNextArg()) {
					println("CLI Error: expect -t to have assocated test or test suite name");
					impl::testCommandHelp(impl::TestCommand::test);
					return ExitCode::failure;
				}

				while (hasNextArg()) {
					const ConstAsciiStringView test = getNextArg();
					if (test.startsWith("-")) {
						i--;
						break;
					} else {
						tests.pushBack(test);
					}
				}
			} else if (arg == "-o" || arg == "--timeout") {
				if (commandHelp()) { return outputCommandHelp(impl::TestCommand::timeout); }
				if (!hasNextArg()) {
					println("CLI Error: expect timout to have assocated time in seconds");
					impl::testCommandHelp(impl::TestCommand::timeout);
					return ExitCode::failure;
				}
				ConstAsciiStringView repeatCountArg = getNextArg();
				Expect<Size, NumericConvertError> timeoutExpect
					= stringDecimalToInt<Size>(repeatCountArg);
				if (timeoutExpect.hasError()) {
					printlnf("CLI Error: timeout parse error: ",
						convertErrorToString(timeoutExpect.error().flag));
					impl::testCommandHelp(impl::TestCommand::timeout);
					return ExitCode::failure;
				}
				timeout = timeoutExpect.value();
				if (timeout.value() == 0) {
					printlnf("CLI Error: timout cannot be zero");
					impl::testCommandHelp(impl::TestCommand::timeout);
					return ExitCode::failure;
				}
			} else if (arg == "-s" || arg == "--seed") {
				if (commandHelp()) { return outputCommandHelp(impl::TestCommand::seed); }
				if (!hasNextArg()) {
					println("CLI Error: expect seed to have assocated integer value as seed");
					impl::testCommandHelp(impl::TestCommand::seed);
					return ExitCode::failure;
				}
				ConstAsciiStringView seedArg = getNextArg();
				Expect<Size, NumericConvertError> seedExpect
					= stringDecimalToInt<Size>(seedArg);
				if (seedExpect.hasError()) {
					printlnf("CLI Error: seed parse error: ",
						convertErrorToString(seedExpect.error().flag));
					impl::testCommandHelp(impl::TestCommand::seed);
					return ExitCode::failure;
				}
				natl::impl::globalTestEnvironment.testSeed = seedExpect.value();
			} else if (arg == "-rp" || arg == "--resource-path") {
				if (commandHelp()) { return outputCommandHelp(impl::TestCommand::resourcePath); }
				if (!hasNextArg()) {
					println("CLI Error: expect --resource-path to have assocated string path");
					impl::testCommandHelp(impl::TestCommand::resourcePath);
					return ExitCode::failure;
				}
				natl::impl::globalTestEnvironment.testResourcePath = getNextArg();
			} else {
				printlnf("CLI Error: unknown argument: ", arg);
				return ExitCode::failure;
			}
		}

		if (runAll && not tests.isEmpty()) {
			printlnf("CLI Error: --run-all and --test were provided");
			return ExitCode::failure;
		}

		Bool onlyInfoCommands = defaultRunAll == false && tests.isEmpty() && runAll == false;

		runAll = runAll || defaultRunAll;
		natl::DynArray<impl::FilteredSuiteTests> testsToRun;
		if (!onlyInfoCommands) {
			testsToRun = runAll ? impl::allTests() : impl::filterTests(tests);

			if (testsToRun.isEmpty()) {
				println("CLI Error: no tests to run");
				println("CLI Error: --help or --list-tests or --list-suites could have caused --ran-all to be defaulted to false");
				return ExitCode::failure;
			}
		}

		auto countFlag = [](Bool flag) noexcept {
			return flag ? 1 : 0;
		};

		if (countFlag(reportTests) + countFlag(reportAsserts) 
			+ countFlag(reportTestsFailed) + countFlag(reportAssertsFailed) > 1) {
			println("CLI Error: only one report flag can be defined");
			return ExitCode::failure;
		}

		if (!noArgs) {
			print("args: ");
			if (args.subview(1).isEmpty()) {
				println("none");
			} else {
				for (auto arg : args.subview(1)) {
					printf(arg, " ");
				}
				println();
			}
		}

		if (help) {
			constexpr auto commands = natl::getEnumValuesAsArray<impl::TestCommand>();
			for (const EnumValueInfo<impl::TestCommand> command : commands) {
				printlnf("-", command.name);
				impl::testCommandHelp(command.value);
			}
		}

		if (listTests) {
			println("Tests:");
			for (auto testSuite : natl::impl::globalTestEnvironment.unitTests) {
				for (auto test : testSuite.value()) {
					printlnf("\t", testSuite.key(), "::", test.name);
				}
			}
		}

		if (listSuites) {
			println("Test Suites:");
			for (auto testSuite : natl::impl::globalTestEnvironment.unitTestSuites) {
				printlnf("\t", testSuite.key());
			}
		}

		if (onlyInfoCommands) {
			return ExitCode::success;
		}

		const natl::Size runAmount = repeat.hasValue() ? repeat.value() : 1;
		StandardRNG rng(natl::impl::globalTestEnvironment.testSeed);
		Bool passed = true;
		natl::Size runsFailed = 0;
		for (natl::Size i = 0; i < runAmount; i++) {
			if (shuffleTests) {
				for (impl::FilteredSuiteTests& filteredSuiteTests : testsToRun) {
					shuffle(filteredSuiteTests.tests.begin(), filteredSuiteTests.tests.end(), rng);
				}
			}
			if (shuffleSuites) {
				shuffle(testsToRun.begin(), testsToRun.end(), rng);
			}

			natl::printlnf("natl_tests: tests run attempt ", i + 1, " begin");
			Bool s = runTests(testsToRun, timeout, reportType);
			natl::printlnf("natl_tests: tests run attempt ", i + 1, " end ", s ? "success" : "failure");
			if (!s) {
				passed = false;
				runsFailed++;
			}
		}

		natl::printlnf("natl_tests: testing attempts (", runAmount - runsFailed, "/", runAmount, ") passed");
		natl::printlnf("natl_tests: testing ", passed ? "success" : "failure");
		return passed ? ExitCode::success : ExitCode::failure;
	}
}