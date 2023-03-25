import os
import string
from typing import List
from typing import Tuple
from pathlib import Path
from enum import Enum
import sys

#string op
def startsWith(string: str, prefix: str) -> bool:
    return string.startswith(prefix)

def removePrefix(string: str, prefix: str) -> str:
    if string.startswith(prefix):
        return string[len(prefix):]
    return string 

def subString(string: str, begin: int, end: int) -> str:
    return string[begin:end]
def subStringFromSize(string: str, begin: int, size: int) -> str:
    return string[begin:begin+size]

def stringRemoveRange(strObj :str, start :int, stop :int):
    if len(strObj) > stop :
        return strObj[0: start:] + strObj[stop + 1::]

def insertStringIntoString(mainStrObject :str, insertionStrObj :str, index :int) -> str:
    if index == 0:
        return insertionStrObj + mainStrObject
    else:
        return mainStrObject[:index] + insertionStrObj + mainStrObject[index:]

def skipChars(string: str, startIndex: int, chars: List[chr]) -> int:
    while str[startIndex] in chars:
        startIndex += 1
    return startIndex

seperatingChars: List[chr] = [' ', '\n', '\t']
def skipSeperatingChars(string: str, startIndex: int) -> int:
    return skipChars(string, startIndex, seperatingChars)

def hasSubStringAtIndex(string: str, testString: str, index: int) -> int:
    if subStringFromSize(string, index, len(testString)) == testString:
        return index + testString
    else: 
        return -1
    
def getSubStringAtIndexUntil(string: str, index: int, endChars: List[chr]) -> str:
    startIndex = index
    while not string[startIndex] in endChars:
       index += 1 
    return subString(string, startIndex, index)

def listOfFilePathsToString(listOfFilePathsStrings: List[Path]) -> str:
    string: str = ""
    if not listOfFilePathsStrings:
        return string

    for filePath in listOfFilePathsStrings:
        filePathString: str = filePath
        filePathString = filePathString.replace('\\', '/')
        string += filePathString + '\n'
    return string

#Commands
def haveNoCommands(commands: List[str]):
    return not commands

def checkForCommand(commandFlag: str, commands: List[str]) -> bool:
    for command in commands:
        if command == commandFlag:
            return True
    return False

def getCommands(inputPrompt: str) -> List[str]:
    userInput = input(inputPrompt)
    userInputs = userInput.split(',')
    for i in range(len(userInputs)):
        #userInputs[i] = userInputs[i].lower()
        userInputs[i] = userInputs[i].replace(' ', '')
    return userInputs

#File search 
def getListOfFiles(dirPath: str, ignoreFolders: List[str]) -> List[Path]:
    listOfFile = os.listdir(dirPath)
    allFiles: List[Path] = []
    for entry in listOfFile:
        fullPath = os.path.join(dirPath, entry)
        if os.path.isdir(fullPath):
            if not entry in ignoreFolders:
                allFiles += getListOfFiles(fullPath, ignoreFolders)
        else:
            allFiles.append(fullPath)
    return allFiles    

#File operation 
def removeBaseDirFromPath(baseDir: str, filePath: Path) -> Path:
    filePathStr = str(filePath)
    if filePathStr.startswith(baseDir):
        return removePrefix(filePathStr, baseDir)
    return filePathStr

def removeBaseDir(baseDir: str, filePaths: List[Path]) -> List[Path]:
    reducePaths: List[Path] = []
    for filePath in filePaths:
        filePathStr = str(filePath)
        if filePathStr.startswith(baseDir):
            reducePaths.append(removePrefix(filePathStr, baseDir))
    return reducePaths

def getFilesWithExt(exts: List[str], filePaths: List[Path]):
    filePathsWithExt: List[Path] = []
    for filePath in filePaths:
        for ext in exts:
            if str(filePath).endswith(ext):
                filePathsWithExt.append(filePath)
                continue
    return filePathsWithExt

def isPathWithName(file: Path, fileNames: List[str]) -> bool:
    for fileNames in fileNames:
            name = Path(file).name
            if name == fileNames:
                return True
    return False

