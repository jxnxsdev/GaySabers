#pragma once

#include "sombrero/shared/FastColor.hpp"

namespace GaySabers {
    class ColorGenerator {
        public:
            static Sombrero::FastColor getNextColor();

        private:
            static Sombrero::FastColor lastColor;
    };
}