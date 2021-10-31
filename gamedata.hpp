#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>

enum class Input { Right, Left, Up, Stop };
enum class State { Playing, Win };

struct GameData {
  State m_state{State::Playing};
  std::bitset<4> m_input;  
};

#endif