#include <Aurora/Code/aurora.h>
#include <Aurora/Code/game_view.h>
#include <Aurora/Code/tiles/tiles.h>
#include <Aurora/Code/gui/menu.h>
#include <VisualEssence/Code/shape/arc.h>
#include <VisualEssence/Code/shape/rectangle.h>
#include <VisualEssence/Code/shader/shader2d_helper.h>
#include "Pong/pong.h"

class MainScene2 : public aurora::GameView {
 public:
  MainScene2() { }
  ~MainScene2() {}
  int Initialize(aurora::Engine* engine);
  int Deinitialize();
  void Update(float delta_time);
  void Draw();
 private:
 
  float move_z,move_x,move_y;
  aurora::animation::CubicMotion motion1;
  aurora::animation::Spiral spiral;
  graphics::font::Writer font_writer_;
  graphics::Buffer                    g_vb;
  graphics::Buffer                    g_ib;
  XMMATRIX world;
  aurora::resource::TextureResource* tex_res1;
  acGraphics::Font* font;
  graphics::Sprite my_sprite;
  graphics::shape::Rectangle paddle1;
  graphics::shape::Rectangle paddle2;
  graphics::shape::Arc my_arc1;
  graphics::shape::Arc my_arc2;
  aurora::tiles::Map map;
  aurora::gui::Gui gui_;
  aurora::gui::Menu mainmenu;
  graphics::shader::Shader2DHelper shader_helper;
  PongSystem pongsystem;
};