// common.cpp : Defines the entry point for the console application.
//
#ifdef __LINUX__
#include <unistd.h>
#else
#include <Windows.h>
#endif

#include <ctime>
#include <cstdint>
#include <random>
#include <vector>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"
using namespace gcommon;


namespace gcommon
{
	/********************************************************************
	* [������]: g_htons g_ntohs
	* [����]: ʵ�ֶ�2�ֽ����ݵ��ֽڵ���
	* [����]:
	*   data: uint16_t
	* [����ֵ]: uint16_t
	*   ���ú��ֵ
	* [�޸ļ�¼]:
	*   2017-03-13,littledj: create
	********************************************************************/
	uint16_t g_htons(const uint16_t data)
	{
		uint16_t tmp = (data & 0xff00) >> 8;
		tmp |= (data & 0x00ff) << 8;
		return tmp;
	}
	uint16_t g_ntohs(const uint16_t data)
	{
		return g_htons(data);
	}

	/********************************************************************
	* [������]: g_htonl g_ntohl
	* [����]: ʵ�ֶ�4�ֽ����ݵ��ֽڵ���
	* [����]:
	*   data: uint32_t
	* [����ֵ]: uint32_t
	*   ���ú��ֵ
	* [�޸ļ�¼]:
	*   2017-03-13,littledj: create
	********************************************************************/
	uint32_t g_htonl(const uint32_t data)
	{
		uint32_t tmp = (data & 0xff000000) >> 24;
		tmp |= (data & 0x00ff0000) >> 8;
		tmp |= (data & 0x0000ff00) << 8;
		tmp |= (data & 0x000000ff) << 24;
		return tmp;
	}
	uint32_t g_ntohl(const uint32_t data)
	{
		return g_htonl(data);
	}

	/********************************************************************
	* [������]: GetParaFromARG
	* [����]: �Ӳ����б��л�ȡָ���Ĳ���
	* [����]
	*   argc����������
	*   argv�������б�
	*   prefix������ǰ����������ΪNULL���߿��ַ���""
	*   pos��ָ���ǵڼ������������Ĳ�������1��ʼ
	* [���]
	*   out����ȡ�Ĳ����������ͷţ�
	* [����ֵ]
	*   >0���ɹ������������Ĳ�������
	*   0��ʧ�ܣ���������
	*   -1��ʧ�ܣ�δ�ҵ�prefix
	*   -2��ʧ�ܣ��ҵ�prefix���������޲���
	* [�޸ļ�¼]
	*   2013-12-09,littledj: create
	*   2014-04-08,littledj: ��Ԫ���ԣ����޸�
	*********************************************************************/
	int GetParaFromARG(int argc, tchar* argv[], tchar* prefix, tchar* &out, int pos)
	{
		int count = 0;
		bool bFind = false;
		bool bFindPrefix = false;

		// �������
		if (argv == NULL || pos <= 0 ||
			argc <= 1 || argv[0] == NULL)
			return 0;
		if (prefix != NULL && prefix[0] != '-' && prefix[0] != 0)	//	ǰ����������'-'��ʼ
			return 0;

		for (int i = 1; i < argc; i++)
		{
			// ����Ϊ��
			if (argv[i] == NULL) return 0;

			// ȥ����ʼ�ո�
			while (argv[i][0] == ' ')
			{
				argv[i] += 1;
				if (argv[i][0] == 0) break;
			}
			if (argv[i][0] == 0) continue;

			// Ѱ����ǰ������
			if (prefix == NULL || prefix[0] == 0)
			{
				if (argv[i][0] != '-')	// ������Ӧ���ԡ�-����Ϊ��һ���ַ�
				{
					count++;
					if (count == pos)
					{
						out = argv[i];
						bFind = true;
					}
				}
				else
				{
					i++; // �ж�Ϊǰ����������һ������
				}
			}
			else if (tcsncmp(argv[i], prefix, tcslen(prefix)) == 0 &&	// Ѱ��ָ��ǰ������
				tcslen(argv[i]) == tcslen(prefix))
			{
				bFindPrefix = true;
				do
				{
					if (++i == argc)	// �жϺ����Ƿ��в���
					{
						if (bFind) return count;
						else return -2;
					}
					if (argv[i] == NULL) return 0;	// ����Ϊ��

													// ȥ����ʼ�ո�
					while (argv[i][0] == ' ')
					{
						argv[i] += 1;
						if (argv[i][0] == 0) break;
					}
				} while (argv[i][0] == 0);

				// �жϺ���Ĳ����Ƿ���ǰ����
				if (argv[i][0] != '-')
				{
					count++;
					if (pos == count)
					{
						bFind = true;
						out = argv[i];
					}
				}
				else
				{
					i++;
				}
			}
		}

		if (bFind)
			return count;	// �ҵ����������Ĳ���
		else if (bFindPrefix)
			return -2;		// ֻ�ҵ�ǰ����
		else
			return -1;		// ʲôҲû�ҵ�
	}

