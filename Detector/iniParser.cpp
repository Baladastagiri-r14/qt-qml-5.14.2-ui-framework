/**
* File: iniParser.cpp
*
* Purpose: The main  implementation of CIniParser Class. 
*
* 
* @author Zhang Yong
* @version 1.0 2015/02/04
*
* Copyright (C) 2009, 2015, iRay Technology (Shanghai) Ltd.
*
*/
// #include "stdafx.h"
#include "iniParser.h"

#define INI_EOL endl

// Constructor
CIniParser::CIniParser()
{
}

// Destructor
CIniParser::~CIniParser()
{
	// Erase all the data structure
	m_vecBlocks.clear();
	m_vecSections.clear();
	m_vecComments.clear();
}

// Read ini file 
bool CIniParser::ReadFile(const string &strPath)
{
	// First Initialize m_strFilePath to strPath which is used to specify the source path 
	m_strFilePath = strPath;

	string   strLine;
	string   strSectionName, strItemName, strValue;
	string::size_type pLeft, pRight;

	// Make sure file can be opened
	fstream fRead;
	fRead.open(m_strFilePath.c_str(), ios::in);
	if (fRead.fail())
	{
		return false;
	}

	while(getline( fRead, strLine) ) 
	{
		// Make sure blank lines are omitted
		if (strLine.length() && (strLine[strLine.length() - 1] == '\r'))
		{
			strLine = strLine.substr(0, strLine.length() - 1);
		}

		if (strLine.length()) 
		{
			//if (!isprint(strLine[0]))
			//{
			//	printf("Failed  on reading  char %d\n", strLine[0]);
			//	fRead.close();

			//	return false;
			//}

			if (string::npos != ( pLeft = strLine.find_first_of(";#[="))) 
			{
				switch (strLine[pLeft]) 
				{
				case '[':
					// Section 
					if (string::npos != (pRight = strLine.find_last_of("]")) && pRight > pLeft) 
				    {
					    strSectionName = strLine.substr(pLeft + 1, pRight - pLeft - 1);
					    AddSection(strSectionName);
				    }
					break;

				case '=':
					// Item
					strItemName = strLine.substr(0, pLeft);
					strValue = strLine.substr(pLeft + 1);
					SetItemValueS(strSectionName, strItemName, strValue);
					break;

				case ';':
					// Comment should start by convention, not using C++ style comments
					// Found on Wikipedia, only ";" and "#" are supported in ini file comment
				case '#':		
					if (!m_vecBlocks.size())
					{
						AddHeaderComment(strLine.substr( pLeft + 1));
					}
					else
					{
						AddSectionComment(strSectionName, strLine.substr(pLeft + 1));
					}

					break;

				default:
					return false;
				}
			}
		}
	}

	fRead.close();
	if (m_vecSections.size())
	{
		return true;
	}

	return false;
}

// Write memory data to ini file stored in hard disk
bool CIniParser::WriteFile(const string &strPath)
{
	// First Initialize m_strFilePath to strPath which is used to specify the destination path 
	m_strFilePath = strPath;

	unsigned nCommentNdex, nSectionNdex, nItemNdex;
	fstream fWrite;

	fWrite.open(m_strFilePath.c_str(), ios::out);
	if (fWrite.fail())
	{
		return false;
	}
	// Write file header comments.
	for (nCommentNdex = 0; nCommentNdex < m_vecComments.size(); ++nCommentNdex)
		fWrite << ';' << m_vecComments[nCommentNdex] << INI_EOL;

	if (m_vecComments.size())
	{
		fWrite << INI_EOL;
	}

	// Write comments and items and corresponding values
	for (nSectionNdex = 0; nSectionNdex < m_vecBlocks.size(); ++nSectionNdex) 
	{
		fWrite << '[' << m_vecSections[nSectionNdex] << ']' << INI_EOL;

		// Section Comments.
		for (nCommentNdex = 0; nCommentNdex < m_vecBlocks[nSectionNdex].vecComments.size(); ++nCommentNdex)
			fWrite << ';' << m_vecBlocks[nSectionNdex].vecComments[nCommentNdex] << INI_EOL;

		// Items and corresponding values
		for (nItemNdex = 0; nItemNdex < m_vecBlocks[nSectionNdex].vecItems.size(); ++nItemNdex)
			fWrite << m_vecBlocks[nSectionNdex].vecItems[nItemNdex] << '=' << m_vecBlocks[nSectionNdex].vecValues[nItemNdex] << INI_EOL;

		fWrite << INI_EOL;
	}
	fWrite.close();

	return true;
}

