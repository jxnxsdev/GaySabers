#include "modules/FootStepColorManager.hpp"
#include "ModConfig.hpp"
#include "modules/ColorGenerator.hpp"
#include "main.hpp"

#include "custom-types/shared/coroutine.hpp"
#include "UnityEngine/WaitForSeconds.hpp"
#include "GlobalNamespace/MainFlowCoordinator.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/SpriteRenderer.hpp"
#include "questui/shared/CustomTypes/Components/MainThreadScheduler.hpp"

using namespace GlobalNamespace;

void rainbowFader() {
    if (!getModConfig().Enabled.GetValue()) return;
    auto colorGenerator = new GaySabers::ColorGenerator();
    bool isValid = true;

    getLogger().info("Starting Rainbow Fader for Feet");

    while(isValid) {
        if (!getModConfig().Enabled.GetValue()) isValid = false;
        auto nextColor = colorGenerator->getNextColor();

        QuestUI::MainThreadScheduler::Schedule([nextColor]() {
            UnityEngine::GameObject::Find("PlayersPlace")
                    ->GetComponentInChildren<UnityEngine::SpriteRenderer *>()
                    ->set_color(nextColor);
        });

        UnityEngine::WaitForSeconds::New_ctor(getModConfig().Delay.GetValue());
    }
}

namespace GaySabers::FootStepColorManager {
    void StartColorCoroutine(GlobalNamespace::MainFlowCoordinator* controller) {
        rainbowFader();
    }
}