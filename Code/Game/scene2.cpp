#include "scene2.h"

struct SimpleVertex
{
    XMFLOAT2 Pos;
    XMFLOAT2 Tex;
    XMFLOAT4 Color;
};

struct CBChangesEveryFrame
{
    XMMATRIX world;
    XMVECTOR ps_color;
};

extern ID3D11BlendState* blend_state;
extern XMFLOAT4    g_vMeshColor;
ID3D11DepthStencilState* m_depthDisabledStencilState = NULL;


int MainScene2::Initialize(game_engine::Engine* engine) { 
  GameView::Initialize(engine);

  game_engine::resource::EffectResource* shader_res = engine_->resource_manager.GetResourceById<game_engine::resource::EffectResource>(3);
  main_effect_ = shader_res->effect();

  {
    font_writer_.Initialize(&engine_->gfx_context());
    game_engine::resource::EffectResource* font_effect_ = engine_->resource_manager.GetResourceById<game_engine::resource::EffectResource>(4);
    game_engine::resource::FontResource* fr = engine_->resource_manager.GetResourceById<game_engine::resource::FontResource>(7);
    font = fr->font();
    font_writer_.set_font(font);
    font_writer_.set_effect(font_effect_->effect());
  }

  int hr;
  

  D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
  // Clear the second depth stencil state before setting the parameters.
  ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

  // Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
  // that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
  depthDisabledStencilDesc.DepthEnable = true;
  depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
  depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
  depthDisabledStencilDesc.StencilEnable = false;
  depthDisabledStencilDesc.StencilReadMask = 0xFF;
  depthDisabledStencilDesc.StencilWriteMask = 0xFF;
  depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
  depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
  depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
  depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the state using the device.
  hr = engine_->gfx_context().device()->CreateDepthStencilState(&depthDisabledStencilDesc, &m_depthDisabledStencilState);
	if (FAILED(hr)){
		return hr;
	}

  D3D11_BLEND_DESC BlendStateDescription;
  ZeroMemory(&BlendStateDescription,sizeof(BlendStateDescription));
  BlendStateDescription.AlphaToCoverageEnable = false;
  BlendStateDescription.RenderTarget[0].BlendEnable = true;

  BlendStateDescription.RenderTarget[0].SrcBlend                  = D3D11_BLEND_SRC_ALPHA;        //D3D11_BLEND_SRC_COLOR;
  BlendStateDescription.RenderTarget[0].DestBlend                 = D3D11_BLEND_INV_SRC_ALPHA;//D3D11_BLEND_DEST_COLOR;
  BlendStateDescription.RenderTarget[0].SrcBlendAlpha             = D3D11_BLEND_ONE;//D3D11_BLEND_SRC_ALPHA;
  BlendStateDescription.RenderTarget[0].DestBlendAlpha    = D3D11_BLEND_ONE;//D3D11_BLEND_DEST_ALPHA;
  BlendStateDescription.RenderTarget[0].BlendOp                   = D3D11_BLEND_OP_ADD;
  BlendStateDescription.RenderTarget[0].BlendOpAlpha              = D3D11_BLEND_OP_ADD;
  BlendStateDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    
  engine_->gfx_context().device()->CreateBlendState(&BlendStateDescription,&blend_state);
  float blendFactor[] = {1,1, 1, 1};
      UINT sampleMask   = 0xffffffff;
  engine_->gfx_context().device_context()->OMSetBlendState(blend_state,blendFactor,sampleMask);
    

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
  my_sprite.SetSize(100,100);
  my_sprite.SetTopLeft(100,100);
  my_sprite.SetColor(XMCOLOR(0xffffff10));
  my_sprite.SetUV(0,0,1,1);
  my_sprite.Construct();
  my_sprite.BuildTransform();


  my_arc1.Initialize(&engine_->gfx_context());
  my_arc1.SetParams(100,0,2,7);
  my_arc1.SetTopLeft(400,100);
  my_arc1.SetColor(XMCOLOR(0xffffff10));
  my_arc1.Construct();
  my_arc1.BuildTransform();

  my_arc2.Initialize(&engine_->gfx_context());
  my_arc2.SetParams(90,0.3,1.6,7);
  my_arc2.SetTopLeft(400,100);
  my_arc2.SetColor(XMCOLOR(0xffffff10));
  my_arc2.Construct();
  my_arc2.BuildTransform();

  
  g_pCBChangesEveryFrame.description.bind_flags = D3D11_BIND_CONSTANT_BUFFER;
  g_pCBChangesEveryFrame.description.usage = D3D11_USAGE_DEFAULT;
  g_pCBChangesEveryFrame.description.byte_width = sizeof(CBChangesEveryFrame);
  g_pCBChangesEveryFrame.description.cpu_access_flags = 0;
  hr = engine_->gfx_context().CreateBuffer(g_pCBChangesEveryFrame,NULL);
  if( FAILED( hr ) )
      return hr;

  // Load the Texture
  //tex_res1 = (game_engine::resource::TextureResource*)engine_->resource_manager.GetResourceById(5);
  


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
  SafeRelease(&blend_state);
  SafeRelease(&m_depthDisabledStencilState);

  font_writer_.Deinitialize();
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
    font_writer_.WriteML(0,0,0,status1,strlen(status1),0);
  }

    //camera_.view() = XMMatrixScaling(t,t,0);// * XMMatrixTranslation(0.5,0,0);
  //camera_.UpdateConstantBuffer();

  const XMVECTOR vAxisZ = { 0, 0.0f, 2.0f, 0 };
  // Rotate cube around the origin
  
  uint32_t centerx = this->engine_->gfx_context().width()/2;
  uint32_t centery = this->engine_->gfx_context().height()/2;
  world = XMMatrixTranslation(centerx+move_x,centery+move_y,0);// * XMMatrixRotationAxis(vAxisZ,t);//XMMatrixRotationY( t );

  // Modify the color
  g_vMeshColor.x = ( sinf( t * 1.0f ) + 1.0f ) * 0.5f;
  g_vMeshColor.y = ( cosf( t * 3.0f ) + 1.0f ) * 0.5f;
  g_vMeshColor.z = ( sinf( t * 5.0f ) + 1.0f ) * 0.5f;

  
  my_sprite.BuildTransform();


  my_arc1.BuildTransform();
  my_arc2.BuildTransform();
}

