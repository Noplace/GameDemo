#include "scene2.h"

struct SimpleVertex
{
    XMFLOAT2 Pos;
    XMFLOAT2 Tex;
    XMFLOAT4 Color;
};

struct ConstantBuffer2Type {
    XMMATRIX world;
    bool enable_texture;
    XMVECTOR ps_color;
};

int MainScene2::Initialize(aurora::Engine* engine) { 
  GameView::Initialize(engine);

  //aurora::resource::EffectResource* shader_res = engine_->resource_manager.GetResourceById<aurora::resource::EffectResource>(3);
  {

    aurora::resource::Resource* shader_res = engine_->resource_manager.GetResourceById<aurora::resource::Resource>(3);
    graphics::Effect::Entry entry_list[] = {
      {"VS","vs_5_0"},
      {"PS","ps_5_0"},
      {"PSTex","ps_5_0"},
      {NULL,NULL},
    };
    main_effect_.Initialize(&engine_->gfx_context());
    main_effect_.CreateFromMemory2(entry_list,shader_res->data_pointer,shader_res->data_length);
    shader_res->Unload();
  }

  {
    font_writer_.Initialize(&engine_->gfx_context());
    aurora::resource::EffectResource* font_effect_ = engine_->resource_manager.GetResourceById<aurora::resource::EffectResource>(4);
    aurora::resource::FontResource* fr = engine_->resource_manager.GetResourceById<aurora::resource::FontResource>(7);
    font = fr->font();
    font_writer_.set_font(font);
    font_writer_.set_effect(font_effect_->effect());
  }

  int hr;
  


 

  camera_.Ortho2D();
  
  // Create vertex buffer
  SimpleVertex vertices[] =
  {
    { XMFLOAT2( 1.0f, -1.0f ), XMFLOAT2( 0.72070313f, 0.65820313f  ),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },  
    { XMFLOAT2( 13.0f, -1.0f ), XMFLOAT2( 0.76757813f, 0.65820313f ),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },
    { XMFLOAT2( 1.0f, 20.0f ), XMFLOAT2( 0.72070313f, 0.74023438f ),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },
    { XMFLOAT2( 13.0f, 20.0f ), XMFLOAT2( 0.76757813f, 0.74023438f ),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },

  };

  SimpleVertex vertices2[] =
  {
    { camera_.OrthoPoint( 0.0f, 0.0f,0.0f, 0.0f ), XMFLOAT2( 0.0f, 0.0f ),XMFLOAT4(1.0f,0.0f,1.0f,1.0f) },  
    { camera_.OrthoPoint( 1.0f, -40.0f,0.0f, 0.0f ), XMFLOAT2( 0.5f, 0.0f ),XMFLOAT4(1.0f,1.0f,0.0f,1.0f) },
    { camera_.OrthoPoint( 0.0f, 0.0f,0.0f, 70.0f ), XMFLOAT2( 0.0f, 0.5f ),XMFLOAT4(0.0f,1.0f,1.0f,1.0f) },
    { camera_.OrthoPoint( 1.0f, -40.0f,0.0f, 70.0f ), XMFLOAT2( 0.5f, 0.5f ),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },

  };
  
 
  
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

  g_pCBChangesEveryFrame.description.bind_flags = D3D11_BIND_CONSTANT_BUFFER;
  g_pCBChangesEveryFrame.description.usage = D3D11_USAGE_DEFAULT;
  g_pCBChangesEveryFrame.description.byte_width = sizeof(ConstantBuffer2Type);
  g_pCBChangesEveryFrame.description.cpu_access_flags = 0;
  hr = engine_->gfx_context().CreateBuffer(g_pCBChangesEveryFrame,NULL);
  if( FAILED( hr ) )
      return hr;

  // Load the Texture
  //tex_res1 = (aurora::resource::TextureResource*)engine_->resource_manager.GetResourceById(5);
  


  // Initialize the world matrices
  world = XMMatrixIdentity();

  camera_.UpdateConstantBuffer();
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

  font_writer_.PrepareWrite(51);
  
  return S_OK; 
}

