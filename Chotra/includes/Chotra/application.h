#pragma once

#include <memory>

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
        virtual void OnUpdate();

    private:
        std::unique_ptr<class Window> window;
    };


}