#include "ModSettingsViewController.hpp"
#include "main.hpp"

#include "HMUI/Touchable.hpp"
#include "bsml/shared/BSML-Lite.hpp"

using namespace BSML::Lite;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace HMUI;

void DidActivate(ViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if(firstActivation) {
        self->get_gameObject()->AddComponent<Touchable*>();
        GameObject* container = BSML::Lite::CreateScrollableSettingsContainer(
                static_cast<Component *>(self->get_transform()));
        Transform* parent = container->get_transform();
        auto layout = BSML::Lite::CreateVerticalLayoutGroup(parent);
        layout->set_spacing(0.4f);

        auto* toggle = BSML::Lite::CreateToggle(parent, "Enabled", getConfig().config["Enabled"].GetBool(), [](bool value) {
            getConfig().config["Enabled"].SetBool(value);
            getConfig().Write();
        });
    }
}