// Get section index
bool CIniParser::GetSectionIndex(const string &strSectionName, unsigned &nIndex) const
{
	for (unsigned nSectionNdex = 0; nSectionNdex < m_vecSections.size(); ++nSectionNdex)
	{
		if (StrCompare(m_vecSections[nSectionNdex], strSectionName))
		{
			// Make sure it's positive
			nIndex = nSectionNdex; 
			return true;
		}
	}

	return false;
}

// Get strItemName corresponding index
bool CIniParser::GetItemIndex(unsigned nSectionNdex, const string &strItemName, unsigned &nIndex) const
{
	if (!m_vecBlocks.size() || nSectionNdex >= m_vecBlocks.size())
	{
		return false;
	}

	for (unsigned nItemIndex = 0; nItemIndex < m_vecBlocks[nSectionNdex].vecItems.size(); ++nItemIndex)
	{
		if (StrCompare(m_vecBlocks[nSectionNdex].vecItems[nItemIndex], strItemName))
		{
			nIndex = nItemIndex;
			return true;
		}
	}

	return false;
}

// Get section total number
unsigned CIniParser::GetSectionCount() const                       
{
	return m_vecSections.size();
}

// Set section name
unsigned CIniParser::AddSection(const string &strSectionName)
{
	unsigned uIndex;
	if (!GetSectionIndex(strSectionName, uIndex))
	{
		m_vecSections.resize(m_vecSections.size() + 1, strSectionName);
		m_vecBlocks.resize(m_vecBlocks.size() + 1);
	}

	return m_vecSections.size() - 1;
}

// Get section name
string CIniParser::GetSectionName(unsigned nSectionNdex) const
{
	if (nSectionNdex < m_vecSections.size())
	{
		return m_vecSections[nSectionNdex];
	}
    else
	{
		return "";
	}
}

// Get item total number in a specific section
unsigned CIniParser::GetItemCountInSection(unsigned nSectionNdex)
{
	if (nSectionNdex < m_vecBlocks.size())
	{
		return m_vecBlocks[nSectionNdex].vecItems.size();
	}

	return 0;
}

// Get item total number in a specific section
unsigned CIniParser::GetItemCountInSection(const string &strSectionName)
{
	unsigned nSectionIndex = 0;
	if (GetSectionIndex(strSectionName, nSectionIndex))	
	{
		return m_vecBlocks[nSectionIndex].vecItems.size();
	}
	return 0;
}

// Get item name
string CIniParser::GetItemName(unsigned nSectionNdex, unsigned nItemNdex) const
{
	if (nSectionNdex < m_vecBlocks.size() && nItemNdex < m_vecBlocks[nSectionNdex].vecItems.size())
	{
		return m_vecBlocks[nSectionNdex].vecItems[nItemNdex];
	}

	return "";
}

// Get item name
string CIniParser::GetItemName(const string &strSectionName, unsigned nItemNdex) const
{
	unsigned nSectionNdex = 0;
	if (GetSectionIndex(strSectionName, nSectionNdex))
	{
		return GetItemName(nSectionNdex, nItemNdex);
	}

	return "";
}

// Set item value
bool CIniParser::SetItemValue(unsigned nSectionNdex, unsigned nItemNdex, const string &strValue)
{
	if (nSectionNdex < m_vecBlocks.size() && nItemNdex < m_vecBlocks[nSectionNdex].vecItems.size())
	{
		m_vecBlocks[nSectionNdex].vecValues[nItemNdex] = strValue;
	}

	return false;
}

