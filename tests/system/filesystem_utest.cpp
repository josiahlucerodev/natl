//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/util/test.h>
#include <natl/system/filesystem.h>
#include <natl/container/smallDynArray.h>
#include <natl/system/printFormatted.h>

struct FilesystemTestSuite : public natl::TestSuite<FilesystemTestSuite> {};

template<typename test_suite_type = FilesystemTestSuite>
struct FilesystemTestSuiteTests {
	constexpr static void tests(test_suite_type& ts) noexcept {
		using namespace natl::literals;
		natl::String testFile = natl::String(ts.getResourcePath()) + "/test.txt";
		ts.trace("test file: ", testFile);
		natl::File file(testFile.c_str(), natl::FileOpenMode::readStart);
		natl::StringByteSize<100> fileContents;
		const natl::LoadAllFileContentError error = natl::loadAllFileContent(file, fileContents);

		ts.assertEqual(natl::LoadAllFileContentError::none, error);
		ts.assertEqual("Hello World!"_natl_sv, fileContents.toStringView());

		ts.assertFalse(natl::getWorkingDirectoryAs<natl::String>().isEmpty());
	}
	NATL_REGISTER_TEST(tests);
};

NATL_REGISTER_STD_TESTS(FilesystemTestSuite);

NATL_TESTS_MAIN();