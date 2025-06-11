#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "GameObject.h"

class Observer
{
public: 
	virtual ~Observer() = default;
	virtual void Notify(GameObject* gameObject, const std::string& event) = 0;
};