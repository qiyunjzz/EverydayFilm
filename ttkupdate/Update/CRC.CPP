////////////////////////////////////////////////////////////////////////////////////////////
//
// CRC.cpp
//
// 作者: 未知
//
// 修改日期: 2007-6-29
//
// 说明: CRC算法函数库
//
//
/////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CRC.h"

ULONG crc32_table[256]; // Lookup table array

void InitCRC()
{
	// This is the official polynomial used by CRC-32 
	// in PKZip, WinZip and Ethernet. 
	ULONG ulPolynomial = 0x04c11db7;
	
	// 256 values representing ASCII character codes.
	for(int i = 0; i <= 0xFF; i++)
	{
		crc32_table[i]=Reflect(i, 8) << 24;
		for (int j = 0; j < 8; j++)
			crc32_table[i] = (crc32_table[i] << 1) ^ (crc32_table[i] & (1 << 31) ? ulPolynomial : 0);
		crc32_table[i] = Reflect(crc32_table[i], 32);
	} 
}

// Reflection is a requirement for the official CRC-32 standard.
// You can create CRCs without it, but they won't conform to the standard.
ULONG Reflect(ULONG ref, char ch)
{// Used only by Init_CRC32_Table()
	ULONG value(0);
	
	// Swap bit 0 for bit 7
	// bit 1 for bit 6, etc.
	for(int i = 1; i < (ch + 1); i++)
	{
		if(ref & 1)
			value |= 1 << (ch - i);
		ref >>= 1;
	}
	return value;
}


// This function uses the crc32_table lookup table
// to generate a CRC for csData
ULONG CRC(char *pData, DWORD dwSize)
{
	InitCRC();

	// Be sure to use unsigned variables,
	// because negative values introduce high bits
	// where zero bits are required.
	ULONG  crc(0xffffffff);
	int len;
	unsigned char* buffer;
	
	len = dwSize;
	// Save the text in the buffer.
	buffer = (unsigned char*)pData;
	// Perform the algorithm on each character
	// in the string, using the lookup table values.
	while(len--)
		crc = (crc >> 8) ^ crc32_table[(crc & 0xFF) ^ *buffer++];
	// Exclusive OR the result with the beginning value.
	return crc^0xffffffff;
}