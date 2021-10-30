#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>

enum class Input { Right, Left, Down, Up, Stop };
enum class State { Playing, Win };

struct GameData {
  State m_state{State::Playing};
  std::bitset<5> m_input;  // [stop, up, down, left, right]
};

#endif