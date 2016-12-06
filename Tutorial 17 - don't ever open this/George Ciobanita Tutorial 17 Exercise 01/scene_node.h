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

	//Tutorial 15
	float m_world_centre_x, m_world_centre_y, m_world_centre_z;
	float m_world_scale;

	//Tutorial 17
	XMMATRIX m_local_world_matrix;
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

	//Tutorial 15
	XMVECTOR GetWorldCentrePosition();
	void UpdateCollisionTree(XMMATRIX* world, float scale);
	bool CheckCollision(scene_node* compareTree);
	bool CheckCollision(scene_node* compareTree, scene_node* objectTreeRoot);
	bool IncX(float in, scene_node* root_node);
	bool IncY(float in, scene_node* root_node);
	bool IncZ(float in, scene_node* root_node);

	//Tutorial 17
	bool CheckCollisionRay(XMVECTOR ray, XMVECTOR rayDirection);
};