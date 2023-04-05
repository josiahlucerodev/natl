import os
import string
from typing import List
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


def main():
    checkArgs: bool = True
    run = True
    oldDir: str = ""
    print("SourceFinder")
    while run == True:
        dir: str = ""
        ignoreFolders: List[str] = ["tests", "external", "out"]

        commands: List[str] = []
        if checkArgs:
            argv = sys.argv
            argv.pop(0)
            if not argv:
                checkArgs = False
                continue
            else:
                run = False
                commands = argv
        else:
            commands = getCommands("input filepath, commands: ")
            
        if haveNoCommands(commands):
            print("error: no filepath and 0 commands provided")
            continue

        oldCommand = checkForCommand("old", commands)
        exitCommand = checkForCommand("exit", commands)
        printCommand = checkForCommand("print", commands)

        if exitCommand:
            run = False 

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

        avoidFiles: List[str] = [
            "pch.h",
            "pch.ixx",
            "pch"
        ]

        files = getListOfFiles(dir, ignoreFolders)
        files = filterFiles(files, avoidFiles)
        files = removeBaseDir(dir, files)

        sourcesFilePath = dir + "\\CMakeSources.cmake"
        if not os.path.exists(sourcesFilePath):
            print("error: could not find cmake file ", sourcesFilePath)
            continue

        sourceContentInfos: List[SourceContentInfo] = [
            SourceContentInfo("HEADER_FILES", [".h"], files),
            SourceContentInfo("SOURCE_FILES", [".cpp"], files)
        ]
    
        outputToCmakeSourcesFile(sourcesFilePath, sourceContentInfos, printCommand)
        
if __name__ == '__main__':
    main()