#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "GameObject.h"
#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "Text2DComponent.h"
#include "Texture2DComponent.h"
#include "InputManager.h"
#include "MovementInputComponent.h"
#include "PlayerComponent.h"
#include "LevelComponent.h"
#include "HUDComponent.h"
#include "SpriteComponent.h"
#include <SDLAudioService.h>
#include "ServiceLocator.h"
#include "PacmanCommands.h"

const int SCREEN_WIDTH = 448;
const int SCREEN_HEIGHT = 496;
const int BORDER_SIZE = 100;

void loadServices()
{
    std::unique_ptr<SDLAudioService> sdlAudioService = std::make_unique<SDLAudioService>();
    ServiceLocator::ProvideAudioService(std::move(sdlAudioService));
}

void load()
{
    loadServices();

    auto& scene = SceneManager::GetInstance().CreateScene("Demo");

    // Background
    auto background = std::make_unique<GameObject>();
    background->AddComponent<Texture2DComponent>()->SetTexture("background.tga");
    scene.Add(std::move(background));

    // Logo
    auto logo = std::make_unique<GameObject>();
    logo->AddComponent<Texture2DComponent>()->SetTexture("logo.tga");
    logo->SetPosition(216, 180);
    scene.Add(std::move(logo));

    // Instructions text
    auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

    auto instructionsP1 = std::make_unique<GameObject>();
    instructionsP1->AddComponent<Text2DComponent>("player1: X for taking lives and A for adding score.", font);
    instructionsP1->SetPosition(20, 20);
    scene.Add(std::move(instructionsP1));

    auto instructionsP2 = std::make_unique<GameObject>();
    instructionsP2->AddComponent<Text2DComponent>("player2: x for taking lives and c for adding score.", font);
    instructionsP2->SetPosition(20, 50);
    scene.Add(std::move(instructionsP2));

    // Level
    auto levelObject = std::make_unique<GameObject>();
    auto* levelSprite = levelObject->AddComponent<Texture2DComponent>();
    levelSprite->SetTexture("LevelSprite.png");
    levelSprite->SetScale(2.0f);
    levelSprite->SetSprite(1, 6);
    levelObject->SetPosition(BORDER_SIZE, BORDER_SIZE);
    levelObject->AddComponent<LevelComponent>(static_cast<float>(SCREEN_WIDTH),
        static_cast<float>(SCREEN_HEIGHT),
        "dot_1.png");
    GameObject* levelPtr = levelObject.get();
    scene.Add(std::move(levelObject));

    // Player 1
    auto player1Object = std::make_unique<GameObject>();
    player1Object->SetParent(levelPtr, false);
    auto* player1Texture = player1Object->AddComponent<Texture2DComponent>();
    player1Texture->SetTexture("Untitled-1.png");
    player1Texture->SetScale(2.0f);
    player1Object->AddComponent<MovementInputComponent>();
    player1Object->SetPosition(250, 250);
    auto* player1Component = player1Object->AddComponent<PlayerComponent>(14, 16, 8, ServiceLocator::GetAudioService());
    GameObject* player1Ptr = player1Object.get();
    scene.Add(std::move(player1Object));

    // Player 1 input bindings
    InputManager::GetInstance().BindAction(ControllerButton::DPAD_UP, 0,
        std::make_unique<Move>(player1Ptr, glm::vec2(0, -1)), ButtonState::Down);
    InputManager::GetInstance().BindAction(ControllerButton::DPAD_DOWN, 0,
        std::make_unique<Move>(player1Ptr, glm::vec2(0, 1)), ButtonState::Down);
    InputManager::GetInstance().BindAction(ControllerButton::DPAD_RIGHT, 0,
        std::make_unique<Move>(player1Ptr, glm::vec2(1, 0)), ButtonState::Down);
    InputManager::GetInstance().BindAction(ControllerButton::DPAD_LEFT, 0,
        std::make_unique<Move>(player1Ptr, glm::vec2(-1, 0)), ButtonState::Down);
    InputManager::GetInstance().BindAction(ControllerButton::BUTTON_LEFT, 0,
        std::make_unique<Damage>(player1Ptr), ButtonState::Pressed);
    InputManager::GetInstance().BindAction(ControllerButton::BUTTON_DOWN, 0,
        std::make_unique<Score>(player1Ptr), ButtonState::Pressed);

    // Player 2
    auto player2Object = std::make_unique<GameObject>();
    player2Object->SetParent(levelPtr, false);

    auto player2Sprite = std::make_unique<GameObject>();
    player2Sprite->SetParent(player2Object.get(), false);
    player2Sprite->SetLocalPosition(glm::vec2(-8, -8));
    auto* player2Texture = player2Sprite->AddComponent<Texture2DComponent>();
    player2Texture->SetTexture("sprite_msPacman.png");
    player2Texture->SetScale(2.0f);
    player2Sprite->AddComponent<SpriteComponent>(3, 4, 10.f);

    player2Object->AddComponent<MovementInputComponent>();
    player2Object->SetPosition(0, 0);
    auto* player2Component = player2Object->AddComponent<PlayerComponent>(1, 1, 0, ServiceLocator::GetAudioService());

    GameObject* player2Ptr = player2Object.get();
    scene.Add(std::move(player2Object));
    scene.Add(std::move(player2Sprite));

    // Player 2 input bindings
    InputManager::GetInstance().BindAction(SDLK_z,
        std::make_unique<Move>(player2Ptr, glm::vec2(0, -1)), ButtonState::Down);
    InputManager::GetInstance().BindAction(SDLK_s,
        std::make_unique<Move>(player2Ptr, glm::vec2(0, 1)), ButtonState::Down);
    InputManager::GetInstance().BindAction(SDLK_d,
        std::make_unique<Move>(player2Ptr, glm::vec2(1, 0)), ButtonState::Down);
    InputManager::GetInstance().BindAction(SDLK_q,
        std::make_unique<Move>(player2Ptr, glm::vec2(-1, 0)), ButtonState::Down);
    InputManager::GetInstance().BindAction(SDLK_x,
        std::make_unique<Damage>(player2Ptr), ButtonState::Pressed);
    InputManager::GetInstance().BindAction(SDLK_c,
        std::make_unique<Score>(player2Ptr), ButtonState::Pressed);

    // HUD setup
    auto hudObject = std::make_unique<GameObject>();
    hudObject->SetPosition(20, 80);

    // Create HUD text elements
    auto player1LivesText = std::make_unique<GameObject>();
    player1LivesText->AddComponent<Text2DComponent>("1", font);
    player1LivesText->SetParent(hudObject.get(), false);
    player1LivesText->SetLocalPosition({ 0, 0 });
    scene.Add(std::move(player1LivesText));

    auto player1ScoreText = std::make_unique<GameObject>();
    player1ScoreText->AddComponent<Text2DComponent>("1", font);
    player1ScoreText->SetParent(hudObject.get(), false);
    player1ScoreText->SetLocalPosition({ 0, 30 });
    scene.Add(std::move(player1ScoreText));

    auto player2LivesText = std::make_unique<GameObject>();
    player2LivesText->AddComponent<Text2DComponent>("2", font);
    player2LivesText->SetParent(hudObject.get(), false);
    player2LivesText->SetLocalPosition({ 0, 60 });
    scene.Add(std::move(player2LivesText));

    auto player2ScoreText = std::make_unique<GameObject>();
    player2ScoreText->AddComponent<Text2DComponent>("2", font);
    player2ScoreText->SetParent(hudObject.get(), false);
    player2ScoreText->SetLocalPosition({ 0, 90 });
    scene.Add(std::move(player2ScoreText));

    // Add HUD component and connect observers
    auto* hudComponent = hudObject->AddComponent<HUDComponent>();
    player1Component->GetSubject()->AddObserver(hudComponent);
    player2Component->GetSubject()->AddObserver(hudComponent);
    scene.Add(std::move(hudObject));

}

int main(int, char* [])
{
    dae::Minigin engine("../Data/", SCREEN_WIDTH, SCREEN_HEIGHT, BORDER_SIZE);
    engine.Run(load);
    return 0;
}