#include "Dynamic.hpp"

Dynamic::Dynamic(const Dynamic& other) : m_address(malloc(other.m_type.GetSize().ToRawValue())), m_type(other.m_type)
{
	m_type.GetCopyConstructor()(other.m_address, m_address);
}

Dynamic::Dynamic(Dynamic&& other) noexcept : m_address(malloc(other.m_type.GetSize().ToRawValue())), m_type(other.m_type)
{
	m_type.GetMoveConstructor()(other.m_address, m_address);
}

Dynamic::~Dynamic()
{
	m_type.GetDestructor()(m_address);
	free(m_address);
}
