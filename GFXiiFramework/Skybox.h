#ifndef skybox_h
#define skybox_h

#include <string>

class Skybox
{
private:


public:

	Skybox();
	~Skybox();

	void LoadTexture(std::string _directory, std::string _front, std::string _back, std::string _left, std::string _right, std::string _top, std::string _bottom);
	void Render();

};

#endif