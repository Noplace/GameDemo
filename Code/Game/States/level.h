
namespace my_app {

class Level : public aurora::State {
 public:
  Level() : aurora::State(1) { }
  ~Level() {}
  void OnStateEnter();
  void OnStateExit();
  void OnUpdate(float delta_time);
  void OnInterpolate(float dt);
  void OnDraw();
  Game* game_;
 private:
  graphics::Sprite my_sprite;
  pong::System pongsystem;
};

}