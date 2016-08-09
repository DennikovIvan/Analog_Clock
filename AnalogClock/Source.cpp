#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include <SFML/Graphics.hpp>
#include <ctime>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <string>

struct Arrows
{
	sf::RectangleShape *hourHand;
	sf::RectangleShape *minuteHand;
	sf::RectangleShape *secondsHand;
};

struct Clock
{
	sf::CircleShape *dot[60];
	sf::Text *numbers[12];
	sf::CircleShape *clockCountour;
	sf::CircleShape *centerCircle;
	Arrows *clockArrows;
};

struct Application
{
	sf::RenderWindow *window;
	sf::ContextSettings settings;
	sf::Font *clockFont;
	int numbersSize;
	float clockCircleSize;
	float clockCircleThickness;
	Clock *analogClock;
};

void initApplication(Application *application, sf::Font *clockFont);
void initClock(Application *application);
void initArrows(Application *application);
void createDots(Application *application);
void configureCircle(sf::CircleShape *circle, int pointCount, float outlineTickness, sf::Color color, int isFilled, float x, float y);
void configureArrow(sf::RectangleShape *arrow, sf::Color color, sf::Vector2u windowSize);
void updateArrows(Application *application);
void createNumbers(Application *application);
void configureNumbers(Application *application);
void drawAFrame(Application *application);
void gameLoop(Application *application);

void initApplication(Application *application, sf::Font *clockFont)
{
	application->settings.antialiasingLevel = 8;
	application->clockFont = clockFont;
	application->numbersSize = 24;
	application->clockCircleSize = 250;
	application->clockCircleThickness = 5;
	application->window = new sf::RenderWindow(sf::VideoMode(800, 600), "My Analog Clock", sf::Style::Close, application->settings);
	application->analogClock = new Clock;
	initClock(application);
}

void initClock(Application *application)
{
	createDots(application);
	createNumbers(application);
	configureNumbers(application);
	application->analogClock->clockCountour = new sf::CircleShape(application->clockCircleSize);
	application->analogClock->clockCountour->setFillColor(sf::Color::Yellow);
	configureCircle(application->analogClock->clockCountour, 100, application->clockCircleThickness, sf::Color::Black, 0, application->window->getSize().x / 2.0f + application->clockCircleThickness, application->window->getSize().y / 2.0f + application->clockCircleThickness);
	application->analogClock->centerCircle = new sf::CircleShape(10);
	configureCircle(application->analogClock->centerCircle, 100, 1, sf::Color::Red, 1, float(application->window->getSize().x / 2.0f), float(application->window->getSize().y / 2.0f));
	application->analogClock->clockArrows = new Arrows;
	initArrows(application);
}

void initArrows(Application *application)
{
	application->analogClock->clockArrows->hourHand = new sf::RectangleShape(sf::Vector2f(5, 150));
	application->analogClock->clockArrows->minuteHand = new sf::RectangleShape(sf::Vector2f(3, 200));
	application->analogClock->clockArrows->secondsHand = new sf::RectangleShape(sf::Vector2f(2, 220));
	configureArrow(application->analogClock->clockArrows->hourHand, sf::Color::Black, application->window->getSize());
	configureArrow(application->analogClock->clockArrows->minuteHand, sf::Color::Black, application->window->getSize());
	configureArrow(application->analogClock->clockArrows->secondsHand, sf::Color::Red, application->window->getSize());
}

void createDots(Application *application)
{
	float angle = 0.0;
	float x;
	float y;
	for (int i = 0; i < 60; i++)
	{
		x = (application->clockCircleSize - 10) * cos(angle);
		y = (application->clockCircleSize - 10) * sin(angle);
		application->analogClock->dot[i] = (i % 5 == 0) ? new sf::CircleShape(3) : new sf::CircleShape(1);
		configureCircle(application->analogClock->dot[i], 12, 1, sf::Color::Black, 1, x + application->window->getSize().x / 2.0f, y + application->window->getSize().y / 2.0f);
		angle = angle + float((2 * M_PI) / 60);
	}
}

