#include "modules/MenuColorManager.hpp"
#include "ModConfig.hpp"
#include "modules/ColorGenerator.hpp"
#include "main.hpp"

#include "UnityEngine/WaitForSeconds.hpp"
#include "GlobalNamespace/MainFlowCoordinator.hpp"

#include "GlobalNamespace/BloomPrePassBackgroundColor.hpp"
#include "GlobalNamespace/CampaignFlowCoordinator.hpp"
#include "GlobalNamespace/MultiplayerModeSelectionFlowCoordinator.hpp"
#include "GlobalNamespace/PartyFreePlayFlowCoordinator.hpp"
#include "GlobalNamespace/SoloFreePlayFlowCoordinator.hpp"
#include "GlobalNamespace/MenuLightsPresetSO.hpp"
#include "GlobalNamespace/SimpleColorSO.hpp"
#include "GlobalNamespace/MenuLightsManager.hpp"
#include "GlobalNamespace/MenuLightsPresetSO_LightIdColorPair.hpp"

#include "UnityEngine/ScriptableObject.hpp"
#include "UnityEngine/Object.hpp"
#include "questui/shared/CustomTypes/Components/MainThreadScheduler.hpp"

#include <map>

using namespace GlobalNamespace;

MenuLightsPresetSO *defaultLights = nullptr;

GlobalNamespace::ColorSO *createColorSO(UnityEngine::Color color) {
    float t = color.r + color.g + color.b;
    auto so = UnityEngine::ScriptableObject::CreateInstance<
            GlobalNamespace::SimpleColorSO *>();
    so->color = color;
    return so;
}

GlobalNamespace::MenuLightsPresetSO *createMenuLights(UnityEngine::Color color) {
    float t = color.r + color.g + color.b;
    auto colorSO = createColorSO(color);
    auto menuPresetSO = UnityEngine::Object::Instantiate(defaultLights);
    auto colorPairs = menuPresetSO->lightIdColorPairs;
    for (int i = 0; i < colorPairs.Length(); ++i) {
        auto pair = GlobalNamespace::MenuLightsPresetSO::LightIdColorPair::
        New_ctor();
        pair->lightId = colorPairs[i]->lightId;
        pair->baseColor = colorSO;
        pair->intensity = colorPairs[i]->intensity;
        colorPairs[i] = pair;
    }
    return menuPresetSO;
}

void rainbowFader(GlobalNamespace::MainFlowCoordinator* self) {
    if (!getModConfig().Enabled.GetValue()) return;
    auto colorGenerator = new GaySabers::ColorGenerator();
    bool isValid = true;

    getLogger().info("Starting Rainbow Fader for Menu");

    if(!defaultLights) {
        defaultLights = self->menuLightsManager->defaultPreset;
    }

    auto mfc = self->menuLightsManager;

    auto soloFreeplay = self->soloFreePlayFlowCoordinator;
    auto partyFreeplay = self->partyFreePlayFlowCoordinator;
    auto campaignCoordinator = self->campaignFlowCoordinator;

    while(isValid) {
        if (!getModConfig().Enabled.GetValue()) isValid = false;
        auto nextColor = colorGenerator->getNextColor();

        QuestUI::MainThreadScheduler::Schedule([nextColor, self, mfc]() {
            self->defaultLightsPreset = createMenuLights(nextColor);
            mfc->SetColorPreset(self->defaultLightsPreset, true);
        });

        UnityEngine::WaitForSeconds::New_ctor(getModConfig().Delay.GetValue());
    }
}

namespace GaySabers::MenuColorManager {
    void StartColorCoroutine(GlobalNamespace::MainFlowCoordinator* controller) {
        rainbowFader(controller);
    }
}