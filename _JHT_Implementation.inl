#pragma once
#include "JHTBasics.hpp"
#include "JHTData.hpp"


namespace jht
{
#pragma region JHTString.hpp
#pragma endregion
#pragma region JHTData.hpp
#pragma region Ptr Function Definitions

	template<typename TContainer>
	typename TContainer::TItem& IndexEnumerator<TContainer>::operator->() const
	{
		return Container->operator[](Index);
	}

	template<typename TContainer>
	typename TContainer::TItem& IndexEnumerator<TContainer>::operator()() const
	{
		return Container->operator[](Index);
	}

	template<typename TContainer>
	inline IndexEnumerator<TContainer>& IndexEnumerator<TContainer>::operator++()
	{
		Index++;
		return *this;
	}

	template<typename TContainer>
	inline IndexEnumerator<TContainer>& IndexEnumerator<TContainer>::operator--()
	{
		Index--;
		return *this;
	}

	template<typename TContainer>
	inline void IndexEnumerator<TContainer>::operator+=(index_t offset)
	{
		Index += offset;
	}

	template<typename TContainer>
	inline void IndexEnumerator<TContainer>::operator-=(index_t offset)
	{
		Index -= offset;
	}

#pragma endregion
#pragma region Buffer Function Definitions

	template<typename T>
	inline void DynamicBuffer<T>::CopyFrom(const IContainer<T>& other)
	{
		const IArrayContainer<T>* arrayContainer = dynamic_cast<const IArrayContainer<T>*>(&other);
		if (arrayContainer)
		{
			ArrCopy(_Data, arrayContainer->Data(), other.Count());
		}
		else
		{
			index_t index = 0;
			for (ConstEnumerator<T> enumerator = other.GetConstEnumerator(); enumerator.Valid(); ++enumerator)
			{
				_Data[index] = enumerator();
				index++;
			}
		}
	}

	template<typename T>
	inline DynamicBuffer<T>::DynamicBuffer(const IContainer<T>& other)
		: _Data(other.IsEmpty() ? nullptr : new T[other.Count()]), _Capacity(other.Count())
	{
		CopyFrom(other);
	}

	template<typename T>
	inline void DynamicBuffer<T>::operator=(const IContainer<T>& other)
	{
		if (other.Count() > _Capacity)
		{
			Resize(other.Count());
		}
		CopyFrom(other);
	}

	template<typename T>
	inline void DynamicBuffer<T>::Resize(int32_t capacity)
	{
		if (_Capacity == capacity)
		{
			return;
		}
		T* newData = new T[capacity];
		size_t copySize = MIN(_Capacity, capacity);
		ArrCopy<T>(newData, _Data, copySize);
		delete _Data;
		_Data = newData;
		_Capacity = capacity;
	}

	template<typename T>
	inline void DynamicBuffer<T>::CleanUp()
	{
		delete _Data;
		_Data = nullptr;
		_Capacity = 0;
	}

	template<typename T>
	inline DynamicBuffer<T>::~DynamicBuffer()
	{
		CleanUp();
	}

#pragma endregion
#pragma region Vector Function Definitions

	template<typename T>
	inline Vector<T>::~Vector()
	{
		CleanUp();
	}

	template<typename T>
	inline void Vector<T>::operator=(const IContainer<T>& other)
	{
		DynamicBuffer<T>::operator=(other);
	}

	template<typename T>
	inline void Vector<T>::Add(const T& item)
	{
		if (DynamicBuffer<T>::_Capacity <= _Count)
		{
			EnlargeStandard();
		}
		DynamicBuffer<T>::_Data[_Count] = item;
		_Count++;
	}

	template<typename T>
	inline void Vector<T>::AddRange(const T* items, int32_t count)
	{
		if (DynamicBuffer<T>::_Capacity < _Count + count)
		{
			DynamicBuffer<T>::Resize(_Count + count);
		}
		for (int32_t i = 0; i < count; i++)
		{
			DynamicBuffer<T>::_Data[_Count] = items[i];
			_Count++;
		}
	}

	template<typename T>
	inline void Vector<T>::AddRange(const Vector<T>& items)
	{
		AddRange(items._Data, items._Count);
	}

	template<typename T>
	inline void Vector<T>::Remove(index_t index)
	{
#ifdef DEBUG
		JHT_ASSERT(index < _Count&& index >= 0, u8"Index out of range!");
#endif
		_Count--;
		for (index_t i = index; i < _Count; i++)
		{
			DynamicBuffer<T>::_Data[i] = DynamicBuffer<T>::_Data[i + 1];
		}
	}

