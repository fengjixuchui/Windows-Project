#pragma once

void byteArr2HexStr(const LPBYTE& lpbArr,DWORD dwBSize,TCHAR* pszHexStr,const TCHAR wcSpace);
enum PETYPE
{
	PE = 0,
	PE64,
	Unkonw
};
VOID HighLight(SIZE_T Addr,const WCHAR* ShowOpc,const WCHAR* FileCode,const WCHAR* pszCom);

#define STATUS_NONE 1
#define STATUS_SUSPENDED 2
#define STATUS_INTERRUPTED 3

// ������ɫ
#define F_BLUE     0x0001						// ����
#define F_H_BLUE   0x0001|0x0008				// ����
#define F_GREEN    0x0002						// ����
#define F_H_GREEN  0x0002|0x0008				// ����
#define F_RED      0x0004						// ���  
#define F_H_RED    0x0004|0x0008				// ����
#define F_YELLOW   0x0002|0x0004				// ���
#define F_H_YELLOW 0x0002|0x0004|0x0008			// ����
#define F_PURPLE   0x0001|0x0004				// ����
#define F_H_PURPLE 0x0001|0x0004|0x0008			// ����
#define F_WHITE    0x0004|0x0002|0x0001			// �Ұ�
#define F_H_WHITE  0x0004|0x0002|0x0001|0x0008	// ����
#define F_H_LIGHTBLUE 0x0003|0x0008				// ����	
// ������ɫ
#define B_BLUE     0x0010						// ����
#define B_H_BLUE   0x0010|0x0080				// ����
#define B_GREEN    0x0020						// ����
#define B_H_GREEN  0x0020|0x0080				// ����
#define B_RED      0x0040						// ���
#define B_H_RED    0x0040|0x0080				// ����
#define B_YELLOW   0x0020|0x0040				// ���
#define B_H_YELLOW 0x0020|0x0040|0x0080			// ����
#define B_PURPLE   0x0010|0x0040				// ����
#define B_H_PURPLE 0x0010|0x0040|0x0080			// ����
#define B_WHITE    0x0010|0x0020|0x0040			// �Ұ�
#define B_H_WHITE  0x0010|0x0020|0x0040|0x0080	// ����
#define B_H_LIGHTBLUE 0x0003|0x0008||0x0080		// ����	
#define REVERSE    COMMON_LVB_REVERSE_VIDEO		// ��ɫ