def filterFiles(files: List[Path], avoidFileNames: List[str]) -> List[str]:
    goodFiles: List[Path] = []
    for file in files:
        if not isPathWithName(file, avoidFileNames):
            goodFiles.append(file)
    return goodFiles


class SourceContentInfo:
    lable: str
    fileExts: List[str]
    files: List[Path]

    def __init__(self, lable: str, fileExts: List[str], files: List[Path]) -> None:
        self.lable = lable
        self.fileExts = fileExts
        self.files = getFilesWithExt(fileExts, files)


#output 
def inFileError(filePath: str):
    print("In file: ", filePath)

def removeTrailingBackSlashFromAll(filepaths: List[Path]) -> List[Path]:
    outputFilePath: List[Path] = []
    for filepath in filepaths:
        outputFilePath.append(str(filepath).lstrip("\\"))
    return outputFilePath

def outputToCmakeSourcesFile(cmakeSourcesFilePath :str, sourceContentInfos: List[SourceContentInfo], printCommand: bool):
    cmakeSourcesFileContent = ""
    with open(cmakeSourcesFilePath, "r") as cmakeSourcesFile:
        cmakeSourcesFileContent = cmakeSourcesFile.read()
        for sourceContentInfo in sourceContentInfos:
            if not sourceContentInfo.files:
                continue
            
            sourceContentInfo.files = removeTrailingBackSlashFromAll(sourceContentInfo.files)
            sourceContent = listOfFilePathsToString(sourceContentInfo.files)
            lableStart = "set(" + sourceContentInfo.lable
            if lableStart in cmakeSourcesFileContent:
                sourceFilesPos = cmakeSourcesFileContent.find(lableStart)
                fullNewFileContent: str = lableStart + "\n" + sourceContent + ")"
                cmakeSourcesFileContent = stringRemoveRange(cmakeSourcesFileContent, 
                    sourceFilesPos, cmakeSourcesFileContent.find(")", sourceFilesPos))
                cmakeSourcesFileContent = insertStringIntoString(cmakeSourcesFileContent, 
                    fullNewFileContent,  sourceFilesPos)
            else:
                fullNewFileContent = lableStart + "\n" + sourceContent + ")" + "\n\n"
                cmakeSourcesFileContent = insertStringIntoString(cmakeSourcesFileContent, fullNewFileContent, 0)

        with open(cmakeSourcesFilePath, "w") as cmakeSourcesFile:
            cmakeSourcesFile.write(cmakeSourcesFileContent)

    print("outputed to ", cmakeSourcesFilePath)
    if printCommand:
        print(cmakeSourcesFileContent)


class BetweenInfo:
    start: str
    end: str
    active: bool = False

    def __init__(self) -> None:
        self.active = False

    def __init__(self, start: str, end: str) -> None:
        self.start = start
        self.end = end
        self.active = True

class ConvertInfo:
    fromStr: str
    toStr: str
    after: str
    between: BetweenInfo
    def __init__(self, fromStr: str, toStr: str, after: str = "", between: BetweenInfo = BetweenInfo):
        self.fromStr = fromStr
        self.toStr = toStr
        self.after = after
        self.between = between
        

def convertReplace(convertInfo: ConvertInfo, sourceString: str) -> str:
    return sourceString.replace(convertInfo.fromStr, convertInfo.toStr)
def convertReplaceAtIndices(sourceString: str,  replaceWith: str, start: int, end: int) -> Tuple[str, int, int]:
    beginSection = sourceString[:start]
    endSection = sourceString[end:]
    return (beginSection + replaceWith + endSection, len(beginSection), len(beginSection) + len(replaceWith))

def replaceAfterIndex(sourceString: str, replaceWith: str, afterIndex: int) -> str:
    return sourceString[:afterIndex] + replaceWith

