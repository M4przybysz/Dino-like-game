#include "SFML-2.5.1\include\SFML/Graphics.hpp"
#include "SFML-2.5.1\include\SFML/Window.hpp"
#include "SFML-2.5.1\include\SFML/System.hpp"

using namespace std;
using namespace sf;

long long score = 0;
constexpr int windowWidth{ 800 }, windowHeight{ 200 };
constexpr float rexWidth{ 20.f }, rexHeight{ 35.f }, rexVelocity{ 4.7f };
constexpr float floorWidth{ 800.f }, floorHeight{ 10.f };
constexpr float spikeRadius{ 18.f }, spikeVelocity{ 5.f }, upSpikeY{ 9.f }, downSpikeY{ windowHeight - 64.f };
bool gameOver = false;

struct Rex
{
	RectangleShape shape;
	Vector2f velocity{ 0, rexVelocity };

	float x() { return shape.getPosition().x; }
	float y() { return shape.getPosition().y; }
	float left() { return x() - shape.getSize().x / 2.f; }
	float right() { return x() + shape.getSize().x / 2.f; }
	float top() { return y() - shape.getSize().y / 2.f; }
	float bottom() { return y() + shape.getSize().y / 2.f; }

	Rex(float mX, float mY)
	{
		shape.setPosition(mX, mY);
		shape.setSize({ rexWidth, rexHeight });
		shape.setFillColor(Color::Green);
		shape.setOrigin(rexWidth / 2.f, rexHeight / 2.f);
	}

	void jump()
	{
		shape.move(velocity);
		if (Keyboard::isKeyPressed(Keyboard::Key::Space) && velocity.y == 0) velocity.y = -rexVelocity;
		else if (top() <= 0) velocity.y = rexVelocity;
	}
};

struct Floor
{
	RectangleShape shape;

	float x() { return shape.getPosition().x; }
	float y() { return shape.getPosition().y; }
	float left() { return x() - shape.getSize().x / 2.f; }
	float right() { return x() + shape.getSize().x / 2.f; }
	float top() { return y() - shape.getSize().y / 2.f; }
	float bottom() { return y() + shape.getSize().y / 2.f; }

	Floor(float mX, float mY)
	{
		shape.setPosition(mX, mY);
		shape.setSize({ floorWidth, floorHeight });
		shape.setFillColor(Color::White);
		shape.setOrigin(floorWidth / 2.f, floorHeight / 2.f);
	}
};

struct Spike
{
	CircleShape shape{ spikeRadius, 3 };
	Vector2f velocity{ -spikeVelocity, 0 };

	float x() { return shape.getPosition().x; }
	float y() { return shape.getPosition().y; }
	float left() { return x() - shape.getRadius(); }
	float right() { return x() + shape.getRadius(); }
	float top() { return y() - shape.getRadius(); }
	float bottom() { return y() + shape.getRadius(); }

	Spike(float mX, float mY, bool updown)
	{
		shape.setPosition(mX, mY);
		shape.setFillColor(Color::Red);
		shape.setOrigin(spikeRadius, spikeRadius);
		if (updown) shape.setRotation(180.f);
	}

	void update()
	{
		shape.move(velocity);
		if (x() < -20) { velocity.x = 0; shape.setPosition(windowWidth + spikeRadius, y()); }
	}
};

template <class T1, class T2> bool isIntersecting(T1& mA, T2& mB)
{
	return mA.right() >= mB.left() && mA.left() <= mB.right() && mA.bottom() >= mB.top() && mA.top() <= mB.bottom();
}

void testCollision(Rex& mRex, Floor& mFloor)
{
	if (!isIntersecting(mFloor, mRex)) return;

	mRex.shape.setPosition(mRex.x(), mRex.y() - 1.f);
	mRex.velocity.y = 0.f;
}

void testCollision(Rex& mRex, Spike& mSpike)
{
	if (!isIntersecting(mSpike, mRex)) return;

	gameOver = true;
}

void setNewDanger(vector <Spike>& mSpikes)
{
	int x = 0;
	float XD;
	bool updown = true;

	for (auto& spike : mSpikes) if (spike.velocity.x == 0 && spike.x() > windowWidth) x++;

	if (x < 6) return;

	mSpikes.clear();
	for (int i = 0; i < 6; i++)
	{
		XD = rand() % 200 + 150;
		if (updown) { mSpikes.emplace_back((windowWidth + spikeRadius) + (i * XD), upSpikeY, updown); updown = false; }
		else { mSpikes.emplace_back((windowWidth + spikeRadius) + (i * XD), downSpikeY, updown); updown = true; }
	}

	for (auto& spike : mSpikes) spike.velocity.x = -spikeVelocity;
}

void scoring(Text& mText, Clock& mClock)
{
	if (gameOver) return;

	if (mClock.getElapsedTime() > milliseconds(250))
	{
		mClock.restart();
		score++;
		mText.setString("Score: " + to_string(score));
	}
}

int main()
{
	srand(time(NULL));

	Floor floor(windowWidth / 2, windowHeight - 50);
	Rex rex(50, 75);
	vector <Spike> spikes;
	Font font; font.loadFromFile("impact.ttf");
	Clock scoreClock;
	Text over("GAME OVER", font, 50); over.setPosition((windowWidth / 2) - (over.getLocalBounds().width / 2), (windowHeight / 2) - over.getLocalBounds().height);
	over.setFillColor(Color::White);
	Text tscore("Score: " + to_string(score), font, 30); tscore.setPosition(10, windowHeight - over.getLocalBounds().height);
	tscore.setFillColor(Color::White);

	for (int i = 0; i < 3; i++) spikes.emplace_back((windowWidth + 20.f), 9.f, 1);
	for (int i = 0; i < 3; i++) spikes.emplace_back((windowWidth + 80.f), (windowHeight - 64.f), 0);

	RenderWindow window{ {windowWidth, windowHeight}, "Rex" };
	window.setFramerateLimit(60);

	while (Keyboard::isKeyPressed(Keyboard::Key::Escape)!=true)
	{
		window.clear(Color::Black);

		setNewDanger(spikes);

		rex.jump();
		for (auto& spike : spikes) spike.update();

		testCollision(rex, floor);
		for (auto& spike : spikes) testCollision(rex, spike);

		window.draw(floor.shape);
		window.draw(rex.shape);
		for (auto& spike : spikes) window.draw(spike.shape);
		window.draw(tscore);
		scoring(tscore, scoreClock);

		if (gameOver)
		{
			rex.velocity = { 0, 0 };
			for (auto& spike : spikes) spike.velocity.x = 0;
			window.draw(over);
		}

		window.display();
	}
}