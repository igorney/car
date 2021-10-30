#include "openglwindow.hpp"

#include <imgui.h>

#include <cppitertools/itertools.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <gsl/gsl>

#include "abcg.hpp"

int m_objects = 0;

void OpenGLWindow::handleEvent(SDL_Event &event) {
  // Keyboard events
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_SPACE)
      m_gameData.m_input.set(static_cast<size_t>(Input::Fire));
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
      m_gameData.m_input.set(static_cast<size_t>(Input::Up));
    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
      m_gameData.m_input.set(static_cast<size_t>(Input::Down));
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.set(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.set(static_cast<size_t>(Input::Right));
  }
  if (event.type == SDL_KEYUP) {
    if (event.key.keysym.sym == SDLK_SPACE)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Fire));
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Up));
    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Down));
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Right));
  }

  // Mouse events
  if (event.type == SDL_MOUSEBUTTONDOWN) {
    if (event.button.button == SDL_BUTTON_LEFT)
      m_gameData.m_input.set(static_cast<size_t>(Input::Fire));
    if (event.button.button == SDL_BUTTON_RIGHT)
      m_gameData.m_input.set(static_cast<size_t>(Input::Up));
  }
  if (event.type == SDL_MOUSEBUTTONUP) {
    if (event.button.button == SDL_BUTTON_LEFT)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Fire));
    if (event.button.button == SDL_BUTTON_RIGHT)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Up));
  }
}
void OpenGLWindow::initializeGL() {
  // Enable Z-buffer test
  glEnable(GL_DEPTH_TEST);
  

  // Create shader program
  m_program = createProgramFromFile(getAssetsPath() + "UnlitVertexColor.vert",
                                    getAssetsPath() + "UnlitVertexColor.frag");

  // Create program to render the other objects
  m_objectsProgram = createProgramFromFile(getAssetsPath() + "objects.vert",
                                           getAssetsPath() + "objects.frag");

  

  // Get location of attributes in the program
  GLint positionAttribute{glGetAttribLocation(m_program, "inPosition")};
  GLint colorAttribute{glGetAttribLocation(m_program, "inColor")};

  // Create VAO
  glGenVertexArrays(1, &m_vao);

  // Bind vertex attributes to current VAO
  glBindVertexArray(m_vao);

  glEnableVertexAttribArray(positionAttribute);
  glBindBuffer(GL_ARRAY_BUFFER, m_vboVertices);
  glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glEnableVertexAttribArray(colorAttribute);
  glBindBuffer(GL_ARRAY_BUFFER, m_vboColors);
  glVertexAttribPointer(colorAttribute, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  glBindVertexArray(0);

  
#if !defined(__EMSCRIPTEN__)
  abcg::glEnable(GL_PROGRAM_POINT_SIZE);
#endif

  // Start pseudo-random number generator
  m_randomEngine.seed(
      std::chrono::steady_clock::now().time_since_epoch().count());

  restart();
}


void OpenGLWindow::restart() {
  m_gameData.m_state = State::Playing;
  m_car.initializeGL(m_objectsProgram);
  m_asteroids.initializeGL(m_objectsProgram, 3);
}

void OpenGLWindow::update() {
  float deltaTime{static_cast<float>(getDeltaTime())};

  // Wait 5 seconds before restarting
  if (m_gameData.m_state != State::Playing &&
      m_restartWaitTimer.elapsed() > 5) {
        m_objects = 0;
    restart();
    return;
  }  

  m_car.update(m_gameData, deltaTime);
  m_asteroids.update(m_car, deltaTime);

  if (m_gameData.m_state == State::Playing) {
  checkCollisions();
  checkWinCondition();
  }
}
 
void OpenGLWindow::paintGL() {
  // Set the clear color
  glClearColor(gsl::at(m_clearColor, 0), gsl::at(m_clearColor, 1),
               gsl::at(m_clearColor, 2), gsl::at(m_clearColor, 3));
  // Clear the color buffer and Z-buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Adjust viewport
  glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  // Start using the shader program
  glUseProgram(m_program);
  // Start using the VAO
  glBindVertexArray(m_vao);


  // End using the VAO
  glBindVertexArray(0);
  // End using the shader program
  glUseProgram(0);

  update();

  abcg::glClear(GL_COLOR_BUFFER_BIT);
  abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);
  m_asteroids.paintGL();
  m_car.paintGL(m_gameData);
}

