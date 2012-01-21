#include "../game.h"

namespace my_app {

void Level::OnStateEnter() { 

  int hr;

    
  my_sprite.Initialize(&engine_->gfx_context());
  my_sprite.SetSize(32,32);
  my_sprite.SetTopLeft(300,100);
  my_sprite.SetColor(XMCOLOR(0xffffffff));
  float uv[4] = {0,0,1,1};//float(48.0f/228) , float(170.0f/224) , float(72.0f/228) , float(195.0f/224)  };
  my_sprite.SetUV(uv[0],uv[1],uv[2],uv[3]);
  my_sprite.SetOrder(101);
  my_sprite.Construct();
  my_sprite.BuildTransform();

  pongsystem.set_engine(this->engine());
  pongsystem.Initialize();

}

void Level::OnStateExit() { 
  pongsystem.Deinitialize();
  my_sprite.Deinitialize();
}


void Level::OnUpdate(float delta_time) {
  pongsystem.Update(delta_time);
}

void Level::OnInterpolate(float dt) {

  // Update our time
  static float t = 0.0f;
  auto ctrl = &engine_->input().controller1;
  {
    t += (3.14f / 5000);
    my_sprite.SetRotate(t);

    
    float f = engine_->total_time();
    int ms = ((int)engine_->total_time()) % 1000;
    int sec = (int)(engine_->total_time()/1000) % 60;
    int min = (int)(engine_->total_time()/60000) % 60;
    char status1[512];
    char input_info[255];
    
    sprintf(input_info,"Controller 1\nEnabled: %d\nA: %d\nB: %d\nUp: %d\nLeft Trig: %0.6f\nRight T D: %0.6f\nRight T M: %0.6f",
      ctrl->enabled(),
      ctrl->A(),
      ctrl->B(),
      ctrl->Up(),
      ctrl->LeftTrigger(),
      ctrl->RightThumbDirection(),
      ctrl->RightThumbMagnitude());
    

    sprintf(status1,"FPS\t: %d\nTime\t: %02u:%02u:%03u\n%s",engine_->fps(),min,sec,ms,input_info);
    game_->font_writer().WriteML(status1,strlen(status1),0);
  }


  {
    XMFLOAT2 pos;
    XMVECTOR ipos = XMVectorLerp(pongsystem.ball.prev_position,pongsystem.ball.position,dt);
    XMStoreFloat2(&pos,ipos);
    //my_sprite.world() = XMMatrixTranslation(centerx+move_x,centery+move_y,10);
    my_sprite.SetTopLeft(pos.x-pongsystem.ball.radius,pos.y-pongsystem.ball.radius);//centerx+move_x,centery+move_y);
    my_sprite.BuildTransform();
  }

  
  game_->font_writer().SetOrder(100);
  game_->font_writer().BuildTransform();
}

void Level::OnDraw() {


  game_->shader_helper().PrepareDraw();
  game_->shader_helper().SetColorShader();

  graphics::shader::ConstantBuffer2Type cb;



  cb.world = XMMatrixTranspose( pongsystem.paddle1.shape.world() );
  cb.ps_color = pongsystem.paddle1.shape.color();
  cb.enable_texture = false;
  game_->shader_helper().UpdateChangesEveryFrame(&cb);
  pongsystem.paddle1.shape.Draw();

  cb.world = XMMatrixTranspose( pongsystem.paddle2.shape.world() );
  cb.ps_color = pongsystem.paddle2.shape.color();
  cb.enable_texture = false;
  game_->shader_helper().UpdateChangesEveryFrame(&cb);
  pongsystem.paddle2.shape.Draw();

  auto sprite_texture = engine_->resource_manager.GetResourceById<aurora::resource::TextureResource>(101);
  ID3D11ShaderResourceView* srv = sprite_texture->srv();
  game_->shader_helper().SetTextureShader();
  engine_->gfx_context().SetShaderResources(graphics::kShaderTypePixel,0,1,(void**)&(srv));
  cb.world = XMMatrixTranspose( my_sprite.world() );
  cb.ps_color = my_sprite.color();
  cb.enable_texture = false;
  game_->shader_helper().UpdateChangesEveryFrame(&cb);
  my_sprite.Draw();

  game_->shader_helper().SetTexturePagesShader();
  game_->shader_helper().UpdateChangesEveryFrame(&game_->font_writer().misc_buffer_shader_);
  game_->font_writer().Draw();  
}

}