void configureCircle(sf::CircleShape *circle, int pointCount, float outlineTickness, sf::Color color, int isFilled, float x, float y)
{
	circle->setPointCount(size_t(pointCount));
	circle->setOutlineThickness(float(outlineTickness));
	circle->setOutlineColor(color);
	if (isFilled)
	{
		circle->setFillColor(color);
	}
	circle->setOrigin(circle->getGlobalBounds().width / 2, circle->getGlobalBounds().height / 2);
	circle->setPosition(x, y);
}

void configureArrow(sf::RectangleShape *arrow, sf::Color color, sf::Vector2u windowSize)
{
	arrow->setFillColor(color);
	arrow->setOrigin(arrow->getGlobalBounds().width / 2, arrow->getGlobalBounds().height);
	arrow->setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);
}

void updateArrows(Application *application)
{
	std::time_t currentTime = std::time(NULL);
	struct tm * ptm = localtime(&currentTime);
	application->analogClock->clockArrows->hourHand->setRotation(float(ptm->tm_hour * 30 + (ptm->tm_min / 2)));
	application->analogClock->clockArrows->minuteHand->setRotation(float(ptm->tm_min * 6 + (ptm->tm_sec / 12)));
	application->analogClock->clockArrows->secondsHand->setRotation(float(ptm->tm_sec * 6));
}

void createNumbers(Application *application)
{
	for (int i = 0; i < 12; i++)
	{
		application->analogClock->numbers[i] = new sf::Text;
	}
}

void configureNumbers(Application *application)
{
	float x;
	float y;
	float angle = 0.0;
	for (int i = 0; i < 12; i++)
	{
		x = (application->clockCircleSize - 30) * cos(angle);
		y = (application->clockCircleSize - 30) * sin(angle);
		application->analogClock->numbers[i]->setFont(*application->clockFont);
		(i < 10) ? application->analogClock->numbers[i]->setString(std::to_string(i + 3)) : application->analogClock->numbers[i]->setString(std::to_string(i - 9));
		application->analogClock->numbers[i]->setCharacterSize(application->numbersSize);
		application->analogClock->numbers[i]->setColor(sf::Color::Blue);
		application->analogClock->numbers[i]->setOrigin(application->analogClock->numbers[i]->getGlobalBounds().width / 2, application->analogClock->numbers[i]->getGlobalBounds().height / 2 + float(application->numbersSize / 4.0));
		application->analogClock->numbers[i]->setPosition(x + application->window->getSize().x / 2.0f, y + application->window->getSize().y / 2.0f);
		angle += float((2 * M_PI) / 12);
	}
}

void drawAFrame(Application *application)
{
	application->window->clear(sf::Color::White);
	application->window->draw(*application->analogClock->clockCountour);
	for (int i = 0; i < 12; i++)
	{
		application->window->draw(*application->analogClock->numbers[i]);
	}
	for (int i = 0; i < 60; i++)
	{
		application->window->draw(*application->analogClock->dot[i]);
	}
	application->window->draw(*application->analogClock->clockArrows->hourHand);
	application->window->draw(*application->analogClock->clockArrows->minuteHand);
	application->window->draw(*application->analogClock->clockArrows->secondsHand);
	application->window->draw(*application->analogClock->centerCircle);
	application->window->display();
}

void gameLoop(Application *application)
{
	while (application->window->isOpen())
	{
		sf::Event event;
		while (application->window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				application->window->close();
		}
		updateArrows(application);
		drawAFrame(application);
	}
}

int main()
{
	Application application;
	sf::Font clockFont;
	if (!clockFont.loadFromFile("../Resources/Ubuntu-R.ttf"))
	{
		return EXIT_FAILURE;
	}
	initApplication(&application, &clockFont);
	gameLoop(&application);
	return EXIT_SUCCESS;
}