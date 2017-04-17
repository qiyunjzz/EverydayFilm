#include "StdAfx.h"
#include "Register.h"

CRegister::CRegister(void)
{
	m_hKey = NULL;
}

CRegister::~CRegister(void)
{
	if (m_hKey)
	{
		RegCloseKey(m_hKey);
		m_hKey = NULL;
	}
}

BOOL CRegister::OpenKeyEx(HKEY hKey, LPCWSTR lpSubKey)
{
	if (m_hKey)
	{
		RegCloseKey(m_hKey);
		m_hKey = NULL;
	}

	LONG lResult = RegOpenKeyEx(hKey, lpSubKey, 0, KEY_ALL_ACCESS, &m_hKey);
	return lResult == ERROR_SUCCESS;
}

BOOL CRegister::CreateKeyA(HKEY hKey, LPCSTR lpSubKey)
{
	LONG lResult = RegCreateKeyA(hKey, lpSubKey, &m_hKey);
	return lResult == ERROR_SUCCESS;
}
	
BOOL CRegister::CreateKeyW(HKEY hKey, LPCWSTR lpSubKey)
{
	LONG lResult = RegCreateKeyW(hKey, lpSubKey, &m_hKey);
	return lResult == ERROR_SUCCESS;
}

BOOL CRegister::DeleteKeyA(LPCSTR lpSubKey)
{
	return RegDeleteKeyA(m_hKey, lpSubKey);
}
	
BOOL CRegister::DeleteKeyW(LPCTSTR lpSubKey)
{
	return RegDeleteKeyW(m_hKey, lpSubKey);
}
	
BOOL CRegister::QueryValueExA(LPCSTR lpValueName, LPBYTE lpData ,DWORD cbData)
{
	DWORD size = cbData;
	LONG lResult = RegQueryValueExA(m_hKey, lpValueName, NULL, NULL, lpData, &size);
	return lResult == ERROR_SUCCESS;
}

BOOL CRegister::QueryValueExW(LPCWSTR lpValueName, LPBYTE lpData ,DWORD cbData)
{
	DWORD size = cbData;
	LONG lResult = RegQueryValueExW(m_hKey, lpValueName, NULL, NULL, lpData, &size);
	return lResult == ERROR_SUCCESS;
}

BOOL CRegister::SetValueExExA(LPCSTR lpValueName, DWORD dwType, CONST BYTE* lpData, DWORD cbData)
{
	LONG lResult = RegSetValueExA(m_hKey, lpValueName, NULL, dwType, (LPBYTE)lpData, cbData);
	return lResult == ERROR_SUCCESS;
}

BOOL CRegister::SetValueExExW(LPCWSTR lpValueName, DWORD dwType, CONST BYTE* lpData, DWORD cbData)
{
	LONG lResult = RegSetValueExW(m_hKey, lpValueName, NULL, dwType, (LPBYTE)lpData, cbData);
	return lResult == ERROR_SUCCESS;
}
