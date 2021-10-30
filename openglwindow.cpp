#include "openglwindow.hpp"

#include <imgui.h>

#include <cppitertools/itertools.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <gsl/gsl>

#include "abcg.hpp"

int m_objects = 0;

void OpenGLWindow::handleEvent(SDL_Event &event) {  
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_SPACE)
      m_gameData.m_input.set(static_cast<size_t>(Input::Stop));
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
      m_gameData.m_input.reset(static_cast<size_t>(Input::Stop));
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Up));
    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Down));
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Right));
  }
  
  if (event.type == SDL_MOUSEBUTTONDOWN) {
    if (event.button.button == SDL_BUTTON_LEFT)
      m_gameData.m_input.set(static_cast<size_t>(Input::Stop));
    if (event.button.button == SDL_BUTTON_RIGHT)
      m_gameData.m_input.set(static_cast<size_t>(Input::Up));
  }
  if (event.type == SDL_MOUSEBUTTONUP) {
    if (event.button.button == SDL_BUTTON_LEFT)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Stop));
    if (event.button.button == SDL_BUTTON_RIGHT)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Up));
  }
}
void OpenGLWindow::initializeGL() {  
  glEnable(GL_DEPTH_TEST);

  ImGuiIO &io{ImGui::GetIO()};
  const auto filename{getAssetsPath() + "Inconsolata-UltraCondensedBlack.ttf"};
  m_font = io.Fonts->AddFontFromFileTTF(filename.c_str(), 45.0f);  
  if (m_font == nullptr) {
    throw abcg::Exception{abcg::Exception::Runtime("Cannot load font file")};
  }  
                              
  
  m_objectsProgram = createProgramFromFile(getAssetsPath() + "objects.vert",
                                           getAssetsPath() + "objects.frag");  
  
  glGenVertexArrays(1, &m_vao);

  glBindVertexArray(m_vao);
  
  
#if !defined(__EMSCRIPTEN__)
  abcg::glEnable(GL_PROGRAM_POINT_SIZE);
  #endif
  
  m_randomEngine.seed(
      std::chrono::steady_clock::now().time_since_epoch().count());

  restart();
}


void OpenGLWindow::restart() {
  m_gameData.m_state = State::Playing;
  m_car.initializeGL(m_objectsProgram);
  m_items.initializeGL(m_objectsProgram, 100);
  m_timerGame.restart();
  
}

void OpenGLWindow::update() {
  float deltaTime{static_cast<float>(getDeltaTime())};
  
  if (m_gameData.m_state != State::Playing &&
      m_restartWaitTimer.elapsed() > 5) {
        m_objects = 0;
    restart();
    return;
  }  

  m_car.update(m_gameData, deltaTime);
  m_items.update(m_car, deltaTime);

  if (m_gameData.m_state == State::Playing) {
  checkCollisions();
  checkWinCondition();
  }
}
 
void OpenGLWindow::paintGL() {  
  glClearColor(gsl::at(m_clearColor, 0), gsl::at(m_clearColor, 1),
               gsl::at(m_clearColor, 2), gsl::at(m_clearColor, 3));  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glViewport(0, 0, m_viewportWidth, m_viewportHeight);  
  
  glBindVertexArray(m_vao);
  
  glBindVertexArray(0);
  
  glUseProgram(0);

  update();

  abcg::glClear(GL_COLOR_BUFFER_BIT);
  abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);
  m_items.paintGL();
  m_car.paintGL(m_gameData);
}

void OpenGLWindow::paintUI() {
  
  abcg::OpenGLWindow::paintUI();
 
  {   
    ImGui::Begin("!!!!!!!!!!CARRINHO DA COLETA!!!!!!!!!!");    
    ImGui::Text("Escolha a cor do seu plano de fundo e divirta-se :)"); 
    ImGui::ColorEdit3("Background", m_clearColor.data());     
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

    if (m_gameData.m_state == State::Win) {
      ImGui::Text("Você coletou: %d itens!!", m_objects);
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
  glDeleteBuffers(1, &m_vboVertices);
  glDeleteBuffers(1, &m_vboColors);
  glDeleteVertexArrays(1, &m_vao);
  abcg::glDeleteProgram(m_objectsProgram);
  m_car.terminateGL();
  m_items.terminateGL();
}

void OpenGLWindow::checkCollisions() {  
  for ( auto &item : m_items.m_items) {
    const auto itemTranslation{item.m_translation};
    const auto distance{
        glm::distance(m_car.m_translation, itemTranslation)};

    if (distance < m_car.m_scale * 0.9f + item.m_scale * 0.85f) {
      item.m_hit = true;
      m_objects++;      
    }
  } 

    for (auto &item : m_items.m_items) {
      if (item.m_hit && item.m_scale > 0.10f) {
        std::uniform_real_distribution<float> m_randomDist{-1.0f, 1.0f};
        std::generate_n(std::back_inserter(m_items.m_items), 3, [&]() {
          const glm::vec2 offset{m_randomDist(m_randomEngine),
                                 m_randomDist(m_randomEngine)};
          return m_items.createItem(
              item.m_translation + offset * item.m_scale * 0.5f,
              item.m_scale * 0.5f);
        });
      }
    }

    m_items.m_items.remove_if(
        []( Items::Item &a) { return a.m_hit; });
  }


void OpenGLWindow::checkWinCondition() {
  if (m_timerGame.elapsed() > 10) {
    m_gameData.m_state = State::Win;
    m_restartWaitTimer.restart();    
  }  
}