void OpenGLWindow::paintUI() {
  // Parent class will show fullscreen button and FPS meter
  abcg::OpenGLWindow::paintUI();

  // Our own ImGui widgets go below
  {
    
    // Window begin
    ImGui::Begin("!!!!!!!!!!THE CAR!!!!!!!!!!");

    // Static text
    ImGui::Text("Escolha a cor do seu plano de fundo e divirta-se :)");   
    

    // Edit background color
    ImGui::ColorEdit3("Background", m_clearColor.data()); 

    // End of window
    ImGui::End();    
  }

   {
    const auto size{ImVec2(300, 85)};
    const auto position{ImVec2((m_viewportWidth - size.x) / 2.0f,
                               (m_viewportHeight - size.y) / 2.0f)};
    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(size);
    ImGuiWindowFlags flags{ImGuiWindowFlags_NoBackground |
                           ImGuiWindowFlags_NoTitleBar |
                           ImGuiWindowFlags_NoInputs};
    ImGui::Begin(" ", nullptr, flags);
    ImGui::PushFont(m_font);

    if (m_gameData.m_state == State::GameOver) {
      ImGui::Text("Game Over!");
    } else if (m_gameData.m_state == State::Win) {
      ImGui::Text("Você coletou: %d objetos!!", m_objects);
    }

    ImGui::PopFont();
    ImGui::End();
  }
}

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

   abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWindow::terminateGL() {
  // Release OpenGL resources
  glDeleteProgram(m_program);
  glDeleteBuffers(1, &m_vboVertices);
  glDeleteBuffers(1, &m_vboColors);
  glDeleteVertexArrays(1, &m_vao);
  abcg::glDeleteProgram(m_objectsProgram);
  m_car.terminateGL();
  m_asteroids.terminateGL();
}

void OpenGLWindow::checkCollisions() {
  // Check collision between ship and asteroids
  //int m_objects = 0;
  for ( auto &asteroid : m_asteroids.m_asteroids) {
    const auto asteroidTranslation{asteroid.m_translation};
    const auto distance{
        glm::distance(m_car.m_translation, asteroidTranslation)};

    if (distance < m_car.m_scale * 0.9f + asteroid.m_scale * 0.85f) {
      asteroid.m_hit = true;
      m_objects++;
      
      //m_gameData.m_state = State::GameOver;
      //m_restartWaitTimer.restart();
    }
  } 

    // Break asteroids marked as hit
    for (auto &asteroid : m_asteroids.m_asteroids) {
      if (asteroid.m_hit && asteroid.m_scale > 0.10f) {
        std::uniform_real_distribution<float> m_randomDist{-1.0f, 1.0f};
        std::generate_n(std::back_inserter(m_asteroids.m_asteroids), 3, [&]() {
          const glm::vec2 offset{m_randomDist(m_randomEngine),
                                 m_randomDist(m_randomEngine)};
          return m_asteroids.createAsteroid(
              asteroid.m_translation + offset * asteroid.m_scale * 0.5f,
              asteroid.m_scale * 0.5f);
        });
      }
    }

    m_asteroids.m_asteroids.remove_if(
        []( Asteroids::Asteroid &a) { return a.m_hit; });
  }


void OpenGLWindow::checkWinCondition() {
  if (m_timerGame.elapsed() > 10) {
    m_gameData.m_state = State::Win;
    m_restartWaitTimer.restart();
    //m_objects = 0;
    m_timerGame.restart();
  }
  
  
}

