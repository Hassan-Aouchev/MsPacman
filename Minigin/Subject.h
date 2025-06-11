#pragma once
#include <vector>
#include <algorithm>
#include "Observer.h"

class Subject {
private:
	std::vector<Observer*> m_pObservers;
public:
	void AddObserver(Observer* pObserver) {
		m_pObservers.emplace_back(pObserver);
	}
	void removeObserver(Observer* pObserver) {
		m_pObservers.erase(std::remove(m_pObservers.begin(), m_pObservers.end(), pObserver), m_pObservers.end());
	}

	void NotifyObservers(GameObject* object, const std::string& event) {
		for (auto pObserver : m_pObservers)
		{
			pObserver->Notify(object,event);
		}
	}
};