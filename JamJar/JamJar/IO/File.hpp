#pragma once

#include "../String.hpp"

#include "Directory.hpp"


class File
{
private:
	MutableString m_name;
	MutableString m_type;

	std::filesystem::path m_path;

	NullableRef<Directory> m_parent;
public:
	File(const String& name, const String& type) : m_name(name), m_type(type), m_parent(nullptr)
	{
		String fullName = GetFullName();

		char* chars = new char[fullName.Length().ToRawValue() + 1U];

		fullName.CopyTo(chars);
		m_path = chars;

		delete[] chars;
	}

	File(const String& name, const String& type, const Directory& parent) : m_name(name), m_type(type), m_parent(New<Directory>(parent))
	{
		String fullName = GetFullName();

		char* chars = new char[fullName.Length().ToRawValue() + 1U];

		fullName.CopyTo(chars);
		m_path = chars;

		delete[] chars;
	}

	const String& GetName() const { return m_name; }
	const String& GetType() const { return m_type; }

	String GetFullName() const 
	{
		if(m_parent.IsNull())
			return m_name + "." + m_type;

		String result = m_parent->GetFullName() + "\\" + m_name + "." + m_type;
		return result;
	}
};