#include "../game.h"
#include <WinCore/delegation/delegation.h>
#include <functional>
#include <Aurora/Code/animation/easing_functions.h>

namespace my_app {




aurora::animation::Custom motion1;
int _y = 0;
int p_y = 0;
void MainMenu::OnStateEnter() { 


  cursor.Initialize(&engine_->gfx_context());
  cursor.SetSize(53,29);
  cursor.SetTopLeft(300,_y);
  cursor.Construct();
  cursor.BuildTransform();


  engine_->animation().AddAnimation(&motion1);

}

void MainMenu::OnStateExit() { 

}

void MainMenu::OnInput() {
  if (this->engine()->input().keyboard_buffer[VK_DOWN] == true) {
    p_y = (int)cursor.y();
    _y += 10 - ((int)p_y % 10);
    if (_y > 460) _y = 460;
    motion1.set_function([=] (float dt,float ct,float mt) {
      float y = aurora::animation::easing::in_out_cubic((float)ct,p_y,_y,(float)mt);
      cursor.SetTopLeft(300,y);
    }).set_max_time(300);
    motion1.Play();
  }
  if (this->engine()->input().keyboard_buffer[VK_UP] == true) {
    p_y = (int)cursor.y();
    _y -= 10 - ((int)p_y % 10);
    if (_y < 0) _y = 0;
    motion1.set_function([=] (float dt,float ct,float mt) {
      float y = aurora::animation::easing::in_out_cubic((float)ct,0,p_y-_y,(float)mt);
      cursor.SetTopLeft(300,p_y-y);
    }).set_max_time(300);
    motion1.Play();
  }
}

void MainMenu::OnUpdate(float delta_time) {
   
  cursor.BuildTransform();
}

float xtime = 0;
void MainMenu::OnInterpolate(float delta_time) {
  graphics::font::Writer& fw = game_->font_writer();
  xtime += delta_time;

  if (xtime >= 3000.0f) {
    //this->state_manager_->ChangeState(1);
    xtime = 3000.0f;
  }

  char str[255];
  sprintf_s(str,"y %d\np_y %d",_y,p_y);
  fw.WriteBox(str,strlen(str),acGraphics::FONT_ALIGN_LEFT,300);

  float center_x = engine_->gfx_context().width() * 0.5f;
  float center_y = engine_->gfx_context().height() * 0.5f;
  center_x -= 150.0f;//fw.font()->GetTextWidth(str,strlen(str)) * 0.5f;
  center_y -= fw.font()->GetHeight() * 0.5f;
  //fw.SetTopLeft(center_x,center_y);
  fw.SetOrder(100);
  fw.BuildTransform();
}

void MainMenu::OnDraw() {
  game_->shader_helper().PrepareDraw();
  game_->shader_helper().SetTexturePagesShader();
  game_->shader_helper().UpdateChangesEveryFrame(&game_->font_writer().misc_buffer_shader_);
  game_->font_writer().Draw(); 

  

  register graphics::shader::ConstantBuffer2Type cb;
  auto sprite_texture = engine_->resource_manager.GetResourceById<aurora::resource::TextureResource>(102);
  ID3D11ShaderResourceView* srv = sprite_texture->srv();
  game_->shader_helper().SetTextureShader();
  engine_->gfx_context().SetShaderResources(graphics::kShaderTypePixel,0,1,(void**)&(srv));
  cb.world = XMMatrixTranspose( cursor.world() );
  cb.ps_color = cursor.color();
  cb.enable_texture = false;
  game_->shader_helper().UpdateChangesEveryFrame(&cb);
  cursor.Draw();
}

}