	/********************************************************************
	* ������: GetCurrentDirPath
	* ����: ��ȡ��ǰ��ִ�г��������ļ��е�ȫ·��
	* ����:
	*   ��
	* ���:
	*   tstring: ��ǰ�ļ���·��
	* �޸ļ�¼:
	*   2013-11-17,littledj: create
	*   2013-12-07,littledj: ����ֵ��ΪCString������ʹ��ȫ�ֱ���
	*   2015-07-21,littledj; ����ֵ��Ϊtstring,���ʧ�ܷ���".\"
	*   2016-07-08,littledj: ���Ӷ�linux��֧��
	********************************************************************/
	tstring GetCurrentDirPath()
	{
		tstring strCurrentDir;
		tchar* pCurrentDir = new tchar[MAX_PATH + 1];
		pCurrentDir[0] = 0;
#ifdef __LINUX__
		ssize_t count = readlink("/proc/self/exe", pCurrentDir, MAX_PATH);
		if (count <= 0)
		{
			delete[] pCurrentDir;
			return TEXT("./");
		}

		pCurrentDir[count] = 0;
		(strrchr(pCurrentDir, '/'))[1] = 0;
		strCurrentDir = pCurrentDir;
		strCurrentDir += "/";
		delete[] pCurrentDir;
#else
		uint32_t nRet = GetModuleFileName(NULL, pCurrentDir, MAX_PATH);
		if (nRet == 0)
		{
			delete[] pCurrentDir;
			return TEXT(".\\");
		}

		(tcsrchr(pCurrentDir, '\\'))[1] = 0;
		strCurrentDir = pCurrentDir;
		delete[] pCurrentDir;
#endif
		return strCurrentDir;
	}

	/********************************************************************
	* [������]: random
	* [����]: ��[start,end)��(����end, ���0xffffffff)��ȡһ���������,start>0
	* [����]:
	*   start: �������ʼλ��
	*   end: ���������λ�ã���������
	* [����ֵ]: uint32_t
	*   ���ɵ������
	* [�޸ļ�¼]:
	*   2013-11-17,littledj: create
	*   2016-01-20,littledj: �Ż�srand����
	*   2016-03-13,littledj: �����ͷ���ֵ����Ϊ�޷��ţ�����Χ
	********************************************************************/
	uint32_t random(uint32_t start, uint32_t end)
	{
		// �������
		if (end <= start)
			return start;

		// ��һ�ε������ʼ�����������
		bool static bFirst = true;
		if (bFirst)
		{
			char* chr = new char[1];
			srand((size_t)chr); // osx ����Ҫlong
			bFirst = false;
		}

		int r1 = rand();
		int r2 = rand();
		int r3 = rand();
		int r4 = rand();
		int rr = (r1 & 0x000000ff) |
			(r2 & 0x000000ff) << 8 |
			(r3 & 0x000000ff) << 16 |
			(r4 & 0x000000ff) << 24;
		double rate = (end - start)*1.0 / (uint32_t)0xFFFFFFFF;
		uint32_t delta = (uint32_t)(rate * (uint32_t)rr);
		return start + delta;
	}

	/********************************************************************
	* [������]: inet_ltot
	* [����]: ��long���͵�ip��ַת�����ַ�����xxx.xxx.xxx.xx��
	* [����]:
	*   ip��ip��ַ
	* [����ֵ]: tchar
	*   ת�����ip��ַ�ַ���
	* [�޸ļ�¼]:
	*   2014-12-17,littledj: create
	********************************************************************/
	tchar* inet_ltot(uint32_t ip)
	{
		uint8_t chIP[4];
		chIP[0] = (uint8_t)(ip);
		chIP[1] = (uint8_t)(ip >> 8);
		chIP[2] = (uint8_t)(ip >> 16);
		chIP[3] = (uint8_t)(ip >> 24);

		static tchar strIP[16];
		memset(strIP, 0, 16 * sizeof(tchar));

		stprintf(strIP, TEXT("%u.%u.%u.%u"), chIP[0], chIP[1], chIP[2], chIP[3]);
		return strIP;
	}

