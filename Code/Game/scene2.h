#include <Aurora/Code/aurora.h>
#include <Aurora/Code/game_view.h>
#include <Aurora/Code/tiles/tiles.h>
#include <VisualEssence/Code/shape/arc.h>

class MainScene2 : public aurora::GameView {
 public:
  MainScene2() { }
  ~MainScene2() {}
  int Initialize(aurora::Engine* engine);
  int Deinitialize();
  void Update(float delta_time);
  void Draw();
 private:
  //graphics::Effect* main_effect_;
   graphics::Effect main_effect_;
  
  float move_z,move_x,move_y;
  aurora::animation::CubicMotion motion1;
  aurora::animation::Spiral spiral;
  graphics::font::Writer font_writer_;
  graphics::Buffer                    g_vb;
  graphics::Buffer                    g_ib;
  graphics::Buffer                        g_pCBChangesEveryFrame;
  //ID3D11ShaderResourceView*           g_pTextureRV ;
  XMMATRIX world;
  aurora::resource::TextureResource* tex_res1;
  acGraphics::Font* font;
  graphics::Sprite my_sprite;
  graphics::shape::Arc my_arc1;
  graphics::shape::Arc my_arc2;
  aurora::tiles::Map map;
};