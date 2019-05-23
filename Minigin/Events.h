#pragma once
#include <vector>
#include <algorithm>
#include <functional>

namespace dae
{
	class Listener
	{
		//TODO: Listener
	public:
		Listener()
			:m_fn{}
		{
			id = total;
			total++;
		}
		void Notify() { m_fn(); }

		void SetFunction(std::function<void()> fn)
		{
			m_fn = fn;
		}

		bool operator==(const Listener& other)
		{
			return id == other.id;
		}
	private:

		std::function<void()> m_fn;
		int id;
		static int total;
	};

	class Event
	{
	public:
		//TODO: Event
		void AddListener(Listener listener) { m_vpListeners.push_back(listener); }
		void RemoveListener(Listener listener) { m_vpListeners.erase(std::remove(m_vpListeners.begin(), m_vpListeners.end(), listener), m_vpListeners.end()); }
		void Invoke()
		{
			for (Listener l : m_vpListeners)
			{
				l.Notify();
			}
		}

	private:
		std::vector<Listener> m_vpListeners;
	};
}