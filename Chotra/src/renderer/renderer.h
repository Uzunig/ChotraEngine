#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <random>
#include <memory>

#include "shader.h"
#include "quad.h"
#include "rendering_settings.h"
#include "renderer_base.h"


namespace Chotra {

    class GaussianBlurer;
            
    class Renderer : public RendererBase {
    public:

                
        //std::vector<std::shared_ptr<Quad>> quads;
        //std::shared_ptr<GaussianBlurer> gaussianBlurer;
                                       
        Shader pbrShader;
        //Shader lightsShader;
        

        Shader screenDivideShader;
        Shader downSamplingShader;
        Shader combineShader;

        Shader shaderBlur;
        Shader shaderBloomFinal;

        Shader backgroundShader;

        Shader shaderSSAO;
        Shader shaderSSAOBlur;

        Shader shaderSSR;
        Shader shaderSSRBlur;
               
        Shader shaderDeferredGeometryPass;
        Shader shaderDeferredPreLightingPass;
        Shader shaderDeferredLightingPass;

        //Shader shaderRenderOnScreen;
                
        //ScreenTexture screenTexture; 
                  
        unsigned int gBuffer;           // G-Buffer
        unsigned int gPosition; //TExtures
        unsigned int gViewPosition; 
        unsigned int gNormal;
        unsigned int gViewNormal;
        unsigned int gAlbedoMap;
        unsigned int gMetalRoughAoMap;
        unsigned int rboG;          //Renderbuffer for depth
        

        // Framebuffer without MSAA
        unsigned int framebuffer;
        unsigned int rbo;

        // Framebuffer without MSAA
        unsigned int framebufferPreLighting;
        unsigned int rboPreLighting;
        ScreenTexture lScreenTexture;
        ScreenTexture lFresnelSchlickRoughness;
        ScreenTexture lDiffuse;
        ScreenTexture lkD;
        ScreenTexture lBrdf;
        ScreenTexture lLo;
        ScreenTexture lRoughAo;

        // Framebuffer with MSAA
        unsigned int miniFramebufferMSAA;

        unsigned int framebufferMSAA;
        unsigned int textureColorBufferMultiSampled;
        unsigned int rboMSAA;
        unsigned int intermediateFBO;

                     
        unsigned int hdrFBO;
        unsigned int colorBuffers[2];
        unsigned int rboDepth;
        
        unsigned int downPingpongFBO[16][2];
        unsigned int downPingpongColorbuffers[16][2];

        unsigned int upFBO[16];
        unsigned int upColorbuffers[16];
                
        unsigned int ssaoFBO;
        unsigned int ssaoBlurFBO;
        unsigned int ssaoColorBuffer;
        unsigned int ssaoMap;
        unsigned int noiseTexture;
        std::vector<glm::vec3> ssaoKernel;

        unsigned int ssrFBO;
        unsigned int ssrUvMap;
                        
        unsigned int quadVAO = 0;
        unsigned int quadVBO;

       

        
        Renderer(unsigned int& width, unsigned int& hight);
                
        void Init();
        void Render(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera);
        void MiniRender(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera, ScreenTexture& icon);
        void ForwardRender(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera);
        void DeferredRender(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera);
        void PassiveRender();
        
        void ConfigureFramebufferMSAA();
        void RenderWithMSAA(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera);

        void ConfigureFramebuffer();
        void RenderWithoutMSAA(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera);

        void ConfigureBloom();
        void RenderBloom();
                        
        void ConfigureSSAO();
        void GenerateSSAOMap();

        void ConfigureSSR();
        void GenerateSSRMap();

        void ConfigureGeometryPass();
        void RenderGeometryPass(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera);
        
        void ConfigurePreLightingPass();
        void ConfigureLightingPass();
        void RenderPreLightingPass(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera);
        void RenderLightingPass();

        //void RenderToScreen();

        unsigned int CreateGeometryIcon(unsigned int i);

    };

} // namespace Chotra


#endif