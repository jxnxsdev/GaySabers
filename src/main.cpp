#include "main.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "UnityEngine/Color.hpp"
#include "GlobalNamespace/SaberManager.hpp"
#include "GlobalNamespace/Saber.hpp"
#include "UnityEngine/Renderer.hpp"
#include "GlobalNamespace/SaberModelController.hpp"
#include "custom-types/shared/macros.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "sombrero/shared/ColorUtils.hpp"
#include "UnityEngine/WaitForSeconds.hpp"
#include "UnityEngine/Random.hpp"
#include "sombrero/shared/FastColor.hpp"
#include "GlobalNamespace/SaberType.hpp"
#include "GlobalNamespace/SaberModelContainer.hpp"
#include "GlobalNamespace/SetSaberGlowColor.hpp"
#include "GlobalNamespace/SetSaberFakeGlowColor.hpp"
#include "GlobalNamespace/ColorManager.hpp"
#include "GlobalNamespace/ColorScheme.hpp"
#include "GlobalNamespace/Parametric3SliceSpriteController.hpp"
#include "GlobalNamespace/MaterialPropertyBlockController.hpp"
#include "GlobalNamespace/MaterialPropertyBlockAnimator.hpp"
#include "GlobalNamespace/MaterialPropertyBlockColorSetter.hpp"
#include "GlobalNamespace/MaterialPropertyBlockFloatAnimator.hpp"
#include "GlobalNamespace/MaterialPropertyBlockVectorAnimator.hpp"
#include "UnityEngine/MaterialPropertyBlock.hpp"
#include "GlobalNamespace/SetSaberGlowColor_PropertyTintColorPair.hpp"
#include "questui/shared/QuestUI.hpp"
#include "ModSettingsViewController.hpp"
#include "ModConfig.hpp"


static ModInfo modInfo;

using namespace GlobalNamespace;

Configuration& getConfig() {
    static Configuration config(modInfo);
    return config;
}

Logger& getLogger() {
    static Logger* logger = new Logger(modInfo);
    return *logger;
}



// delay between saber color fade
float delay = 0.05;


Sombrero::FastColor lastColor = Sombrero::FastColor::HSVToRGB(0.0f, 1.0f, 1.0f);

Sombrero::FastColor getNextColor() {
    float h = 0;
    float s = 0;
    float v = 0;
    Sombrero::FastColor::RGBToHSV(lastColor, h, s, v);
    h += 0.01f;
    if (h > 1.0f) {
        h = 0.0f;
    }
    Sombrero::FastColor nextColor = Sombrero::FastColor::HSVToRGB(h, s, v);
    lastColor = nextColor;
    return nextColor;
}

custom_types::Helpers::Coroutine rainbowFader(GlobalNamespace::SaberModelController* controller, GlobalNamespace::Saber* saber)
{
    getLogger().info("rainbowFader start");

    while (true)
    {
        if(!getModConfig().Enabled.GetValue()) {
            co_yield reinterpret_cast<System::Collections::IEnumerator*>(UnityEngine::WaitForSeconds::New_ctor(1.0f));
            continue;
        }

        auto nextColor = getNextColor();

        for (int i = 0; i < controller->setSaberFakeGlowColors.Length(); i++) {
            auto color = controller->setSaberFakeGlowColors[i];
            color->parametric3SliceSprite->color = nextColor;
            color->parametric3SliceSprite->Refresh();
        }

        for (int i = 0; i < controller->setSaberGlowColors.Length(); i++) {
            auto color = controller->setSaberGlowColors[i];
            auto tinPairs = color->propertyTintColorPairs;

            for (int j = 0; j < tinPairs.Length(); j++) {
                GlobalNamespace::SetSaberGlowColor::PropertyTintColorPair* tinPair = tinPairs[j];
                color->materialPropertyBlock->SetColor(tinPair->property, nextColor);
            }
        }
        co_yield reinterpret_cast<System::Collections::IEnumerator*>(UnityEngine::WaitForSeconds::New_ctor(delay));
    }

    co_return;
}


MAKE_HOOK_MATCH(SaberModelController_init, &GlobalNamespace::SaberModelController::Init, void,  GlobalNamespace::SaberModelController* self, UnityEngine::Transform* parent, GlobalNamespace::Saber* saber) {
    SaberModelController_init(self, parent, saber);
    getLogger().info("SaberModelController_init");
    self->StartCoroutine(custom_types::Helpers::CoroutineHelper::New(rainbowFader(self, saber)));

}



extern "C" void setup(ModInfo& info) {
    info.id = MOD_ID;
    info.version = VERSION;
    modInfo = info;

    getConfig().Load();

    getModConfig().Init(modInfo);
    getLogger().info("Completed setup!");
}

extern "C" void load() {
    il2cpp_functions::Init();
    QuestUI::Init();

    QuestUI::Register::RegisterMainMenuModSettingsViewController(modInfo, DidActivate);

    getLogger().info("Installing hooks...");

    // Install the hook for SaberManager::Start
    INSTALL_HOOK(getLogger(), SaberModelController_init);
    getLogger().info("Installed all hooks!");
}
