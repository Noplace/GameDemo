
namespace my_app {

class MainMenu : public aurora::State {
 public:
  MainMenu() : aurora::State(0) { }
  ~MainMenu() {}
  void OnStateEnter();
  void OnStateExit();
  void OnInput();
  void OnUpdate(float delta_time);
  void OnInterpolate(float dt);
  void OnDraw();
  Game* game_;
 private:
  graphics::Sprite cursor;
};

}