int MainScene2::Deinitialize() { 
  GameView::Deinitialize();

//  if( g_pTextureRV ) g_pTextureRV->Release();
  my_sprite.Deinitialize();
  if (engine_) {
    engine_->gfx_context().DestroyBuffer(g_pCBChangesEveryFrame);
    engine_->gfx_context().DestroyBuffer(g_vb);
    engine_->gfx_context().DestroyBuffer(g_ib);
  }

  font_writer_.Deinitialize();

  main_effect_.Deinitialize();
  return S_OK;
}



void MainScene2::Update(float delta_time) {
    // Update our time
  static float t = 0.0f;
 
  {
    t += (3.14f / 5000);
    my_sprite.SetRotate(t);
    //my_arc1.SetRotate(t);
    my_arc2.SetRotate(-t);
    float f = engine_->total_time();
    int ms = ((int)engine_->total_time()) % 1000;
    int sec = (int)(engine_->total_time()/1000) % 60;
    int min = (int)(engine_->total_time()/60000) % 60;
    char status1[50];
    sprintf(status1,"FPS\t: %d\nTime\t: %02u:%02u:%03u",engine_->fps(),min,sec,ms);
    font_writer_.WriteML(status1,strlen(status1),0);
  }

    //camera_.view() = XMMatrixScaling(t,t,0);// * XMMatrixTranslation(0.5,0,0);
  //camera_.UpdateConstantBuffer();

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

  
  //main_effect_->Begin();
  
  engine_->gfx_context().SetInputLayout(main_effect_.input_layout());
  engine_->gfx_context().SetShader(main_effect_.vs(0));
  engine_->gfx_context().SetShader(main_effect_.ps(0));

  engine_->gfx_context().ClearShader(graphics::kShaderTypeGeometry);
  camera_.SetConstantBuffer(0);

  ConstantBuffer2Type cb;

  engine_->gfx_context().SetConstantBuffers(graphics::kShaderTypeVertex,2,1,&g_pCBChangesEveryFrame);
  engine_->gfx_context().SetConstantBuffers(graphics::kShaderTypePixel,2,1,&g_pCBChangesEveryFrame);

  cb.world = XMMatrixTranspose( my_arc1.world() );
  cb.ps_color = my_arc1.color();
  cb.enable_texture = false;
  engine_->gfx_context().UpdateSubresource(g_pCBChangesEveryFrame,&cb,NULL,0,0);
  my_arc1.Draw();

  cb.world = XMMatrixTranspose( my_arc2.world() );
  cb.ps_color = my_arc2.color();
  cb.enable_texture = false;
  engine_->gfx_context().UpdateSubresource(g_pCBChangesEveryFrame,&cb,NULL,0,0);
  my_arc2.Draw();


  cb.world = XMMatrixTranspose( map.world() );
  cb.ps_color = XMLoadColor(&XMCOLOR(0xffffffff));
  cb.enable_texture = false;
  engine_->gfx_context().UpdateSubresource(g_pCBChangesEveryFrame,&cb,NULL,0,0);
  //map.Draw();


  aurora::resource::TextureResource* sprite_texture = engine_->resource_manager.GetResourceById<aurora::resource::TextureResource>(100);
  ID3D11ShaderResourceView* srv = sprite_texture->srv();
  engine_->gfx_context().SetShader(main_effect_.ps(1));
  engine_->gfx_context().SetShaderResources(graphics::kShaderTypePixel,0,1,(void**)&(srv));
  cb.world = XMMatrixTranspose( my_sprite.world() );
  cb.ps_color = my_sprite.color();
  cb.enable_texture = true;
  engine_->gfx_context().UpdateSubresource(g_pCBChangesEveryFrame,&cb,NULL,0,0);
  my_sprite.Draw();
  

  font_writer_.UpdateConstantBuffer();
  font_writer_.Draw();  
}



