#include "car.hpp"

#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>

void Car::initializeGL(GLuint program) {
  terminateGL();

  me_program = program; 
  m_rotationLoc = abcg::glGetUniformLocation(me_program, "rotation");
  m_scaleLoc = abcg::glGetUniformLocation(me_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(me_program, "translation");

  m_rotation = 0.0f;
  m_translation = glm::vec2(0, -0.5);
  m_velocity = glm::vec2(0);

  
  std::array<glm::vec2, 26> positions{      
      glm::vec2{-02.5f, +12.5f}, glm::vec2{-15.5f, +02.5f},
      glm::vec2{-15.5f, -12.5f}, glm::vec2{-09.5f, -07.5f},
      glm::vec2{-03.5f, -12.5f}, glm::vec2{+03.5f, -12.5f},
      glm::vec2{+09.5f, -07.5f}, glm::vec2{+15.5f, -12.5f},
      glm::vec2{+15.5f, +02.5f}, glm::vec2{+02.5f, +12.5f},	  
	  
      glm::vec2{-12.5f, +10.5f}, glm::vec2{-12.5f, +04.0f},
      glm::vec2{-09.5f, +04.0f}, glm::vec2{-09.5f, +10.5f},	  
      
      glm::vec2{+09.5f, +10.5f}, glm::vec2{+09.5f, +04.0f},
      glm::vec2{+12.5f, +04.0f}, glm::vec2{+12.5f, +10.5f},	  
	  
      glm::vec2{-12.0f, -10.5f}, glm::vec2{-12.0f, -04.0f}, 
      glm::vec2{-09.5f, -04.0f}, glm::vec2{-09.5f, -10.5f},
      
      glm::vec2{+09.5f, -10.5f}, glm::vec2{+09.5f, -04.0f},
      glm::vec2{+12.5f, -04.0f}, glm::vec2{+12.5f, -10.5f},
  };

  
  std::array<glm::vec4, 26> colors{     
      glm::vec4{0,1,0,0}, glm::vec4{0,1,0,0},
      glm::vec4{0,1,0,0}, glm::vec4{0,1,0,0},
      glm::vec4{0,1,0,0}, glm::vec4{0,1,0,0},
      glm::vec4{0,1,0,0}, glm::vec4{0,1,0,0},
      glm::vec4{0,1,0,0}, glm::vec4{0,1,0,0},      
	  
	  
      glm::vec4{0,0,0,0}, glm::vec4{0,0,0,0},
      glm::vec4{0,0,0,0}, glm::vec4{0,0,0,0},	  
      
       glm::vec4{0,0,0,0}, glm::vec4{0,0,0,0},
      glm::vec4{0,0,0,0}, glm::vec4{0,0,0,0},	  
	 
       glm::vec4{0,0,0,0}, glm::vec4{0,0,0,0},
      glm::vec4{0,0,0,0}, glm::vec4{0,0,0,0},
      
       glm::vec4{0,0,0,0}, glm::vec4{0,0,0,0},
      glm::vec4{0,0,0,0}, glm::vec4{0,0,0,0},
  };
  
  for (auto &position : positions) {
    position /= glm::vec2{15.5f, 15.5f};
  }

  const std::array indices{0, 3, 4,
                           0, 4, 5,
                           9, 0, 5,
                           9, 5, 6,
						  
                           10, 11, 12,
                           10, 12, 13,
                           14, 15, 16,
                           14, 16, 17,
						   
                           18, 19, 20,
						               18, 20, 21,
                           22, 23, 24,
						               22, 24, 25
						   };
  

  abcg::glGenBuffers(1, &m_vbo);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);
  
  abcg::glGenBuffers(1, &m_vbo_color);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vbo_color);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0); 
  
  abcg::glGenBuffers(1, &m_ebo);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  
  GLint positionAttribute{abcg::glGetAttribLocation(me_program, "inPosition")};

  GLint colorAttribute{abcg::glGetAttribLocation(me_program, "inColor")};

  abcg::glGenVertexArrays(1, &m_vao);

  abcg::glBindVertexArray(m_vao);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glEnableVertexAttribArray(colorAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vbo_color);
  abcg::glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  
  abcg::glBindVertexArray(0);
}

void Car::paintGL(const GameData &gameData) {
  if (gameData.m_state != State::Playing) return;

  abcg::glUseProgram(me_program);

  abcg::glBindVertexArray(m_vao);

  abcg::glUniform1f(m_scaleLoc, m_scale);
  abcg::glUniform1f(m_rotationLoc, m_rotation);
  abcg::glUniform2fv(m_translationLoc, 1, &m_translation.x);
  
  if (m_trailBlinkTimer.elapsed() > 100.0 / 1000.0) m_trailBlinkTimer.restart();

  if (gameData.m_input[static_cast<size_t>(Input::Up)]) {    
    if (m_trailBlinkTimer.elapsed() < 50.0 / 1000.0) {
      abcg::glEnable(GL_BLEND);
      abcg::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);   
      
      abcg::glDrawElements(GL_TRIANGLES, 14 * 3, GL_UNSIGNED_INT, nullptr);

      abcg::glDisable(GL_BLEND);
    }
  }
 
  abcg::glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, nullptr);

  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
}

void Car::terminateGL() {
  abcg::glDeleteBuffers(1, &m_vbo);
  abcg::glDeleteBuffers(1, &m_ebo);
  abcg::glDeleteVertexArrays(1, &m_vbo_color);
  abcg::glDeleteVertexArrays(1, &m_vao);

}

void Car::update(const GameData &gameData, float deltaTime) {  
  if (gameData.m_input[static_cast<size_t>(Input::Left)]) {
    m_rotation = glm::wrapAngle(m_rotation + 4.0f * deltaTime);
    m_translation.x = m_translation.x - 0.5f * deltaTime;
  }
  if (gameData.m_input[static_cast<size_t>(Input::Right)]) {
    m_rotation = glm::wrapAngle(m_rotation - 4.0f * deltaTime);
    m_translation.x = m_translation.x + 0.5f * deltaTime;
  }
  
  if (gameData.m_input[static_cast<size_t>(Input::Up)] &&
      gameData.m_state == State::Playing) {    
    glm::vec2 forward = glm::rotate(glm::vec2{0.0f, 1.0f}, m_rotation);
    m_velocity += forward * deltaTime;
  }
}