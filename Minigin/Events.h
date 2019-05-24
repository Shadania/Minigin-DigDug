#pragma once
#include <vector>
#include <algorithm>
#include <functional>

namespace dae
{
	class Listener;

	class Event
	{
	public:
		~Event();
		void AddListener(std::shared_ptr<Listener> listener);
		void RemoveListener(size_t listenerId);
		void Invoke();

	private:
		std::vector<std::shared_ptr<Listener>> m_vpListeners;
	};

	class Listener
	{
	public:
		Listener()
			:m_fn{}
		{
			id = total;
			total++;
		}

		void StopListening()
		{
			m_pMyEvent->RemoveListener(id);
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

		size_t getId() { return id; }
	private:
		friend class Event;
		std::function<void()> m_fn;
		Event* m_pMyEvent;
		int id;
		static int total;
	};

	
}