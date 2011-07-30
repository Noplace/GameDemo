#include "scene2.h"
#include <VisualEssence/Code/font/bmfont/font_loader.h>

struct SimpleVertex
{
    XMFLOAT2 Pos;
    XMFLOAT2 Tex;
    XMFLOAT4 Color;
};


struct CBChangesEveryFrame
{
    XMMATRIX mWorld;
    XMFLOAT4 vMeshColor;
};

extern ID3D11BlendState* blend_state;
extern XMFLOAT4    g_vMeshColor;
ID3D11DepthStencilState* m_depthDisabledStencilState = NULL;


int MainScene2::Initialize(game_engine::Engine* engine) { 
  GameView::Initialize(engine);

  game_engine::resource::Resource* shader_res = engine_->resource_manager.GetResourceById(3);

  main_effect_.Initialize(&engine_->gfx_context());
  
  {
    font_effect_.Initialize(&engine_->gfx_context());
    {
      game_engine::resource::Resource* font_shader_res = engine_->resource_manager.GetResourceById(4);
      int hr = font_effect_.CreateFromMemory(font_shader_res->data_pointer,font_shader_res->data_length);
    }
  

    font_writer_.Initialize(&engine_->gfx_context());
    {
      game_engine::resource::FontResource* fr = (game_engine::resource::FontResource*)engine_->resource_manager.GetResourceById(7);
      //acGraphics::FontLoaderBinaryFormat font_loader(engine_->resource_manager.GetResourceById(7)->filename());
      font = fr->font();//font_loader.GenerateFont();
      game_engine::resource::TextureResource* p1 = (game_engine::resource::TextureResource*)engine_->resource_manager.GetResourceById(5);
      game_engine::resource::TextureResource* p2 = (game_engine::resource::TextureResource*)engine_->resource_manager.GetResourceById(6);
      font->pages[0] = (p1->srv());
      font->pages[1] = (p2->srv());

      font_writer_.set_font(font);
      font_writer_.set_effect(&font_effect_);
    }
  }

  int hr;
  hr = main_effect_.CreateFromMemory(shader_res->data_pointer,shader_res->data_length);
  if (FAILED(hr)) {
    return hr;
  }

  D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
  // Clear the second depth stencil state before setting the parameters.
  ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

  // Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
  // that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
  depthDisabledStencilDesc.DepthEnable = false;
  depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
  depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
  depthDisabledStencilDesc.StencilEnable = true;
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

  //D3D11_BUFFER_DESC bd;
  //ZeroMemory( &bd, sizeof(bd) );
  {
  
    g_vb.description.bind_flags = D3D11_BIND_VERTEX_BUFFER;
    g_vb.description.usage = D3D11_USAGE_DEFAULT;
    g_vb.description.byte_width = sizeof( SimpleVertex ) * 8;
    g_vb.description.cpu_access_flags = 0;
    engine_->gfx_context().CreateBuffer(g_vb,NULL);
    //uint32_t index,uint32_t type,
    //graphics::BufferSubresource subresource;
    engine_->gfx_context().CopyToVertexBuffer(g_vb,vertices,sizeof(SimpleVertex),0,4);
    engine_->gfx_context().CopyToVertexBuffer(g_vb,vertices2,sizeof(SimpleVertex),4,4);

    //engine_->gfx_context().device_context()->UpdateSubresource((ID3D11Resource*)g_vb.internal_pointer,0,NULL,vertices,sizeof(SimpleVertex),0);
  }

  

  
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
  motion1.set_from(0);
  motion1.set_to(400);
  motion1.set_max_time(1000000);
  motion1.set_value_ptr(&move_z);
  engine_->animation().tween_list.push_back(&motion1);
  move_z=0;
  motion1.Play();

  font_writer_.PrepareWrite(51);
  
  return S_OK; 
}

