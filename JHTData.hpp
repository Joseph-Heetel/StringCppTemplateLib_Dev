#pragma once
/*

*/

#ifndef JHT_ENUMERATOR_MAX_SIZE
#define JHT_ENUMERATOR_MAX_SIZE 32
#endif
#ifndef JHT_ENUMERATOR_ALIGN
#define JHT_ENUMERATOR_ALIGN 8
#endif

#include "JHTBasics.hpp"
#include <stdio.h>

namespace jht
{
	using index_t = int32_t;

	template<typename T>
	class IndexEnumerator;
	template<typename T>
	class ConstIndexEnumerator;
	template<typename T>
	class IContainer;
	template<typename T>
	class IArrayContainer;
	template<typename T>
	class Vector;
	template<typename T, int32_t A>
	class AVector;
	template<typename T>
	class DynamicBuffer;

#pragma region Producer & Consumer Patterns

	template<typename _TItem>
	class Producer
	{
	public:
		using TItem = _TItem;
		bool GetNext(_TItem& item);
	};

	template<typename _TItem>
	class Consumer
	{
	public:
		using TItem = _TItem;
		bool PushNext(const _TItem& item);
	};

	template<class TProducer, class TConsumer>
	void PumpAll(TProducer& producer, TConsumer& consumer);

	template<typename TItem, size_t BUFFERSIZE>
	class BufferedProducer : public Producer<TItem>
	{
	protected:
		TItem m_Buffer[BUFFERSIZE];

		TItem* m_WritePos;
		TItem* m_ReadPos;

		size_t BufferedCount() const;

		size_t SpaceRemaining() const { return BUFFERSIZE - 1 - BufferedCount(); }

		bool PushBack(const TItem& item);

		bool PopFront(TItem& item);

		virtual void RetrieveNext() = 0;
	public:
		BufferedProducer() : m_Buffer{}, m_ReadPos(m_Buffer), m_WritePos(m_Buffer){}

		bool GetNext(TItem& item);
	};

	template<class TContainer>
	class StandardContainerProducer : public Producer<typename TContainer::value_type>
	{
		TContainer::iterator m_Iter;
		TContainer::iterator m_End;
	public:
		inline StandardContainerProducer(TContainer& container) : m_Iter(container.begin()), m_End(container.end()) {}

		inline bool GetNext(typename TContainer::value_type& item);
	};

	template<class TContainer>
	class StandardContainerConsumer : public Consumer<typename TContainer::value_type>
	{
		TContainer& m_Container;
	public:
		inline StandardContainerConsumer(TContainer& container) : m_Container(container) {}

		inline bool PushNext(typename const TContainer::value_type& item) { m_Container.push_back(item); return true; }
	};

	template<class TStream, typename TItem = TStream::_Myios::char_type>
	class StandardStreamConsumer : public Consumer<TItem>
	{
		TStream& m_Stream;
	public:
		inline StandardStreamConsumer(TStream& container) : m_Stream(container) {}

		inline bool PushNext(const TItem& item) { bool good = m_Stream.good(); if (good) { m_Stream << item; } return good; }
	};

	template<typename TItem, class TItemOrig, bool dynamicCast = false>
	class CastProducer : public Producer<TItem>
	{
		Producer<TItemOrig> m_Producer;
	public:
		CastProducer(const Producer<TItemOrig>& producer) : m_Producer(producer) {}

		bool GetNext(TItem& item);
	};

	template<typename TItem>
	class DelegateConsumer : public Consumer<TItem>
	{
	public:
		using TDelegate = std::function<bool(const TItem&)>;
	private:
		TDelegate m_Delegate;
	public:
		inline DelegateConsumer(TDelegate del) : m_Delegate(del) {}

		inline bool PushNext(const TItem& item) { return m_Delegate(item); }
	};

#endif
#pragma endregion
#pragma region Enumerator Class Definitions

	/**
	* @class IEnumerator interface class
	* @brief base interface shared by all enumerator types
	*/
	template<typename t>
	class IEnumerator
	{
	public:
		using TItem = t;

		virtual t& operator->() const = 0;
		virtual t& operator()() const = 0;
		virtual IEnumerator<t>& operator++() = 0;
		virtual IEnumerator<t>& operator--() = 0;
		virtual bool Valid() const = 0;

		virtual ~IEnumerator() {};
	};

#define JHT_ENUMERATE(type, iter, in) for(type::TEnumerator iter = in.GetEnumerator(); iter.Valid(); iter++)
#define JHT_CONSTENUMERATE(type, iter, in) for(type::TConstEnumerator iter = in.GetConstEnumerator(); iter.Valid(); iter++)

	template<typename TContainer>
	class IndexEnumerator : public IEnumerator<typename TContainer::TItem>
	{
	private:
		using Base = IEnumerator<typename TContainer::TItem>;
	public:
		TContainer& Container;
		index_t Index;

		IndexEnumerator(TContainer& container, index_t index) : Container(container), Index(index) {}

