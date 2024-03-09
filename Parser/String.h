#pragma once


class String
{
	char* str{};
public:
	size_t len{};

	String() = default;
	String(const char* c)
	{
		len = str_len(c) + 1;
		str = new char[len] {};
		str_cpy(str, c);
	}
	String(char c)
	{
		len = 2;
		str = new char[len] {};
		str[0] = c;
	}
	String(const String& o)
	{
		len = o.len;
		str = new char[len] {};
		str_cpy(str, o.str);
	}
	~String()
	{
		if (str == nullptr) return;
		delete[] str;
	}
	String& operator=(const String& o)
	{
		len = o.len;
		if (str != nullptr)
		{
			delete[] str;
		}
		str = new char[len] {};
		str_cpy(str, o.str);
		return *this;
	}


	char& operator[](size_t i)
	{
		return str[i];
	}

	operator char* ()
	{
		return str;
	}

	void operator+= (char c)
	{
		char* temp{ str };
		str = new char[len + 1] {};
		str_cpy(str, temp);
		str[len-1] = c;
		len++;
		delete temp;
	}

	operator int()
	{
		if ((str[0] < '0' || str[0] > '9') && str[0] != '-') return 0;
		bool neg{ str[0] == '-' };
		size_t end{neg};
		for (; str[end] >= '0' && str[end] <= '9'; ++end);
		return to_int(str, neg, neg, end);
	}

	bool operator==(const String& o)
	{
		return str_eq(str, o.str);
	}
	bool operator==(const char* o)
	{
		return str_eq(str, o);
	}
	bool operator==(char c)
	{
		return str[0] == c;
	}

	void resize(size_t newsize)
	{
		if (str != nullptr)
		{
			delete[] str;
		}
		len = newsize;
		str = new char[len] {};
	}


	static bool str_eq(const char* a, const char* b)
	{
		if (str_len(a) != str_len(b)) return false;
		for (size_t i{}; a[i] != '\0'; ++i)
		{
			if (a[i] == b[i]) continue;
			return false;
		}
		return true;
	}


	//Assumes a can fit b;
	static void str_cpy(char* a, const char* b)
	{
		for (size_t i{}; b[i] != '\0'; ++i)
		{
			a[i] = b[i];
		}
	}

	static void str_cpy(char* a, const char* b, size_t start, size_t end)
	{
		for (size_t i{ start }; i < end; ++i)
		{
			a[i] = b[i];
		}
	}

	static size_t str_len(const char* a)
	{
		size_t i{};
		for (; a[i] != '\0'; ++i);
		return i;
	}
	
	static int to_int(const char* a, bool neg, size_t start = 0, size_t end = 0)
	{
		int total{};
		size_t len{ (end - start)-1 };
		for (size_t i{ start }; i < end; ++i)
		{
			char c{ a[i] };
			total += (int)((c - '0') * pow(10, len - (i - start)));
		}
		if (neg) total *= -1;
		return total;
	}

};