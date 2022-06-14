#pragma once

#include "../String.hpp"

#include <filesystem>

class DirectoryIterator : public Iterator<const Directory>
{
private:
	const Directory& m_parent;
	std::filesystem::directory_iterator m_iterator;
public:
	DirectoryIterator(const Directory& parent, const std::filesystem::directory_iterator& iterator) : m_parent(parent), m_iterator(iterator) {}

	virtual void MoveNext() override { ++m_iterator; }

	virtual const Directory& Current() const override { return Directory(m_iterator->path().filename().c_str(), m_parent); }

	virtual Boolean Equals(const Iterator<const Directory>& other) const override { return m_iterator == ((const DirectoryIterator&)other).m_iterator; }
};

class DirectoryCollection : public IIterable<const Directory>
{
private:
	const Directory& m_parent;
public:
	DirectoryCollection(const Directory& parent) : m_parent(parent) {}

	virtual SharedRef<Iterator<const Directory>> Start() override;
	virtual SharedRef<Iterator<const Directory>> End()   override;

	virtual SharedRef<Iterator<const Directory>> Start() const override;
	virtual SharedRef<Iterator<const Directory>> End()   const override;
};

class Directory
{
private:
	MutableString m_name;

	std::filesystem::path m_path;

	NullableRef<Directory> m_parent;

	DirectoryCollection m_directories;
public:
	Directory(const String& name) : m_name(name), m_parent(nullptr), m_directories(*this)
	{
		String fullName = GetFullName();

		char* chars = new char[fullName.Length().ToRawValue() + 1U];

		fullName.CopyTo(chars);
		m_path = chars;

		delete[] chars;
	}

	Directory(const String& name, const Directory& parent) : m_name(name), m_parent(New<Directory>(parent)), m_directories(*this)
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

	friend class DirectoryCollection;
};

