#include <fstream>
#include <iostream>
#include "View.h"

using namespace std;

View::View(const string &view_file)
{
	Init(view_file);
}

View::View()
{
}

View::~View()
{
}

void View::Init(const string &view_file)
{
	LoadView(view_file);
	rotation = 0;
	zoom.clear();
}

void View::LoadView(const string &view_file)
{
	fstream fin(view_file, fstream::in);
	string term;
	while (fin >> term)
	{
		if (term == "eye")
		{
			for (size_t i = 0; i < 3; ++i)
				fin >> eye[i];
		}
		else if (term == "vat")
		{
			for (size_t i = 0; i < 3; ++i)
				fin >> vat[i];
		}
		else if (term == "vup")
		{
			for (size_t i = 0; i < 3; ++i)
				fin >> vup[i];
		}
		else if (term == "fovy")
		{
			fin >> fovy;
		}
		else if (term == "dnear")
		{
			fin >> dnear;
		}
		else if (term == "dfar")
		{
			fin >> dfar;
		}
		else if (term == "viewport")
		{
			for (size_t i = 0; i < 4; ++i)
				fin >> viewport[i];
		}
		else
		{
			cerr << "View: load file error" << endl;
			return;
		}
	}
	fin.close();
	Print();
}

void View::Apply()
{
	// viewport transformation
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	// projection transformation
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (GLfloat)viewport[2] / (GLfloat)viewport[3], dnear, dfar);
	// viewing and modeling transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// either this: start
	/*
	gluLookAt(Vec3d(eye+zoom)[0], Vec3d(eye+zoom)[1], Vec3d(eye+zoom)[2],// eye
		vat[0], vat[1], vat[2],     // center
		vup[0], vup[1], vup[2]);    // up
	*/
	// end
	// or this: start
	gluLookAt(eye[0], eye[1], eye[2],// eye
		vat[0], vat[1], vat[2],     // center
		vup[0], vup[1], vup[2]);    // up
	glTranslatef(-zoom[0], -zoom[1], -zoom[2]); // camera zoom in / out
	// end
	// would work
	glRotatef(rotation, vup[0], vup[1], vup[2]); // rotate the camera about viewing center
}

void View::Print()
{
	cout << "View" << endl;
	cout << "eye: ";
	for (int i = 0; i < 3; ++i)
		cout << eye[i] << ' ';
	cout << endl;
	cout << "vat: ";
	for (int i = 0; i < 3; ++i)
		cout << vat[i] << ' ';
	cout << endl;
	cout << "vup: ";
	for (int i = 0; i < 3; ++i)
		cout << vup[i] << ' ';
	cout << endl;
	cout << "fovy: " << fovy << endl;
	cout << "dnear: " << dnear << endl;
	cout << "dfar: " << dfar << endl;
	cout << "viewport: ";
	for (int i = 0; i < 4; ++i)
		cout << viewport[i] << ' ';
	cout << endl;
	cout << endl;
}
