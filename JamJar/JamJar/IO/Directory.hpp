#pragma once

#include "../String.hpp"

#include <filesystem>

class Directory
{
private:
	MutableString m_name;

	std::filesystem::path m_path;

	NullableRef<Directory> m_parent;
public:
	Directory(const String& name) : m_name(name), m_parent(nullptr)
	{
		String fullName = GetFullName();

		char* chars = new char[fullName.Length().ToRawValue() + 1U];

		fullName.CopyTo(chars);
		m_path = chars;

		delete[] chars;
	}

	Directory(const String& name, const Directory& parent) : m_name(name), m_parent(New<Directory>(parent)) 
	{
		String fullName = GetFullName();

		char* chars = new char[fullName.Length().ToRawValue() + 1U];

		fullName.CopyTo(chars);
		m_path = chars;

		delete[] chars;
	}

	const String& GetName() const { return m_name; }

	String GetFullName() const 
	{
		if(m_parent.IsNull())
			return m_name;

		String result = m_parent->GetFullName() + "\\" + m_name;
		return result;
	}

	const Directory& GetParent() const { return *m_parent; }

	Boolean Exists() const { return std::filesystem::exists(m_path); }

	void Create() const { if(!Exists()) std::filesystem::create_directory(m_path); }
	void Delete() const { if( Exists()) std::filesystem::remove(m_path);           }

	void Rename(const String& newName) 
	{
		m_name = newName;

		String fullName = GetFullName();

		char* chars = new char[fullName.Length().ToRawValue() + 1U];

		fullName.CopyTo(chars);
		std::filesystem::rename(m_path, chars);

		m_path = chars;

		delete[] chars;
	}

	void CopyTo(const Directory& directory) { std::filesystem::copy(m_path, directory.m_path); }

	String ToString() const { return GetFullName(); }
};