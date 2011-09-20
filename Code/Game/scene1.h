#include <Aurora/Code/aurora.h>
#include <Aurora/Code/game_view.h>

class MainScene1 : public aurora::GameView {
 public:
  MainScene1() : g_pTextureRV(NULL) { }
  ~MainScene1() {}
  int Initialize(aurora::Engine* engine);
  int Deinitialize();
  void Update(float delta_time);
  void Draw();
 private:
  graphics::Effect* main_effect_;
  float move_z;
  aurora::animation::CubicMotion motion1;
  graphics::Buffer                    g_vb;
  graphics::Buffer                    g_ib;
  graphics::Buffer                    g_pCBChangesEveryFrame;
  ID3D11ShaderResourceView*           g_pTextureRV;
  XMMATRIX                            g_World;
  
};