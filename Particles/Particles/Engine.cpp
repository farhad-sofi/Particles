#include "Engine.h"

Engine::Engine()
{
	int pixelWidth = VideoMode::getDesktopMode().width;
	int pixelHeight = VideoMode::getDesktopMode().height;
	VideoMode vm(pixelWidth, pixelHeight);
	m_Window.create( vm, "Particles", Style::Default );

}

void Engine::input()
{

    Event event;

    while (m_Window.isOpen())
    {
        while (m_Window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                m_Window.close();

            if (event.type == Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == Mouse::Left)
                {
                    for (size_t i = 0; i < 5; i++)
                    {
                        int numPoints = rand() % 26 + 25;
                        int mouseX = event.mouseButton.x;
                        int mouseY = event.mouseButton.y;
                        Vector2i location(mouseX, mouseY);
                        Particle P(m_Window, numPoints, location);

                        m_particles.push_back(P);
                    }
                }
            }
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
            {
                m_Window.close();
            }
        }
    }
}

void Engine::update(float dtAsSeconds)
{
    for (vector<Particle>::iterator P = m_particles.begin(); P != m_particles.end();)
    {
        if (P->getTTL() > 0.0)
        {
            P->update(dtAsSeconds);
            ++P;
        }
        else if (P->getTTL() == 0.0)
        {
            P = m_particles.erase(P);

        }
    }
}

void Engine::draw()
{
    m_Window.clear();
    for (size_t i = 0; i < m_particles.size(); i++)
    {
        m_Window.draw();
    }

    m_Window.display();
}

void Engine::run()
{
	Clock clock;
	Time time1 = clock.getElapsedTime();


	cout << "Starting Particle unit tests..." << endl;
	Particle p(m_Window, 4, { (int)m_Window.getSize().x / 2, (int)m_Window.getSize().y / 2 });
	p.unitTests();
	cout << "Unit tests complete.  Starting engine..." << endl;

	clock.restart();
	float dt = time1.asSeconds();

	input;
	update(dt);
	draw;
}
