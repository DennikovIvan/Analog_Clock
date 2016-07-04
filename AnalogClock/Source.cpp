#define _CRT_SECURE_NO_WARNINGS
#include <SFML/Graphics.hpp>
#include <ctime>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <string>

const float PI = 3.1415927f;

struct Arrows
{
	sf::RectangleShape *hourHand;
	sf::RectangleShape *minuteHand;
	sf::RectangleShape *secondsHand;
};

struct Clock
{
	int screenWidth;
	int screenHeight;
	int numbersSize;
	float clockCircleSize;
	float clockCircleThickness;
	sf::Texture *clockBackground;
	sf::Font *numbersFont;
	sf::ContextSettings settings;
	sf::RenderWindow *window;
	sf::Vector2f *windowCenter;
	sf::CircleShape *dot[60];
	sf::Text *numbers[12];
	sf::CircleShape *clockCountour;
	sf::CircleShape *centerCircle;
	Arrows *clockArrows;
};

void initClock(Clock *analogClock, sf::Texture *clockBackground, sf::Font *numbersFont);
void initArrows(Clock *analogClock);
void createDots(Clock *analogClock);
void configureCircle(sf::CircleShape *circle, int pointCount, float outlineTickness, sf::Color color, int isFilled, float x, float y);
void configureArrow(sf::RectangleShape *arrow, sf::Color color, sf::Vector2f *center);
void updateArrows(Clock *analogClock);
void createNumbers(Clock *analogClock);
void configureNumbers(Clock *analogClock);
void drawing(Clock *analogClock);
void gameLoop(Clock *analogClock);

void initClock(Clock *analogClock, sf::Texture *clockBackground, sf::Font *numbersFont)
{
	analogClock->screenWidth = 800;
	analogClock->screenHeight = 600;
	analogClock->clockCircleSize = 250;
	analogClock->clockCircleThickness = 5;
	analogClock->numbersSize = 18;
	analogClock->clockBackground = clockBackground;
	analogClock->numbersFont = numbersFont;
	analogClock->settings.antialiasingLevel = 8;
	analogClock->window = new sf::RenderWindow(sf::VideoMode(analogClock->screenWidth, analogClock->screenHeight), "My Analog Clock", sf::Style::Close, analogClock->settings);
	analogClock->windowCenter = new sf::Vector2f(analogClock->screenWidth / 2.0f, analogClock->screenHeight / 2.0f);
	createDots(analogClock);
	createNumbers(analogClock);
	configureNumbers(analogClock);
	analogClock->clockCountour = new sf::CircleShape(analogClock->clockCircleSize);
	analogClock->clockCountour->setTexture(analogClock->clockBackground);
	analogClock->clockCountour->setTextureRect(sf::IntRect(40, 0, 500, 500));
	configureCircle(analogClock->clockCountour, 100, analogClock->clockCircleThickness, sf::Color::Black, 0, analogClock->windowCenter->x + analogClock->clockCircleThickness, analogClock->windowCenter->y + analogClock->clockCircleThickness);
	analogClock->centerCircle = new sf::CircleShape(10);
	configureCircle(analogClock->centerCircle, 100, 1, sf::Color::Red, 1, analogClock->windowCenter->x, analogClock->windowCenter->y);
	analogClock->clockArrows = new Arrows;
	initArrows(analogClock);
}

void initArrows(Clock *analogClock)
{
	analogClock->clockArrows->hourHand = new sf::RectangleShape(sf::Vector2f(5, 150));
	analogClock->clockArrows->minuteHand = new sf::RectangleShape(sf::Vector2f(3, 200));
	analogClock->clockArrows->secondsHand = new sf::RectangleShape(sf::Vector2f(2, 220));
	configureArrow(analogClock->clockArrows->hourHand, sf::Color::Black, analogClock->windowCenter);
	configureArrow(analogClock->clockArrows->minuteHand, sf::Color::Black, analogClock->windowCenter);
	configureArrow(analogClock->clockArrows->secondsHand, sf::Color::Red, analogClock->windowCenter);
}

