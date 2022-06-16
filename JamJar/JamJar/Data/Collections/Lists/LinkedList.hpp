#pragma once

#include "../List.hpp"


template<typename T>
class LinkedList : public IList<T>
{
private:
	class Node
	{
	private:
		T m_value;

		NullableWeakRef<Node> m_prev;
		NullableRef<Node>     m_next;
	public:
		template<typename... Args>
		Node(Args&&... args) requires ConstructibleFrom<T, Args...> : m_value(args...), m_prev(nullptr), m_next(nullptr) {}

		Node(const T& value, const NullableRef<Node>& prev = nullptr, const NullableRef<Node>& next = nullptr) : m_value(value), m_prev(prev), m_next(next) {}

		      T& GetValue()       { return m_value; }
		const T& GetValue() const { return m_value; }

		NullableRef<Node> GetPrev() const { return m_prev; }
		NullableRef<Node> GetNext() const { return m_next; }

		void SetPrev(NullableRef<Node> prev) { m_prev = prev; }
		void SetNext(NullableRef<Node> next) { m_next = next; }
	};

	class NodeIterator : public Iterator<T>
	{
	private:
		NullableRef<Node> m_current;
	public:
		NodeIterator(const NullableRef<Node>& node) : m_current(node) {}

		virtual void MoveNext() { m_current = m_current->GetNext(); }

		virtual T& Current() const { return m_current->GetValue(); }

		virtual Boolean Equals(const Iterator<T>& other) const { return m_current == ((const NodeIterator&)other).m_current; }
	};

	class ConstNodeIterator : public Iterator<const T>
	{
	private:
		NullableRef<Node> m_current;
	public:
		ConstNodeIterator(const NullableRef<Node>& node) : m_current(node) {}

		virtual void MoveNext() { m_current = m_current->GetNext(); }

		virtual const T& Current() const { return m_current->GetValue(); }

		virtual Boolean Equals(const Iterator<const T>& other) const { return m_current == ((const ConstNodeIterator&)other).m_current; }
	};

	NullableRef<Node> m_first;
	NullableRef<Node> m_last;

	Size m_count;

	NullableRef<Node> GetNode(Size index)
	{
		NullableRef<Node> result;
		if(index < m_count / 2U)
		{
			result = m_first;
			for(Size i = 0U; i < index; i++)
				result = result->GetNext();
		}
		else
		{
			result = m_last;
			for(Size i = m_count - 1U; i > index; i--)
				result = result->GetPrev();
		}

		return result;
	}
public:
	LinkedList() : m_first(nullptr), m_last(nullptr), m_count(0U) {}

	virtual Size Count() const override { return m_count; }

	virtual       T& operator[](Size index)       override { return GetNode(index)->GetValue(); }
	virtual const T& operator[](Size index) const override { return GetNode(index)->GetValue(); }

	virtual void Add     (const T& item)               override;
	virtual void AddRange(const ICollection<T>& items) override;

	virtual void Insert     (Size index, const T& item)               override;
	virtual void InsertRange(Size index, const ICollection<T>& items) override;

	virtual void RemoveAt   (Size index)             override;
	virtual void RemoveRange(Size index, Size count) override;

	virtual void Clear() override;

	SInt64 IndexOf(const T& item) const requires Equatable<T>
	{
		Size i = 0U;
		for(NullableRef<Node> curr = m_first; curr != nullptr; curr = curr->GetNext(), i++)
		{
			if(curr->GetValue() == item)
				return SInt64(i);
		}

		return -1;
	}

	virtual Boolean Remove(const T& item) override;

	ArrayRef<T> ToArray() const requires CopyConstructible<T>
	{
		T* array = (T*)malloc(sizeof(T) * m_count.ToRawValue());

		for(Size i = 0U; i < m_count; i++)
			new(array + i.ToRawValue()) T((*this)[i]);

		return ArrayRef<T>(array, m_count);
	}

	virtual SharedRef<Iterator<T>> Start() override { return New<NodeIterator>(m_first); }
	virtual SharedRef<Iterator<T>> End()   override { return New<NodeIterator>(nullptr); }

