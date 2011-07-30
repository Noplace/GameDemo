#include <Aurora/Code/game_engine.h>
#include <Aurora/Code/game_view.h>


class MainScene2 : public game_engine::GameView {
 public:
  MainScene2() { }
  ~MainScene2() {}
  int Initialize(game_engine::Engine* engine);
  int Deinitialize();
  void Draw();
 private:
  graphics::Effect main_effect_;
  graphics::Effect font_effect_;
  float move_z;
  core::animation::CubicMotion motion1;
  graphics::font::Writer font_writer_;
  graphics::Buffer                    g_vb;
  graphics::Buffer                    g_ib;
  graphics::Buffer                        g_pCBChangesEveryFrame;
  //ID3D11ShaderResourceView*           g_pTextureRV ;
  XMMATRIX world;
  game_engine::resource::TextureResource* tex_res1;
  acGraphics::Font* font;
};