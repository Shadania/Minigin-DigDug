#include "MiniginPCH.h"
#include "Events.h"

int dae::Listener::total{};


dae::Event::~Event()
{
	// m_vpListeners.clear();
}

void dae::Event::AddListener(std::shared_ptr<Listener> listener)
{
	m_vpListeners.push_back(listener);
	listener->m_pMyEvent = this;
}
void dae::Event::RemoveListener(size_t listenerId)
{
	m_vpListeners.erase(std::remove_if(m_vpListeners.begin(), m_vpListeners.end(), [listenerId](std::shared_ptr<Listener> listener) {return listener->getId() == listenerId; }), m_vpListeners.end());

}
void dae::Event::Invoke()
{
	for (auto l : m_vpListeners)
	{
		l->Notify();
	}
}