	virtual SharedRef<Iterator<const T>> Start() const override { return New<ConstNodeIterator>(m_first); }
	virtual SharedRef<Iterator<const T>> End()   const override { return New<ConstNodeIterator>(nullptr); }
};

template<typename T>
inline void LinkedList<T>::Add(const T& item)
{
	NullableRef<Node> newNode = New<Node>(item, m_last);

	if(m_count == 0U)
		m_first = newNode;
	else
		m_last->SetNext(newNode);

	m_last = newNode;
	++m_count;
}

template<typename T>
inline void LinkedList<T>::AddRange(const ICollection<T>& items)
{
	NullableRef<Node> first = nullptr;

	NullableRef<Node> last = nullptr;
	for(const T& item : items)
	{
		NullableRef<Node> curr = New<Node>(item, last);

		if(last != nullptr)
			last->SetNext(curr);
		else
			first = curr;

		last = curr;
	}

	if(m_last == nullptr)
		m_first = first;
	else
		m_last->SetNext(first);

	m_last = last;

	m_count += items.Count();
}

template<typename T>
inline void LinkedList<T>::Insert(Size index, const T& item)
{
	NullableRef<Node> next = GetNode(index);
	NullableRef<Node> prev = next->GetPrev();

	NullableRef<Node> newNode = New<Node>(item, prev, next);

	if(prev != nullptr)
		prev->SetNext(newNode);
	else
		m_first = newNode;

	if(next != nullptr)
		next->SetPrev(newNode);
	else
		m_last = newNode;

	++m_count;
}

template<typename T>
inline void LinkedList<T>::InsertRange(Size index, const ICollection<T>& items)
{
	if(items.Count() == 0U)
		return;

	NullableRef<Node> next = GetNode(index);
	NullableRef<Node> prev = next->GetPrev();

	NullableRef<Node> curr = nullptr;
	for(const T& item : items)
	{
		curr = New<Node>(item, prev);
		if(prev != nullptr)
			prev->SetNext(curr);
		else
			m_first = curr;

		prev = curr;
	}

	if(next != nullptr)
		next->SetPrev(prev);
	else
		m_last = prev;

	m_count += items.Count();
}

template<typename T>
inline void LinkedList<T>::RemoveAt(Size index)
{
	NullableRef<Node> foundNode = GetNode(index);

	NullableRef<Node> prev = foundNode->GetPrev();
	NullableRef<Node> next = foundNode->GetNext();

	if(prev != nullptr)
		prev->SetNext(next);
	else
		m_first = next;

	if(next != nullptr)
		next->SetPrev(prev);
	else
		m_last = prev;

	--m_count;
}

template<typename T>
inline void LinkedList<T>::RemoveRange(Size index, Size count)
{
	if(count == 0U)
		return;

	NullableRef<Node> foundNode = GetNode(index);

	NullableRef<Node> prev = foundNode->GetPrev();

	NullableRef<Node> next = foundNode;
	for(Size i = 0U; i < count; i++)
		next = next->GetNext();

	if(prev != nullptr)
		prev->SetNext(next);
	else
		m_first = next;

	if(next != nullptr)
		next->SetPrev(prev);
	else
		m_last = prev;

	m_count -= count;
}

template<typename T>
inline void LinkedList<T>::Clear()
{
	m_first = nullptr;
	m_last  = nullptr;
}

template<typename T>
inline Boolean LinkedList<T>::Remove(const T& item)
{
	for(NullableRef<Node> curr = m_first; curr != nullptr; curr = curr->GetNext(), i++)
	{
		if(curr->GetValue() == item)
		{
			NullableRef<Node> prev = curr->GetPrev();
			NullableRef<Node> next = curr->GetNext();

			if(prev != nullptr)
				prev->SetNext(next);
			else
				m_first = next;

			if(next != nullptr)
				next->SetPrev(prev);
			else
				m_last = prev;

			--m_count;
			return true;
		}
	}

	return false;
}
