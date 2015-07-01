#include "stdafx.h"
#include "class_f.h"

//Методы класса Cell
Cell::Cell()
{
	free = true;
	value = ".";
}
std::ostream& operator<< (std::ostream& os, const Cell& toShowCell)
{
	return (os << toShowCell.value);
}
void Cell::make_free() //Добавлять свободные в ptr_free
{
	free = true;
	value = ".";
}

//Методы класса Line
Line::Line(int rank)
{
	Line::rank = rank;
}
std::string& operator*= (std::string &left, const int right)
{
	left = itos(std::stoi(left) * right);
	return left;
}

//Методы классa Column
Column::Column(int rank): Line(rank) {};
int Line::plus_aw()
{
	int i, j, p = 0;
	for (i = 0; i < rank - 1; ++i)
	{
		if (vec[i]->value == ".")
			continue;
		for (j = i + 1; j < rank; ++j) 
		{
			if (vec[j]->value == ".")
				continue;
			else if (vec[j]->value == vec[i]->value)
			{
				p += std::stoi(vec[i]->value);
				vec[i]->value *= 2;
				vec[j]->make_free();
				break;
			}
			else 
				break;
		}
	}
	return p;
}
int Line::plus_ds()
{
	int i, j, p = 0;
	for (i = rank - 1; i >= 1; --i)
	{
		if (vec[i]->value == ".")
			continue;
		for (j = i - 1; j >= 0; --j) 
		{
			if (vec[j]->value == ".")
				continue;
			else if (vec[j]->value == vec[i]->value)
			{
				p += std::stoi(vec[i]->value);
				vec[i]->value *= 2 ;
				vec[j]->make_free();
				break;
			}
			else 
				break;
		}
	}
	return p;
}
bool Line::sort_aw()
{
	bool ischanged = false;
	for (int i = 0; i < rank - 1; ++i)
	{
		if (vec[i]->value == ".")
		{
			for (int j = i + 1; j < rank; ++j)
				if (vec[j]->value != ".")
				{
					std::swap(vec[i]->value, vec[j]->value);
					std::swap(vec[i]->free, vec[j]->free);
					ischanged = true;
					break;
				}
		}
	}
	return ischanged;
}
bool Line::sort_ds()
{
	bool ischanged = false;
	for (int i = rank - 1; i >= 1; --i)
	{
		if (vec[i]->value == ".")
		{
			for (int j = i - 1; j >= 0; --j)
				if (vec[j]->value != ".")
				{
					std::swap(vec[i]->value, vec[j]->value);
					std::swap(vec[i]->free, vec[j]->free);
					ischanged = true;
					break;
				}
		}
	}
	return ischanged;
}

