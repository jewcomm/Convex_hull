#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <cmath>
#include <list>

#define PI 3.14159265


//��������� ��� �������� ��������� �����
struct point
{
	int x; // ���������� �� �
	int y; // ���������� �� y
};

// ��������� ������������ �������
int lowPointFound(point* Point, int amount);
int maxPointFound(point* Point, int amount);
float foundAngle(point PointBase, point PointSecond);

int main()
{
	setlocale(LC_ALL, "rus");

	srand((unsigned int)time(0)); // ��������� �������� ��� ���

	const int amount = 20; // ����� ���������� ����� � ������������
	point Point[amount] = {}; // ������ ���������

	for (int i = 0; i < amount; i++) // �������� ��������� ������ ����� ������������
	{
		Point[i].x = 5 + rand() % 390; 
		Point[i].y = 5 + rand() % 390;
	}

	for (int i = 0; i < amount; i++) // ������� ���������� ������ �����
	{
		std::cout << "����� ������ = " << i << std::endl;
		std::cout << "���������� �� � = " << Point[i].x << std::endl;
		std::cout << "���������� �� Y = " << Point[i].y << std::endl;
		std::cout << std::endl;
	}

	int lowPoint = lowPointFound(Point, amount); // ������� ������ �����
	int maxPoint = maxPointFound(Point, amount); // ������� ������� �����

	int outShell[amount]; // ������ ������ ��� �������� ���� ������ ������� ��������
	for (int i = 1; i < amount; i++) // ��������� ������ ��������� -1 ��� ������������ ����� ����
	{
		outShell[i] = -1; 
	}
	outShell[0] = lowPoint; // � ������ ������ ���������� ������ �����

	int right_part = 0; // ���� ���������� ������� ����� 
	// ��� ���������� ��������� �����, ���������� 1 � 2 �������� 

	for (int count = 1; count < amount; count++) {
		float min_angle = 361.0; // ������ ����������� ���� ����������� ���������
		int next_point = -1; // ������ ������ ��������� ����� -1
		for (int i = 0; i < amount; i++) // ���� �� ���� ������
		{
			if (i == outShell[count - 1]) continue; // ���������� ������� �����
			float angle = foundAngle(Point[outShell[count - 1]], Point[i]); // ������� ���� ����� ������� ������, � �������
			if (angle == -1) return -1; // ��������� ���������� ��������� 
			if (angle < 180.0 && right_part == 1) continue; // ���� �������� ��������� �����, �� ���������� ����� � ����� ������ 180
			if (angle < min_angle) // ���������� ���������� ���� � �����������
			{
				min_angle = angle; 
				next_point = i; 
			}
		}
		if (next_point == lowPoint) // ���� ��������� ����� ����� ������, �� ��������� ����
		{
			break;
		}
		outShell[count] = next_point; // ���������� ��������� ����� � ������ ��� ������ 
		if (next_point == maxPoint) right_part = 1; // ���� �������� ������� �����, �� ���������� 1 � 2 ��������
	}

	sf::RenderWindow window(sf::VideoMode(400, 400), "SFML Works!"); 
	
	while (window.isOpen())
	{
		// ������������ ������� ������� � �����
		sf::Event event;
		while (window.pollEvent(event))
		{
			// ������������ ����� �� �������� � ����� ������� ����?
			if (event.type == sf::Event::Closed)
				// ����� ��������� ���
				window.close();
		}
		// ��������� ����	
		window.clear();

		// ������� ��� �����
		for (int i = 0; i < amount; i++)
		{
			sf::CircleShape circle(4.f);
			if (i == lowPoint) circle.setFillColor(sf::Color::Green); // ���� ������ �����
			else circle.setFillColor(sf::Color(230, 55, 230));
			circle.setPosition(Point[i].x - (float)4, Point[i].y - (float)4);
			window.draw(circle);
		}

		// ������� ����� �� ������ ������� ��������
		for (int i = 1; i < amount && outShell[i] != -1; i++)
		{
			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f((float)Point[outShell[i - 1]].x, (float)Point[outShell[i - 1]].y)),
				sf::Vertex(sf::Vector2f((float)Point[outShell[i]].x, (float)Point[outShell[i]].y))
			};
			window.draw(line, 2, sf::Lines);
			if (outShell[i + 1] == -1 || (i + 1) == amount)
			{
				sf::Vertex line[] =
				{
					sf::Vertex(sf::Vector2f((float)Point[outShell[i]].x, (float)Point[outShell[i]].y)),
					sf::Vertex(sf::Vector2f((float)Point[outShell[0]].x, (float)Point[outShell[0]].y))
				};
				window.draw(line, 2, sf::Lines);
			}
		}

		// ����� �������� �� ������� �������� ���������� � �������
		for (int i = 0; i < amount; i++)
		{
			sf::CircleShape circle(4.f);
			circle.setFillColor(sf::Color::Red);
			circle.setPosition(Point[outShell[i]].x - (float)4, Point[outShell[i]].y - (float)4);
			window.draw(circle);
		}

		window.display();
	}
	return 0;
}