		virtual typename TContainer::TItem& operator->() const override;
		virtual typename TContainer::TItem& operator()() const override;
		virtual IndexEnumerator<TContainer>& operator++() override;
		virtual IndexEnumerator<TContainer>& operator--() override;
		virtual bool Valid() const override
		{
			return Container->IndexValid(Index);
		}
		void operator+=(index_t offset);
		void operator-=(index_t offset);
	};

#pragma endregion
#pragma region Container Interface Class Definition

	template<typename T>
	class IContainer
	{
	public:
		virtual GetEnumerator() = 0;
		virtual int32_t Count() const = 0;
		virtual int32_t Capacity() const = 0;
		virtual bool IsEmpty() const { return Count() == 0; }
	};

	template<typename T>
	class IArrayContainer : public IContainer<T>
	{
	public:
		virtual const T* Data() const = 0;
		virtual T* Data() = 0;
		virtual T const& operator[](index_t index) const = 0;
		virtual T& operator[](index_t index) = 0;
		virtual bool IndexValid(index_t index) const = 0;
	};

#pragma endregion
#pragma region Buffer Class Definition

	template<typename T>
	class DynamicBuffer : public IArrayContainer<T>
	{
	public:
		using TItem = T;
		using TConstEnumerator = IndexEnumerator<const DynamicBuffer<T>>;
		using TEnumerator = IndexEnumerator<DynamicBuffer<T>>;
	protected:
		T* _Data;
		int32_t _Capacity;

		DynamicBuffer(T* data, int32_t capacity) : _Data(data), _Capacity(capacity) {}
		void CopyFrom(const IContainer<T>& other);
	public:
		DynamicBuffer() : _Data(nullptr), _Capacity(0) {}
		explicit DynamicBuffer(int32_t capacity) : _Data(new T[capacity]), _Capacity(capacity) {}
		explicit DynamicBuffer(const IContainer<T>& other);
		virtual void operator=(const IContainer<T>& other);

		virtual void Resize(int32_t capacity);
		void CleanUp();
		virtual ~DynamicBuffer();

		// Abstract functions from IArrayContainer

		virtual const T* Data() const override { return _Data; }
		virtual T* Data() override { return _Data; }
		virtual T const& operator[](index_t index) const override { return _Data[index]; }
		virtual T& operator[](index_t index) override { return _Data[index]; }
		virtual bool IndexValid(index_t index) const override { return index >= 0 && index < _Capacity; }

		virtual IndexEnumerator<DynamicBuffer<T>> GetEnumerator() override { return IndexEnumerator<DynamicBuffer<T>>(*this, 0); }
		virtual IndexEnumerator<const DynamicBuffer<T>> GetConstEnumerator() const override { return IndexEnumerator<const DynamicBuffer<T>>(*this, 0); }
		virtual int32_t Count() const override { return _Capacity; }
		virtual int32_t Capacity() const override { return _Capacity; }

	};

#pragma endregion
#pragma region Vector Class Definitions

	/// <summary>
	/// Simple efficient vector class template
	/// </summary>
	/// <typeparam name="T">Values</typeparam>
	template<typename T>
	class Vector : public DynamicBuffer<T>
	{
	protected:
		int32_t _Count;

		Vector(T* data, int32_t capacity, int32_t count) : DynamicBuffer<T>(data, capacity), _Count(count) {}
	public:

		Vector() : DynamicBuffer<T>(), _Count(0) {}
		Vector(int32_t capacity) : DynamicBuffer<T>(capacity), _Count(0) {}
		Vector(const IContainer<T>& other) : DynamicBuffer<T>(other), _Count(other.Count()) {}

		virtual void operator=(const IContainer<T>& other);

		Vector(const Vector<T>& other) = delete;
		Vector(const Vector<T>&& other) = delete;
		void operator=(const Vector<T>& other) { operator=(dynamic_cast<const IContainer<T>&>(other)); }

		virtual ~Vector() override;

		virtual void Add(const T& item);
		virtual void AddRange(const T* items, int32_t count);
		virtual void AddRange(const Vector<T>& items);
		virtual void Remove(index_t index);
		virtual void Insert(index_t index, const T& item);
		virtual void Replace(index_t index, const T& item);

		void CleanUp();

		// Abstract functions from IArrayContainer

		virtual bool IndexValid(index_t index) const override { return index >= 0 && index < _Count; }

		// Abstract functions from IContainer

		virtual int32_t Count() const override { return _Count; }

	protected:
		virtual	void EnlargeStandard();
	};

	template<typename T, int32_t A>
	class AVector : public Vector<T>
	{
		T _Array[A];

	public:

		AVector() : Vector<T>(_Array, A, 0), _Array{}
		{}

		AVector(const Vector<T>& source);

		void CleanUp();

		virtual ~AVector() override;

		virtual void operator=(const IContainer<T>& other) override;
		virtual void Resize(int32_t capacity) override;
	};

#pragma endregion
#pragma region Queue Class Definition

	template<typename T>
	class Queue : public DynamicBuffer<T>
	{
	protected:
		index_t InIndex;
		index_t OutIndex;

	public:
		Queue() : DynamicBuffer<T>(), InIndex(0), OutIndex(0) {}

		int32_t Count() const;
		bool Empty() const;

	protected:
		index_t IndexDistance(index_t from, index_t to) const;
	};

#pragma endregion
}