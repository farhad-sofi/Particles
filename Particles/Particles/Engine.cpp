#include "Engine.h"



Engine::Engine()
{
    int pixelWidth = VideoMode::getDesktopMode().width;
    int pixelHeight = VideoMode::getDesktopMode().height;
    VideoMode vm(pixelWidth, pixelHeight);
    m_Window.create(vm, "Particles", Style::Default);

    if (!m_trailTexture.create(m_Window.getSize().x, m_Window.getSize().y))
    {
        throw std::runtime_error("Failed to create trail texture");
    }
    m_trailTexture.setView(m_Window.getDefaultView());
    m_trailSprite.setTexture(m_trailTexture.getTexture());
    if (!m_fireworkBuffer.loadFromFile("firework.wav"))
        throw std::runtime_error("Failed to load firework.wav");

    m_fireworkSound.setBuffer(m_fireworkBuffer);

 

}
void Engine::input()
{
    Event event;
    while (m_Window.pollEvent(event))
    {
        if (event.type == Event::Closed || (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape))
        {
            m_Window.close();
        }
        if (event.type == Event::KeyPressed)
        {
            if (event.key.code == Keyboard::S)
            {
                Particle::mode = (Particle::mode == ParticleType::Spiral ? ParticleType::Normal : ParticleType::Spiral);
            }
        }

        if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
        {
            
            m_mouseHeld = true;
            
        }
        else if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left)
        {
            m_mouseHeld = false;
        }
    }
}



void Engine::update(float dtAsSeconds)
{


    if (m_mouseHeld && m_Window.hasFocus())
    {
        Vector2i mousePos = Mouse::getPosition(m_Window);

        // Spawn 2 particles per frame while mouse is held
        for (int i = 0; i < 2; ++i)
        {
            int numPoints = 25 + rand() % 26;
            m_particles.push_back(Particle(m_Window, numPoints, mousePos));
            m_fireworkSound.play();
        }
    }


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
            it = m_particles.erase(it);  // erase returns valid next iterator
        }
    }
}



void Engine::draw()
{

    Font font;
    if (!font.loadFromFile("times.ttf")) {
        cerr << "Failed to load font!" << endl;
    }
    Text currentMode("", font, 50);
    currentMode.setPosition(20, 20);
    currentMode.setFillColor(Color(255, 255, 255, 255));

    RectangleShape fade(Vector2f(m_Window.getSize()));
    fade.setFillColor(Color(0, 0, 0, 10)); // 0 = no fade, 255 = instant erase
    m_trailTexture.draw(fade);


    // Clear the trail texture
    if (Particle::mode == Spiral)
    {
        currentMode.setString("Current Mode : Spiral");

    }
    else
    {
        currentMode.setString("Current Mode : Normal");
    }


    for (auto& particle : m_particles)
    {
        m_trailTexture.draw(particle);
        m_trailTexture.draw(currentMode);
    }




    m_trailTexture.display();


    // Draw trail texture to window
    m_Window.clear();

    m_Window.draw(m_trailSprite);

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



