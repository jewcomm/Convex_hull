#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <cmath>
#include <list>

#define PI 3.14159265


//структура для хранения координат точки
struct point
{
	int x; // координата по х
	int y; // координата по y
};

// прототипы используемых функций
int lowPointFound(point* Point, int amount);
int maxPointFound(point* Point, int amount);
float foundAngle(point PointBase, point PointSecond);

int main()
{
	setlocale(LC_ALL, "rus");

	srand((unsigned int)time(0)); // указываем значение для ГСЧ

	const int amount = 20; // общее количество точек в пространстве
	point Point[amount] = {}; // массив структуры

	for (int i = 0; i < amount; i++) // случайно заполняем массив точек пространства
	{
		Point[i].x = 5 + rand() % 390;
		Point[i].y = 5 + rand() % 390;
	}

	for (int i = 0; i < amount; i++) // выводим полученный массив точек
	{
		std::cout << "Номер ячейки = " << i << std::endl;
		std::cout << "Координата по Х = " << Point[i].x << std::endl;
		std::cout << "Координата по Y = " << Point[i].y << std::endl;
		std::cout << std::endl;
	}

	int lowPoint = lowPointFound(Point, amount); // находим нижнюю точку
	int maxPoint = maxPointFound(Point, amount); // находим верхнюю точку

	int outShell[amount]; // задаем массив для хранения пути обхода внешней оболочки
	for (int i = 1; i < amount; i++) // заполняем ячейки значением -1 для отслеживания конца пути
	{
		outShell[i] = -1;
	}
	outShell[0] = lowPoint; // в первую ячейку записываем нижнюю точку

	int right_part = 0; // флаг достижения верхней точки 
	// при достижении наивысшей точки, игнорируем 1 и 2 четверть 

	for (int count = 1; count < amount; count++) {
		float min_angle = 361.0; // задаем минимальный угол невозможным значением
		int next_point = -1; // задаем индекс следующей точки -1
		for (int i = 0; i < amount; i++) // цикл по всем точкам
		{
			if (i == outShell[count - 1]) continue; // игнорируем базовую точку
			float angle = foundAngle(Point[outShell[count - 1]], Point[i]); // находим угол между текущей точкой, и базовой
			if (angle == -1) return -1; // аварийное завершение программы 
			if (angle < 180.0 && right_part == 1) continue; // если достигли наивысшей точки, то игнорируем точки с углом меньше 180
			if (angle < min_angle) // сравниваем полученный угол с минимальным
			{
				min_angle = angle;
				next_point = i;
			}
		}
		if (next_point == lowPoint) // если следующая точка самая нижняя, то завершаем цикл
		{
			break;
		}
		outShell[count] = next_point; // записываем следующую точку в список для обхода 
		if (next_point == maxPoint) right_part = 1; // если достигли верхней точки, то игнорируем 1 и 2 четверть
	}

	sf::RenderWindow window(sf::VideoMode(400, 400), "SFML Works!");

	while (window.isOpen())
	{
		// Обрабатываем очередь событий в цикле
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Пользователь нажал на «крестик» и хочет закрыть окно?
			if (event.type == sf::Event::Closed)
				// тогда закрываем его
				window.close();
		}
		// Отрисовка окна	
		window.clear();

		// выводим все точки
		for (int i = 0; i < amount; i++)
		{
			sf::CircleShape circle(4.f);
			if (i == lowPoint) circle.setFillColor(sf::Color::Green); // если нижняя точка
			else circle.setFillColor(sf::Color(230, 55, 230));
			circle.setPosition(Point[i].x - (float)4, Point[i].y - (float)4);
			window.draw(circle);
		}

		// выводим линии по точкам внешней оболочки
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

		// точки входящие во внешнюю оболочку окрашиваем в красный
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
Так как точка 0.0 находится в левом верхнем углу окна, то:
нижняя точка - точка с максимальной координатой по y
верхняя точка - точка с минимальной координатой по y
*/


/*
Функция нахождения нижней точки
Point - указатель на массив точек
amount - общее количество точек
*/
int lowPointFound(point* Point, int amount) // функция нахождения нижней точки 
{
	int lowPoint = 0; // задаем точку с индексом 0 как самую нижнюю
	for (int i = 1; i < amount; i++) // в цикле сравниваем остальные точки с самой нижней
	{
		if (Point[i].y > Point[lowPoint].y) lowPoint = i;
	}
	return lowPoint;
}


/*
Функция нахождения верхней точки
Point - указатель на массив точек
amount - общее количество точек
*/
int maxPointFound(point* Point, int amount)
{
	int maxPoint = 0; // задаем точку с индексом 1 как самую верхнюю 
	for (int i = 1; i < amount; i++) // в цикле сравниваем остальные точки с самой верхней
	{
		if (Point[i].y < Point[maxPoint].y) maxPoint = i;
	}
	return maxPoint;
}

/*
Функция нахождения угла между двумя точками
PointBase - базовая точка, принимаемая за начало системы координат
PointSecond - вторая точка, угол которой требуется найти относительно начала системы координат
*/
float foundAngle(point PointBase, point PointSecond)
{
	float deltaX = (float)(PointBase.x - PointSecond.x); // находим разницу между точками по Y
	float deltaY = (float)(PointBase.y - PointSecond.y); // находим разницу между точками по X
	if (deltaX == 0 && deltaY > 0) return 90.0; // угол 90
	if (deltaX == 0 && deltaY < 0) return 270.0; // угол 270
	if (deltaY == 0 && deltaX > 0) return 0.0; // угол 0
	if (deltaY == 0 && deltaX < 0) return 180.0; // угол 180
	float tan = deltaY / deltaX; // находим тангенс
	float result = (float)(atan(tan) * 180.0 / PI); // переводим тангенс в угол
	if (result < 0) result = result + (float)360.0; // если угол представлен как отрицательный

	if (deltaY > 0) // 1 и 2 четверть
	{
		if (result > 0 && result < 180) return result; // если угол в пределах от 0 до 180 то возвращаем его
		else return (result - (float)180.0); // в случае некорректного перевода из отрицательного в положительный
	}

	if (deltaY < 0) // 3 и 4 четверть
	{
		if (result > 180 && result < 360) return result; // если угол в пределах от 180 до 360 то возвращаем его
		else return (result + (float)180.0); // в случае некорректного перевода из отрицательного в положительный
	}
	return -1;
}