void MainScene2::Draw() {

  
  //engine_->gfx_context().device_context()->OMSetDepthStencilState(m_depthDisabledStencilState, 0);
  UINT stride = sizeof( SimpleVertex );
  UINT offset = 0;
  engine_->gfx_context().SetVertexBuffers(0,1,&g_vb,&stride,&offset);
  engine_->gfx_context().ClearIndexBuffer();
  engine_->gfx_context().SetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

  main_effect_->Begin();
  camera_.SetConstantBuffer(0);

  CBChangesEveryFrame cb;
  cb.world = XMMatrixTranspose( world );
  cb.ps_color = XMLoadFloat4(&g_vMeshColor);
  engine_->gfx_context().UpdateBuffer(g_pCBChangesEveryFrame,&cb,NULL,0,0);
    
  //((graphics::ContextD3D11*)&engine_->gfx_context())->device_context()->PSSetSamplers(0,1,NULL);
  engine_->gfx_context().SetConstantBuffers(graphics::kShaderTypeVertex,2,1,&g_pCBChangesEveryFrame);
  engine_->gfx_context().SetConstantBuffers(graphics::kShaderTypePixel,2,1,&g_pCBChangesEveryFrame);
  //((graphics::ContextD3D11*)&engine_->gfx_context())->device_context()->VSSetConstantBuffers( 2, 1, &g_pCBChangesEveryFrame );
  //((graphics::ContextD3D11*)&engine_->gfx_context())->device_context()->PSSetConstantBuffers( 2, 1, &g_pCBChangesEveryFrame );
  //ID3D11ShaderResourceView* src = tex_res1->srv();
  //((graphics::ContextD3D11*)&engine_->gfx_context())->device_context()->PSSetShaderResources(0,1,&font_writer_.font()->pages[0]);
  engine_->gfx_context().Draw(4,0);


  cb.world = XMMatrixTranspose( my_arc1.world() );
  cb.ps_color = my_arc1.color();
  engine_->gfx_context().UpdateBuffer(g_pCBChangesEveryFrame,&cb,NULL,0,0);
  my_arc1.Draw();

  cb.world = XMMatrixTranspose( my_arc2.world() );
  cb.ps_color = my_arc2.color();
  engine_->gfx_context().UpdateBuffer(g_pCBChangesEveryFrame,&cb,NULL,0,0);
  my_arc2.Draw();


  cb.world = XMMatrixTranspose( my_sprite.world() );
  cb.ps_color = my_sprite.color();
  engine_->gfx_context().UpdateBuffer(g_pCBChangesEveryFrame,&cb,NULL,0,0);

  my_sprite.Draw();

  
  
  font_writer_.Draw(40);
}



