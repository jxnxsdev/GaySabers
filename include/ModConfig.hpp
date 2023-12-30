#pragma once
#include "config-utils/shared/config-utils.hpp"

DECLARE_CONFIG(ModConfig,
               CONFIG_VALUE(Enabled, bool, "Enabled", true);
                  CONFIG_VALUE(Delay, float, "Delay", 0.05f);
                       CONFIG_INIT_FUNCTION(
                               CONFIG_INIT_VALUE(Enabled);
                                 CONFIG_INIT_VALUE(Delay);

               )

               )