//Методы класса Field
Field::Field(int k)
{
	std::srand(unsigned(time(NULL))); //Инициализация
	Field::rank = k;
	int i;
	try{
		matrix = new Cell*[rank]();
		for (i = 0; i < rank; ++i)
			matrix[i] = new Cell[rank]();
	}
	catch (std::bad_alloc& ex){
		std::cout << "Error while allocating...\n" <<
			"Error::" << ex.what();
		std::exit(EXIT_FAILURE);
	}
	this->refresh_free();

	Column col(rank);
	int t = 0;
	for (i = t; t < rank; i+=rank)
	{
		if (i >= rank * rank)
		{
			i = ++t - rank;
			this->col.push_back(col);
			col.vec.clear();
		}
		else
			col.vec.push_back(ptr_free[i]);	
	}

	Line line(rank);
	for (i = 1; i <= (rank * rank); ++i)
	{
		line.vec.push_back(ptr_free[i-1]);
		if (i % rank == 0)
		{
			this->line.push_back(line);
			line.vec.clear();
		}
	}

	points = 0;
}
Field::~Field()
{
	for (int i = 0; i < rank; ++i)
			delete[] matrix[i];
	Field::points = 0;
	Field::rank = 0;
}
bool Field::wcontinue()
{
	std::string wtc;
	std::cout << "Хотите продолжить игру, начав её заново?" << 
		"\n0. Нет\n1. Да\nВаш выбор: ";
		//2. Не знаю\n
	do
	{
		std::getline(std::cin, wtc, '\n');
	} while (!check(wtc));
	if (wtc[0] + '0' == 0)
	{
		this->~Field();
		return false;
	}
	return true;
}
void Field::show_field()
{
	std::cout << "Очки: " << points << "\n\n";
	for (int i = 0; i < rank; ++i)
	{
		for(int j = 0; j < rank; ++j)
		{
			std::cout << std::setw(6) << matrix[i][j];
		}
		std::cout << "\n\n";
	}
	std::cout << std::endl;
}
void Field::move(int dir)
{
	system("cls");
	inf();
	const int w = 167, s = 163, a = 145, d = 148;
	int i = 0;
	long long p = points;
	bool ischanged = false;
	switch (dir)
	{
	case (w):
		{
			for (; i < rank; ++i)
			{
				points += this->col[i].plus_aw();
				if (this->col[i].sort_aw())
					ischanged = true;
			}
			break;
		}
	case (a):
		{
			for (; i < rank; ++i)
			{
				points += this->line[i].plus_aw();
				if (this->line[i].sort_aw())
					ischanged = true;
			}
			break;
		}
	case (s):
		{
			for (; i < rank; ++i)
			{
				points += this->col[i].plus_ds();
				if (this->col[i].sort_ds())
					ischanged = true;
			}
			break;
		}
	case (d):
		{
			for (; i < rank; ++i)
			{
				points += this->line[i].plus_ds();
				if (this->line[i].sort_ds())
					ischanged = true;
			}
			break;
		}
	}
	if (!ischanged && p == points)
	{
		if (iscollected())
		{
			std::cout << "Игра окончена. Ваш рекорд - " << points << " очк(а)ов\n";
			return;
		}
		this->show_field();
		this->move(get_dir());
	}
}
void Field::refresh_free()
{
	ptr_free.clear();
	for (int i = 0; i < rank; ++i)
		for (int j = 0; j < rank; ++j)
			if(matrix[i][j].free)
			ptr_free.push_back(&matrix[i][j]);
}
void Field::save_record_in_file()
{
	std::string name;
	std::ofstream ofs("records.txt", std::ostream::out);
	std::cout << "Введите Ваше имя: ";
	std::getline(std::cin, name, '\n');
	ofs << name << ": " << points << std::endl;
	ofs.close();
}
bool Field::add() //не добавлять, если хода не было
{
	if (!ptr_free.empty())
	{
		while (true)
		{
			int i = rand() % ptr_free.size();
			Cell* ptr = ptr_free[i];
			if (ptr->free == true)
			{
				ptr->value = itos(2 * (rand() % 2 + 1));
				ptr->free = false;
				ptr_free.erase(ptr_free.begin() + i);
				return true;
			}
		}
	}
	return !iscollected();	
}
bool Field::iscollected()
{
	int i = 0, j = 1;
	for (;i < rank; ++i)
		for (; j < rank; ++j)
			if (matrix[i][j-1].value == matrix[i][j].value)
				return false;
	for (i = 1; i < rank; ++i)
		for (j = 0; j < rank; ++j)
			if (matrix[i-1][j].value == matrix[i][j].value)
				return false;
	return true;
}

//Описание функций
void inf()
{
	setlocale(LC_ALL, "Russian");
	system("color 3F");
	std::cout << "\t\t\t\tИгра \"2048\"\n";
}
void error()
{
	std::cout << "Ошибка проверки введенных даннаых! Новый ввод: ";
}
int get_rank()
{
	std::string str;
	std::cout << "Введите размер поля (NxN), {N | N=[4,16], N - чётное}. N = ";
	do
	{
		std::getline(std::cin, str, '\n');
	} while (!monkey_check(str));
	return stoi(str);
}
bool monkey_check(std::string toCheck)
{
	size_t length = toCheck.length(), i;
	for (i = 0; i < length; ++i)
		if (!isdigit(toCheck[i]))
		{
			error();
			return false;
		}
	size_t rank = stoi(toCheck);
	if ((rank < 4)|| (rank > 16) || (rank % 2 == 1))
	{
		error();
		return false;
	}
	return true;
}
bool check(std::string str)
{
	if (str.length() == 1)
	{
		if (str[0] == '1')
		{
			system("cls");
			inf();
			return true;
		}
		//else if (str[0] == '2')
			//std::cout << "Значит, Вы - Катя";
		else if (str[0] == '0')
			exit(EXIT_SUCCESS);
	}
	error();
	return false;
}
std::string itos(int x)
{
	std::stringstream ss;
	ss << x;
	std::string str;
	ss >> str;
	return str;
}
int get_dir()
{
	char dir;
	while (true)
	{
		dir = _getch();
		if (dir == 'w' || dir == 's' || dir == 'a' || dir == 'd')
			break;
	}
	std::cout << std::endl;
	return (dir + '0');
}