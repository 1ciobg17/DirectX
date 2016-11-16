#include  "scene_node.h"
#include<math.h>

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

XMVECTOR scene_node::GetWorldCentrePosition()
{
	return XMVectorSet(m_world_centre_x,
						m_world_centre_y,
						m_world_centre_z, 0.0);
}

void scene_node::UpdateCollisionTree(XMMATRIX* world, float scale)
{
	//the localWorld matrix will be used to calculate the local transformations for this node
	XMMATRIX localWorld = XMMatrixIdentity();

	//calculate the rotation of this node
	localWorld = XMMatrixRotationX(XMConvertToRadians(m_xangle));
	localWorld = XMMatrixRotationY(XMConvertToRadians(m_yangle));
	localWorld = XMMatrixRotationZ(XMConvertToRadians(m_zangle));

	//calculate the concatenated scale value for this node relative to its parent
	localWorld *= XMMatrixScaling(m_scale, m_scale, m_scale);

	//the position of the node
	localWorld *= XMMatrixTranslation(m_x, m_y, m_z);
	//you can notice we went rotation>scale>translation

	//the local matrix is multiplied by the passed in world matrix that contains the concatenated
	//transformations of all parent nodes so that this nodes transformations are relative to those
	localWorld *= *world;

	//calculating the world space scale of this object, is needed to calculate the 
	//correct bounding sphere radius of an object in a scaled hierarchy
	m_world_scale = scale*m_scale;

	XMVECTOR v;
	if (m_p_model)
	{
		v = XMVectorSet(m_p_model->GetBoundingSphereX(),
			m_p_model->GetBoundingSphereY(),
			m_p_model->GetBoundingSphereZ(), 0.0);
	}
	else v = XMVectorSet(0, 0, 0, 0); //no model, default to 0

	//find and store the world space bounding sphere centre
	v = XMVector3Transform(v, localWorld);
	m_world_centre_x = XMVectorGetX(v);
	m_world_centre_y = XMVectorGetY(v);
	m_world_centre_z = XMVectorGetZ(v);

	//tranverse all child nodes, passing in the concatenated world matrix and scale
	for (int i = 0; i < m_children.size(); i++)
	{
		m_children[i]->UpdateCollisionTree(&localWorld, m_world_scale);
	}
}

bool scene_node::CheckCollision(scene_node* compareTree)
{
	return CheckCollision(compareTree, this);
}

bool scene_node::CheckCollision(scene_node* compareTree, scene_node* objectTreeRoot)
{
	//check to see if root of tree being compared is same as root node of object tree being checked
	//i.e stop object node and children being checked against each other
	if (objectTreeRoot == compareTree) return false;

	//only check for collision if both nodes contain a model
	if (m_p_model&&compareTree->m_p_model)//so I can't have invisible walls, need to be careful
	{
		XMVECTOR v1 = GetWorldCentrePosition();
		XMVECTOR v2 = compareTree->GetWorldCentrePosition();
		XMVECTOR vdiff = v1 - v2;

		//XMVECTOR a = XMVector3Length(vdiff); do this later
		float x1 = XMVectorGetX(v1);
		float x2 = XMVectorGetX(v2);
		float y1 = XMVectorGetY(v1);
		float y2 = XMVectorGetY(v2);
		float z1 = XMVectorGetZ(v1);
		float z2 = XMVectorGetZ(v2);
		float dx = x1 - x2;
		float dy = y1 - y2;
		float dz = z1 - z2;

		if ((sqrt(dx*dx + dy*dy + dz*dz))<((compareTree->m_p_model->GetBoundingSphereRadius()*compareTree->m_world_scale) + (this->m_p_model->GetBoundingSphereRadius()*m_world_scale)))
		{
			return true;
		}
	}

	//iterate through compared tree child nodes
	for (int i = 0; i < compareTree->m_children.size(); i++)
	{
		//check for collision against all compared tree child nodes
		if (CheckCollision(compareTree->m_children[i], objectTreeRoot) == true) return true;
	}

	//iterate through composite object child nodes
	for (int i = 0; i < m_children.size(); i++)
	{
		//check all the child nodes of the composite object against compared tree
		if (m_children[i]->CheckCollision(compareTree, objectTreeRoot) == true) return true;
	}

	return false;
}

bool scene_node::IncX(float in, scene_node* root_node)
{
	float oldX = m_x; //save current state
	m_x += in; // update state

	XMMATRIX identity = XMMatrixIdentity();

	//since state has changed, need to update collision tree
	//this basic system requires entire hierarchy to be update
	//so start at root node passing in identity matrix
	root_node->UpdateCollisionTree(&identity, 1.0);

	//check for collision of this node(and children) against all other nodes
	if (CheckCollision(root_node) == true)
	{
		//if collision restore state
		m_x = oldX;

		return true;
	}

	return false;
}

bool scene_node::IncY(float in, scene_node* root_node)
{
	float oldY = m_y; //save current state
	m_y += in; // update state

	XMMATRIX identity = XMMatrixIdentity();

	root_node->UpdateCollisionTree(&identity, 1.0);

	//check for collision of this node(and children) against all other nodes
	if (CheckCollision(root_node) == true)
	{
		//if collision restore state
		m_y = oldY;

		return true;
	}

	return false;
}

bool scene_node::IncZ(float in, scene_node* root_node)
{
	float oldZ = m_z; //save current state
	m_z += in; // update state

	XMMATRIX identity = XMMatrixIdentity();

	root_node->UpdateCollisionTree(&identity, 1.0);

	//check for collision of this node(and children) against all other nodes
	if (CheckCollision(root_node) == true)
	{
		//if collision restore state
		m_z = oldZ;

		return true;
	}

	return false;
}