#pragma once

#include <memory>

#include "Chotra/Events/event.h"

namespace Chotra {

    class Application {
    public:
        Application();
        virtual ~Application();

        Application(const Application&) = delete;
        Application(Application&&) = delete;

        Application& operator=(const Application&) = delete;
        Application& operator=(Application&&) = delete;

        virtual int Start();
        virtual void OnUpdate(float deltaTime);

    private:
        std::unique_ptr<class Window> mainWindow;

        EventDispatcher eventDispatcher;
        bool closeMainWindow = false;

        float currentTime = 0.0f;
        float deltaTime = 0.0f;
        float lastFrame = 0.0f;
          

    };


} // namspace Chotra
