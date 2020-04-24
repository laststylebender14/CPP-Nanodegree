#include "game.h"
#include <iostream>
#include "SDL.h"
#include<chrono>
#include<mutex>
#include<memory>
#include<thread>
//User Implementation
User::User():_username("default"){}

User::User(std::string uname){
  _username = uname;
}

void User::writeToFile(){
    std::ofstream fopner(_filepath,std::ios::app);
    if(fopner.is_open()){
      fopner<<_username+" : "+std::to_string(score)<<std::endl;
      fopner.close();
    }
}

bool User::wantToPlayAgain(){
  char ans;
  std::cout<<"Do you want to play again? [y/n]\n";
  std::cin>>ans;
  if(ans == 'y'){
    return 1;
  }
  return 0;
}

void User::LoadAll(){
    std::string uname,line,bluff;
    int score;
    std::ifstream opener(_filepath);
    if(opener.is_open()){
      while (std::getline(opener,line)) {
        std::istringstream reader(line);
        reader >> uname >> bluff >> score;
        _users.push_back({uname,score});
      }
    }
}

void User::getTopPlayers(){
  if(!_users.empty()){
    for(int i=0;i<_users.size();i++){
      std::cout<<"Player : "<<_users[i].first<<" Score : "<<_users[i].second<<std::endl;
    }
  }
}



void User::getTopPlayers(int k){
  if(!_users.empty()){
    std::sort(_users.begin(),_users.end(),[](std::pair<std::string,int> a,std::pair<std::string,int> b){
      return a.second > b.second;
    });
    for(int i=0;i<(k<_users.size()?k:_users.size());i++){
      std::cout<<"Player : "<<_users[i].first<<" Score : "<<_users[i].second<<std::endl;
    }
  }
}



Game::Game(std::size_t grid_width, std::size_t grid_height,std::string uname)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width)),
      random_h(0, static_cast<int>(grid_height)),User(uname) {
  PlaceFood();
}




bool Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;
    bool flag=0;

  while (running) {
    frame_start = SDL_GetTicks();
    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    renderer.Render(snake, food);
    frame_end = SDL_GetTicks();


    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score,_username);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    if(!snake.alive){
      //ask user if he wants to play again
        writeToFile();  //write current score to file
        bool res = wantToPlayAgain();
        if(res){
          score = 0;
          flag = 1;
        }else{
          LoadAll();
          flag = 0;
        }
        running = 0;
    }
    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
  return flag;
}




void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}



void Game::Update() {
  if (!snake.alive) return;

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }
