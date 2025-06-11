#pragma once
#include "Singleton.h"
#include <vector>
#include <memory>
#include "Controller.h"
#include <SDL.h>
#include <unordered_set>
#include "Command.h"

enum class ButtonState {
	Down,
	Up,
	Pressed
};
enum class ControllerButton {
	DPAD_UP = 0x0001,
	DPAD_DOWN = 0x0002,
	DPAD_LEFT = 0x0004,
	DPAD_RIGHT = 0x0008,
	BUTTON_DOWN = 0x1000,
	BUTTON_RIGHT = 0x2000,
	BUTTON_LEFT = 0x4000,
	BUTTON_UP = 0x8000
};

struct KeyboardState final
{
	KeyboardState(const SDL_Keycode key, const ButtonState buttonState, std::unique_ptr<Command>&&pCommand) :
		key{key},
		buttonState{buttonState},
		pCommand{std::move(pCommand)}
	{}
	~KeyboardState() = default;
	SDL_Keycode key;
	ButtonState buttonState;
	std::unique_ptr<Command> pCommand;

	KeyboardState(const KeyboardState&) = delete;
	KeyboardState& operator=(const KeyboardState&) = delete;
	KeyboardState(KeyboardState&&) noexcept = default;
	KeyboardState& operator=(KeyboardState&&) noexcept = default;
};

struct ControllerState final
{
	ControllerState(const ControllerButton button,unsigned int controllerId, const ButtonState buttonState, std::unique_ptr<Command>&&pCommand):
		button{button},
		controllerId{controllerId},
		buttonState{buttonState},
		pCommand{std::move(pCommand)}
	{}
	~ControllerState() = default;
	ControllerButton button;
	unsigned int controllerId;
	ButtonState buttonState;
	std::unique_ptr<Command> pCommand;

	ControllerState(const ControllerState&) = delete;
	ControllerState& operator=(const ControllerState&) = delete;
	ControllerState(ControllerState&&) noexcept = default;
	ControllerState& operator=(ControllerState&&) noexcept = default;
};

class InputManager final : public Singleton<InputManager>
{
	const unsigned int MAX_CONTROLLERS{2};
	std::vector<std::unique_ptr<Controller>> m_pControllers{};

	std::vector<KeyboardState> m_KeyboardState{};
	std::vector<ControllerState> m_ControllerState{};

	std::unordered_set<SDL_Keycode> m_keysPressedThisFrame;
	std::unordered_set<SDL_Keycode> m_keysReleasedThisFrame;
	std::unordered_set<SDL_Keycode> m_keysPressedLastFrame;
	std::unordered_set<SDL_Keycode> m_keysCurrentlyPressed;


public:

	Command* BindAction(SDL_Keycode key, std::unique_ptr<Command>&& pCommand, ButtonState state);
	Command* BindAction(ControllerButton button, unsigned int controllerId, std::unique_ptr<Command>&& pCommand, ButtonState state);

	void HandleController();
	void HandleKeyboard();

	Controller* GetController(unsigned int controllerId);

	void Unbind(Command* pCommand);

	bool ProcessInput();
};
