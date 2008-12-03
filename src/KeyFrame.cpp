#include "stdafx.h"
#include "KeyFrame.h"

KeyFrame::KeyFrame(Mesh* mesh)
{
	meshes.push_back(mesh);
}

KeyFrame::KeyFrame(vector<Mesh*> Meshes)
: meshes(Meshes)
{}

KeyFrame::KeyFrame(const KeyFrame &keyframe)
{
	const vector<Mesh*> &m = keyframe.getMeshes();
	for(vector<Mesh*>::const_iterator i = m.begin(); i != m.end(); ++i)
	{
		Mesh *mesh = new Mesh(*i);
		meshes.push_back(mesh);
	}
}

KeyFrame& KeyFrame::operator=(const KeyFrame &keyframe)
{
	meshes = keyframe.meshes;
	return(*this);
}

bool KeyFrame::merge(const KeyFrame &o)
{
	if(getMeshes().size() == 0)
	{
		meshes = o.getMeshes();
	}
	else
	{
		if(getMeshes().size() != o.getMeshes().size())
			return false;

		for(vector<Mesh*>::const_iterator iter=o.getMeshes().begin();
            iter != o.getMeshes().end();
            ++iter)
		{
			meshes.push_back(*iter);
		}
	}

	return true;
}

void KeyFrame::applySkinToModel(vector<Mesh*> &model, const Material &mat)
{
    for_each(model.begin(), model.end(), bind(&Mesh::setMaterial, _1, mat));
}

void KeyFrame::applySkin(const Material &material)
{
    applySkinToModel(getMeshes(), material);
}
