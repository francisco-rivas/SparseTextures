// ------------------------------------------------------------
// Copyright(c) 2018-2022 Jesse Yurkovich
// Licensed under the MIT License <http://opensource.org/licenses/MIT>.
// See the LICENSE file in the repo root for full license information.
// ------------------------------------------------------------
#pragma once

#include <string>

struct EngineOptions
{
    int Width{ 1920 };
    int Height{ 1080 };

    bool AutoRotate{};
    bool EnableMaterials{};
    bool EnableIBL{};
    bool EnableGround{};
    bool WireFrame{};

    float CameraX{ 0 };
    float CameraY{ 0 };
    float CameraZ{ 8.0f };

    std::string ModelPath{};

    bool EnableMSAA = false;
    UINT MSAASamples = 1;
};
