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
		Listener(const std::function<int()>& fn)
			:m_fn{ fn }
		{}
		void Notify() { m_fn(); }

		void SetFunction(std::function<int()>& fn) { m_fn = fn; }
	private:

		std::function<int()> m_fn;
	};

	class Event
	{
	public:
		//TODO: Event
		void AddListener(Listener* listener) { m_vpListeners.push_back(listener); }
		void RemoveListener(Listener* listener) { m_vpListeners.erase(std::remove(m_vpListeners.begin(), m_vpListeners.end(), listener), m_vpListeners.end()); }
		void FireEvent()
		{
			for (Listener* l : m_vpListeners)
			{
				l->Notify();
			}
		}

	private:
		std::vector<Listener*> m_vpListeners;
	};
}