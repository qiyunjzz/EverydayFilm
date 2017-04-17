#ifndef REGISTER_H
#define REGISTER_H
#pragma once

class CRegister
{
	HKEY m_hKey;
public:
	CRegister(void);
	~CRegister(void);

	BOOL OpenKeyEx(HKEY hKey, LPCWSTR lpSubKey);

	BOOL CreateKeyA(HKEY hKey, LPCSTR lpSubKey);
	BOOL CreateKeyW(HKEY hKey, LPCWSTR lpSubKey);

	BOOL DeleteKeyA(LPCSTR lpSubKey);
	BOOL DeleteKeyW(LPCTSTR lpSubKey);
	
	BOOL QueryValueExA(LPCSTR lpValueName, LPBYTE lpData, DWORD cbData);
	BOOL QueryValueExW(LPCTSTR lpValueName, LPBYTE lpData, DWORD cbData);
	
	BOOL SetValueExExA(LPCSTR lpValueName, DWORD dwType, CONST BYTE* lpData, DWORD cbData);
	BOOL SetValueExExW(LPCWSTR lpValueName, DWORD dwType, CONST BYTE* lpData, DWORD cbData);
};

//extern CRegister m_register;

#endif