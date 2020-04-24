#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include <vector>
#include <string>
#include <utility>
#include <fstream>
#include <sstream>
#include<algorithm>

class User {
public:
  User();
  User(std::string);
  void writeToFile();
  void getTopPlayers(int k);
  void getTopPlayers();
  bool wantToPlayAgain();
  void LoadAll();
  std::string _username;
  int score{0};
private:
  std::vector<std::pair<std::string,int>> _users;
  const std::string _filepath = "leaderboard.txt";
};


class Game : public User {
 public:
  Game(std::size_t grid_width, std::size_t grid_height,std::string);
  bool Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;
  int getScore() const;

 private:
  Snake snake;
  SDL_Point food;
  SDL_Point Bfood;


  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;


  void PlaceBiggerFood();
  void PlaceFood();
  void Update();
  void BUpdate();

};

#endif
