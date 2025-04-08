//natl
#include <natl/system/filesystem.h>
#include <natl/container/smallDynArray.h>
#include <natl/system/printFormatted.h>

//own
#include "../../testUtils.h"

int main() {
	natl::Path testFilePath = natl::getTestResourcePath();
	testFilePath /= "filesystem/test.txt";

	natl::File file(testFilePath.c_str(), natl::FileOpenMode::readStart);
	natl::StringByteSize<100> fileContents;
	const natl::LoadAllFileContentError error = natl::loadAllFileContent(file, fileContents);
	if (error != natl::LoadAllFileContentError::none) {
		natl::printlnf("file load failed: ", natl::loadAllFileContentErrorToString(error));
	} else {
		natl::print("file contents: ");
		natl::println(fileContents);
	}
	natl::printlnf("working dir: ", natl::getWorkingDirectoryIn<natl::String>());
}