def convertReplaceAfter(convertInfo: ConvertInfo, fileContents: str) -> str:
    afterIndex = fileContents.rfind(convertInfo.after)
    if afterIndex:
        afterIndex += len(convertInfo.after)
        afterFileContents = subString(fileContents, afterIndex, len(fileContents))
        afterFileContents = afterFileContents.replace(convertInfo.fromStr, convertInfo.toStr)
        fileContents = replaceAfterIndex(fileContents, afterFileContents, afterIndex)
    
def covertReplaceBetween(convertInfo: ConvertInfo, sourceString: str) -> str:
    if not convertInfo.between.active:
        return sourceString
    else:
        beginReplace = 0
        while True:
            between = convertInfo.between
            start = sourceString.find(between.start, beginReplace)
            if start == -1:
                break
            end = sourceString.find(between.end, beginReplace)
            if end == -1:
                break
            if start > end:
                break

            replaceSection = subString(sourceString, start, end)
            replaceSection = convertReplace(convertInfo, replaceSection)
            sourceString, newStart, newEnd = convertReplaceAtIndices(sourceString, replaceSection, start, end)
            beginReplace = newEnd
    return sourceString


def convert(baseDir: str, 
            baseOutputDir: str, 
            filePath: Path, 
            convertInfos: List[ConvertInfo], 
            fileExtCovertInfos: List[ConvertInfo]):
    fullFilePath = baseDir + filePath
    with open(fullFilePath, "r") as file:
        fileContents = file.read()
        for covertInfo in convertInfos:
            if covertInfo.after:
                afterIndex = fileContents.rfind(covertInfo.after)
                if afterIndex:
                   fileContents = convertReplaceAfter(covertInfo, fileContents)
            if covertInfo.between.active:
                    fileContents = covertReplaceBetween(covertInfo, fileContents)
            else:
                fileContents = fileContents.replace(covertInfo.fromStr, covertInfo.toStr)
        
        for fileExtCovertInfo in fileExtCovertInfos:
            if str(filePath).endswith(fileExtCovertInfo.fromStr):
                filePath = filePath.replace(fileExtCovertInfo.fromStr, fileExtCovertInfo.toStr)
                break

        fullFilePathOutput = baseOutputDir + removeBaseDirFromPath("\\" + Path(baseDir).name, filePath)
        with open(fullFilePathOutput, "w+") as fileOutput:
            fileOutput.write(fileContents)

def headersCmakeSourcesOutput(dir: str, files: List[Path], printCommand: bool):
    headerCmakeSourcesFilePath = dir + "\\CMakeSources.cmake"
    if not os.path.exists(headerCmakeSourcesFilePath):
        print("error: could not find cmake file ", headerCmakeSourcesFilePath)

    headerSourceContentInfos: List[SourceContentInfo] = [
        SourceContentInfo("HEADER_FILES", [".h"], files),
        SourceContentInfo("SOURCE_FILES", [".cpp"], files)
    ]
    
    outputToCmakeSourcesFile(headerCmakeSourcesFilePath, headerSourceContentInfos, printCommand)

def modulesCmakeSourcesOutput(dir: str, files: List[Path], printCommand: bool):
    moduleCmakeSourcesFilePath = dir + "\\CMakeSources.cmake"
    if not os.path.exists(moduleCmakeSourcesFilePath):
        print("error: could not find cmake file ", moduleCmakeSourcesFilePath)
        
    moduleSourceContentInfos: List[SourceContentInfo] = [
        SourceContentInfo("MODULE_FILES", [".ixx"], files),
        SourceContentInfo("SOURCE_FILES", [".cpp"], files)
    ]

    outputToCmakeSourcesFile(moduleCmakeSourcesFilePath, moduleSourceContentInfos, printCommand)

