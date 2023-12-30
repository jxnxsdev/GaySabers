#include "modules/SaberColorManager.hpp"
#include "modules/ColorGenerator.hpp"
#include "main.hpp"
#include "sombrero/shared/FastColor.hpp"
#include "GlobalNamespace/SetSaberGlowColor.hpp"
#include "GlobalNamespace/SetSaberFakeGlowColor.hpp"
#include "GlobalNamespace/ColorManager.hpp"
#include "GlobalNamespace/ColorScheme.hpp"
#include "GlobalNamespace/Parametric3SliceSpriteController.hpp"
#include "UnityEngine/MaterialPropertyBlock.hpp"
#include "GlobalNamespace/SetSaberGlowColor_PropertyTintColorPair.hpp"
#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/WaitForSeconds.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "GlobalNamespace/CoroutineHelpers.hpp"
#include "GlobalNamespace/SharedCoroutineStarter.hpp"
using namespace GlobalNamespace;

custom_types::Helpers::Coroutine rainbowFader(GlobalNamespace::SaberModelController* controller, GlobalNamespace::Saber* saber) {
    if (!getModConfig().Enabled.GetValue()) co_return;
    bool isValid = true;
    auto* colorGenerator = new GaySabers::ColorGenerator;
    getLogger().info("Starting Rainbow Fader");

    while(isValid) {
        if (!getModConfig().Enabled.GetValue()) isValid = false;
        if(saber == nullptr || controller == nullptr) {
            break;
        }

        getLogger().info("1");

        auto nextColor = colorGenerator->getNextColor();

        getLogger().info("2");

        auto setSaberGlowColor = controller->setSaberGlowColors;
        auto setSaberFakeGlowColor = controller->setSaberFakeGlowColors;

        getLogger().info("3");

        for (int i = 0; i < setSaberGlowColor->Length(); i++) {
            auto glowColor = controller->setSaberGlowColors[i];
            auto tintPais = glowColor->propertyTintColorPairs;

            for (int j = 0; j < tintPais.Length(); j++) {
                auto tintPair = tintPais->values[j];
                glowColor->materialPropertyBlock->SetColor(tintPair->property, nextColor);
            }
            glowColor->meshRenderer->SetPropertyBlock(glowColor->materialPropertyBlock);
        }

        for (int i = 0; i < setSaberFakeGlowColor->Length(); i++) {
            auto fakeGlowColor = controller->setSaberFakeGlowColors[i];
            fakeGlowColor->parametric3SliceSprite->color = nextColor;
            fakeGlowColor->parametric3SliceSprite->Refresh();
        }

        getLogger().info("4");

        co_yield reinterpret_cast<System::Collections::IEnumerator*>(UnityEngine::WaitForSeconds::New_ctor(getModConfig().Delay.GetValue() / 100.0f));
    }

    delete colorGenerator;
    co_return;
}

namespace GaySabers::SaberColorManager {
    void StartColorCoroutine(GlobalNamespace::SaberModelController* controller, GlobalNamespace::Saber* saber) {
        GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(
            custom_types::Helpers::CoroutineHelper::New(rainbowFader(controller, saber)
                ));
    }
}