
#include "InputManager.h"
#include <backends/imgui_impl_sdl2.h>
#include <iostream>


bool InputManager::ProcessInput()
{


	m_keysPressedThisFrame.clear();
	m_keysReleasedThisFrame.clear();
	m_keysPressedLastFrame = m_keysCurrentlyPressed;
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		ImGui_ImplSDL2_ProcessEvent(&e);
		if (e.type == SDL_KEYDOWN) {
			m_keysPressedThisFrame.insert(e.key.keysym.sym);
			m_keysCurrentlyPressed.insert(e.key.keysym.sym);
		}

		if (e.type == SDL_KEYUP) {
			m_keysReleasedThisFrame.insert(e.key.keysym.sym);
			m_keysCurrentlyPressed.erase(e.key.keysym.sym);
		}
		
	}

	for (const auto& Controller : m_pControllers)
	{
		Controller->Update();
	}

	HandleController();

	HandleKeyboard();

	return true;
}


void InputManager::HandleController()
{
	for (const ControllerState& controllerState : m_ControllerState)
	{
		const auto& pController{ GetController(controllerState.controllerId) };

		if (pController == nullptr) continue;

		if (pController->IsDisconnected()) continue;

		switch (controllerState.buttonState)
		{
		case ButtonState::Down:

			if (!pController->IsPressed(static_cast<unsigned int>(controllerState.button))) continue;

			break;
		case ButtonState::Up:

			if (!pController->IsUp(static_cast<unsigned int>(controllerState.button))) continue;
			break;

		case ButtonState::Pressed:

			if (!pController->IsDown(static_cast<unsigned int>(controllerState.button))) continue;
			break;

		}

		controllerState.pCommand->Execute();

	}
}
void InputManager::HandleKeyboard()
{
	const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);

	for (const KeyboardState& kbState : m_KeyboardState)
	{
		SDL_Scancode scanCode = SDL_GetScancodeFromKey(kbState.key);

		switch (kbState.buttonState)
		{
		case ButtonState::Down:
			if (!keyboardState[scanCode]) continue;
			break;
		case ButtonState::Up:
			if (m_keysReleasedThisFrame.find(kbState.key) == m_keysReleasedThisFrame.end()) continue;
			break;
		case ButtonState::Pressed:
			if (m_keysPressedThisFrame.find(kbState.key) == m_keysPressedThisFrame.end()) continue;
			break;
		}

		kbState.pCommand->Execute();
	}
}

Command* InputManager::BindAction(SDL_Keycode key, std::unique_ptr<Command>&& pCommand, ButtonState state)
{
	Command* rCommand = pCommand.get();

	m_KeyboardState.emplace_back(KeyboardState(key, state, std::move(pCommand)));

	return rCommand;
}
Command* InputManager::BindAction(ControllerButton button,unsigned int controllerId, std::unique_ptr<Command>&& pCommand, ButtonState state)
{
	if (controllerId >= MAX_CONTROLLERS) return nullptr;
	if (GetController(controllerId) == nullptr)
	{
		m_pControllers.emplace_back(std::make_unique<Controller>(controllerId));
	}

	Command* rCommand = pCommand.get();

	m_ControllerState.emplace_back(ControllerState(button, controllerId,  state, std::move(pCommand)));
	return rCommand;
}
Controller* InputManager::GetController(unsigned int controllerId)
{
	for (const auto& pController : m_pControllers)
	{
		if (pController->GetControllerId() == controllerId) return pController.get();
	}
	return nullptr;
}

void InputManager::Unbind(Command* pCommand)
{
	auto keyboardIt = std::remove_if(m_KeyboardState.begin(), m_KeyboardState.end(),
		[pCommand](const KeyboardState& state) {
			return state.pCommand.get() == pCommand;
		}
	);
	m_KeyboardState.erase(keyboardIt, m_KeyboardState.end());

	auto controllerIt = std::remove_if(m_ControllerState.begin(), m_ControllerState.end(),
		[pCommand](const ControllerState& state) {
			return state.pCommand.get() == pCommand;
		}
	);
	m_ControllerState.erase(controllerIt, m_ControllerState.end());
}

