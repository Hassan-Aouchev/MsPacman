#pragma once
#include <memory>

class Controller final
{
public:
	Controller(unsigned int controllerId);
	~Controller();

	Controller(const Controller& other) = delete;
	Controller(Controller&& other) = delete;
	Controller& operator=(const Controller& other) = default;
	Controller& operator=(Controller&& other) = default;

	void Update();

	unsigned int GetControllerId()const;

	bool IsDown(unsigned int button) const;
	bool IsPressed(unsigned int button) const;
	bool IsUp(unsigned int button) const;
	bool IsDisconnected() const;

private:
	class ControllerImpl;
	std::unique_ptr<ControllerImpl> m_CntrlrImpl;

};

