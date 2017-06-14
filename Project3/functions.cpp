#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <fstream>
#include "Header.h"

Grid::Grid() : content(NULL)
{
	setColumn(column);
	setRow(row);
	setContent(content);
}
void Grid::setColumn(int column)
{
	this->column = column;
}
void Grid::setRow(int row)
{
	this->row = row;
}
void Grid::setContent(char* content)
{
	if (!content)
	{
		delete[] this->content;
		this->content = NULL;
		return;
	}
    delete[] this->content;
	this->content = new char[strlen(content)+1];
	strcpy(this->content, content);
}
Grid::Grid(const Grid& table): content(NULL)
{
	setColumn(table.column);
	setRow(table.row);
	setContent(table.content);
}
Grid& Grid::operator=(const Grid& table)
{
	if (this != &table)
	{
		setColumn(table.column);
		setRow(table.row);
		setContent(table.content);
	}
	return *this;
}
int Grid:: getColumn()
{
	return column;
}
int Grid::getRow()
{
	return row;
}
const char* Grid::getContent()
{
	return content;
}
Grid::~Grid()
{
	delete[] content;
}

bool Grid::checkStr(Grid cell)
{
		int count = 0;
		for (int i = 0; i < strlen(cell.getContent()); i++)
		{
			if ((cell.getContent()[i]<'0' || cell.getContent()[i]>'9'  )&& cell.getContent()[i] != '.' && cell.getContent()[0]!='=' || cell.getContent()[0] == '.')
				return false;
			if (cell.getContent()[i] == '.')
				count++;
		}
		if (count > 1)
			return false;
		return true;
}
void Grid::printError()
{
  std::cout << "Error: row " << getRow() << ", col " << getColumn() << ", "
			<< getContent() << " is unknown data type." << std::endl;
}


