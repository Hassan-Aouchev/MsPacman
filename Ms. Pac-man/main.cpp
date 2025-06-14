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
#include "GhostComponent.h"

const int SCREEN_WIDTH = 448;
const int SCREEN_HEIGHT = 496;
const int BORDER_SIZE = 100;

void loadServices()
{
    std::unique_ptr<SDLAudioService> sdlAudioService = std::make_unique<SDLAudioService>();
    ServiceLocator::ProvideAudioService(std::move(sdlAudioService));

    InputManager::GetInstance().BindAction(SDLK_F2,
        std::make_unique<MuteCommand>(), ButtonState::Pressed);
}

static PlayerComponent* LoadPlayer(Scene& scene, GameObject* pLevel,bool useKeyboard,int playerIndex=-1)
{
    auto playerObject = std::make_unique<GameObject>();
    playerObject->SetParent(pLevel, false);

    auto playerSprite = std::make_unique<GameObject>();
    playerSprite->SetParent(playerObject.get(), false);
    playerSprite->SetLocalPosition(glm::vec2(-8, -8));
    auto* playerTexture = playerSprite->AddComponent<Texture2DComponent>();
    playerTexture->SetTexture("sprite_msPacman.png");
    playerTexture->SetScale(2.0f);
    playerSprite->AddComponent<SpriteComponent>(3, 4, 10.f);

    playerObject->AddComponent<MovementInputComponent>();
    playerObject->SetPosition(0, 0);

    auto* playerComponent = playerObject->AddComponent<PlayerComponent>(13, 23, 0);

    GameObject* playerPtr = playerObject.get();
    scene.Add(std::move(playerObject));
    scene.Add(std::move(playerSprite));
    if(useKeyboard)
    {
        InputManager::GetInstance().BindAction(SDLK_z,
            std::make_unique<Move>(playerPtr, glm::vec2(0, -1)), ButtonState::Down);
        InputManager::GetInstance().BindAction(SDLK_s,
            std::make_unique<Move>(playerPtr, glm::vec2(0, 1)), ButtonState::Down);
        InputManager::GetInstance().BindAction(SDLK_d,
            std::make_unique<Move>(playerPtr, glm::vec2(1, 0)), ButtonState::Down);
        InputManager::GetInstance().BindAction(SDLK_q,
            std::make_unique<Move>(playerPtr, glm::vec2(-1, 0)), ButtonState::Down);
    }
    if(playerIndex>=0)
    {
        InputManager::GetInstance().BindAction(ControllerButton::DPAD_UP, playerIndex,
            std::make_unique<Move>(playerPtr, glm::vec2(0, -1)), ButtonState::Down);
        InputManager::GetInstance().BindAction(ControllerButton::DPAD_DOWN, playerIndex,
            std::make_unique<Move>(playerPtr, glm::vec2(0, 1)), ButtonState::Down);
        InputManager::GetInstance().BindAction(ControllerButton::DPAD_RIGHT, playerIndex,
            std::make_unique<Move>(playerPtr, glm::vec2(1, 0)), ButtonState::Down);
        InputManager::GetInstance().BindAction(ControllerButton::DPAD_LEFT, playerIndex,
            std::make_unique<Move>(playerPtr, glm::vec2(-1, 0)), ButtonState::Down);
    }
    return playerComponent;
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
    auto font = ResourceManager::GetInstance().LoadFont("Emulogic-zrEw.ttf", 20);

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

    auto player1Component = LoadPlayer(scene,levelPtr,true,0);//also add a controller
    //auto player2Component = LoadPlayer(scene, levelPtr, false, 0);

    auto ghostRedObject = std::make_unique<GameObject>();

    auto redPtr = ghostRedObject.get();
    {
        ghostRedObject->SetParent(levelPtr, false);

        auto ghostSprite = std::make_unique<GameObject>();
        ghostSprite->SetParent(ghostRedObject.get(), false);
        ghostSprite->SetLocalPosition(glm::vec2(-8, -8));
        auto* playerTexture = ghostSprite->AddComponent<Texture2DComponent>();
        playerTexture->SetTexture("Sprite_RedGhost.png");
        playerTexture->SetScale(2.0f);
        ghostSprite->AddComponent<SpriteComponent>(2, 4, 10.f);

        ghostRedObject->AddComponent<MovementInputComponent>();
        ghostRedObject->SetPosition(0, 0);

        auto ghostComponent = ghostRedObject->AddComponent<GhostComponent>(10, 11,GhostType::Red);

        ghostComponent->SetPacman(player1Component->GetOwner());

        player1Component->GetSubject()->AddObserver(ghostComponent);

        scene.Add(std::move(ghostRedObject));
        scene.Add(std::move(ghostSprite));
    }

    auto ghostPinkObject = std::make_unique<GameObject>();
    {
        ghostPinkObject->SetParent(levelPtr, false);

        auto ghostSprite = std::make_unique<GameObject>();
        ghostSprite->SetParent(ghostPinkObject.get(), false);
        ghostSprite->SetLocalPosition(glm::vec2(-8, -8));
        auto* playerTexture = ghostSprite->AddComponent<Texture2DComponent>();
        playerTexture->SetTexture("Sprite_PinkGhost.png");
        playerTexture->SetScale(2.0f);
        ghostSprite->AddComponent<SpriteComponent>(2, 4, 10.f);

        ghostPinkObject->AddComponent<MovementInputComponent>();
        ghostPinkObject->SetPosition(0, 0);

        auto ghostComponent = ghostPinkObject->AddComponent<GhostComponent>(11, 11, GhostType::Pink);

        ghostComponent->SetPacman(player1Component->GetOwner());

        player1Component->GetSubject()->AddObserver(ghostComponent);

        scene.Add(std::move(ghostPinkObject));
        scene.Add(std::move(ghostSprite));
    }

    auto ghostCyanObject = std::make_unique<GameObject>();
    {
        ghostCyanObject->SetParent(levelPtr, false);

        auto ghostSprite = std::make_unique<GameObject>();
        ghostSprite->SetParent(ghostCyanObject.get(), false);
        ghostSprite->SetLocalPosition(glm::vec2(-8, -8));
        auto* playerTexture = ghostSprite->AddComponent<Texture2DComponent>();
        playerTexture->SetTexture("Sprite_CyanGhost.png");
        playerTexture->SetScale(2.0f);
        ghostSprite->AddComponent<SpriteComponent>(2, 4, 10.f);

        ghostCyanObject->AddComponent<MovementInputComponent>();
        ghostCyanObject->SetPosition(0, 0);

        auto ghostComponent = ghostCyanObject->AddComponent<GhostComponent>(16, 11, GhostType::Cyan);

        ghostComponent->SetPacman(player1Component->GetOwner());
        ghostComponent->SetBlinky(redPtr);

        player1Component->GetSubject()->AddObserver(ghostComponent);

        scene.Add(std::move(ghostCyanObject));
        scene.Add(std::move(ghostSprite));
    }

    auto ghostOrangeObject = std::make_unique<GameObject>();
    {
        ghostOrangeObject->SetParent(levelPtr, false);

        auto ghostSprite = std::make_unique<GameObject>();
        ghostSprite->SetParent(ghostOrangeObject.get(), false);
        ghostSprite->SetLocalPosition(glm::vec2(-8, -8));
        auto* playerTexture = ghostSprite->AddComponent<Texture2DComponent>();
        playerTexture->SetTexture("Sprite_OrangeGhost.png");
        playerTexture->SetScale(2.0f);
        ghostSprite->AddComponent<SpriteComponent>(2, 4, 10.f);

        ghostOrangeObject->AddComponent<MovementInputComponent>();
        ghostOrangeObject->SetPosition(0, 0);

        auto ghostComponent = ghostOrangeObject->AddComponent<GhostComponent>(17, 11, GhostType::Orange);

        ghostComponent->SetPacman(player1Component->GetOwner());

        player1Component->GetSubject()->AddObserver(ghostComponent);

        scene.Add(std::move(ghostOrangeObject));
        scene.Add(std::move(ghostSprite));
    }

    // HUD setup
    auto hudObject = std::make_unique<GameObject>();
    hudObject->SetPosition(20, 20);

    // Create HUD text elements
    auto player1LivesText = std::make_unique<GameObject>();
    player1LivesText->AddComponent<Text2DComponent>("1", font);
    player1LivesText->SetParent(hudObject.get(), false);
    player1LivesText->SetLocalPosition({ 0, 0 });
    scene.Add(std::move(player1LivesText));

    auto player1ScoreText = std::make_unique<GameObject>();
    player1ScoreText->AddComponent<Text2DComponent>("1", font);
    player1ScoreText->SetParent(hudObject.get(), false);
    player1ScoreText->SetLocalPosition({ 300, 0 });
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
    //player2Component->GetSubject()->AddObserver(hudComponent);
    scene.Add(std::move(hudObject));


}

int main(int, char* [])
{
    dae::Minigin engine("../Data/", SCREEN_WIDTH, SCREEN_HEIGHT, BORDER_SIZE);
    engine.Run(load);
    return 0;
}