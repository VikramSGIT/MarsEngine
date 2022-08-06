#ifndef ME_ARRAY
#define ME_ARRAY

#pragma once

template<typename T, size_t count> class Array
{
public:
	Array(const T& initialize)
	{
		size_t i;
		*arr = initialize;
		for (i = 1; i * 2 < count; i *= 2)
			memcpy(arr + i, arr, sizeof(T) * i);
		memcpy(arr + i, arr, sizeof(T) * (count - i - 1));
	}
	template<typename ...Args>
	Array(Args&& ...initialize)
	{
		size_t i;
		new (arr) T(initialize...);
		for (i = 1; i * 2 < count; i *= 2)
			memcpy(arr + i, arr, sizeof(T) * i);
		memcpy(arr + i, arr, sizeof(T) * (count - i - 1));
	}

	T* begin() { return arr; }
	T* end() { return arr + count; }

	T& operator[](const size_t& index) { return arr[index]; }
	inline T& at(const size_t& index) { return arr[index]; }
	inline size_t size() const { return count; }
private:
	T arr[count];
};
#endif