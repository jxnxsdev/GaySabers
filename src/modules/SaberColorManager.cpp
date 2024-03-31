#include "modules/SaberColorManager.hpp"
#include "modules/ColorGenerator.hpp"
#include "main.hpp"
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
#include "bsml/shared/BSML/SharedCoroutineStarter.hpp"
using namespace GlobalNamespace;

custom_types::Helpers::Coroutine rainbowFader(GlobalNamespace::SaberModelController* controller, GlobalNamespace::Saber* saber) {
    if(!getConfig().config["Enabled"].GetBool()) {
        co_return;
    }

    bool isValid = true;
    auto* colorGenerator = new GaySabers::ColorGenerator;
    logger.info("Starting Rainbow Fader");

    while(isValid) {
        if(!getConfig().config["Enabled"].GetBool()) {
            isValid = false;
        }

        if(saber == nullptr || controller == nullptr) {
            delete colorGenerator;
            co_return;
        }

        UnityW<Saber> unitySaber = saber;
        UnityW<SaberModelController> unityController = controller;

        if(!unitySaber.isAlive() || !unityController.isAlive()) {
            delete colorGenerator;
            co_return;
        }

        auto nextColor = colorGenerator->getNextColor();

        auto setSaberGlowColor = controller->____setSaberGlowColors;
        auto setSaberFakeGlowColor = controller->____setSaberFakeGlowColors;

        for (int i = 0; i < setSaberGlowColor->get_Length(); i++) {
            auto glowColor = controller->____setSaberGlowColors[i];
            auto tintPais = glowColor->_propertyTintColorPairs;

            for (int j = 0; j < tintPais->get_Length(); j++) {
                auto tintPair = tintPais->_values[j];
                glowColor->_materialPropertyBlock->SetColor(tintPair->property, nextColor);
            }
            glowColor->_meshRenderer->SetPropertyBlock(glowColor->_materialPropertyBlock);
        }

        for (int i = 0; i < setSaberFakeGlowColor->get_Length(); i++) {
            auto fakeGlowColor = controller->____setSaberFakeGlowColors[i];
            fakeGlowColor->_parametric3SliceSprite->color = nextColor;
            fakeGlowColor->_parametric3SliceSprite->Refresh();
        }

        co_yield reinterpret_cast<System::Collections::IEnumerator*>(UnityEngine::WaitForSeconds::New_ctor(0.01f));
    }

    delete colorGenerator;
    co_return;
}

namespace GaySabers::SaberColorManager {
    void StartColorCoroutine(GlobalNamespace::SaberModelController* controller, GlobalNamespace::Saber* saber) {
        logger.info("Starting shared coroutine");
        BSML::SharedCoroutineStarter::StartCoroutine(rainbowFader(controller, saber));
    }
}