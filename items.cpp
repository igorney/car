#include "items.hpp"

#include <cppitertools/itertools.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

void Items::initializeGL(GLuint program, int quantity) {
  terminateGL();

  
  m_randomEngine.seed(
      std::chrono::steady_clock::now().time_since_epoch().count());

  m_program = program;
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_rotationLoc = abcg::glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  
  m_items.clear();
  m_items.resize(quantity);

  for (auto &item : m_items) {
    item = createItem();

    
    do {
      item.m_translation = {m_randomDist(m_randomEngine),
                                m_randomDist(m_randomEngine)};
    } while (glm::length(item.m_translation) < 0.5f);
  }
}

void Items::paintGL() {
  abcg::glUseProgram(m_program);

  for (const auto &item : m_items) {
    abcg::glBindVertexArray(item.m_vao);

    abcg::glUniform4fv(m_colorLoc, 1, &item.m_color.r);
    abcg::glUniform1f(m_scaleLoc, item.m_scale);
    abcg::glUniform1f(m_rotationLoc, item.m_rotation);

    for (auto i : {-2, 0, 2}) {
      for (auto j : {-2, 0, 2}) {
        abcg::glUniform2f(m_translationLoc, item.m_translation.x + j,
                          item.m_translation.y + i);

        abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, item.m_polygonSides + 2);
      }
    }

    abcg::glBindVertexArray(0);
  }

  abcg::glUseProgram(0);
}

void Items::terminateGL() {
  for (auto item : m_items) {
    abcg::glDeleteBuffers(1, &item.m_vbo);
    abcg::glDeleteVertexArrays(1, &item.m_vao);
  }
}

void Items::update(const Car &car, float deltaTime) {
  for (auto &item : m_items) {
    item.m_translation -= car.m_velocity * deltaTime;
    item.m_rotation = glm::wrapAngle(
        item.m_rotation + item.m_angularVelocity * deltaTime);
    item.m_translation += item.m_velocity * deltaTime;
    
    if (item.m_translation.x < -1.0f) item.m_translation.x += 2.0f;
    if (item.m_translation.x > +1.0f) item.m_translation.x -= 2.0f;
    if (item.m_translation.y < -1.0f) item.m_translation.y += 2.0f;
    if (item.m_translation.y > +1.0f) item.m_translation.y -= 2.0f;
  }
}

Items::Item Items::createItem(glm::vec2 translation,
                                              float scale) {
  Item item;

  auto &re{m_randomEngine}; 

  
  std::uniform_int_distribution<int> randomSides(5, 9);
  item.m_polygonSides = randomSides(re);

  
  std::uniform_real_distribution<float> randomIntensity(0.5f, 1.0f);
  item.m_color = glm::vec4(1) * randomIntensity(re);

  item.m_color.a = 1.0f;
  item.m_rotation = 0.0f;
  item.m_scale = scale;
  item.m_translation = translation;

  
  item.m_angularVelocity = m_randomDist(re);

  
  glm::vec2 direction{m_randomDist(re), m_randomDist(re)};
  item.m_velocity = glm::normalize(direction) / 7.0f;

  
  std::vector<glm::vec2> positions(0);
  positions.emplace_back(0, 0);
  const auto step{M_PI * 2 / item.m_polygonSides};
  std::uniform_real_distribution<float> randomRadius(0.8f, 1.0f);
  for (const auto angle : iter::range(0.0, M_PI * 2, step)) {
    const auto radius{randomRadius(re)};
    positions.emplace_back(radius * std::cos(angle), radius * std::sin(angle));
  }
  positions.push_back(positions.at(1));

  
  abcg::glGenBuffers(1, &item.m_vbo);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, item.m_vbo);
  abcg::glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2),
                     positions.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  
  GLint positionAttribute{abcg::glGetAttribLocation(m_program, "inPosition")};

  
  abcg::glGenVertexArrays(1, &item.m_vao);

  
  abcg::glBindVertexArray(item.m_vao);

  abcg::glBindBuffer(GL_ARRAY_BUFFER, item.m_vbo);
  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);
  
  abcg::glBindVertexArray(0);

  return item;
}