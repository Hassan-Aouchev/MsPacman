#pragma once
#include "State.h"
#include <stack>
#include <memory>
#include <map>
#include <vector>

namespace dae
{
    template<typename Context, typename StateType = State<Context>, typename ConditionType = Condition<Context>>
    class StateManager
    {
    public:
        StateManager(Context* context, StateType* pStartState) : m_Context(context) { ChangeState(pStartState); }

        void AddTransition(StateType* startState,
            StateType* toState,
            ConditionType* condition)
        {
            StateType* startStatePtr = startState;

            if (m_States.find(startState) == m_States.end())
            {
                m_States[startState] = std::unique_ptr<StateType>(startState);
            }

            StateType* toStatePtr = toState;
            if (m_States.find(toState) == m_States.end())
            {
                m_States[toState] = std::unique_ptr<StateType>(toState);
            }

            if (m_Transitions.find(startStatePtr) == m_Transitions.end())
            {
                m_Transitions[startStatePtr] = Transitions();
            }
            m_Transitions[startStatePtr].push_back(std::make_pair(std::unique_ptr<ConditionType>(condition), toStatePtr));
        }

        void ChangeState(StateType* newState)
        {
            if (m_CurrentState)
                m_CurrentState->Exit(m_Context);

            m_CurrentState = newState;

            if (m_CurrentState)
                m_CurrentState->Enter(m_Context);
        }


        void PushState(StateType* state)
        {
            if (m_CurrentState)
            {
                m_CurrentState->OnSuspend(m_Context);
                m_StateStack.push(m_CurrentState);
            }

            m_CurrentState = state;

            if (m_CurrentState)
                m_CurrentState->Enter(m_Context);
        }

        void PopState()
        {
            if (m_CurrentState)
            {
                m_CurrentState->Exit(m_Context);
                delete m_CurrentState;
            }
                
            if (!m_StateStack.empty())
            {
                m_CurrentState = m_StateStack.top();
                m_StateStack.pop();
                if (m_CurrentState)
                    m_CurrentState->OnResume(m_Context);
            }
            else
            {
                m_CurrentState = nullptr;
            }
        }

        void Update(float deltaTime)
        {
            if (m_CurrentState)
            {
                auto it = m_Transitions.find(m_CurrentState);
                if (it != m_Transitions.end())
                {
                    for (const auto& transitionPair : it->second)
                    {
                        const std::unique_ptr<ConditionType>& pCondition = transitionPair.first;
                        StateType* pToState = transitionPair.second;

                        // Actually call Evaluate() method
                        if (pCondition && pCondition->Evaluate(m_Context,deltaTime))
                        {
                            ChangeState(pToState);
                            break;
                        }
                    }
                }
                if(m_Context)
                m_CurrentState->Update(m_Context, deltaTime);
            }
        }

        void HandleInput()
        {
            if (m_CurrentState)
                m_CurrentState->HandleInput(m_Context);
        }

        StateType* GetCurrentState() const { return m_CurrentState; }
        bool HasState() const { return m_CurrentState != nullptr; }
        size_t GetStackSize() const { return m_StateStack.size(); }

    private:
        typedef std::pair<std::unique_ptr<ConditionType>, StateType*> TransitionPair;
        typedef std::vector<TransitionPair> Transitions;

        Context* m_Context;
        StateType* m_CurrentState = nullptr;
        std::stack<StateType*> m_StateStack;
        std::map<StateType*, Transitions> m_Transitions;
        std::map<StateType*, std::unique_ptr<StateType>> m_States;
    };
}