int MainScene2::Deinitialize() { 
  GameView::Deinitialize();

//  if( g_pTextureRV ) g_pTextureRV->Release();

  if (engine_) {
    engine_->gfx_context().DestroyBuffer(g_pCBChangesEveryFrame);
    engine_->gfx_context().DestroyBuffer(g_vb);
    engine_->gfx_context().DestroyBuffer(g_ib);
  }
  SafeRelease(&blend_state);
  SafeRelease(&m_depthDisabledStencilState);
  main_effect_.Deinitialize();
  font_writer_.Deinitialize();
  return S_OK;
}

char status1[20];

void MainScene2::Draw() {

  
  //engine_->gfx_context().device_context()->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
  UINT stride = sizeof( SimpleVertex );
  UINT offset = 0;
  engine_->gfx_context().SetVertexBuffers(0,1,&g_vb,&stride,&offset);
  engine_->gfx_context().ClearIndexBuffer();
  engine_->gfx_context().SetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

  // Update our time
  static float t = 0.0f;
 
  {
      static DWORD dwTimeStart = 0;
      DWORD dwTimeCur = (DWORD)engine_->global_time();
      if( dwTimeStart == 0 )
          dwTimeStart = dwTimeCur;
      t = ( dwTimeCur - dwTimeStart ) / 1000000.0f;
      sprintf(status1,"T: %f",t);
      font_writer_.Write(0,0,0,status1,strlen(status1),0);
  }

  //camera_.view() = XMMatrixScaling(t,t,0);// * XMMatrixTranslation(0.5,0,0);
  //camera_.UpdateConstantBuffer();

  const XMVECTOR vAxisZ = { 0, 0.0f, 2.0f, 0 };
  // Rotate cube around the origin
  world = XMMatrixTranslation(move_z,0,0);// * XMMatrixRotationAxis(vAxisZ,t);//XMMatrixRotationY( t );
  
  // Modify the color
  g_vMeshColor.x = ( sinf( t * 1.0f ) + 1.0f ) * 0.5f;
  g_vMeshColor.y = ( cosf( t * 3.0f ) + 1.0f ) * 0.5f;
  g_vMeshColor.z = ( sinf( t * 5.0f ) + 1.0f ) * 0.5f;





  main_effect_.Begin();
  camera_.SetConstantBuffer(0);

  CBChangesEveryFrame cb;
  cb.mWorld = XMMatrixTranspose( world );
  cb.vMeshColor = g_vMeshColor;
  engine_->gfx_context().UpdateBuffer(g_pCBChangesEveryFrame,&cb,NULL,0,0);



  // Render the cube
  //
  //engine_->gfx_context().SetVertexShader(vertex_shader_);
    

  //((graphics::ContextD3D11*)&engine_->gfx_context())->device_context()->PSSetSamplers(0,1,NULL);
  engine_->gfx_context().SetConstantBuffers(graphics::kShaderTypeVertex,2,1,&g_pCBChangesEveryFrame);
  engine_->gfx_context().SetConstantBuffers(graphics::kShaderTypePixel,2,1,&g_pCBChangesEveryFrame);
  //((graphics::ContextD3D11*)&engine_->gfx_context())->device_context()->VSSetConstantBuffers( 2, 1, &g_pCBChangesEveryFrame );
  //((graphics::ContextD3D11*)&engine_->gfx_context())->device_context()->PSSetConstantBuffers( 2, 1, &g_pCBChangesEveryFrame );
  //ID3D11ShaderResourceView* src = tex_res1->srv();
  //((graphics::ContextD3D11*)&engine_->gfx_context())->device_context()->PSSetShaderResources(0,1,&font_writer_.font()->pages[0]);
  //((graphics::ContextD3D11*)&engine_->gfx_context())->device_context()->Draw(4,0);


  //world = XMMatrixTranslation(move_z,200,0);
  //cb.mWorld = XMMatrixTranspose( world );
  //engine_->gfx_context().UpdateBuffer(g_pCBChangesEveryFrame,&cb,NULL,0,0);

 // ((graphics::ContextD3D11*)&engine_->gfx_context())->device_context()->Draw(4,4);


  

  font_writer_.Draw(20);
}



