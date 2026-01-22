#ifndef GAME_HPP
#define GAME_HPP

namespace dryout {

class Game {
  public:
    static Game *getInstance();

    void run();

  private:
    Game();
    ~Game();

    Game(const Game &) = delete;
    Game &operator=(const Game &) = delete;
    Game(Game &&) = delete;
    Game &operator=(Game &&) = delete;

    void init();

    static Game *instance;
};

} // namespace dryout

#endif // GAME_HPP
