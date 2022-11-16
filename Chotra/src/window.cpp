
#include "window.h"

namespace Chotra {

    static bool GLFW_initialized = false;

    Window::Window(std::string title, unsigned int width, unsigned int height)
        : windowData({ title, width, height }) {

        int resultCode = Init();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplOpenGL3_Init();
        ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
    }

    Window::~Window() {

        Shutdown();
    }

    int Window::Init() {
        /* Initialize GLFW */
        if (!GLFW_initialized) {
            if (!glfwInit()) {
                std::cout << "Failed to initialize GLFW" << std::endl;
                return -1;
            }
            glfwWindowHint(GLFW_SAMPLES, 4);
            GLFW_initialized = true;
        }

        /* Create a windowed mode window and its OpenGL context */
        glfwWindow = glfwCreateWindow(windowData.width, windowData.height, windowData.title.c_str(), NULL, NULL);
        
        if (!glfwWindow) {
            glfwTerminate();
            std::cout << "Failed to create the window" << std::endl;
            return -2;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(glfwWindow);

        // glad: load all OpenGL function pointers
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return -3;
        }

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_MULTISAMPLE);

        glfwSetWindowUserPointer(glfwWindow, &windowData);

        glfwSetWindowSizeCallback(glfwWindow,
            [](GLFWwindow* window, int width, int height) {

                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
                data.width = width;
                data.height = height;
               
                WindowResizeEvent event(width, height);
                data.eventCallbackFn(event);
            }
        );

        glfwSetWindowCloseCallback(glfwWindow,
            [](GLFWwindow* window) {

                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

                WindowCloseEvent event;
                data.eventCallbackFn(event);
            }
        );

        glfwSetKeyCallback(glfwWindow,
            [](GLFWwindow* window, int key, int scancode, int action, int mods) {

                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
                if (action == GLFW_PRESS) {
                    KeyPressedEvent event(key, scancode, action, mods);
                    data.eventCallbackFn(event);

                } else if ((action == GLFW_RELEASE)) {
                    KeyReleasedEvent event(key, scancode, action, mods);
                    data.eventCallbackFn(event);
                }

            }
        );

        glfwSetMouseButtonCallback(glfwWindow,
            [](GLFWwindow* window, int button, int action, int mods) {

                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
                if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
                    MouseRightButtonPressedEvent event(button, action, mods);
                    data.eventCallbackFn(event);

                } else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
                    MouseRightButtonReleasedEvent event(button, action, mods);
                    data.eventCallbackFn(event);
                }

            }
        );

        glfwSetCursorPosCallback(glfwWindow,
            [](GLFWwindow* window, double newX, double newY) {

                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

                MouseMovedEvent event(newX, newY);
                data.eventCallbackFn(event);
            }
        );

        // tell GLFW to captur our mouse
        //glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        camera = std::make_unique<Camera>(glm::vec3(0.0f, 5.0f, 25.0f));
        scene = std::make_unique<Scene>();
        background = std::make_unique<Background>(*scene);
        renderer = std::make_unique<Renderer>(windowData.width, windowData.height, *camera, *scene, *background);
        renderer->Init(glfwWindow);

        lastMousePosition = glm::vec2(GetWidth() / 2, GetHeight() / 2);

        return 0;
    }

    void Window::Shutdown() {

        glfwDestroyWindow(glfwWindow);
        glfwTerminate();
    }

    void Window::OnUpdate(float deltaTime) {

        camera->ProcessKeyboard(deltaTime);
        //scene->DemoUpdate(deltaTime);
        scene->Update(deltaTime);
        renderer->Render();

        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize.x = static_cast<float>(GetWidth());
        io.DisplaySize.y = static_cast<float>(GetHeight());

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //ImGui::ShowDemoWindow();

        ImGui::SetNextWindowPos(ImVec2(GetWidth() - 350, 0));
        ImGui::SetNextWindowSize(ImVec2(350, GetHeight()));

         ImGui::Begin("Rendering configuration");

        if (ImGui::CollapsingHeader("Background")) {
            ImGui::ColorPicker4("Color", renderer->backgroundColor);
            ImGui::Checkbox("Draw skybox", &renderer->drawSkybox);
            ImGui::SliderFloat("Background exposure", &renderer->backgroundExposure, 0.0f, 10.0f);
        }

        if (ImGui::CollapsingHeader("Camera settings")) {
            ImGui::SliderFloat("Speed", &camera->MovementSpeed, 3.0f, 30.0f);
            ImGui::SliderFloat("Zoom", &camera->Zoom, 15.0f, 90.0f);
            //ImGui::Checkbox("Perspective projection", &renderer->perspectiveProjection);

        }

        if (ImGui::CollapsingHeader("Point lights")) {
            
            if (ImGui::Button("Light 0", ImVec2(0.f, 20.0f))) {
                tab = 0;
            }
            ImGui::SameLine();
            if (ImGui::Button("Light 1", ImVec2(0.f, 20.0f))) {
                tab = 1;
            }
            ImGui::SameLine();
            if (ImGui::Button("Light 2", ImVec2(0.f, 20.0f))) {
                tab = 2;
            }
            ImGui::SameLine();
            if (ImGui::Button("Light 3", ImVec2(0.f, 20.0f))) {
                tab = 3;
            }
            if (ImGui::Button("Light 4", ImVec2(0.f, 20.0f))) {
                tab = 4;
            }
            ImGui::SameLine();
            if (ImGui::Button("Light 5", ImVec2(0.f, 20.0f))) {
                tab = 5;
            }
            ImGui::SameLine();
            if (ImGui::Button("Light 6", ImVec2(0.f, 20.0f))) {
                tab = 6;
            }
            ImGui::SameLine();
            
            ImGui::Text(("Light " + std::to_string(tab)).c_str());
            ImGui::InputFloat("x", &scene->sceneLights[tab].position.x, 0.5f, 1.0f, "%.1f");
            ImGui::InputFloat("y", &scene->sceneLights[tab].position.y, 0.5f, 1.0f, "%.1f");
            ImGui::InputFloat("z", &scene->sceneLights[tab].position.z, 0.5f, 1.0f, "%.1f");
            ImGui::SliderFloat("r", &scene->sceneLights[tab].color.r, 0.005f, 1.0f);
            ImGui::SliderFloat("g", &scene->sceneLights[tab].color.g, 0.005f, 1.0f);
            ImGui::SliderFloat("b", &scene->sceneLights[tab].color.b, 0.005f, 1.0f);
            ImGui::SliderInt("Brightness", &scene->sceneLights[tab].brightness, 0, 10000);
        }

        if (ImGui::CollapsingHeader("Post effects")) {
            ImGui::SliderFloat("Exposure", &renderer->exposure, 0.0f, 10.0f);
            ImGui::Checkbox("Bloom", &renderer->bloom);
            ImGui::Checkbox("Gamma correction", &renderer->gammaCorrection);
        }

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(glfwWindow);
        glfwPollEvents();
    }

    void Window::SetEventCallbackFn(const EventCallbackFn& callback) {

        windowData.eventCallbackFn = callback;
    }

    void Window::SetPlayerMode(bool playerMode) {
        if (this->playerMode = playerMode) {
            glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        } else {
            glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }

    bool Window::GetPlayerMode() {
        if (playerMode) {
            return true;
        } else {
            return false;
        }

    }

    void Window::SetFirstMouse(bool firstMouse) {
        this->firstMouse = firstMouse;
    }

    bool Window::GetFirstMouse() {
        if (firstMouse) {
            return true;
        } else {
            return false;
        }

    }
} // namspace Chotra
