#include "Particles.h"

ParticleType Particle::mode = ParticleType::Normal;

Particle::Particle(RenderTarget& target, int numPoints, Vector2i mouseClickPosition) : m_A(2, numPoints)
{
	m_ttl = TTL;
	m_numPoints = numPoints;
	m_radiansPerSec = ((float)rand() / RAND_MAX) * M_PI;
	m_cartesianPlane.setCenter(0, 0);
	m_cartesianPlane.setSize(target.getSize().x, (-1.0) * target.getSize().y);
	m_centerCoordinate = target.mapPixelToCoords(mouseClickPosition, m_cartesianPlane);


	if (mode == Normal) {
		m_vx = 100 + rand() % 400; // 100 to 500
		if (rand() % 2) m_vx *= -1;
		m_vy = 100 + rand() % 400;
		if (rand() % 2) m_vy *= -1;
	}
	else {
		m_vx = 900 + rand() % 1000; // 100 to 500
		if (rand() % 2) m_vx *= -1;
		m_vy = 900 + rand() % 1000;
		if (rand() % 2) m_vy *= -1;
	}




	m_color1 = Color::White;
	m_color2 = Color(rand() % 256, rand() % 256, rand() % 256);

	float theta = ((float)rand() / RAND_MAX) * (M_PI / 2);
	float dTheta = (2 * M_PI) / (numPoints - 1);
	for (int j = 0; j < numPoints; j++)
	{
		float r, dx, dy;
		r = 20 + rand() % 101; // 20 to 120
		dx = r * cos(theta);
		dy = r * sin(theta);
		m_A(0, j) = m_centerCoordinate.x + dx;
		m_A(1, j) = m_centerCoordinate.y + dy;
		theta += dTheta;
	}

}
void Particle::draw(RenderTarget& target, RenderStates states) const
{
	VertexArray lines(TriangleFan, m_numPoints + 1);
	Vector2f center = static_cast<Vector2f>(target.mapCoordsToPixel(m_centerCoordinate, m_cartesianPlane));
	lines[0].position = center;
	lines[0].color = m_color1;

	for (int j = 1; j <= m_numPoints; ++j)
	{
		Vector2f vertex;
		vertex.x = m_A(0, j - 1);
		vertex.y = m_A(1, j - 1);

		lines[j].position = static_cast<Vector2f>(target.mapCoordsToPixel(vertex, m_cartesianPlane));
		lines[j].color = m_color2;
	}
	target.draw(lines, states);

}

void Particle::update(float dt)
{
	m_ttl -= dt;
	rotate(m_radiansPerSec * dt);
	scale(SCALE);
	
	float dx;
	float dy;
	if (mode == Normal) {
		dx = m_vx * dt;
		dy = m_vy * dt;

		m_vy -= G * dt;
		translate(dx, dy);
	}
	else {
		applyExtraCredit(dt);
	}








}

void Particle::translate(double xShift, double yShift)
{
	TranslationMatrix T(xShift, yShift, m_A.getCols());
	m_A = m_A + T;
	m_centerCoordinate.x += xShift;
	m_centerCoordinate.y += yShift;

}

void Particle::rotate(double theta)
{
	Vector2f temp = m_centerCoordinate;
	translate(-m_centerCoordinate.x, -m_centerCoordinate.y);
	RotationMatrix R(theta);

	m_A = R * m_A;

	translate(temp.x, temp.y);
}

void Particle::scale(double c)
{
	Vector2f temp = m_centerCoordinate;
	translate(-m_centerCoordinate.x, -m_centerCoordinate.y);
	ScalingMatrix S(c);
	m_A = S * m_A;

	translate(temp.x, temp.y);
}
bool Particle::almostEqual(double a, double b, double eps)
{
	return fabs(a - b) < eps;
}

