#include "StdAfx.h"
#include "Common.h"
#include "BreakPoint.h"
#include "DebuggerMainDlg.h"

extern std::list<PointInfo>  g_BpList;
extern CDebugger1Dlg *MainDlg;

//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
// ������: byteArr2HexStr
///��  ��: �����ε�����ת����ʮ�������ַ���
// ��  ��: const LPBYTE lpbArr
// ��  ��: DWORD dwSize
// ��  ��: TCHAR * pszHexStr
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
void byteArr2HexStr(const LPBYTE& lpbArr,DWORD dwBSize,TCHAR* pszHexStr,const TCHAR wcSpace)
{
	// һ���ֽ�ת����һ��TCHAR
	DWORD	i = 0;
	char*	pszStr = (char*)pszHexStr;
	TCHAR	ucNum[3] = {0};
	BYTE	byteNum = 0;
	DWORD	dwIndex = wcSpace == 0 ? 2 : 3;
	DWORD	j = 0;
	while(j<dwBSize)
	{
		byteNum = *((PBYTE)(lpbArr+j));
		// ת���ַ���
		swprintf_s(pszHexStr + i,3+1,L"%02X%c",byteNum,wcSpace);
		i += dwIndex;
		++j;
	}
}


VOID HighLight(SIZE_T Addr,const WCHAR* ShowOpc,const WCHAR* FileCode,const WCHAR* pszCom)
{
	CString Address,ShowOpc1,FileCode1,pszCom1;
	//�����ַ�Ƕϵ� ��ʾһ�¶ϵ����ɫ
	for (std::list<PointInfo>::const_iterator it = g_BpList.cbegin();
		it != g_BpList.cend();
		++it) {

			if (it->lpPointAddr == Addr) {
		
			
				//SetConsoleTextAttribute(MainDlg->m_Text,B_H_RED);//��ɫ
				Address.Format(L"  %08X",Addr);
				MainDlg->m_Text +=Address;
				
			}
			else
			{
				Address.Format(L"  %08X",Addr);
				MainDlg->m_Text +=Address;
			}
	}

	//���OPCODE
	//SetConsoleTextAttribute(hStdOut,F_WHITE);//�ָ�Ĭ����ɫ
	ShowOpc1.Format(L" | %24s | ",ShowOpc);
	MainDlg->m_Text +=ShowOpc1;

	//wprintf(L"  %08X | %-24s | ", Addr, ShowOpc);
	SIZE_T nLen = wcslen(FileCode);
	//��ʼ������������������
	for(int i = 0; i < nLen; i++)
	{
	//	SetConsoleTextAttribute(hStdOut,F_WHITE);		//Ĭ�ϵİ�ɫ
		//��ת������J��
		if(FileCode[i] == 'j')
		{
			if(FileCode[i + 1] == 'm'&&FileCode[i + 2] == 'p')
			{
				//SetConsoleTextAttribute(hStdOut,B_H_GREEN);//��ɫ
				while(FileCode[i] != ' '&&FileCode[i] != '\t')
				{
					FileCode1.Format(L"%c",FileCode[i]);
					MainDlg->m_Text += FileCode1;
					i++;
				}
				i--;
				continue;
			}
			else
			{
				//SetConsoleTextAttribute(hStdOut,B_H_YELLOW);//��ɫ
				//while(FileCode[i] != ' '&&FileCode[i] != '\t')
				{
					FileCode1.Format(L"%c",FileCode[i]);
					MainDlg->m_Text += FileCode1;
					i++;
				}
				i--;
				continue;
			}
		}
		//CALL����
		if(FileCode[i] == 'c'&&FileCode[i + 1] == 'a'&&FileCode[i + 2] == 'l'&&FileCode[i + 3] == 'l')
		{
			//SetConsoleTextAttribute(hStdOut,B_H_RED);//��ɫ
			while(FileCode[i] != ' '&&FileCode[i] != '\t')
			{
				FileCode1.Format(L"%c",FileCode[i]);
				MainDlg->m_Text += FileCode1;
				i++;
			}
			i--;
			continue;
		}
		//RET����
		if((FileCode[i] == 'r'&&FileCode[i + 1] == 'e'&&FileCode[i + 2] == 't') ||
			(FileCode[i] == 'i'&&FileCode[i + 1] == 'r'&&FileCode[i + 2] == 'e'))
		{
			//SetConsoleTextAttribute(hStdOut,F_H_LIGHTBLUE | REVERSE);//��ɫ
			while(FileCode[i] != ' '&&FileCode[i] != '\t')
			{
				FileCode1.Format(L"%c",FileCode[i]);
				MainDlg->m_Text += FileCode1;
				i++;
			}
			i--;
			continue;
		}
		//PUSH POP����
		if((FileCode[i] == 'p'&&FileCode[i + 1] == 'u'&&FileCode[i + 2] == 's') ||
			(FileCode[i] == 'p'&&FileCode[i + 1] == 'o'&&FileCode[i + 2] == 'p'))
		{
			//SetConsoleTextAttribute(hStdOut,F_H_PURPLE);//��ѡ�񣡣���
			while(FileCode[i] != ' '&&FileCode[i] != '\t')
			{
				FileCode1.Format(L"%c",FileCode[i]);
				MainDlg->m_Text += FileCode1;
				i++;
			}
			i--;
			continue;
		}
		//����������
		if((FileCode[i + 8] == 'h') && (FileCode[i - 1] == ' ' || FileCode[i - 1] == ',')
			&& i > 5)
		{
			//SetConsoleTextAttribute(hStdOut,F_H_YELLOW);//��ɫ
			while(FileCode[i] != ' '&&FileCode[i] != '\0')
			{
				FileCode1.Format(L"%c",FileCode[i]);
				MainDlg->m_Text += FileCode1;
				i++;
			}
			i--;
			continue;
		}
		//�ڴ��ַ����
		if((FileCode[i] == 'b'&&FileCode[i + 1] == 'y'&&FileCode[i + 2] == 't'
			&&FileCode[i + 3] == 'e') ||
			(FileCode[i] == 'w'&&FileCode[i + 1] == 'o'&&FileCode[i + 2] == 'r'
			&&FileCode[i + 3] == 'd') ||
			(FileCode[i] == 'd'&&FileCode[i + 1] == 'w'&&FileCode[i + 2] == 'o'
			&&FileCode[i + 3] == 'r'&&FileCode[i + 4] == 'd') ||
			(FileCode[i] == 'f'&&FileCode[i + 1] == 'w'&&FileCode[i + 2] == 'o'
			&&FileCode[i + 3] == 'r'&&FileCode[i + 4] == 'd') ||
			(FileCode[i] == 'q'&&FileCode[i + 1] == 'w'&&FileCode[i + 2] == 'o'
			&&FileCode[i + 3] == 'r'&&FileCode[i + 4] == 'd'))
		{
			if((FileCode[i - 2] == 'l'&&FileCode[i - 3] == 'l') &&
				(FileCode[i - 4] == 'a'&&FileCode[i - 5] == 'c'))
			{
				//CALL��ַ����
				//SetConsoleTextAttribute(hStdOut,F_H_PURPLE);
				while(FileCode[i] != ',' && FileCode[i] != '\0')
				{
					FileCode1.Format(L"%c",FileCode[i]);
					MainDlg->m_Text += FileCode1;
					i++;
				}
				i--;
				continue;
			}
			else
			{
				//DS����
				//SetConsoleTextAttribute(hStdOut,F_H_LIGHTBLUE);
				while(FileCode[i] != ',' && FileCode[i] != '\0')
				{
					FileCode1.Format(L"%c",FileCode[i]);
					MainDlg->m_Text += FileCode1;
					i++;
				}
				i--;
				continue;
			}
		}
		//�ڴ��ַ����������
		if((FileCode[i - 6] == 'j' || FileCode[i - 4] == 'j' || FileCode[i - 3] == 'j') ||
			(FileCode[i - 5] == 'c'&&FileCode[i - 4] == 'a' &&FileCode[i - 3] == 'l' &&FileCode[i - 2] == 'l') ||
			(FileCode[i - 5] == 'l'&& FileCode[i - 4] == 'o'&&FileCode[i - 3] == 'o'&&FileCode[i - 1] == '\t') ||
			(FileCode[i - 6] == 'l'&&FileCode[i - 5] == 'o'&&FileCode[i - 4] == 'o'&&FileCode[i - 1] == '\t') ||
			(FileCode[i - 7] == 'l'&&FileCode[i - 6] == 'o'&&FileCode[i - 5] == 'o'&&FileCode[i - 1] == '\t'))
		{
			//SetConsoleTextAttribute(hStdOut,F_H_GREEN);//��ɫ
			while(FileCode[i] != ',' && FileCode[i] != '\0')
			{
				FileCode1.Format(L"%c",FileCode[i]);
				MainDlg->m_Text += FileCode1;
				i++;
			}
			i--;
			continue;
		}
		//SetConsoleTextAttribute(hStdOut,F_H_WHITE);
		FileCode1.Format(L"%c",FileCode[i]);
		MainDlg->m_Text += FileCode1;  //���û�и������ַ�
	}

	if(pszCom != NULL)
	{
		//SetConsoleTextAttribute(hStdOut,F_H_GREEN);//��ɫ
		pszCom1.Format(L"%*c %s",48 - _tcslen(FileCode),'|',pszCom);
		MainDlg->m_Text += pszCom1;
	}
	//SetConsoleTextAttribute(hStdOut,F_WHITE);//�ָ�Ĭ����ɫ
	//wprintf(L"\n");//�������
	MainDlg->m_Text += L"\r\n";
};
