#include "scene2.h"

int MainScene2::Initialize(aurora::Engine* engine) { 
  GameView::Initialize(engine);

  gui_.set_engine(engine);
  engine->process_manager().AddProcess(&gui_);
  shader_helper.Initialize(&engine->gfx_context());

  {
    aurora::resource::Resource* shader_res = engine_->resource_manager.GetResourceById<aurora::resource::Resource>(3);
    shader_helper.CreateFromMemory(shader_res->data_pointer,shader_res->data_length);
    shader_res->Unload();
  }

  {
    font_writer_.Initialize(&engine_->gfx_context());
    aurora::resource::FontResource* fr = engine_->resource_manager.GetResourceById<aurora::resource::FontResource>(7);
    font = fr->font();
    font_writer_.set_font(font);
  }

  int hr;
  
  mainmenu.set_font(font);
  mainmenu.AddItem("Item 1");
  mainmenu.AddItem("Item 2");
  mainmenu.AddItem("Item 2");

  gui_.AddElement(&mainmenu);
 

    
  my_sprite.Initialize(&engine_->gfx_context());
  my_sprite.SetSize(20,25);
  my_sprite.SetTopLeft(300,100);
  my_sprite.SetColor(XMCOLOR(0xffffffff));
  float uv[4] = {float(48.0f/228) , float(170.0f/224) , float(72.0f/228) , float(195.0f/224)  };
  my_sprite.SetUV(uv[0],uv[1],uv[2],uv[3]);
  my_sprite.SetOrder(-1);
  my_sprite.Construct();
  my_sprite.BuildTransform();


  my_arc1.Initialize(&engine_->gfx_context());
  my_arc1.SetParams(100,0,2,7);
  my_arc1.SetTopLeft(400,100);
  my_arc1.SetColor(XMCOLOR(0xffffff10));
  my_arc1.Construct();
  my_arc1.SetOrder(-2);
  my_arc1.BuildTransform();

  my_arc2.Initialize(&engine_->gfx_context());
  my_arc2.SetParams(90,0.3f,1.6f,7);
  my_arc2.SetTopLeft(400,100);
  my_arc2.SetColor(XMCOLOR(0xffffff10));
  my_arc2.Construct();
  my_arc2.BuildTransform();

  
  map.Initialize(&engine_->gfx_context(),20,20,16,16);
  map.AddLayer();
  map.Construct();


  // Load the Texture
  //tex_res1 = (aurora::resource::TextureResource*)engine_->resource_manager.GetResourceById(5);
  


  // Initialize the world matrices
  world = XMMatrixIdentity();

  
  motion1.set_context(&engine_->animation());
  motion1.set_delay_time(300);
  motion1.set_from(0);
  motion1.set_to(3.14);
  motion1.set_max_time(1000);
  motion1.set_repeat_count(1000);
  motion1.set_value_ptr(my_arc1.angle_ptr());
  engine_->animation().tween_list.push_back(&motion1);
  move_z=0;
  motion1.Play();


  spiral.set_context(&engine_->animation());
  spiral.set_amplitude(5);
  spiral.set_max_time(1000);
  spiral.set_repeat_count(1000);
  float* ptrs[] = {&move_x,&move_y};
  spiral.set_value_ptrs(ptrs);
  engine_->animation().tween_list.push_back(&spiral);
  spiral.Play();

  font_writer_.PrepareWrite(512);
  
  return S_OK; 
}

int MainScene2::Deinitialize() { 
  GameView::Deinitialize();

//  if( g_pTextureRV ) g_pTextureRV->Release();
  my_sprite.Deinitialize();
  if (engine_) {
    engine_->gfx_context().DestroyBuffer(g_vb);
    engine_->gfx_context().DestroyBuffer(g_ib);
  }

  font_writer_.Deinitialize();

  shader_helper.Deinitialize();
  return S_OK;
}



void MainScene2::Update(float delta_time) {
    // Update our time
  static float t = 0.0f;
 aurora::input::Controller* ctrl = &engine_->input().controller1;
  {
    t += (3.14f / 5000);
    my_sprite.SetRotate(t);
    //my_arc1.SetRotate(t);
    my_arc2.SetRotate(-ctrl->RightThumbDirection());
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
    font_writer_.WriteML(status1,strlen(status1),0);
  }


  const XMVECTOR vAxisZ = { 0, 0.0f, 2.0f, 0 };
  // Rotate cube around the origin
  
  uint32_t centerx = this->engine_->gfx_context().width()/2;
  uint32_t centery = this->engine_->gfx_context().height()/2;
  world = XMMatrixTranslation(centerx+move_x,centery+move_y,0);// * XMMatrixRotationAxis(vAxisZ,t);//XMMatrixRotationY( t );

  //my_sprite.world() = XMMatrixTranslation(centerx+move_x,centery+move_y,10);
  //my_sprite.SetTopLeft(centerx+move_x,centery+move_y);
  my_sprite.BuildTransform();


  my_arc1.BuildTransform();
  my_arc2.BuildTransform();
  map.BuildTransform();
  font_writer_.BuildTransform();
}

void MainScene2::Draw() {


  
  //engine_->gfx_context().SetDepthState(null);

  /*UINT stride = sizeof( SimpleVertex );
  UINT offset = 0;
  engine_->gfx_context().SetVertexBuffers(0,1,&g_vb,&stride,&offset);
  engine_->gfx_context().ClearIndexBuffer();
  engine_->gfx_context().SetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
  */


  this->shader_helper.PrepareDraw();
  shader_helper.SetColorShader();

  graphics::shader::ConstantBuffer2Type cb;


  cb.world = XMMatrixTranspose( my_arc1.world() );
  cb.ps_color = my_arc1.color();
  cb.enable_texture = false;
  //engine_->gfx_context().UpdateSubresource(g_pCBChangesEveryFrame,&cb,NULL,0,0);
  shader_helper.UpdateChangesEveryFrame(&cb);
  my_arc1.Draw();

  cb.world = XMMatrixTranspose( my_arc2.world() );
  cb.ps_color = my_arc2.color();
  cb.enable_texture = false;
  //engine_->gfx_context().UpdateSubresource(g_pCBChangesEveryFrame,&cb,NULL,0,0);
  shader_helper.UpdateChangesEveryFrame(&cb);
  my_arc2.Draw();

  /*cb.world = XMMatrixTranspose( map.world() );
  cb.ps_color = XMLoadColor(&XMCOLOR(0xffffffff));
  cb.enable_texture = false;
  engine_->gfx_context().UpdateSubresource(g_pCBChangesEveryFrame,&cb,NULL,0,0);
  map.Draw();*/

  aurora::resource::TextureResource* sprite_texture = engine_->resource_manager.GetResourceById<aurora::resource::TextureResource>(100);
  ID3D11ShaderResourceView* srv = sprite_texture->srv();
  shader_helper.SetTextureShader();
  engine_->gfx_context().SetShaderResources(graphics::kShaderTypePixel,0,1,(void**)&(srv));
  cb.world = XMMatrixTranspose( my_sprite.world() );
  cb.ps_color = my_sprite.color();
  cb.enable_texture = true;
  shader_helper.UpdateChangesEveryFrame(&cb);
  my_sprite.Draw();


  shader_helper.SetTexturePagesShader();
  shader_helper.UpdateChangesEveryFrame(&font_writer_.misc_buffer_shader_);
  font_writer_.Draw();  

  //gui_.Draw();
}