void createDots(Clock *analogClock)
{
	float angle = 0.0;
	float x;
	float y;
	for (int i = 0; i < 60; i++)
	{
		x = (analogClock->clockCircleSize - 10) * cos(angle);
		y = (analogClock->clockCircleSize - 10) * sin(angle);
		analogClock->dot[i] = (i % 5 == 0) ? new sf::CircleShape(3) : new sf::CircleShape(1);
		configureCircle(analogClock->dot[i], 12, 1, sf::Color::Black, 1, x + analogClock->windowCenter->x, y + analogClock->windowCenter->y);
		angle = angle + ((2 * PI) / 60);
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

void configureArrow(sf::RectangleShape *arrow, sf::Color color, sf::Vector2f *center)
{
	arrow->setFillColor(color);
	arrow->setOrigin(arrow->getGlobalBounds().width / 2, arrow->getGlobalBounds().height);
	arrow->setPosition(*center);
}

void updateArrows(Clock *analogClock)
{
	std::time_t currentTime = std::time(NULL);
	struct tm * ptm = localtime(&currentTime);
	analogClock->clockArrows->hourHand->setRotation(float(ptm->tm_hour * 30 + (ptm->tm_min / 2)));
	analogClock->clockArrows->minuteHand->setRotation(float(ptm->tm_min * 6 + (ptm->tm_sec / 12)));
	analogClock->clockArrows->secondsHand->setRotation(float(ptm->tm_sec * 6));
}

void createNumbers(Clock *analogClock)
{
	for (int i = 0; i < 12; i++)
	{
		analogClock->numbers[i] = new sf::Text;
	}
}

void configureNumbers(Clock *analogClock)
{
	float x;
	float y;
	float angel = 0.0;
	for (int i = 0; i < 12; i++)
	{
		x = (analogClock->clockCircleSize - 30) * cos(angel);
		y = (analogClock->clockCircleSize - 30) * sin(angel);
		analogClock->numbers[i]->setFont(*analogClock->numbersFont);
		(i < 10) ? analogClock->numbers[i]->setString(std::to_string(i + 3)) : analogClock->numbers[i]->setString(std::to_string(i - 9));
		analogClock->numbers[i]->setCharacterSize(analogClock->numbersSize);
		analogClock->numbers[i]->setColor(sf::Color::Red);
		analogClock->numbers[i]->setOrigin(analogClock->numbers[i]->getGlobalBounds().width / 2, analogClock->numbers[i]->getGlobalBounds().height / 2 + float(analogClock->numbersSize / 4.0));
		analogClock->numbers[i]->setPosition(x + analogClock->windowCenter->x, y + analogClock->windowCenter->y);
		angel += ((2 * PI) / 12);
	}
}

void drawing(Clock *analogClock)
{
	analogClock->window->clear(sf::Color::White);
	analogClock->window->draw(*analogClock->clockCountour);
	for (int i = 0; i < 60; i++)
	{
		if (i < 12)
		{
			analogClock->window->draw(*analogClock->numbers[i]);
		}
		analogClock->window->draw(*analogClock->dot[i]);
	}
	analogClock->window->draw(*analogClock->clockArrows->hourHand);
	analogClock->window->draw(*analogClock->clockArrows->minuteHand);
	analogClock->window->draw(*analogClock->clockArrows->secondsHand);
	analogClock->window->draw(*analogClock->centerCircle);
	analogClock->window->display();
}

void gameLoop(Clock *analogClock)
{
	while (analogClock->window->isOpen())
	{
		sf::Event event;
		while (analogClock->window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				analogClock->window->close();
		}
		updateArrows(analogClock);
		drawing(analogClock);
	}
}

int main()
{
	Clock analogClock;
	sf::Font clockFont;
	if (!clockFont.loadFromFile("C:/Workspace/AnalogClock/Resources/Ubuntu-R.ttf"))
	{
		return EXIT_FAILURE;
	}
	sf::Texture clockImage;
	if (!clockImage.loadFromFile("C:/Workspace/AnalogClock/Resources/background.jpg"))
	{
		return EXIT_FAILURE;
	}
	initClock(&analogClock, &clockImage, &clockFont);
	gameLoop(&analogClock);
	return EXIT_SUCCESS;
}