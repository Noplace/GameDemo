#include "scene1.h"
#include <WinCore/delegation/delegation.h>

core::delegation::none::Function<int> func;
namespace sup = core::delegation;


struct SimpleVertex
{
    XMFLOAT3 Pos;
    XMFLOAT2 Tex;
};


struct CBChangesEveryFrame
{
    XMMATRIX mWorld;
    XMFLOAT4 vMeshColor;
};


XMFLOAT4                            g_vMeshColor( 0.7f, 0.7f, 0.7f, 1.0f );
ID3D11BlendState* blend_state = NULL;

int test(int a) {
  return a*a;
}

int MainScene1::Initialize(game_engine::Engine* engine) { 
  GameView::Initialize(engine);
  
  //core::delegation::one::pair<graphics::Effect,int,void> p(&main_effect_,0);
  func.Bind<graphics::Effect>(main_effect_,&graphics::Effect::Begin);
  func = core::delegation::none::pair<graphics::Effect,int>(main_effect_,&graphics::Effect::Begin);

  game_engine::resource::EffectResource* res = engine_->resource_manager.GetResourceById<game_engine::resource::EffectResource>(2);
  main_effect_ = res->effect();
  /*main_effect_.Initialize(&engine_->gfx_context());
  int hr;
  hr = main_effect_.CreateFromMemory(res->data_pointer,res->data_length);
  if (FAILED(hr)) {
    return hr;
  }*/
  int hr;

  // Initialize the view matrix
  XMVECTOR Eye = XMVectorSet( 0.0f, 3.0f, -6.0f, 0.0f );
  XMVECTOR At = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
  XMVECTOR Up = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
  camera_.view() = XMMatrixLookAtLH( Eye, At, Up );
  camera_.Perspective();
  camera_.UpdateConstantBuffer();
  //camera_.Ortho2D();
  //camera_.view() = XMMatrixScaling(0.4f,0.4f,0.4f);// * XMMatrixTranslation(0.5,0,0);

  // Create vertex buffer
  SimpleVertex vertices[] =
  {
      { XMFLOAT3( -1.0f, 1.0f, -1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
      { XMFLOAT3( 1.0f, 1.0f, -1.0f ), XMFLOAT2( 0.5f, 0.0f ) },
      { XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT2( 0.5f, 0.5f ) },
      { XMFLOAT3( -1.0f, 1.0f, 1.0f ), XMFLOAT2( 0.2f, 0.5f ) },

      { XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
      { XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
      { XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
      { XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },

      { XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
      { XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
      { XMFLOAT3( -1.0f, 1.0f, -1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
      { XMFLOAT3( -1.0f, 1.0f, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },

      { XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
      { XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
      { XMFLOAT3( 1.0f, 1.0f, -1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
      { XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },

      { XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
      { XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
      { XMFLOAT3( 1.0f, 1.0f, -1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
      { XMFLOAT3( -1.0f, 1.0f, -1.0f ), XMFLOAT2( 0.0f, 1.0f ) },

      { XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
      { XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
      { XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
      { XMFLOAT3( -1.0f, 1.0f, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },
  };

  //D3D11_BUFFER_DESC bd;
  //ZeroMemory( &bd, sizeof(bd) );

  {
  
    g_vb.description.bind_flags = D3D11_BIND_VERTEX_BUFFER;
    g_vb.description.usage = D3D11_USAGE_DEFAULT;
    g_vb.description.byte_width = sizeof( SimpleVertex ) * 24;
    g_vb.description.cpu_access_flags = 0;
    engine_->gfx_context().CreateBuffer(g_vb,NULL);
    //uint32_t index,uint32_t type,
    //graphics::BufferSubresource subresource;
    engine_->gfx_context().device_context()->UpdateSubresource((ID3D11Resource*)g_vb.internal_pointer,0,NULL,vertices,sizeof(vertices),0);
  }

  WORD indices[] =
  {
      3,1,0,
      2,1,3,

      6,4,5,
      7,4,6,

      11,9,8,
      10,9,11,

      14,12,13,
      15,12,14,

      19,17,16,
      18,17,19,

      22,20,21,
      23,20,22
  };

  {

    g_ib.description.bind_flags = D3D11_BIND_INDEX_BUFFER;
    g_ib.description.usage = D3D11_USAGE_DEFAULT;
    g_ib.description.byte_width = sizeof( uint16_t ) * 36;
    g_ib.description.cpu_access_flags = 0;
    engine_->gfx_context().CreateBuffer(g_ib,NULL);
    //uint32_t index,uint32_t type,
    //graphics::BufferSubresource subresource;
    engine_->gfx_context().device_context()->UpdateSubresource((ID3D11Resource*)g_ib.internal_pointer,0,NULL,indices,sizeof(indices),0);
  }



    
  //bd.ByteWidth = sizeof(CBChangeOnResize);
  //hr = gfx_context_.device()->CreateBuffer( &bd, NULL, &g_pCBChangeOnResize );
  //if( FAILED( hr ) )
  //    return hr;
    
  g_pCBChangesEveryFrame.description.bind_flags = D3D11_BIND_CONSTANT_BUFFER;
  g_pCBChangesEveryFrame.description.usage = D3D11_USAGE_DEFAULT;
  g_pCBChangesEveryFrame.description.byte_width = sizeof(CBChangesEveryFrame);
  g_pCBChangesEveryFrame.description.cpu_access_flags = 0;
  hr = engine_->gfx_context().CreateBuffer(g_pCBChangesEveryFrame,NULL);
  if( FAILED( hr ) )
      return hr;

  // Load the Texture
  hr = D3DX11CreateShaderResourceViewFromFile( engine_->gfx_context().device(), "Content\\arial31_0.png", NULL, NULL, &g_pTextureRV, NULL );
  if( FAILED( hr ) )
      return hr;

    
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
    

  // Initialize the world matrices
  g_World = XMMatrixIdentity();


  motion1.set_context(&engine_->animation());
  motion1.set_from(0);
  motion1.set_to(2);
  motion1.set_max_time(1000000);
  motion1.set_value_ptr(&move_z);
  engine_->animation().tween_list.push_back(&motion1);
  move_z=0;
  motion1.Play();
  return S_OK; 
}

int MainScene1::Deinitialize() { 
  GameView::Deinitialize();

  if( g_pTextureRV ) g_pTextureRV->Release();

  //if( g_pCBChangeOnResize ) g_pCBChangeOnResize->Release();
  //if( g_pCBChangesEveryFrame ) g_pCBChangesEveryFrame->Release();
  if (engine_) {
  engine_->gfx_context().DestroyBuffer(g_pCBChangesEveryFrame);
  engine_->gfx_context().DestroyBuffer(g_vb);
  engine_->gfx_context().DestroyBuffer(g_ib);
  }
  SafeRelease(&blend_state);

  return S_OK;
}

void MainScene1::Draw() {
    

    UINT stride = sizeof( SimpleVertex );
  UINT offset = 0;
  engine_->gfx_context().SetVertexBuffers(0,1,&g_vb,&stride,&offset);
  engine_->gfx_context().SetIndexBuffer(g_ib,0);
    // Set primitive topology
  engine_->gfx_context().device_context()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

  main_effect_->Begin();

  // Update our time
  static float t = 0.0f;
 
  {
      static DWORD dwTimeStart = 0;
      DWORD dwTimeCur = (DWORD)engine_->global_time();
      if( dwTimeStart == 0 )
          dwTimeStart = dwTimeCur;
      t = ( dwTimeCur - dwTimeStart ) / 1000000.0f;
  }

  // Rotate cube around the origin
  g_World = XMMatrixTranslation(move_z,0,0);//XMMatrixRotationY( t );

  // Modify the color
  g_vMeshColor.x = ( sinf( t * 1.0f ) + 1.0f ) * 0.5f;
  g_vMeshColor.y = ( cosf( t * 3.0f ) + 1.0f ) * 0.5f;
  g_vMeshColor.z = ( sinf( t * 5.0f ) + 1.0f ) * 0.5f;




  CBChangesEveryFrame cb;
  cb.mWorld = XMMatrixTranspose( g_World );
  cb.vMeshColor = g_vMeshColor;
  engine_->gfx_context().UpdateBuffer(g_pCBChangesEveryFrame,&cb,NULL,0,0);


  //
  // Render the cube
  //
  //engine_->gfx_context().SetVertexShader(vertex_shader_);
    
  camera_.SetConstantBuffer(0);
  //((graphics::ContextD3D11*)&engine_->gfx_context())->device_context()->PSSetSamplers(0,1,NULL);
  engine_->gfx_context().SetConstantBuffers(graphics::kShaderTypeVertex,2,1,&g_pCBChangesEveryFrame);
  engine_->gfx_context().SetConstantBuffers(graphics::kShaderTypePixel,2,1,&g_pCBChangesEveryFrame);
  //((graphics::ContextD3D11*)&engine_->gfx_context())->device_context()->VSSetConstantBuffers( 2, 1, &g_pCBChangesEveryFrame );
  //((graphics::ContextD3D11*)&engine_->gfx_context())->device_context()->PSSetConstantBuffers( 2, 1, &g_pCBChangesEveryFrame );
  ((graphics::ContextD3D11*)&engine_->gfx_context())->device_context()->PSSetShaderResources( 0, 1, &g_pTextureRV );
  ((graphics::ContextD3D11*)&engine_->gfx_context())->device_context()->DrawIndexed( 36, 0, 0 );
    
}


