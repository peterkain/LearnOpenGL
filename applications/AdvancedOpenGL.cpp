#include "AdvancedOpenGL.h"
#include "Resource.h"
#include "Data.h"

#include <vector>

#define CUBE_OUTLINING 0

std::vector<glm::vec3> grassPositions{
    glm::vec3{-1.5f, -.15f,  0.48f},
    glm::vec3{ 1.5f, -.15f,  0.81f},
    glm::vec3{ 0.f,  -.15f,   .7f},
    glm::vec3{-.3f,  -.15f, -2.3f},
    glm::vec3{ .5f,  -.15f, -.6f}
};

void AdvancedOpenGL::init() {
    glGenBuffers(3, &VBO[0]);
    glGenVertexArrays(4, &VAO[0]);

    glBindVertexArray(VAO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Data::cubeVTN), Data::cubeVTN, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(VAO[1]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Data::planeVT), Data::planeVT, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(VAO[2]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Data::quadVT), Data::quadVT, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(VAO[3]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    glGenFramebuffers(1, &fbo);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, &textureBuffer);
    glBindTexture(GL_TEXTURE_2D, textureBuffer);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window.width, window.height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureBuffer, 0);

    GLuint rbo;
    glGenRenderbuffers(1, &rbo);

    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, window.width, window.height);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Framebuffer is not complete" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Image::flip_vertically(true);

    Resource::shader("advanced", std::move(Shader{ "shaders/advanced_vs.glsl", "shaders/advanced_fs.glsl" }));
    Resource::shader("framebuffer", std::move(Shader{ "shaders/framebuffer_vs.glsl", "shaders/framebuffer_fs.glsl" }));
    Resource::texture("alhambra", std::move(Texture{ "textures/alhambra.jpg" }));
    Resource::texture("grass", std::move(Texture{ "textures/grass.png" }));

    glClearColor(.529f, .808f, .98f, 1.f);  // Sky Blue
    //glClearColor(.1f, .1f, .1f, 1.f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

#if CUBE_OUTLINING
    glEnable(GL_STENCIL_TEST);
#endif
}

void AdvancedOpenGL::loop() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glEnable(GL_DEPTH_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glm::mat4 model{ 1.f };
    // For "mirror"
    // glm::mat4 view{ glm::rotate(glm::mat4{1.f}, glm::radians(180.f), camera.worldUp) * camera.GetViewMatrix() };
    glm::mat4 view{ camera.GetViewMatrix() };
    glm::mat4 projection{ glm::perspective(glm::radians(camera.zoom), (float)window.width / (float)window.height, .1f, 100.f) };

    Shader advanced{ Resource::shader("advanced") };
    advanced.use();

    advanced.setInt("Texture", 0);
    advanced.setBool("drawOutline", false);
    advanced.setBool("visualizeDepth", glfwGetKey(window.glfw_window, GLFW_KEY_F1) == GLFW_PRESS);
    advanced.setMat4("view", view);
    advanced.setMat4("projection", projection);

    glStencilMask(0x00);
    glActiveTexture(GL_TEXTURE0);

    glDisable(GL_CULL_FACE);

    glBindVertexArray(VAO[2]);
    Resource::texture("grass").bind();
    for (int i{}; i < grassPositions.size(); i++) {
        float radians{ std::atan2f(grassPositions[i].x - camera.position.x, grassPositions[i].z - camera.position.z) };
        model = glm::translate(glm::mat4(1.f), grassPositions[i]);
        model = glm::rotate(model, radians, glm::vec3{ 0.f, 1.f, 0.f });
        model = glm::scale(model, glm::vec3{ .35f });
        advanced.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glBindVertexArray(VAO[1]);
    Resource::texture("alhambra").bind();
    advanced.setMat4("model", glm::mat4(1.f));
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(VAO[0]);
    glStencilFunc(GL_ALWAYS, 1, 0xff);
    glStencilMask(0xff);

    model = glm::translate(glm::mat4(1.f), glm::vec3(-1.f, 0.f, -1.f));
    advanced.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    model = glm::translate(glm::mat4(1.f), glm::vec3(2.f, 0.f, 0.f));
    advanced.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

#if CUBE_OUTLINING
    glStencilFunc(GL_NOTEQUAL, 1, 0xff);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);

    advanced.setBool("drawOutline", true);
    model = glm::translate(glm::mat4(1.f), glm::vec3(-1.f, 0.f, -1.f));
    model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
    advanced.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    model = glm::translate(glm::mat4(1.f), glm::vec3(2.f, 0.f, 0.f));
    model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
    advanced.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glEnable(GL_DEPTH_TEST);
    glStencilMask(0xff);
    glStencilFunc(GL_KEEP, 1, 0xff);
#endif

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    Shader fbShader{ Resource::shader("framebuffer") };
    fbShader.use();
    fbShader.setFloat("offset", 1. / window.height);
    fbShader.setBool("inverseColor", glfwGetKey(window.glfw_window, GLFW_KEY_F2) == GLFW_PRESS);
    fbShader.setBool("grayscale", glfwGetKey(window.glfw_window, GLFW_KEY_F3) == GLFW_PRESS);
    fbShader.setBool("sharpen", glfwGetKey(window.glfw_window, GLFW_KEY_F4) == GLFW_PRESS);
    fbShader.setBool("blur", glfwGetKey(window.glfw_window, GLFW_KEY_F5) == GLFW_PRESS);
    fbShader.setBool("edgeDetect", glfwGetKey(window.glfw_window, GLFW_KEY_F6) == GLFW_PRESS);
    fbShader.setBool("custom", glfwGetKey(window.glfw_window, GLFW_KEY_F7) == GLFW_PRESS);

    glBindVertexArray(VAO[3]);
    glBindTexture(GL_TEXTURE_2D, textureBuffer);
    glDisable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
}