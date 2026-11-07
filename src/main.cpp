#pragma once

#include "GameApp.h"
#include "../engine/main/EntryPoint.h"

Application* yamaneko_engine::CreateApplication() {
    return new GameApp();
}
