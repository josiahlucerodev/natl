//natl
#include <natl/filesystem.h>
#include <natl/smallDynArray.h>

//own
#include "../testUtils.h"

int main() {
	natl::Path testFilePath = natl::getTestResourcePath();
	testFilePath /= "filesystem/test.txt";

	natl::File file(testFilePath.c_str(), natl::FileOpenMode::readStart);
	natl::StringByteSize<100> fileContents;
	const natl::LoadAllFileContentError error = natl::loadAllFileContent(file, fileContents);
	if (error != natl::LoadAllFileContentError::none) {
		std::cout << "file load failed: " << natl::loadAllFileContentErrorToString(error) << "\n";
	} else {
		std::cout << "file contents: " << fileContents << "\n";
	}
	std::cout << "working dir: " << natl::getWorkingDirectoryIn<natl::String>() << "\n";
}