def convertToModules(headerDir: str, moduleDir: str, files: List[Path]):
    headerSourceContentInfos: List[SourceContentInfo] = [
        SourceContentInfo("HEADER_FILES", [".h"], files),
        SourceContentInfo("SOURCE_FILES", [".cpp"], files)
    ]

    if not os.path.exists(moduleDir):
        print("error: could not find modules output dir ", moduleDir)
        return

    convertInfos: List[ConvertInfo] = [
        ConvertInfo("/*export*/", "export"),
        ConvertInfo("/*module", "//module"),
        ConvertInfo("endModule*/", "//endModule"),
        ConvertInfo("//header", "/*header"),
        ConvertInfo("//endHeader", "endHeader*/"),
    ]

    headerFileExtConvert: List[ConvertInfo] = [
        ConvertInfo(".h", '.ixx')
    ]

    outputDir = moduleDir + "\\" + Path(moduleDir).name

    for headerSourceContentInfo in headerSourceContentInfos:
        for file in headerSourceContentInfo.files:
            convert(headerDir, outputDir, file, convertInfos, headerFileExtConvert)

def convertToHeader(headerDir: str, moduleDir: str, files: List[Path]):
    headerSourceContentInfos: List[SourceContentInfo] = [
        SourceContentInfo("MODULE_FILES", [".ixx"], files),
        SourceContentInfo("SOURCE_FILES", [".cpp"], files)
    ]

    convertInfos: List[ConvertInfo] = [
        ConvertInfo("export", "/*export*/"),
        ConvertInfo("/*export*/", "export", "", BetweenInfo("//module", "//endModule")),
        ConvertInfo("//module", "/*module"),
        ConvertInfo("//endModule", "endModule*/"),
        ConvertInfo("/*header", "//header"),
        ConvertInfo("endHeader*/", "//endHeader"),
    ]

    moduleFileExtConvert: List[ConvertInfo] = [
        ConvertInfo(".ixx", '.h')
    ]

    outputDir = headerDir + "\\" +Path(headerDir).name

    for headerSourceContentInfo in headerSourceContentInfos:
        for file in headerSourceContentInfo.files:
            convert(moduleDir, outputDir, file, convertInfos, moduleFileExtConvert)

def main():
    run = True
    oldDir: str = ""
    print("CXXHeaderModuleCovert")
    while run:
        dir: str = ""
        ignoreFolders: List[str] = ["tests", "external", "out"]
        commands = sys.argv

        if commands:
            commands.pop(0)
        run = False

        if not commands:
            run = True
            commands = getCommands("input filepath, commands: ")

        if haveNoCommands(commands):
            print("error: no filepath and 0 commands provided")
            continue

        oldCommand = checkForCommand("old", commands)
        printCommand = checkForCommand("print", commands)
        moduleCommand = checkForCommand("module", commands)
        headerCommand = checkForCommand("header", commands)

        if oldCommand:
            if not oldDir and not os.path.isdir(oldDir):
                print("error: oldDir in null or is not a dir")
                continue
            else:
                dir = oldDir
        else:
            dir = commands[0]
            if not os.path.isdir(dir):
                print("error: first argument in input must be a valid filepath")
                continue
        oldDir = dir

        convertToModulesCommand: bool = True
        headerDir = dir
        moduleDir = dir

        if dir.endswith("Modules"):
            headerDir = subString(dir, 0, len(dir) - len("Modules")) 
            convertToModulesCommand = False
        else:
            moduleDir = dir + "Modules"

        files = getListOfFiles(dir, ignoreFolders)
        files = removeBaseDir(dir, files)

        avoidFiles: List[str] = [
            "pch.h",
            "pch.ixx",
            "pch"
        ]

        if convertToModulesCommand:
            convertToModules(headerDir, moduleDir, filterFiles(files, avoidFiles))
        else:
            convertToHeader(headerDir, moduleDir, files)
        
        headerDirFiles = getListOfFiles(headerDir, ignoreFolders)
        headerDirFiles = removeBaseDir(headerDir, headerDirFiles)

        modulesDirFiles = getListOfFiles(moduleDir, ignoreFolders)
        modulesDirFiles = filterFiles(modulesDirFiles, avoidFiles)
        modulesDirFiles = removeBaseDir(moduleDir, modulesDirFiles)

        headersCmakeSourcesOutput(headerDir, headerDirFiles, printCommand)
        modulesCmakeSourcesOutput(moduleDir, modulesDirFiles, printCommand)
        
if __name__ == '__main__':
    main()