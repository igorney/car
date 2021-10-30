#include <fmt/core.h>

#include "abcg.hpp"
#include "openglwindow.hpp"

int main(int argc, char **argv) {
  try {    
    abcg::Application app(argc, argv);
    
    auto window{std::make_unique<OpenGLWindow>()};
    window->setOpenGLSettings({.profile = abcg::OpenGLProfile::Core,
                               .majorVersion = 4,
                               .minorVersion = 1});
    window->setWindowSettings(
        {.width = 600, .height = 600, .showFPS = false, .title = "CARRINHO DA COLETA!"});
    
    app.run(std::move(window));
  } catch (const abcg::Exception &exception) {
    fmt::print(stderr, "{}", exception.what());
    return -1;
  }
  return 0;
}