	/********************************************************************
	* [������]: inet_ttol
	* [����]: ���ַ������͵�ip��ַת����long
	* [����]:
	*   strIP��ip�ַ���
	* [����ֵ]:
	*   ת�����ip��ַ
	* [�޸ļ�¼]:
	*   2014-12-17,littledj: create
	********************************************************************/
	uint32_t inet_ttol(const tchar* strIP)
	{
		if (strIP == NULL)
		{
			return 0;
		}

		uint32_t ip = 0;
		size_t ip_len = tcslen(strIP);
		size_t data_len = 0;
		size_t pos = 0;
		tchar* strIPTmp = new tchar[ip_len + 1];
		tcscpy(strIPTmp, strIP);
		for (size_t i = 0; i < ip_len; i++)
		{
			if (strIPTmp[i] == '.')
			{
				strIPTmp[i] = 0;
			}
		}

		data_len = tcslen(strIPTmp + pos);
		ip |= (uint8_t)ttoi(strIPTmp + pos);
		ip <<= 8;
		pos += data_len; pos++;	// skip '\0'
		if (pos >= ip_len)
			goto convert_end;

		data_len = tcslen(strIPTmp + pos);
		ip |= (uint8_t)ttoi(strIPTmp + pos);
		ip <<= 8;
		pos += data_len; pos++;	// skip '\0'
		if (pos >= ip_len)
			goto convert_end;

		data_len = tcslen(strIPTmp + pos);
		ip |= (uint8_t)ttoi(strIPTmp + pos);
		ip <<= 8;
		pos += data_len; pos++;	// skip '\0'
		if (pos >= ip_len)
			goto convert_end;

		data_len = tcslen(strIPTmp + pos);
		ip |= (uint8_t)ttoi(strIPTmp + pos);
		pos += data_len;
		if (pos == ip_len)
		{
			delete[] strIPTmp;
			return g_ntohl(ip);
		}

	convert_end:
		delete[] strIPTmp;
		return 0;
	}

	/********************************************************************
	* [������]: wtoa
	* [����]: �����ֽ��ַ���ת���ɵ��ֽ��ַ��������ֽڵĸ�λ��Ϣ����ʧ
	* [����]:
	*   data�����ֽ��ַ���
	*   len: ָ���ַ������ȣ����Ϊ0��ʹ��_tcslen(data)
	* [����ֵ]:
	*   ת����ĵ��ֽ��ַ����������ڲ����룬�ɵ������ͷţ�
	* [�޸ļ�¼]:
	*   2014-12-17,littledj: create
	*   2015-04-23,littledj: ��char��wchar�����ݣ����Ӳ���len
	********************************************************************/
	char* wtoa(const wchar_t* data, int len)
	{
		if (data == NULL)
		{
			return NULL;
		}

		size_t data_len = wcslen(data);
		if (len) data_len = len;
		char* retData = new char[data_len + 1];
		for (size_t i = 0; i < data_len; i++)
		{
			retData[i] = (char)data[i];
		}
		retData[data_len] = 0;

		return retData;
	}

	/********************************************************************
	* [������]: atow
	* [����]: �����ֽ��ַ���ת���ɿ��ֽ��ַ���
	* [����]:
	*   data�����ֽ��ַ���
	*   len: ָ���ַ������ȣ����Ϊ0��ʹ��_tcslen(data)
	* [����ֵ]:
	*   ת����Ŀ��ֽ��ַ����������ڲ����룬�ɵ������ͷţ�
	* [�޸ļ�¼]:
	*   2014-12-17,littledj: create
	*   2015-04-23,littledj: ���Ӳ���len
	********************************************************************/
	wchar_t* atow(const char* data, int len)
	{
		if (data == NULL)
		{
			return NULL;
		}

		size_t data_len = strlen(data);
		if (len) data_len = len;
		wchar_t* retData = new wchar_t[data_len + 1];
		for (size_t i = 0; i < data_len; i++)
		{
			retData[i] = data[i];
		}
		retData[data_len] = 0;

		return retData;
	}

