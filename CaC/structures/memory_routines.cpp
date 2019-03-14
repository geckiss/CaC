#include "memory_routines.h"
#include <cstring>

namespace structures {
	byte & MemoryRoutines::byteSet(byte & B)
	{
		return B |= 128;
	}
	byte & MemoryRoutines::byteReset(byte & B)
	{
		return B &= 0;
	}
	byte & MemoryRoutines::byteXOR(byte & B)
	{
		return B ^= B;
	}
	byte & MemoryRoutines::byteSHL(byte & B)
	{
		return B <<= 1;
	}
	byte & MemoryRoutines::byteSHR(byte & B)
	{
		return B >>= 1;
	}
	bool MemoryRoutines::byteNthBitGet(byte & B, int n)
	{
		if (n > 0 && n < 9) {
			int mask = 1;
			return (B & (mask <<= n) == mask);
		}
		return false;
	}

	byte & MemoryRoutines::byteNthBitTo0(byte & B, int n)
	{
		if (n > 0 && n < 9) {
			// Shiftne masku, flipne ju, spravi AND s bytom
			return B &= ~(1 << n);
		}
		return B;
	}

	byte & MemoryRoutines::byteNthBitTo1(byte & B, int n)
	{
		if (n > 0 && n < 9) {
			return B |= (1 << n);
		}
		return B;
	}

	byte & MemoryRoutines::byteNthBitToggle(byte & B, int n)
	{
		if (n > 0 && n < 9) {
			int mask = 255;
			mask -= (128 >> (7 - n));
			if (B | mask ==  255) {			// n-ty bit je 1
				return B &= mask;
			}
			else {
				return B |= (~mask);
			}
		}
		return B;
	}

	int MemoryRoutines::bitIndexOfBitMask(const byte * bytes, int bytesCount, byte bitMask)
	{
		int firstBit = 0;
		// Maska na porovnavanie s nasledujucim prvkom pola
		// Zistujem, ci po SHL musim este pripocitat 1
		int k = 128;
		// Znaci, o kolko bytes som posunul pointer
		int mov = 0;

		byte* ptrMaska = &bitMask;
		// Hodnoty pola bytes nemozem menit, preto kopia
		byte* copiedBytes = new byte[bytesCount];
		memcpy(copiedBytes, bytes, bytesCount);

		for (int i = 0; i <= bytesCount * 8 - 8; i++) {
			if (k == 0) {
				copiedBytes = copiedBytes++;
				mov++;
				k = 128;
			}

			if (memcmp(ptrMaska, copiedBytes, 1) == 0) {
				// Vratim pointer na zaciatok pola
				copiedBytes -= mov;
				delete[] copiedBytes;
				return firstBit;
			}
			if ((*(copiedBytes + 1) & k) != 0) {
				*copiedBytes <<= 1;
				(*copiedBytes)++;
			}
			else {
				*copiedBytes <<= 1;
			}

			firstBit++;
			k /= 2;
		}

		copiedBytes -= mov;
		delete[] copiedBytes;
		return -1;

	}

	MemoryRoutines::MemoryRoutines()
	{
	}

}
