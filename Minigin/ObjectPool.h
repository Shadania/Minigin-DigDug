#pragma once
#include <exception>
#include <string>

namespace dae
{
	class ObjectPoolException final : public std::exception
	{
	public:
		ObjectPoolException(const std::string& msg)
			:std::exception(msg.c_str())
		{}
	};

	template<typename T>
	class ObjectPool
	{
	public:

		ObjectPool(unsigned int size)
			:m_Size(size)
			, m_pPool{ new char[size*sizeof(T)]{} }
			, m_TakenBlocks{std::vector<bool>()}
		{
			for (size_t i{}; i < size; ++i)
			{
				m_TakenBlocks.push_back(false);
			}
		}
		~ObjectPool() {
			delete[] m_pPool;
		}



		T* RequestMemory()
		{
			for (size_t i{}; i < m_Size; ++i)
			{
				if (!m_TakenBlocks[(i)])
				{
					m_TakenBlocks[i] = true;
					memset(&m_pPool[(i * sizeof(T))], 0x00, sizeof(T));
					return (T*)(&m_pPool[(i * sizeof(T))]);
				}
			}

			throw ObjectPoolException("Not enough memory in Object Pool!");
		}
		
		void ReleaseMemory(T* ptr)
		{
			unsigned int offset{ ptr - m_pPool };
			offset /= sizeof(T);
			m_TakenBlocks[offset] = false;
		}


	private:
		char* m_pPool;
		unsigned int m_Size;
		std::vector<bool> m_TakenBlocks;
	};
}