	/********************************************************************
	* [������]: SplitString
	* [����]: ���ַ�����ָ���ַ��ָ�����طָ���
	* [����]:
	*   str: �����ַ���
	*   ch: ָ���ķָ��ַ�
	* [����ֵ]:
	*   �ָ���
	* [�޸ļ�¼]:
	*   2017-03-13,littledj: create
	********************************************************************/
	vector<wstring> SplitString(const wstring& str, const wchar_t ch)
	{
		vector<wstring> strs;
		size_t start = 0;
		size_t pos = str.find(ch);
		while (pos != wstring::npos)
		{
			strs.push_back(str.substr(start, pos - start));
			start = pos + 1;
			pos = str.find(ch, start);
		}
		strs.push_back(str.substr(start, str.length() - start));
		return strs;
	}
	vector<string> SplitString(const string& str, const char ch)
	{
		if (str.empty())
		{
			return vector<string>();
		}

		vector<string> strs;
		size_t start = 0;
		size_t pos = str.find(ch);
		while (pos != string::npos)
		{
			strs.push_back(str.substr(start, pos - start));
			start = pos + 1;
			pos = str.find(ch, start);
		}
		strs.push_back(str.substr(start, str.length() - start));
		return strs;
	}

	/********************************************************************
	* [������]: TrimString
	* [����]: ɾ���ַ�����ͷ�ͽ�β��ָ���ַ�
	* [����]:
	*   str: �����޸ģ������ַ���
	*   ch: ��Ҫɾ�����ַ�
	* [����ֵ]:
	*   ɾ��ָ���ַ�����ַ���
	* [�޸ļ�¼]:
	*   2017-03-13,littledj: create
	********************************************************************/
	string& TrimString(string& str, const char ch)
	{
		if (str.empty() || str.size() == 0)
			return str;

		size_t delch = string::npos;
		while ((delch = str.find(ch)) == 0)
			str.erase(delch, 1);

		if (str.empty() || str.size() == 0)
			return str;

		delch = str.rfind(ch);
		while ((delch = str.rfind(ch)) == str.length() - 1)
			str.erase(delch, 1);

		return str;
	}
	wstring& TrimString(wstring& str, const wchar_t ch)
	{
		if (str.empty() || str.size() == 0)
			return str;

		size_t delch = wstring::npos;
		while ((delch = str.find(ch)) == 0)
			str.erase(delch, 1);

		if (str.empty() || str.size() == 0)
			return str;

		delch = str.rfind(ch);
		while ((delch = str.rfind(ch)) == str.length() - 1)
			str.erase(delch, 1);

		return str;
	}

	/********************************************************************
	* [������]: RemoveAllChar
	* [����]: ɾ���ַ����е�����ָ���ַ�
	* [����]:
	*   str: �����޸ģ������ַ���
	*   ch: ��Ҫɾ�����ַ�
	* [����ֵ]:
	*   ��
	* [�޸ļ�¼]:
	*   2017-03-13,littledj: create
	********************************************************************/
	void RemoveAllChar(string& str, const char ch)
	{
		size_t delch = string::npos;
		while ((delch = str.find(ch)) != string::npos)
			str.erase(delch, 1);
	}
	void RemoveAllChar(wstring& str, const wchar_t ch)
	{
		size_t delch = wstring::npos;
		while ((delch = str.find(ch)) != wstring::npos)
			str.erase(delch, 1);
	}

	/********************************************************************
	* [������]: DeleteEmptyItems
	* [����]: ɾ��vector<string>�еĿ��ַ���
	* [����]:
	*   strs: �����޸ģ��ַ�������
	* [����ֵ]:
	*   ��
	* [�޸ļ�¼]:
	*   2017-03-13,littledj: create
	********************************************************************/
	void DeleteEmptyItems(vector<string>& strs)
	{
		for (auto i = strs.begin(); i < strs.end();)
		{
			if ((*i).empty())
				strs.erase(i++);
			else
				i++;
		}
	}

