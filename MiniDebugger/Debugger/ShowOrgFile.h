#pragma once
void OnShowSourceLines() ;

void DisplaySourceLines(PCHAR sourceFile, int lineNum, unsigned int address, int after, int before);


//��ʾԴ�ļ��е�һ�С�
void DisplayLine(PCHAR sourceFile, const std::wstring& line, int lineNumber, BOOL isCurLine) ;
