#pragma once

#include "Data/Reflection.hpp"

class Dynamic
{
private:
	      void*     m_address;
	const TypeInfo& m_type;

	Dynamic(void* address, const TypeInfo& type) : m_address(address), m_type(type) {}
public:
	template<DefaultConstructible T>
	static Dynamic Create();

	template<typename T, typename... Args>
	static Dynamic Create(Args&&... args) requires ConstructibleFrom<T, Args...>;

	template<CopyConstructible T>
	Dynamic(const T& item);

	Dynamic(const Dynamic& other);

	~Dynamic();

	template<CopyConstructible T>
	operator T() const;

	friend Dynamic operator+(const Dynamic& left, const Dynamic& right);
	friend Dynamic operator-(const Dynamic& left, const Dynamic& right);
	friend Dynamic operator*(const Dynamic& left, const Dynamic& right);
	friend Dynamic operator/(const Dynamic& left, const Dynamic& right);
	friend Dynamic operator%(const Dynamic& left, const Dynamic& right);

	friend Dynamic operator&(const Dynamic& left, const Dynamic& right);
	friend Dynamic operator|(const Dynamic& left, const Dynamic& right);
	friend Dynamic operator^(const Dynamic& left, const Dynamic& right);

	friend Dynamic operator<<(const Dynamic& left, const Dynamic& right);
	friend Dynamic operator>>(const Dynamic& left, const Dynamic& right);

	friend Boolean operator< (const Dynamic& left, const Dynamic& right);
	friend Boolean operator> (const Dynamic& left, const Dynamic& right);
	friend Boolean operator<=(const Dynamic& left, const Dynamic& right);
	friend Boolean operator>=(const Dynamic& left, const Dynamic& right);
	friend Boolean operator==(const Dynamic& left, const Dynamic& right);
	friend Boolean operator!=(const Dynamic& left, const Dynamic& right);

	Dynamic operator+() const;
	Dynamic operator-() const;
	Dynamic operator~() const;

	Dynamic& operator++() const;
	Dynamic& operator--() const;
};

template<DefaultConstructible T>
inline Dynamic Dynamic::Create() { return Dynamic(new T(), Reflect::Type<T>()); }

template<typename T, typename... Args>
inline Dynamic Dynamic::Create(Args&&... args) requires ConstructibleFrom<T, Args...> { return Dynamic(new T(args...), Reflect::Type<T>()); }

template<CopyConstructible T>
inline Dynamic::Dynamic(const T& item) : m_address(new T(item)), m_type(Reflect::Type<T>()) {}

template<CopyConstructible T>
inline Dynamic::operator T() const 
{
	if(Reflect::Type<T>() != m_type)
		InvalidCastException(m_type, Reflect::Type<T>()).Throw();

	return T(*(T*)m_address); 
}

