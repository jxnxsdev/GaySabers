#include "main.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "GlobalNamespace/SaberManager.hpp"
#include "GlobalNamespace/Saber.hpp"
#include "GlobalNamespace/SaberModelController.hpp"
#include "questui/shared/QuestUI.hpp"
#include "ModSettingsViewController.hpp"
#include "ModConfig.hpp"
#include "modules/SaberColorManager.hpp"

#include "modules/FootStepColorManager.hpp"
#include "modules/MenuColorManager.hpp"

// Gay Menu
#include "GlobalNamespace/BloomPrePassBackgroundColor.hpp"
#include "GlobalNamespace/CampaignFlowCoordinator.hpp"
#include "GlobalNamespace/MultiplayerModeSelectionFlowCoordinator.hpp"
#include "GlobalNamespace/PartyFreePlayFlowCoordinator.hpp"
#include "GlobalNamespace/SoloFreePlayFlowCoordinator.hpp"

#include <map>

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

MAKE_HOOK_MATCH(SaberModelController_init, &GlobalNamespace::SaberModelController::Init, void,  GlobalNamespace::SaberModelController* self, UnityEngine::Transform* parent, GlobalNamespace::Saber* saber) {
    SaberModelController_init(self, parent, saber);
    getLogger().info("SaberModelController_init");
    GaySabers::SaberColorManager::StartColorCoroutine(self, saber);
}

/*MAKE_HOOK_MATCH(MainMenu, &GlobalNamespace::MainFlowCoordinator::DidActivate,
                void, GlobalNamespace::MainFlowCoordinator *self, bool a,
                bool b, bool c) {

    MainMenu(self, a, b, c);



    // new thread
    std::thread([self]() {
        GaySabers::FootStepColorManager::StartColorCoroutine(self);
    }).detach();

}*/

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
    //INSTALL_HOOK(getLogger(), MainMenu);
    getLogger().info("Installed all hooks!");
}
