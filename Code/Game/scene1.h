#include <GameEngine/game_engine.h>
#include <GameEngine/game_view.h>

class MainScene1 : public game_engine::GameView {
 public:
  MainScene1() : g_pTextureRV(NULL) { }
  ~MainScene1() {}
  int Initialize(game_engine::Engine* engine);
  int Deinitialize();
  void Draw();
 private:
  graphics::Effect main_effect_;
  float move_z;
  core::animation::CubicMotion motion1;
graphics::Buffer                    g_vb;
graphics::Buffer                    g_ib;
graphics::Buffer                    g_pCBChangesEveryFrame;
ID3D11ShaderResourceView*           g_pTextureRV;
XMMATRIX                            g_World;
  
};