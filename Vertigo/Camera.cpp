#include "Camera.h"
#include "FreeCamera.h"
#include "OrbitCamera.h"

namespace Camera
{
    CameraBase* g_camera = nullptr;
    FreeCamera g_freeCamera;
    OrbitCamera g_orbitCamera;
};