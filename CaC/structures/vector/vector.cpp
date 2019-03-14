#include "vector.h"
#include "../ds_routines.h"
#include <cstdlib>
#include <cstring>

namespace structures {

	Vector::Vector(size_t size) :
		memory_(calloc(size, 1)),
		size_(size)
	{
	}

	// Vector(other.size_) je kosntruktor parametricky
	Vector::Vector(const Vector& other) :
		Vector(other.size_)
	{
		memcpy(memory_, other.memory_, other.size_);
	}

	Vector::~Vector()
	{
		// dealokuje cast pamate alokovanej malloc-om, calloc-om, na ktoru ukazuje pointer
		free(memory_);			
		memory_ = nullptr;
		size_ = 0;
	}

	Structure* Vector::clone() const
	{
		return new Vector(*this);
	}

	size_t Vector::size() const
	{
		return size_;
	}

	Structure & Vector::operator=(const Structure & other)
	{
		if (this != &other)
		{
			*this = dynamic_cast<const Vector&>(other);
		}
		return *this;
	}

	Vector& Vector::operator=(const Vector& other)
	{
		if (this != &other) {
			size_ = other.size_;
			memory_ = realloc(memory_, size_);		// ukladam to do premennej, lebo povodnu pamat na povodnom mieste moze dat na ine miesto
			memcpy(memory_, other.memory_, size_);
		}
		return *this;
	}

	bool Vector::operator==(const Vector& other) const
	{
		return (this == &other || size_ == other.size_ && memcmp(memory_, other.memory_, other.size_) == 0);
	}

	byte& Vector::operator[](const int index)
	{
		DSRoutines::rangeCheckExcept(index, size_, "Invalid index");
		return *(static_cast<byte*>(memory_) + index);
	}

	byte Vector::operator[](const int index) const
	{
		DSRoutines::rangeCheckExcept(index, size_, "Invalid index");
		return *(static_cast<byte*>(memory_) + index);
		//return *(static_cast<byte*>(memory_) + index);
	}

	// musim to skonvertovat, lebo je to void*, void ptr + 1 je neviem co
	byte& Vector::readBytes(const int index, const int count, byte& dest)
	{
		//prvy rangeCheck sa kontroluje aj v readBytes, zakomentujem aby som to nekontroloval 2x
		//DSRoutines::rangeCheckExcept(index, size_, "Invalid index in vector");

		// kvoli poslednemu, lebo ak chcem citat od indexu 1 tri bajty, msim ceknut ci index 1 +3 existuje, size_ ide len do 4, nie vratane 4
		DSRoutines::rangeCheckExcept(index + count, size_ + 1, "Invalid end index");
		memcpy(&dest, getBytePointer(index), count);
		return dest;
	}

	// Ak sa index zdroja nachadza v intervale indexov, kam to mam vlozit, pouzijem memmove
	void Vector::copy(const Vector& src, const int srcStartIndex, Vector& dest, const int destStartIndex, const int length)
	{
		if (length != 0) {
			DSRoutines::rangeCheckExcept(srcStartIndex + length, src.size_ + 1, "Invalid end index");
			DSRoutines::rangeCheckExcept(destStartIndex + length, dest.size_ + 1, "Invalid end index");

			if ((src == dest) && (((srcStartIndex >= destStartIndex) && (srcStartIndex < destStartIndex + length)))) {
				memmove(dest.getBytePointer(destStartIndex), src.getBytePointer(srcStartIndex), length);
			}
			else {
				memcpy(dest.getBytePointer(destStartIndex), src.getBytePointer(srcStartIndex), length);
			}
		}
	}

	// podobne ako operator []
	byte* Vector::getBytePointer(const int index) const
	{
		DSRoutines::rangeCheckExcept(index, size_, "Invalid index");
		return static_cast<byte*>(memory_) + index;
	}

}