	/********************************************************************
	* [������]: StringToWString/StringToTString/TStringToString
	*           WStringToString/WStringToTString/TStringToWString
	* [����]: wstring tstring string����໥ת���������ڱ���ѡ��UNICODE��
	* [����]:
	*   str: ����ַ���
	* [����ֵ]:
	*   ת������ַ���
	* [�޸ļ�¼]:
	*   2017-03-13,littledj: create
	********************************************************************/
	wstring StringToWString(const string& str)
	{
		size_t len = str.size() * 2;// Ԥ���ֽ���
		if (len == 0)
			return wstring(L"");
		setlocale(LC_CTYPE, "");     //������ô˺���
		wchar_t *p = new wchar_t[len];// ����һ���ڴ���ת������ַ���
		mbstowcs(p, str.c_str(), str.size());// ת��
		p[str.size()] = 0;
		wstring str1(p);
		delete[] p;// �ͷ�������ڴ�
		return str1;
	}
	string WStringToString(const wstring& str)
	{
		size_t len = str.size() * 4;
		if (len == 0)
			return string("");
		setlocale(LC_CTYPE, "");
		char *p = new char[len];
		wcstombs(p, str.c_str(), str.size());
		p[str.size()] = 0;
		string str1(p);
		delete[] p;
		return str1;
	}
	tstring StringToTString(const string & str)
	{
#ifdef UNICODE	
		return StringToWString(str);
#else
		return str;
#endif
	}
	tstring WStringToTString(const wstring & str)
	{
#ifdef UNICODE	
		return str;
#else
		return WStringToString(str);
#endif
	}
	string TStringToString(const tstring & str)
	{
#ifdef UNICODE	
		return WStringToString(str);
#else
		return str;
#endif
	}
	wstring TStringToWString(const tstring & str)
	{
#ifdef UNICODE	
		return str;
#else
		return StringToWString(str);
#endif
	}

	/********************************************************************
	* [������]: ReplaseAllSubString
	* [����]: �滻�ַ����е�ָ���Ӵ�
	* [����]:
	*   str: �����޸ģ�������ַ���
	*   src: ���滻��ԭ�Ӵ�
	*   dst: Ŀ���Ӵ�
	* [����ֵ]:
	*   ��
	* [�޸ļ�¼]:
	*   2017-03-13,littledj: create
	********************************************************************/
	void ReplaseAllSubString(string & str, const string & src, const string & dst)
	{
		size_t fd = string::npos;
		while ((fd = str.find(src)) != string::npos)
		{
			str.replace(fd, src.size(), dst);
		}
	}
	void ReplaseAllSubString(wstring & str, const wstring & src, const wstring & dst)
	{
		size_t fd = wstring::npos;
		while ((fd = str.find(src)) != wstring::npos)
		{
			str.replace(fd, src.size(), dst);
		}
	}

	/********************************************************************
	* [������]: SetConfigInt
	* [����]: �޸������ļ�ָ��������Ϊĳintֵ
	* [����]:
	*   filename: �����ļ�
	*   key: ������
	*   value: ֵ
	*   title: ���ñ�ͷ
	* [����ֵ]:
	*   ��
	* [�޸ļ�¼]:
	*   2017-03-13,littledj: create
	********************************************************************/
	void SetConfigInt(const tstring& filename, const tstring& key,
		const int value, const tstring& title)
	{
		SetConfigString(filename, key, to_tstring(value), title);
	}

	/********************************************************************
	* [������]: SetConfigString
	* [����]: �޸������ļ�ָ��������Ϊĳstringֵ
	* [����]:
	*   filename: �����ļ�
	*   key: ������
	*   value: ֵ
	*   title: ���ñ�ͷ
	* [����ֵ]:
	*   ��
	* [�޸ļ�¼]:
	*   2017-03-13,littledj: create
	********************************************************************/
	void SetConfigString(const tstring& filename, const tstring& key,
		const tstring& value, const tstring& title)
	{
		FILE *fp;
		if ((fp = tfopen(filename.c_str(), TEXT("r"))) == NULL)
			return;

		// ��ȡconfig�ļ�
		char* buff = new char[4096]; // max config size		
		memset(buff, 0, 4096);
		size_t flen = fread(buff, 1, 4096, fp);
		fclose(fp);				

		size_t newlen = 0;
		char* newbuff = new char[4096];
		memset(newbuff, 0, 4096);

		// ���ļ���Ѱ��key		
		size_t keypos = 0;
		size_t remain = 0;
		bool line = false;
		string skey = TStringToString(key);
		string svalue = TStringToString(value);
		for (keypos = 0; keypos < flen; keypos++)
		{
			if (keypos == 0)
			{
				if (strncmp(skey.c_str(), buff + keypos, skey.size()) == 0)
					break;
			}

			if (buff[keypos] == '\n' || buff[keypos] == '\r')
			{
				line = true;
				newbuff[keypos] = buff[keypos];
				continue;
			}

			if (line)
			{
				line = false;
				if (strncmp(skey.c_str(), buff + keypos, skey.size()) == 0)
					break;				
			}
			newbuff[keypos] = buff[keypos];
		}		
				
		if (keypos == flen)
		{
			// δ�ҵ�, д���ļ�β
			if (newbuff[keypos - 1] != '\n')
				newbuff[keypos++] = '\n';

			newlen = keypos;
			newlen += sprintf(newbuff + newlen, "\n%s=%s\n",
				skey.c_str(), svalue.c_str());
		}
		else
		{
			// �ҵ���β
			for (remain = keypos; remain < flen; remain++)
			{
				if (buff[remain] == '\n' || buff[remain] == '\r')
					break;
			}

			newlen = keypos;
			newlen += sprintf(newbuff + newlen, "%s=%s",
				skey.c_str(), svalue.c_str());
			memcpy(newbuff + newlen, buff + remain, flen - remain);
			newlen += flen - remain;
		}		

		// ����ļ�������
		if ((fp = tfopen(filename.c_str(), TEXT("w"))) == NULL)
		{
			delete[] buff;
			delete[] newbuff;
			return;
		}
		fwrite(newbuff, 1, newlen, fp);
		fclose(fp);
		delete[] buff;
		delete[] newbuff;
	}

