#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <array>
#include <imgui.h>

#include <random>

#include "abcg.hpp"
#include "car.hpp"
#include "asteroids.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
 void handleEvent(SDL_Event& event) override;
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

 private:
  GLuint m_vao{};
  GLuint m_vboVertices{};
  GLuint m_vboColors{};
  GLuint m_program{};
  GLuint m_objectsProgram{};

  int m_viewportWidth{};
  int m_viewportHeight{};

  GameData m_gameData; 

  Car m_car;
  Asteroids m_asteroids;



  abcg::ElapsedTimer m_restartWaitTimer;

  ImFont* m_font{};

  std::default_random_engine m_randomEngine;


  void restart();
  void update();

  bool m_showDemoWindow{false};
  bool m_showAnotherWindow{false};
  std::array<float, 4> m_clearColor{0.906f, 0.910f, 0.918f, 1.00f};
};

#endif