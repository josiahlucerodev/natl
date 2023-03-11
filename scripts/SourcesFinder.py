import os
from pathlib import Path
import string
from typing import List

def stringRemoveRange(strObj :str, start :int, stop :int):
    if len(strObj) > stop :
        return strObj[0: start:] + strObj[stop + 1::]

def insertStringIntoString(mainStrObject :str, insertionStrObj :str, index :int) -> str:
    if index == 0:
        return insertionStrObj + mainStrObject
    else:
        return mainStrObject[:index] + insertionStrObj + mainStrObject[index:]

def printlistOfPaths(listOfPaths: List[Path]):
    filePathsString: str = listOfFilePathsToString(listOfPaths)
    print(filePathsString)
  
def listOfFilePathsToString(listOfFilePathsStrings: List[Path]):
    if not listOfFilePathsStrings:
        return

    string: str = ""
    for filePath in listOfFilePathsStrings:
        filePathString: str = filePath
        filePathString = filePathString.replace('\\', '/')
        string += filePathString + '\n'
    return string

def isAFolderToAvoid(excludeFolderNames: List[str], folderName: str) -> bool:
    for excludeFolderName in excludeFolderNames: 
        if excludeFolderName == folderName:
            return True
    return False

def hasFileExt(fileExts: List[str], entry: str) -> bool:
    for fileExt in fileExts:
        if fileExt == Path(entry).suffix:
            return True
    return False

def hasFileEnding(excludeFileEndings: List[str], entry: str) -> bool:
    for excludeFileEnding in excludeFileEndings:
        if entry.endswith(excludeFileEnding):
            return True
    return False
def isFileWithRequiredFileEnding(includeOnlyFileEndings: List[str], entry: str) -> bool:
    for includeOnlyFileEnding in includeOnlyFileEndings:
        if entry.endswith(includeOnlyFileEnding):
            return True 
    
    return False
    


class FileListingInfo:
    fileExts: List[str]
    excludeFolderNames: List[str]
    excludeFileEndings: List[str]
    includeOnlyFileEndings: List[str]

    def __init__(self, 
        fileExts: List[str], 
        excludeFolderNames: List[str], 
        excludeFileEndings: List[str],
        includeOnlyFileEndings: List[str]) -> None:

        self.fileExts = fileExts
        self.excludeFolderNames = excludeFolderNames
        self.excludeFileEndings = excludeFileEndings
        self.includeOnlyFileEndings = includeOnlyFileEndings

def getSubFilePath(dirPath: str, extraPath: str, fullPath: str, entry: str) -> str:
    newFolderPath = dirPath.replace(extraPath, '').lstrip('\\')
    subFilePath = os.path.join(newFolderPath, entry)
    return subFilePath

def getListOfFiles(dirPath: str, extraPath: str, listingInfo: FileListingInfo) -> List[Path]:
    listOfFile = os.listdir(dirPath)
    allFiles: List[Path] = []
    for entry in listOfFile:
        fullPath = os.path.join(dirPath, entry)
        if os.path.isdir(fullPath):
            if not isAFolderToAvoid(listingInfo.excludeFolderNames,entry):
                allFiles = allFiles + getListOfFiles(fullPath, extraPath, listingInfo)
                continue
        if hasFileExt(listingInfo.fileExts, entry):
            if not hasFileEnding(listingInfo.excludeFileEndings, Path(entry).stem):
                if not listingInfo.includeOnlyFileEndings:
                    allFiles.append(getSubFilePath(dirPath, extraPath, fullPath, entry))
                    continue
                if isFileWithRequiredFileEnding(listingInfo.includeOnlyFileEndings, Path(entry).stem):
                    allFiles.append(getSubFilePath(dirPath, extraPath, fullPath, entry))
    return allFiles    

class SourceContentInfo:
    lable: str
    fileExts: List[str]
    files: List[Path]
    excludeFolderNames: List[str]
    excludeFileEndings: List[str]
    includeOnlyFileEndings: List[str]

    sourceContent: str
    def __init__(self, 
        lable: str, 
        fileExts: List[str], 
        excludeFolderNames: List[str], 
        excludeFileEndings: List[str],
        includeOnlyFileEndings: List[str]) -> None:

        self.lable = lable
        self.fileExts = fileExts
        self.excludeFolderNames = excludeFolderNames
        self.excludeFileEndings = excludeFileEndings
        self.includeOnlyFileEndings = includeOnlyFileEndings