// Set item value
bool CIniParser::SetItemValueS(const string &strSectionName, const string &strItemName, const string &strValue, bool bCreate)
{
	// Actually if strSectionName and strItemName are both already existing, when SetItemValueS is still invoked it is meant to setting value to strValue
	unsigned nSectionNdex = 0;
	// If Section specified by strSectionName does not exist, Create it; if it is already existing, its index will be used later
	if (!GetSectionIndex(strSectionName, nSectionNdex))
	{
		if (bCreate)
		{
			nSectionNdex = AddSection(strSectionName);
		}
		else
		{
			return false;
		}
	}

	// Do not worry about isSucceeded is used twice
	unsigned nValueNdex = 0;
	if (!GetItemIndex(unsigned(nSectionNdex), strItemName, nValueNdex)) 
	{
		if (bCreate)
		{
			m_vecBlocks[nSectionNdex].vecItems.resize(m_vecBlocks[nSectionNdex].vecItems.size() + 1, strItemName);
			m_vecBlocks[nSectionNdex].vecValues.resize(m_vecBlocks[nSectionNdex].vecValues.size() + 1, strValue);
		}
		else
		{
			return false;
		}
	} 
	else
	{
		m_vecBlocks[nSectionNdex].vecValues[nValueNdex] = strValue;
	}

	return true;
}

// Set item value in Integer
bool CIniParser::SetItemValueI(const string &strSectionName, const string &strItemName, int nValue, bool bCreate)
{
	char szvalue[MAX_ITEM_VALUE_LENGTH];

	sprintf(szvalue, "%d", nValue);
	return SetItemValueS(strSectionName, strItemName, szvalue);
}

// Set item value in BOOL 
bool CIniParser::SetItemValueB(const string &strSectionName, const string &strValueName, bool bValue, bool bCreate) 
{
	return SetItemValueI(strSectionName, strValueName, int(bValue), bCreate);
}

// Set item value in floating-point type
bool CIniParser::SetItemValueF(const string &strSectionName, const string &strItemName, double fValue, bool bCreate)
{
	char szvalue[MAX_ITEM_VALUE_LENGTH];

	sprintf(szvalue, "%f", fValue);
	return SetItemValueS(strSectionName, strItemName, szvalue);
}

// Get item value
bool CIniParser::GetItemValue(unsigned nSectionNdex, unsigned nItemNdex, string &strValue)
{
	if (nSectionNdex < m_vecBlocks.size() && nItemNdex < m_vecBlocks[nSectionNdex].vecItems.size())
	{
		// If succeeded, reference isSucceeded would be set to true
        strValue = m_vecBlocks[nSectionNdex].vecValues[nItemNdex];
		return true;
	}
	else
	{
		// Used to indicate whether succeeded, still return something
		strValue = "";
		return false;
	}
}

// Overloading function for getting item value 
bool CIniParser::GetItemValueS(const string &strSectionName, const string &strItemName, string &strValue) const
{
	// Section not exist, will return NULL to indicate
	unsigned nSectionNdex = 0;  ;
	if (!GetSectionIndex(strSectionName, nSectionNdex))
	{
		strValue = "";
		return false;
	}

	// Item not exist, will return NULL to indicate
	unsigned nItemNdex = 0; ;
	if (!GetItemIndex(unsigned(nSectionNdex), strItemName, nItemNdex))
	{
		strValue = "";
		return false;
	}

	// Return Item corresponding value
	strValue = m_vecBlocks[nSectionNdex].vecValues[nItemNdex];

	return true;
}

// Get item value in floating-point type
bool CIniParser::GetItemValueF(const string &strSectionName, const string &strValueName, double &fValue) const
{
	//return atof(GetItemValueS(strSectionName, strValueName).c_str()); 
	string strTemp;
    bool bResult = GetItemValueS(strSectionName, strValueName, strTemp);
	if (bResult)
	{
		fValue = atof(strTemp.c_str());
	}

	return bResult; 
}

