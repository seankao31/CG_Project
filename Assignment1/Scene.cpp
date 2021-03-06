#include <fstream>
#include <iostream>
#include "Scene.h"
#include "TextureManager.h"

using namespace std;

Scene::Scene(const string& scene_file)
{
	Init(scene_file);
}

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::Init(const string& scene_file)
{
	TextureManager& tm = TextureManager::GetInstance();
	select = -1;
	LoadScene(scene_file);
	cout << "Scene" << endl;
	for (auto &model : models)
	{
		model.object = new Mesh(model.object_file);
	}
}

void Scene::LoadScene(const string& scene_file)
{
	TextureManager& tm = TextureManager::GetInstance();
	fstream fin(scene_file, fstream::in);
	string term;

	size_t texture_index = 0;
	tm.textures.push_back(new NoTexture());

	while (fin >> term)
	{
		if (term == "model")
		{
			Model model;
			fin >> model.object_file;
			fin >> model.scale[0] >> model.scale[1] >> model.scale[2];
			fin >> model.rotate[0] >> model.rotate[1] >> model.rotate[2] >> model.rotate[3];
			fin >> model.translate[0] >> model.translate[1] >> model.translate[2];

			model.texture_index = texture_index;
			
			models.push_back(model);
		}
		else if (term == "no-texture")
		{
			texture_index = tm.textures.size();
			tm.textures.push_back(new NoTexture());
		}
		else if (term == "single-texture")
		{
			fin >> term;
			texture_index = tm.textures.size();
			tm.textures.push_back(new SingleTexture(term));
		}
		else if (term == "multi-texture")
		{
			string file1, file2;
			fin >> file1 >> file2;
			texture_index = tm.textures.size();
			tm.textures.push_back(new MultiTexture(file1, file2));
		}
		else if (term == "cube-map")
		{
			vector<string> files;
			for (size_t i = 0; i < 6; ++i)
			{
				fin >> term;
				files.push_back(term);
			}
			texture_index = tm.textures.size();
			tm.textures.push_back(new CubeMapTexture(files));
		}
		else
		{
			cerr << "Scene: load file error" << endl;
			return;
		}
	}
	fin.close();
}

void Scene::Apply()
{
	for (auto model : models)
	{
		model.Render();
	}
		
}

void Scene::Select(int id)
{
	if (0 <= id && id < models.size())
	{
		select = id;
	}
	else
	{
		select = -1;
	}
}