void Particle::unitTests()
{
	int score = 0;
	cout << "Testing RotationMatrix constructor...";
	double theta = M_PI / 4.0;
	RotationMatrix r(M_PI / 4);
	if (r.getRows() == 2 && r.getCols() == 2 && almostEqual(r(0, 0), cos(theta))
		&& almostEqual(r(0, 1), -sin(theta))
		&& almostEqual(r(1, 0), sin(theta))
		&& almostEqual(r(1, 1), cos(theta)))
	{
		cout << "Passed. +1" << endl;
		score++;
	}
	else
	{
		cout << "Failed." << endl;
	}
	cout << "Testing ScalingMatrix constructor...";
	ScalingMatrix s(1.5);
	if (s.getRows() == 2 && s.getCols() == 2
		&& almostEqual(s(0, 0), 1.5)
		&& almostEqual(s(0, 1), 0)
		&& almostEqual(s(1, 0), 0)
		&& almostEqual(s(1, 1), 1.5))
	{
		cout << "Passed. +1" << endl;
		score++;
	}
	else
	{
		cout << "Failed." << endl;
	}
	cout << "Testing TranslationMatrix constructor...";
	TranslationMatrix t(5, -5, 3);
	if (t.getRows() == 2 && t.getCols() == 3
		&& almostEqual(t(0, 0), 5)
		&& almostEqual(t(1, 0), -5)
		&& almostEqual(t(0, 1), 5)
		&& almostEqual(t(1, 1), -5)
		&& almostEqual(t(0, 2), 5)
		&& almostEqual(t(1, 2), -5))
	{
		cout << "Passed. +1" << endl;
		score++;
	}
	else
	{
		cout << "Failed." << endl;
	}
	cout << "Testing Particles..." << endl;
	cout << "Testing Particle mapping to Cartesian origin..." << endl;
	if (m_centerCoordinate.x != 0 || m_centerCoordinate.y != 0)
	{
		cout << "Failed. Expected (0,0). Received: (" << m_centerCoordinate.x <<
			"," << m_centerCoordinate.y << ")" << endl;
	}
	else
	{
		cout << "Passed. +1" << endl;
		score++;
	}
	cout << "Applying one rotation of 90 degrees about the origin..." << endl;
	Matrix initialCoords = m_A;
	rotate(M_PI / 2.0);
	bool rotationPassed = true;
	for (int j = 0; j < initialCoords.getCols(); j++)
	{
		if (!almostEqual(m_A(0, j), -initialCoords(1, j)) || !almostEqual(m_A(1,
			j), initialCoords(0, j)))
		{
			cout << "Failed mapping: ";
			cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
			rotationPassed = false;
		}
	}
	if (rotationPassed)
	{
		cout << "Passed. +1" << endl;
		score++;
	}
	else
	{
		cout << "Failed." << endl;
	}
	cout << "Applying a scale of 0.5..." << endl;
	initialCoords = m_A;
	scale(0.5);
	bool scalePassed = true;
	for (int j = 0; j < initialCoords.getCols(); j++)
	{
		if (!almostEqual(m_A(0, j), 0.5 * initialCoords(0, j)) || !
			almostEqual(m_A(1, j), 0.5 * initialCoords(1, j)))
		{
			cout << "Failed mapping: ";
			cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
			scalePassed = false;
		}
	}
	if (scalePassed)
	{
		cout << "Passed. +1" << endl;
		score++;
	}
	else
	{
		cout << "Failed." << endl;
	}
	cout << "Applying a translation of (10, 5)..." << endl;
	initialCoords = m_A;
	translate(10, 5);
	bool translatePassed = true;
	for (int j = 0; j < initialCoords.getCols(); j++)
	{
		if (!almostEqual(m_A(0, j), 10 + initialCoords(0, j)) || !
			almostEqual(m_A(1, j), 5 + initialCoords(1, j)))
		{
			cout << "Failed mapping: ";
			cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
			translatePassed = false;
		}
	}
	if (translatePassed)
	{
		cout << "Passed. +1" << endl;
		score++;
	}
	else
	{
		cout << "Failed." << endl;
	}
	cout << "Score: " << score << " / 7" << endl;

}


void Particle::applyExtraCredit(float dt)
{
	Vector2f mouse = getMappedMousePosition();

	float attractionStrength = -2.0f; // attraction strength
	
	float mx = (mouse.x - m_centerCoordinate.x) * dt * attractionStrength;
	float my = (mouse.y - m_centerCoordinate.y) * dt * attractionStrength;
	translate(mx, my);

	Uint8 r = (m_color2.r + 1) % 256;
	Uint8 g = (m_color2.g + 2) % 256;
	Uint8 b = (m_color2.b + 3) % 256;
	m_color2 = Color(r, g, b);




}




Vector2f Particle::getMappedMousePosition() const
{
	Vector2i pixelPos = Mouse::getPosition();

	Vector2f viewCenter = m_cartesianPlane.getCenter();
	Vector2f viewSize = m_cartesianPlane.getSize();

	float windowWidth = static_cast<float>(VideoMode::getDesktopMode().width);
	float windowHeight = static_cast<float>(VideoMode::getDesktopMode().height);

	Vector2f mapped;
	mapped.x = ((float)pixelPos.x / windowWidth - 0.5f) * viewSize.x + viewCenter.x;
	mapped.y = ((float)pixelPos.y / windowHeight - 0.5f) * viewSize.y + viewCenter.y;

	return mapped;
}


