
#pragma once

#include "graphics/d3d11/d3d11_renderer.h"
//#include "model.h"


namespace crib
{
	namespace scene
	{
		namespace hello3d
		{

			class hello3d_scene;
			class model;

			class hello3d_d3d11_renderer : public graphics::d3d11_renderer
			{
			public:
				hello3d_d3d11_renderer(graphics::d3d11_context& context, hello3d_scene& hello_scene);

				virtual void render() override;
				virtual void resize(const float width, const float height) override;

			protected:
				hello3d_scene& scene;

				float width;
				float height;


				struct model_buffers
				{
					CComPtr<ID3D11Buffer> vertex;
					CComPtr<ID3D11Buffer> index;
					UINT vertex_stride;
					UINT idx_count;
				};

				struct constant_buffer_layout
				{
					DirectX::XMMATRIX world;
					DirectX::XMMATRIX projection_view;
				};

				std::map<std::wstring, model_buffers> models;
				CComPtr<ID3D11Buffer> const_buffer;

				void create_buffers(const model& model, model_buffers& buffers);
				void draw_model(model_buffers& model);
				DirectX::XMMATRIX get_projection_matrix() const;


				void draw_stats();
				void draw_stat(std::wstring title, std::wstring value, float top);

				CComPtr<IDWriteTextFormat> tf_value;
				CComPtr<IDWriteTextFormat> tf_title;
				CComPtr<ID2D1SolidColorBrush> brush;
			};

		}
	}
}