// Get item value in BOOL  type
bool CIniParser::GetItemValueB(const string &strSectionName, const string &strValueName, bool &bValue) const 
{
	int nTemp = 0;
	bool bResult = GetItemValueI(strSectionName, strValueName, nTemp);
	if (bResult)
	{
		bValue = nTemp ? true : false;
	}

	return bResult;
}

// Get item value in Integer type
bool  CIniParser::GetItemValueI(const string &strSectionName, const string &strItemName, int &nValue) const
{
	string strTemp = "";
	bool bResult = GetItemValueS(strSectionName, strItemName, strTemp);
	if (bResult)
	{
		nValue = atoi(strTemp.c_str());
	}

	return bResult; 
}

// Delete item in a specified section
bool CIniParser::DeleteItem(const string &strSectionName, const string &strItemName)
{
	unsigned nSectionNdex = 0;
	if (!GetSectionIndex(strSectionName, nSectionNdex))
	{
		return false;
	}

	unsigned nItemNdex = 0; 
	if (!GetItemIndex(unsigned(nSectionNdex), strItemName, nItemNdex))
	{
		return false;
	}

	vector<string>::iterator iter1 = m_vecBlocks[nSectionNdex].vecItems.begin() + nItemNdex;
	vector<string>::iterator iter2 = m_vecBlocks[nSectionNdex].vecValues.begin() + nItemNdex;
	m_vecBlocks[nSectionNdex].vecItems.erase(iter1, iter1 + 1);
	m_vecBlocks[nSectionNdex].vecValues .erase(iter2, iter2 + 1);

	return true;
}

// Delete a specified section using section name
bool CIniParser::DeleteSection(const string &strSectionName)
{
	unsigned nSectionNdex = 0;
	if (!GetSectionIndex(strSectionName, nSectionNdex))
	{
		return false;
	}
	vector<IniSection>::iterator iter1 = m_vecBlocks.begin() + nSectionNdex;
	vector<string>::iterator iter2 = m_vecSections.begin() + nSectionNdex;
	m_vecBlocks.erase(iter1, iter1 + 1);
	m_vecSections.erase(iter2, iter2 + 1);

	return true;
}

// Erase all content stored in computer memory
void CIniParser::EraseAllContent()
{
	m_vecBlocks.clear();
	m_vecSections.clear();
	m_vecComments.clear();
}

// Transforming to lower case string if bCasesensitive is FALSE
bool CIniParser::StrCompare(const string& str1, const string& str2, bool bCaseSensitive) const
{
	if (str1.size() != str2.size())
	{
		return false;
	}

	for (string::size_type i = 0; i < str1.size(); ++i)
	{
		if (str1[i] != str2[i])
		{
			if (!bCaseSensitive && toupper(str1[i]) == toupper(str2[i]))
			{
				continue;
			}
			return false;
		}
	}	

	return true;
}

// ini file header comment handling
void CIniParser::AddHeaderComment(const string &strComment)
{
	m_vecComments.resize(m_vecComments.size() + 1, strComment);
}

// ini file section header comment handling
bool CIniParser::AddSectionComment(unsigned  nSectionNdex, const string &strComment)
{
	if (nSectionNdex < m_vecBlocks.size()) 
	{
		m_vecBlocks[nSectionNdex].vecComments.resize(m_vecBlocks[nSectionNdex].vecComments.size() + 1, strComment);
		return true;
	}

	return false;
}

// ini file section header comment handling
bool CIniParser::AddSectionComment(const string &strSectionName, const string &strComment)
{
	unsigned nSectionNdex = 0; 
	if (!GetSectionIndex(strSectionName, nSectionNdex))
	{
		return false;
	}

	return AddSectionComment(unsigned(nSectionNdex), strComment);
}