/*
��� ��� ����� 0.0 ��������� � ����� ������� ���� ����, ��: 
������ ����� - ����� � ������������ ����������� �� y
������� ����� - ����� � ����������� ����������� �� y
*/


/*
������� ���������� ������ �����
Point - ��������� �� ������ �����
amount - ����� ���������� �����
*/
int lowPointFound(point* Point, int amount) // ������� ���������� ������ ����� 
{
	int lowPoint = 0; // ������ ����� � �������� 0 ��� ����� ������
	for(int i = 1; i < amount; i++) // � ����� ���������� ��������� ����� � ����� ������
	{
		if (Point[i].y > Point[lowPoint].y) lowPoint = i; 
	}
	return lowPoint;
}


/* 
������� ���������� ������� �����
Point - ��������� �� ������ �����
amount - ����� ���������� �����
*/
int maxPointFound(point* Point , int amount) 
{
	int maxPoint = 0; // ������ ����� � �������� 1 ��� ����� ������� 
	for (int i = 1; i < amount; i++) // � ����� ���������� ��������� ����� � ����� �������
	{
		if (Point[i].y < Point[maxPoint].y) maxPoint = i;
	}
	return maxPoint;
}

/*
������� ���������� ���� ����� ����� �������
PointBase - ������� �����, ����������� �� ������ ������� ���������
PointSecond - ������ �����, ���� ������� ��������� ����� ������������ ������ ������� ���������
*/
float foundAngle(point PointBase, point PointSecond)
{
	float deltaX = (float)(PointBase.x - PointSecond.x); // ������� ������� ����� ������� �� Y
	float deltaY = (float)(PointBase.y - PointSecond.y); // ������� ������� ����� ������� �� X
	if (deltaX == 0 && deltaY > 0) return 90.0; // ���� 90
	if (deltaX == 0 && deltaY < 0) return 270.0; // ���� 270
	if (deltaY == 0 && deltaX > 0) return 0.0; // ���� 0
	if (deltaY == 0 && deltaX < 0) return 180.0; // ���� 180
	float tan = deltaY / deltaX; // ������� �������
	float result = (float)(atan(tan) * 180.0 / PI); // ��������� ������� � ����
	if (result < 0) result = result + (float)360.0; // ���� ���� ����������� ��� �������������

	if (deltaY > 0) // 1 � 2 ��������
	{
		if (result > 0 && result < 180) return result; // ���� ���� � �������� �� 0 �� 180 �� ���������� ���
		else return (result - (float)180.0); // � ������ ������������� �������� �� �������������� � �������������
	}
	
	if(deltaY < 0) // 3 � 4 ��������
	{
		if (result > 180 && result < 360) return result; // ���� ���� � �������� �� 180 �� 360 �� ���������� ���
		else return (result + (float)180.0); // � ������ ������������� �������� �� �������������� � �������������
	}
	return -1;
}