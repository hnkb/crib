
#pragma once

#include "crib.h"
#include "pipeline.h"


namespace crib_scenes
{
	namespace hello3d
	{

		class hello3d_scene;
		class mesh;

		class hello3d_d3d11_renderer : public crib::graphics::dx11::renderer_3d<hello3d_scene>
		{
		public:
			hello3d_d3d11_renderer(crib::graphics::dx11::context& context, hello3d_scene& hello_scene);

			virtual void render() override;
			virtual void resize(const float w, const float h) override { renderer_3d::resize(width = w, height = h); }

		protected:
			struct model_assets
			{
				CComPtr<ID3D11Buffer> vertex;
				CComPtr<ID3D11Buffer> index;
				UINT vertex_stride;
				UINT idx_count;
			};

			void create_model_assets(const mesh& model, model_assets& buffers);
			void draw_model(model_assets& model);

			std::map<std::wstring, model_assets> models;
			CComPtr<ID3D11Buffer> cb_vs_perobject;
			CComPtr<ID3D11Buffer> cb_ps_perframe;


			void draw_stats();
			void draw_stat(std::wstring title, std::wstring value, float top, float line_width = 200);

			CComPtr<IDWriteTextFormat> tf_value;
			CComPtr<IDWriteTextFormat> tf_title;
			CComPtr<ID2D1SolidColorBrush> brush;
			float width;
			float height;
		};

	}
}