def getSourceContentFiles(dirPath :str, sourceContentInfo: SourceContentInfo) -> SourceContentInfo:
    listingInfo = FileListingInfo(sourceContentInfo.fileExts, 
        sourceContentInfo.excludeFolderNames, 
        sourceContentInfo.excludeFileEndings, 
        sourceContentInfo.includeOnlyFileEndings)

    sourceContentInfo.files = getListOfFiles(dirPath, dirPath, listingInfo)
    if sourceContentInfo.files:
        sourceContentInfo.sourceContent = listOfFilePathsToString(sourceContentInfo.files)
    return sourceContentInfo


def outputToCmakeSourcesFile(cmakeSourcesFilePath :str, sourceContentInfos: List[SourceContentInfo]):
    cmakeSourcesFileContent = ""
    with open(cmakeSourcesFilePath, "r") as cmakeSourcesFile:
        cmakeSourcesFileContent = cmakeSourcesFile.read()

        for sourceContentInfo in sourceContentInfos:
            if not sourceContentInfo.files:
                continue
            lableStart = "set(" + sourceContentInfo.lable
            if lableStart in cmakeSourcesFileContent:
                sourceFilesPos = cmakeSourcesFileContent.find(lableStart)
                fullNewFileContent: str = lableStart + "\n" + sourceContentInfo.sourceContent + ")"
                cmakeSourcesFileContent = stringRemoveRange(cmakeSourcesFileContent, 
                    sourceFilesPos, cmakeSourcesFileContent.find(")", sourceFilesPos))
                cmakeSourcesFileContent = insertStringIntoString(cmakeSourcesFileContent, 
                    fullNewFileContent,  sourceFilesPos)
            else:
                fullNewFileContent = lableStart + "\n" + sourceContentInfo.sourceContent + ")" + "\n\n"
                cmakeSourcesFileContent = insertStringIntoString(cmakeSourcesFileContent, fullNewFileContent, 0)

        with open(cmakeSourcesFilePath, "w") as cmakeSourcesFile:
            cmakeSourcesFile.write(cmakeSourcesFileContent)

def printSourceFileContentInfo(sourceContentInfos: List[SourceContentInfo]):
    for sourceContentInfo in sourceContentInfos:
        print(sourceContentInfo.lable)
        if sourceContentInfo.files:
            printlistOfPaths(sourceContentInfo.files)
        else:
            print(("empty"))
        print("\n")

def main():
    oldDirName = ''
    while True:
        userInput = input("Folder path, command:")
        userInputs = userInput.split(',')
        printCommand = False
        useOldDirName = False
        dirName = userInputs[0]
        
        for i in range(len(userInputs)):
            userInputs[i] = userInputs[i].lower()
            userInputs[i] = userInputs[i].replace(' ', '')

        for command in userInputs:
            if command == 'print':
                printCommand = True
            if command == 'old':
                if oldDirName == '':
                    print("Error: old path is null")
                    continue
                useOldDirName = True
            if command == 'exit':
                return

        if useOldDirName:
            dirName = oldDirName

        if not os.path.exists(dirName):
            print("Folder path does not exist")
            continue
        oldDirName = dirName

        print("Finding sources and header files")
        cmakeSourcesFilePath = dirName + "\\CMakeSources.cmake"

        folderAvoidList = ["external", "ext", "examples", "tests", "out", "bin"] 
        fileEndingAvoidList: List[str] = []
        includeOnlyEndingListNone: List[str] = []

        sourceContentInfos: List[SourceContentInfo] = [
                SourceContentInfo("SHADER_FILES", [".asl"], folderAvoidList, fileEndingAvoidList, includeOnlyEndingListNone),
                SourceContentInfo("MODULE_SOURCE_FILES", [".cpp"], folderAvoidList, fileEndingAvoidList, ['Module']),
                SourceContentInfo("MODULE_FILES", [".ixx"], folderAvoidList, fileEndingAvoidList, includeOnlyEndingListNone),
                SourceContentInfo("SOURCE_FILES", [".cpp"], folderAvoidList, ['Module'], includeOnlyEndingListNone),
                SourceContentInfo("HEADER_FILES", [".h"], folderAvoidList, fileEndingAvoidList, includeOnlyEndingListNone)
            ]

        for sourceContentInfo in sourceContentInfos:
            sourceContentInfo = getSourceContentFiles(dirName, sourceContentInfo)

        if os.path.exists(cmakeSourcesFilePath) and printCommand:
            printSourceFileContentInfo(sourceContentInfos)

    #C:\Users\Administrator\Desktop\Projects\HeaderSourcesFinder\Affect
        if(os.path.exists(cmakeSourcesFilePath)):
            outputToCmakeSourcesFile(cmakeSourcesFilePath, sourceContentInfos)
            print("Header and Sources files outputed to ", cmakeSourcesFilePath)
        
            
if __name__ == '__main__':
    main()