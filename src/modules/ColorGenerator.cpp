#include "modules/ColorGenerator.hpp"

namespace GaySabers {
    Sombrero::FastColor ColorGenerator::lastColor = Sombrero::FastColor::HSVToRGB(0.0f, 1.0f, 1.0f);

    Sombrero::FastColor ColorGenerator::getNextColor() {
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
}