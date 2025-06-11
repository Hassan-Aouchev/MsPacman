#include "Controller.h"
#include "Xinput.h"
#include "Windows.h"
#include <minwinbase.h>

class Controller::ControllerImpl final
{
private:
	unsigned int m_ButtonsPressed{};
	unsigned int m_ButtonsUp{};

	unsigned int m_ControllerId{};
	bool m_IsDisconnected{ false };

	XINPUT_STATE m_CurrentState{};
	XINPUT_STATE m_PreviousState{};
public:
	void Update() {
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		if (XInputGetState(m_ControllerId, &state) == ERROR_DEVICE_NOT_CONNECTED) {
			m_IsDisconnected = true;
			return;
		}
		m_CurrentState = state;

		auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
		m_ButtonsPressed = buttonChanges & m_CurrentState.Gamepad.wButtons;
		m_ButtonsUp = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
		m_PreviousState = m_CurrentState;
	}

	bool IsDisconnected() const { return m_IsDisconnected; }
	unsigned int GetId() const { return m_ControllerId; }

	bool IsDown(unsigned int button) const
	{
		return m_ButtonsPressed & button;
	}
	bool IsUp(unsigned int button) const
	{
		return m_ButtonsUp & button;
	}
	bool IsPressed(unsigned int button) const
	{
		return m_CurrentState.Gamepad.wButtons & button;
	}

	ControllerImpl(unsigned int controllerId) : m_ControllerId{ controllerId } {}
	~ControllerImpl() = default;
	ControllerImpl(const ControllerImpl& other) = delete;
	ControllerImpl(ControllerImpl&& other) = delete;
	ControllerImpl& operator=(const ControllerImpl& other) = delete;
	ControllerImpl& operator=(ControllerImpl&& other) = delete;
};

Controller::Controller(unsigned int controllerId): m_CntrlrImpl{std::make_unique<ControllerImpl>(controllerId)} {}

Controller::~Controller()
{
}

void Controller::Update()
{
	m_CntrlrImpl->Update();
}

unsigned int Controller::GetControllerId() const
{
	return m_CntrlrImpl->GetId();
}

bool Controller::IsDown(unsigned int button) const
{
	return m_CntrlrImpl->IsDown(button);
}

bool Controller::IsPressed(unsigned int button) const
{
	return m_CntrlrImpl->IsPressed(button);
}

bool Controller::IsUp(unsigned int button) const
{
	return m_CntrlrImpl->IsUp(button);
}

bool Controller::IsDisconnected() const
{
	return m_CntrlrImpl->IsDisconnected();
}
