#include "Directory.hpp"

SharedRef<Iterator<const Directory>> DirectoryCollection::Start() { return New<DirectoryIterator>(m_parent, std::filesystem::directory_iterator(m_parent.m_path)); }	
SharedRef<Iterator<const Directory>> DirectoryCollection::End()   { return New<DirectoryIterator>(m_parent, std::filesystem::directory_iterator());                }
									 
SharedRef<Iterator<const Directory>> DirectoryCollection::Start() const { return New<DirectoryIterator>(m_parent, std::filesystem::directory_iterator(m_parent.m_path)); }
SharedRef<Iterator<const Directory>> DirectoryCollection::End()   const { return New<DirectoryIterator>(m_parent, std::filesystem::directory_iterator());                }
