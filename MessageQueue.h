// MessageQueue.h (c) Copyright 2020 Piotr Macha <me@piotrmacha.pl>
// This code is licensed under MIT license

#pragma once

#include <array>
#include <functional>

template <typename T, size_t Size>
class MessageQueueView
{
public:
	MessageQueueView(std::array<T, Size>* bufferPtr, size_t* indexPtr, size_t* cyclesPtr) :
		m_BufferPtr(bufferPtr),
		m_IndexPtr(indexPtr),
		m_CyclesPtr(cyclesPtr),
		m_ViewIndex(*indexPtr),
		m_Cycles(*cyclesPtr)
	{};

	T* Next()
	{
		if (m_ViewIndex == Size) {
			m_ViewIndex = 0;
			m_Cycles++;
		}

		if (m_ViewIndex != *m_IndexPtr || m_Cycles != *m_CyclesPtr)
		{
			auto ptr = &m_BufferPtr->at(m_ViewIndex);
			m_ViewIndex++;
			return ptr;
		}

		return nullptr;
	}

	void ForEach(const std::function<void(T*)>& consumer)
	{
		auto message = Next();
		while (message)
		{
			consumer(message);
			message = Next();
		}
	}

private:
	std::array<T, Size>* m_BufferPtr;
	size_t* m_IndexPtr;
	size_t* m_CyclesPtr;
	size_t m_ViewIndex;
	size_t m_Cycles;
};

template <typename T, size_t Size>
class MessageQueue
{
public:
	void Publish(T&& message)
	{
		m_Buffer[m_Index] = std::move(message);
		m_Index++;
		if (m_Index == Size)
		{
			m_Index = 0;
			m_Cycles++;
		}
	}

	MessageQueueView<T, Size> CreateView()
	{
		return MessageQueueView<T, Size>(&m_Buffer, &m_Index, &m_Cycles);
	}
	
private:
	std::array<T, Size> m_Buffer;
	size_t m_Index = 0;
	size_t m_Cycles = 0;
};