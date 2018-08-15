#pragma once
void OnShowSourceLines() ;

void DisplaySourceLines(PCHAR sourceFile, int lineNum, unsigned int address, int after, int before);


//显示源文件中的一行。
void DisplayLine(PCHAR sourceFile, const std::wstring& line, int lineNumber, BOOL isCurLine) ;
