#pragma once

#include "ModConfig.hpp"
#include "modules/ColorGenerator.hpp"

#include "GlobalNamespace/Saber.hpp"
#include "GlobalNamespace/SaberModelController.hpp"
#include "sombrero/shared/FastColor.hpp"
#include "custom-types/shared/coroutine.hpp"

namespace GaySabers::SaberColorManager {
    void StartColorCoroutine(GlobalNamespace::SaberModelController* controller, GlobalNamespace::Saber* saber);
}