	/********************************************************************
	* [������]: GetConfigString
	* [����]: �������ļ�ָ���������ȡ�ַ�����������
	* [����]:
	*   filename: �����ļ�
	*   key: ������
	*   dft: Ĭ��ֵ
	*   title: ���ñ�ͷ
	* [����ֵ]:
	*   ��
	* [�޸ļ�¼]:
	*   2017-03-13,littledj: create
	********************************************************************/
	tstring GetConfigString(const tstring& filename, const tstring& key,
		const tstring& dft, const tstring& title)
	{
		FILE *fp;
		tchar szLine[1024];
		szLine[0] = 0;
		static tchar tmpstr[1024];
		int rtnval;
		int i = 0;
		int flag = 0;
		tchar *tmp;

		if ((fp = tfopen(filename.c_str(), TEXT("r"))) == NULL)
		{
			return dft;
		}
		while (!feof(fp))
		{
			rtnval = fgetc(fp);
			if (rtnval == EOF)
			{
				if (tcslen(szLine) > 0)
				{
					tmp = tcschr(szLine, '=');
					if ((tmp != NULL) && (flag == 1))
					{
						if (tcsstr(szLine, key.c_str()) != NULL)
						{
							//ע����
							if ('#' == szLine[0])
							{}
							else if ('/' == szLine[0] && '/' == szLine[1])
							{}
							else
							{
								fclose(fp);

								if (tcslen(tmp) == 1)
									return dft;

								//��key��Ӧ����
								tcscpy(tmpstr, tmp + 1);
								return tmpstr;
							}
						}
					}
				}

				break;
			}
			else
			{
				szLine[i++] = rtnval;
				szLine[i] = 0;
			}
			if (rtnval == '\n' || rtnval == '\r')
			{
				szLine[i - 1] = 0;
				i = 0;
				tmp = tcschr(szLine, '=');
				if ((tmp != NULL) && (flag == 1))
				{
					if (tcsstr(szLine, key.c_str()) != NULL)
					{
						//ע����
						if ('#' == szLine[0])
						{}
						else if ('/' == szLine[0] && '/' == szLine[1])
						{}
						else
						{
							fclose(fp);

							if (tcslen(tmp) == 1)
								return dft;

							//��key��Ӧ����
							tcscpy(tmpstr, tmp + 1);							
							return tmpstr;
						}
					}
				}
				else
				{
					tcscpy(tmpstr, TEXT("["));
					tcscat(tmpstr, title.c_str());
					tcscat(tmpstr, TEXT("]"));
					if (tcsncmp(tmpstr, szLine, tcslen(tmpstr)) == 0)
					{
						//�ҵ�title
						flag = 1;
					}
				}
			}
		}
		fclose(fp);
		return dft;
	}

	/********************************************************************
	* [������]: GetConfigInt
	* [����]: �������ļ�ָ���������ȡ����������
	* [����]:
	*   filename: �����ļ�
	*   key: ������
	*   dft: Ĭ��ֵ
	*   title: ���ñ�ͷ
	* [����ֵ]:
	*   ��
	* [�޸ļ�¼]:
	*   2017-03-13,littledj: create
	********************************************************************/
	int GetConfigInt(const tstring& filename, const tstring& key,
		const int dft, const tstring& title)
	{
		return ttoi(GetConfigString(filename, key, to_tstring(dft), title).c_str());
	}

}