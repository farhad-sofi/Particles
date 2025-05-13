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
    while (m_Window.pollEvent(event))
    {
        if (event.type == Event::Closed ||
            (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape))
        {
            m_Window.close();
        }

        if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
        {
            for (int i = 0; i < 5; ++i)
            {
                int numPoints = 25 + rand() % 26;  // [25, 50]
                m_particles.push_back(Particle(m_Window, numPoints, { event.mouseButton.x, event.mouseButton.y }));
            }
        }
    }
}


void Engine::update(float dtAsSeconds)
{
    auto it = m_particles.begin();
    while (it != m_particles.end())
    {
        if (it->getTTL() > 0.0f)
        {
            it->update(dtAsSeconds);
            ++it;
        }
        else
        {
            it = m_particles.erase(it);
        }
    }
}

void Engine::draw()
{
    m_Window.clear();

    for (auto& particle : m_particles)
    {
        m_Window.draw(particle);  
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
    
    while (m_Window.isOpen())
    {
        Time dt = clock.restart();
        float dtAsSeconds = dt.asSeconds();

        input();
        update(dtAsSeconds);
        draw();
    }
}
