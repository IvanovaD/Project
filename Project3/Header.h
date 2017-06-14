#pragma once
class Grid
{
	int column;
	int row;
	char* content;
public:
	Grid();
	void setColumn(int);
	void setRow(int);
	void setContent(char*);
	Grid(const Grid&);
	int getColumn();
	int getRow();
    const char* getContent();
    Grid& operator=(const Grid&);
	~Grid();
	friend std::ostream& operator<<(std::ostream& out, Grid& cell)
	{
		int j = strlen(cell.getContent());
		for (int i = 0; i < j; i++)
		{
			out << cell.getContent();
		}
		return out;
	}
    bool checkStr(Grid cell);
	void printError();
};
