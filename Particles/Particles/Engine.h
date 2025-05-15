#pragma once

#include <SFML/Graphics.hpp>
#include "Particles.h"
using namespace sf;
using namespace std;


class Engine
{
private:

	bool m_mouseHeld = false;
	// A regular RenderWindow
	RenderWindow m_Window;
	//vector for Particles
	vector<Particle> m_particles;
	// Private functions for internal use only
	void input();
	void update(float dtAsSeconds);
	void draw();
	RenderTexture m_trailTexture;
	Sprite m_trailSprite;



	


public:

	Engine();
	void run();
	enum Mode { Normal, BlackHole };

	static Mode mode;
};

