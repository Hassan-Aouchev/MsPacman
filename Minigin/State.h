#pragma once
#include <memory>

namespace dae
{

	template<typename Context>
	class State
	{
	public:
		virtual ~State() = default;

		virtual void Enter(Context* context) = 0;
		virtual void Exit(Context* context) = 0;
		virtual void Update(Context* context,float deltaTime) = 0;

		virtual void HandleInput(Context*) {}
		virtual void OnSuspend(Context* ) {}
		virtual void OnResume(Context* ) {}
	};
	template<typename Context>
	class Condition
	{
	public:
		Condition() = default;
		virtual ~Condition() = default;
		virtual bool Evaluate(Context* const context,float deltaTime) = 0;
	};
}