	template<typename T>
	inline void Vector<T>::Insert(index_t index, const T& item)
	{
#ifdef DEBUG
		JHT_ASSERT(index <= _Count && index >= 0, JHT_U8STR"Vector Indexing", L"Index out of range!");
#endif
		if (DynamicBuffer<T>::_Capacity <= _Count)
		{
			EnlargeStandard();
		}
		_Count++;
		for (index_t i = _Count; i > index; i--)
		{
			DynamicBuffer<T>::_Data[i] = DynamicBuffer<T>::_Data[i - 1];
		}
		DynamicBuffer<T>::_Data[index] = item;
	}

	template<typename T>
	inline void Vector<T>::Replace(index_t index, const T& item)
	{
#ifdef DEBUG
		TECT_ASSERT_CAT(index <= _Count && index >= 0, L"Vector Indexing", L"Index out of range!");
#endif
		DynamicBuffer<T>::_Data[index] = item;
	}

	template<typename T>
	inline void Vector<T>::CleanUp()
	{
		_Count = 0;
	}

	template<typename T>
	inline void Vector<T>::EnlargeStandard()
	{
		if (DynamicBuffer<T>::_Capacity == 0)
		{
			DynamicBuffer<T>::_Capacity = 8;
			DynamicBuffer<T>::_Data = new T[DynamicBuffer<T>::_Capacity];
			return;
		}
		int32_t newSize = DynamicBuffer<T>::_Capacity + MIN(DynamicBuffer<T>::_Capacity / 2, 1024LL);
		DynamicBuffer<T>::Resize(newSize);
	}

	template<typename T, int32_t A>
	inline jht::AVector<T, A>::AVector(const Vector<T>& source)
		: Vector<T>(), _Array{}
	{
		if (source.Count() <= A)
		{
			ArrCopy<T>(_Array, source.Data(), source.Count());
			DynamicBuffer<T>::_Capacity = A;
			DynamicBuffer<T>::_Data = _Array;
		}
		else
		{
			Vector<T>::operator=(source);
		}
	}

	template<typename T, int32_t A>
	inline void jht::AVector<T, A>::operator=(const IContainer<T>& source)
	{
		if (source.Count() <= A) // Reuse existing buffer
		{
			if (DynamicBuffer<T>::_Data != _Array)
			{
				delete DynamicBuffer<T>::_Data;
			}
			DynamicBuffer<T>::_Capacity = A;
			DynamicBuffer<T>::_Data = _Array;
		}
		Vector<T>::operator=(source);
	}

	template<typename T, int32_t A>
	inline void AVector<T, A>::CleanUp()
	{
		if (DynamicBuffer<T>::_Data == _Array)
		{
			DynamicBuffer<T>::_Data = nullptr;
		}
	}

	template<typename T, int32_t A>
	inline AVector<T, A>::~AVector()
	{
		CleanUp();
	}

	template<typename T, int32_t A>
	inline void AVector<T, A>::Resize(int32_t capacity)
	{
		if (DynamicBuffer<T>::_Capacity == capacity)
		{
			return;
		}
		if (capacity > A)
		{
			if (DynamicBuffer<T>::_Data != _Array)
			{
				return DynamicBuffer<T>::Resize(capacity);
			}
			T* newData = new T[capacity];
			size_t copySize = MIN(DynamicBuffer<T>::_Capacity, capacity);
			ArrCopy<T>(newData, DynamicBuffer<T>::_Data, copySize);
			DynamicBuffer<T>::_Data = newData;
			DynamicBuffer<T>::_Capacity = capacity;
		}
		else
		{
			if (DynamicBuffer<T>::_Data == _Array)
			{
				return;
			}
			T* newData = _Array;
			size_t copySize = MIN(DynamicBuffer<T>::_Capacity, capacity);
			ArrCopy<T>(newData, DynamicBuffer<T>::_Data, copySize);
			delete DynamicBuffer<T>::_Data;
			DynamicBuffer<T>::_Data = newData;
			DynamicBuffer<T>::_Capacity = capacity;
		}
	}


#pragma endregion

	template<typename T>
	inline int32_t Queue<T>::Count() const
	{
		return IndexDistance(OutIndex, InIndex);
	}

	template<typename T>
	inline bool Queue<T>::Empty() const
	{
		return IndexDistance(OutIndex, InIndex) == 0;
	}

	template<typename T>
	inline index_t Queue<T>::IndexDistance(index_t from, index_t to) const
	{
		index_t capacity = DynamicBuffer<T>::_Capacity;
		if (to >= from)
		{
			return to - from;
		}
		else
		{
			return capacity - from + to;
		}
	}
#pragma endregion
}