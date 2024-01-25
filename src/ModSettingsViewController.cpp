#include "ModSettingsViewController.hpp"

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

        BSML::Lite::CreateText(parent, "Mod Settings", true)->set_alignment(TMPro::TextAlignmentOptions::TopJustified);
        /*AddConfigValueToggle(parent, getModConfig().Enabled);
        AddConfigValueIncrementFloat(parent, getModConfig().Delay, 2, 0.01f, 0.01f, 1.0f);*/
    }
}