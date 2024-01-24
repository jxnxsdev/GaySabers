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
#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/WaitForSeconds.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "GlobalNamespace/CoroutineHelpers.hpp"
using namespace GlobalNamespace;

custom_types::Helpers::Coroutine rainbowFader(GlobalNamespace::SaberModelController* controller, GlobalNamespace::Saber* saber) {
    bool isValid = true;
    auto* colorGenerator = new GaySabers::ColorGenerator;
    getLogger().info("Starting Rainbow Fader");

    while(isValid) {
        if(saber == nullptr || controller == nullptr) {
            break;
        }
        auto nextColor = colorGenerator->getNextColor();

        auto setSaberGlowColor = controller->____setSaberGlowColors;
        auto setSaberFakeGlowColor = controller->____setSaberFakeGlowColors;

        for (int i = 0; i < setSaberGlowColor->get_Length(); i++) {
            auto glowColor = controller->____setSaberGlowColors[i];
            auto tintPais = glowColor->ptr()->_propertyTintColorPairs;

            for (int j = 0; j < tintPais->get_Length(); j++) {
                auto tintPair = tintPais->values[j];
                glowColor->ptr()->_materialPropertyBlock->SetColor(tintPair->property, nextColor);
            }
            glowColor->ptr()->_meshRenderer->SetPropertyBlock(glowColor->ptr()->_materialPropertyBlock);
        }

        for (int i = 0; i < setSaberFakeGlowColor->get_Length(); i++) {
            auto fakeGlowColor = controller->____setSaberFakeGlowColors[i];
            fakeGlowColor->ptr()->_parametric3SliceSprite->color = nextColor;
            fakeGlowColor->ptr()->_parametric3SliceSprite->Refresh();
        }

        co_yield reinterpret_cast<System::Collections::IEnumerator*>(UnityEngine::WaitForSeconds::New_ctor(0.01f));
    }

    delete colorGenerator;
    co_return;
}

namespace GaySabers::SaberColorManager {
    void StartColorCoroutine(GlobalNamespace::SaberModelController* controller, GlobalNamespace::Saber* saber) {
        controller->StartCoroutine(custom_types::Helpers::CoroutineHelper::New(rainbowFader(controller, saber)));
    }
}