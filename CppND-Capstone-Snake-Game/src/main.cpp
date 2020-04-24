#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include<memory>
void RunGame(){
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};

  std::string uname;
    while(1){
      std::cout<<"Enter Your Username : ";
      std::cin>>uname;
      if(uname != " "){
        break;
      }
    }
    Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
    Controller controller;
    Game game(kGridWidth, kGridHeight,uname);
    bool ok = 1;
    while(ok){
      ok =game.Run(controller, renderer, kMsPerFrame);
    }
}
void showTopPlayers(){
  std::unique_ptr<User> usr = std::make_unique<User>();
  usr->LoadAll();
  std::cout<<"\n";
  usr->getTopPlayers(5);
}
void showHistory(){
  std::unique_ptr<User> usr = std::make_unique<User>();
  usr->LoadAll();
  std::cout<<"\n";
  usr->getTopPlayers();
}


int main() {

  bool out=1;
  int inp;
  while(out){
    std::cout<<"\n--------------Welcome To The Game -----------------\n";
    std::cout<<"1.Start New Game\n";
    std::cout<<"2.View Highest Scores\n";
    std::cout<<"3.View History\n";
    std::cout<<"4.Quite\n\n";
    std::cout<<"Enter Your Choice : ";
    std::cin>>inp;
    switch (inp) {
      case 1:
        RunGame();
        break;
      case 2:
        showTopPlayers();
        break;
      case 3:
        showHistory();
      default:
        out=0;
        break;
    }
  }
  return 0;
}
