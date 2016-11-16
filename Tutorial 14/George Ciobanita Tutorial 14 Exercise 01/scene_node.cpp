#include  "scene_node.h"

scene_node::scene_node(void)
{
	m_p_model = NULL;

	m_x = 0.0f;
	m_y = 0.0f;
	m_z = 0.0f;

	m_xangle = 0.0f;
	m_yangle = 0.0f;
	m_zangle = 0.0f;

	m_scale = 1.0f;
}

void scene_node::SetXPos(float x)
{
	m_x += x;
}
void scene_node::SetYPos(float y)
{
	m_y = y;
}

void scene_node::SetZPos(float z)
{
	m_z = z;
}

void scene_node::SetXRot(float x)
{
	m_xangle = x;
}

void scene_node::SetYRot(float y)
{
	m_yangle = y;
}

void scene_node::SetZRot(float z)
{
	m_zangle = z;
}

void scene_node::SetScale(float scale)
{
	m_scale = scale;
}

float scene_node::GetXPos()
{
	return m_x;
}

float scene_node::GetYPos()
{
	return m_y;
}

float scene_node::GetZPos()
{
	return m_z;
}

float scene_node::GetXRot()
{
	return m_xangle;
}

float scene_node::GetYRot()
{
	return m_yangle;
}

float scene_node::GetZRot()
{
	return m_zangle;
}

float scene_node::GetScale()
{
	return m_scale;
}

void scene_node::addChildNode(scene_node *n)
{
	m_children.push_back(n);
}

bool scene_node::detachNode(scene_node *n)
{
	//traverse tree to find node to detach
	for (int i = 0; i < m_children.size(); i++)
	{
		if (n == m_children[i])
		{
			m_children.erase(m_children.begin() + i);
			return true;
		}

		if (m_children[i]->detachNode(n) == true) return true;
	}
	return false;//node not in this tree;
}

void scene_node::execute(XMMATRIX* world, XMMATRIX* view, XMMATRIX* projection, XMVECTOR ambient_light_colour, XMVECTOR directional_light_colour, XMVECTOR directional_light_shines_from)
{
	// the local_world matrix will be used to calc the local transformations for this node
	XMMATRIX local_world = XMMatrixIdentity();
	local_world = XMMatrixRotationX(XMConvertToRadians(m_xangle));
	local_world *= XMMatrixRotationY(XMConvertToRadians(m_yangle));
	local_world *= XMMatrixRotationZ(XMConvertToRadians(m_zangle));
	local_world *= XMMatrixScaling(m_scale, m_scale, m_scale);
	local_world *= XMMatrixTranslation(m_x, m_y, m_z);
	// the local matrix is multiplied by the passed in world matrix that contains the concatenated
	// transformations of all parent nodes so that this nodes transformations are relative to those
	local_world *= *world;
	// only draw if there is a model attached
	if (m_p_model) 
		m_p_model->Draw(&local_world, view, projection, ambient_light_colour, directional_light_colour, directional_light_shines_from);
	// traverse all child nodes, passing in the concatenated world matrix
	for (int i = 0; i< m_children.size(); i++)
	{
		m_children[i]->execute(&local_world, view, projection, ambient_light_colour, directional_light_colour, directional_light_shines_from);
	}
}

void scene_node::SetModel(Model* model)
{
	m_p_model = model;
}
