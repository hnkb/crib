
cbuffer CB_PERFRAME : register(b0)
{
	DIRECTIONAL_LIGHT light;
};

cbuffer CB_PEROBJECT : register(b1)
{
	matrix world;
	matrix world_view_proj; // world * view * projection
};
