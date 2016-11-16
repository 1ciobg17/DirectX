#pragma once
#include"Model.h"

class scene_node
{
private:
	Model* m_p_model;
	vector<scene_node*> m_children;

	float m_x, m_y, m_z;
	float m_xangle, m_zangle, m_yangle;
	float m_scale;
public:
	scene_node();
	void SetXPos(float x);
	void SetYPos(float y);
	void SetZPos(float z);
	float GetXPos();
	float GetYPos();
	float GetZPos();
	void SetXRot(float x);
	void SetYRot(float y);
	void SetZRot(float z);
	float GetXRot();
	float GetYRot();
	float GetZRot();
	void SetScale(float scale);
	float GetScale();
	void SetModel(Model* model);

	void addChildNode(scene_node *n);
	bool detachNode(scene_node *n);
	void execute(XMMATRIX* world, XMMATRIX* view, XMMATRIX* projection, XMVECTOR ambient_light_colour, XMVECTOR directional_light_colour, XMVECTOR directional_light_shines_from);
};