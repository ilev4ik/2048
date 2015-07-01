#include "stdafx.h"
#include <conio.h>
#include <Windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <ostream>
#include <iomanip>
#include <ctime>
#include <vector>
#include <sstream>

//Прототипы функций
void inf();
void error();
int get_dir();
int get_rank();
std::string itos(int);
bool monkey_check(std::string);
bool check(std::string);

 //Прототипы Классов
class Cell
{
public:
	Cell();
	friend std::ostream& operator<< (std::ostream&, const Cell&);
	void make_free();
	friend class Line;
	friend class Field;
private:
	std::string value;
	bool free;
};

class Line
{
public:
	Line(int);
	int plus_aw();
	int plus_ds();
	bool sort_aw();
	bool sort_ds();
	friend std::string& operator*= (std::string&, const int);
	std::vector <Cell*> vec;
private:
	short int rank;
};

class Column: public Line {public:Column(int);};

class Field
{
public:
	Field(int);
	~Field();
	void move(int);
	void show_field();
	void save_record_in_file();
	void refresh_free();
	bool add();
	bool iscollected();
	bool wcontinue();
private:
	short int rank;
	long long int points;
	Cell** matrix;
	std::vector <Cell*> ptr_free;
	std::vector <Line> line;
	std::vector <Column> col;
};