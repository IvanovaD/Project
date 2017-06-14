#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cstring>
#include<iomanip>
#include<string>
#include<cmath>
#include "Header.h"

using namespace std;
const int MAX = 30;
const char Tmp_fileName[] = "buff.txt"; //name of a buffer-file which is used to convert a double number
                                           //to a char* 
int edit(fstream&, const char fileName[], int k, Grid* cells);
int print(fstream&, const char fileName[], int k, Grid* cells);
int save(fstream&, const char fileName[], int k, Grid* cells);
int save_as(fstream&, const char fileName[], int k, Grid* cells);
int quit(fstream&, const char fileName[], int k, Grid* cells);

struct command
{
	char         key;        
	const char * name;       
	int(*function)(fstream&, const char file_name[], int k, Grid* cells); // a pointer to a command function
	;
} commands[] = {             
	{ 'e', "Edit",    edit },
	{ 'p', "Print",   print },
	{ 's', "Save",    save },
	{ 'a', "Save as", save_as },
	{ 'q', "Quit",    quit },
	{ 0, NULL, NULL } };

int space(Grid* cells, int k, int column) //a help function which makes the grid even
{                                         //it returns the max length of the column
	int j=0;
	for (int i = 0; i < k; i++)
	{
		if (j < strlen(cells[i].getContent()) && cells[i].getColumn() == column)
			j = strlen(cells[i].getContent());
	}
	return j;
}


int maxCol(fstream& file)  // a function which counts how many commas
{                         //are found and returns the number of commas
	char ch;              // of the line with maximum commas
	int max = 0;
	int count = 0;
	file.clear();
	file.seekg(0, file.beg);
	while (file)
	{
		ch = file.get();
		if (ch == '\n')
			count = 0;
		if (ch == ',')
			count++;
		if (count > max)
			max = count;
	}
	return max;
}
double calculator(double number1, double number2, char operation) // calculator which
{                                                                 // does the basic operations
	switch (operation)
	{
	case '+':return number1 + number2; break;
	case '-':return number1 - number2; break;
	case '/':return number1 / number2; break;
	case '*':return number1 * number2; break;
	case '^':return pow(number1, number2); break;
	default:
		cout << "Invalid operation" << endl;

	}
	return 0;
}
char* convertTochar(double n, Grid cell) // a function which convert a double number 
{                                        // to a char* 
	ofstream otmp(Tmp_fileName);
	otmp << n;
	otmp.close();
	ifstream itmp(Tmp_fileName, ios::in);
	itmp.seekg(0, itmp.end);
	int length = itmp.tellg();
	itmp.seekg(0, itmp.beg);
	char * buffer = new char[length+1];
	int a = 0;
	char ch;
	while (itmp)
	{
		ch = itmp.get();
	    buffer[a] = ch;
		a++;
	}
	buffer[--a] = {'\0'};

	itmp.close();
	_unlink(Tmp_fileName);
	return buffer;
}
void calculateCells(Grid* cells, int k) //function which check if a cell contains formula
{                                        //and if so, calculates it
	int b = 0;
	double number, number2, number1;
	int column, row;
	bool first = true;
	char operation = 'o';
	for (int i = 0; i < k; i++)
	{
		if (cells[i].getContent()[0] == '=')
		{
			first = true;
			for (int j = 1; j < strlen(cells[i].getContent()); j++)
			{
				if (cells[i].getContent()[j] == 'R')
				{
					j++;
				}
				char* buff = new char[strlen(cells[i].getContent()) + 1];
				while (cells[i].getContent()[j] >= '0' && cells[i].getContent()[j] <= '9'
					|| cells[i].getContent()[j] == '.')
				{
					buff[b] = cells[i].getContent()[j];
					b++;
					j++;
				}
				buff[b] = { '\0' };
				if (cells[i].getContent()[j] == 'C')
				{
					row = atoi(buff);
					j++;
					b = 0;
					while (cells[i].getContent()[j] >= '0' && cells[i].getContent()[j] <= '9')
					{
						buff[b] = cells[i].getContent()[j];
						b++;
						j++;
					}
					buff[b] = { '\0' };
					column = atoi(buff);
					int t = 0;
					while (t < k && (cells[t].getRow() != row || cells[t].getColumn() != column))
					{
						t++;
					}
					if (cells[t].getRow() != row || cells[t].getColumn() != column || cells[t].checkStr(cells[t])==false)
						number = 0;
					else
						number = atof(cells[t].getContent());
					if (first)
					{
						number1 = number;
						first = false;
						operation = cells[i].getContent()[j];
					}
					else
						number2 = number;
				}
				else
				{
					if (first)
					{
						number1 = atof(buff);
						first = false;
						operation = cells[i].getContent()[j];
					}
					else
					{
						number2 = atof(buff);
					}
				}

				b = 0;
				delete[] buff;
			}
			if (operation == '/' && number2 == 0)
				cells[i].setContent("ERROR");
			else
			{
				double n = calculator(number1, number2, operation);
				cells[i].setContent(convertTochar(n, cells[i]));
			}
		}
	}
}
int edit(fstream& file, const char fileName[], int k, Grid* cells) //edit command
{
	int row, column;
	cout << "Row:";
	do {
		cin >> row;
		if (row > k / (maxCol(file) + 1))
			cout<<"Please, enter a number, which is not bigger than " << k / (maxCol(file) + 1) << endl;
	} while (row > k / (maxCol(file) + 1));
	cout << "Column:";
	do {
		cin >> column;
		if(column>maxCol(file)+1)
			cout<<"Please, enter a number, which is not bigger than " <<(maxCol(file) + 1) << endl;
	} while (column > maxCol(file) + 1);
	for (int i = 0; i < k; i++)
	{
		if (cells[i].getRow() == row && cells[i].getColumn() == column)
		{
			int j = strlen(cells[i].getContent());
			cout << "Content:";
			char* content = new char[j];
			cin >> content;
			cells[i].setContent(content);
		}
	}
	calculateCells(cells, k);
	return 0;
}

int save(fstream& file, const char fileName[], int k, Grid* cells) //save command
{
	int columns= maxCol(file)+1;
	ofstream output(fileName);
		for (int i = 0; i < k; i++)
		{
			output << cells[i].getContent();
			if (cells[i].getColumn() != columns)
				output << ",";
			else
				output << endl;
		}
		file.clear();
		file.seekg(0);
		output.close();
		cout<<fileName << " successfully saved! " << endl;
		return 0;
	}
int save_as(fstream& file, const char fileName[], int k, Grid* cells) //save as command
{ 
	char path[50];
	ofstream output;
	bool exist = 1;
	int columns = maxCol(file) + 1;
	do
	{
		cin >> path;
		ofstream output(path);
		if (!output)
		{
			cout << "The directory has not been found!" << endl
				<< "Please enter another directory!" << endl;
			exist = 0;
		}
		else
			exist = 1;
	} while(!exist);
	output.open(path);
    for (int i = 0; i < k; i++)
	{
		output << cells[i].getContent();
		if (cells[i].getColumn() != columns)
			output << ",";
		else
			output << endl;
	}
	file.clear();
	file.seekg(0);
	output.close();
	cout << "Successfully saved to "<<path<< endl;
	return 0;

}
int print(fstream& file, const char fileName[], int k, Grid* cells) //print command
{
	for (int i = 0; i < k; i++)
	{
		int column = cells[i].getColumn();
		if (column == 1 && i != 0)
			cout << endl;
		cout << " ";
		int dist = space(cells, k, column);
		cout << setw(dist + 1) << setiosflags(ios::left) << cells[i].getContent();
		cout<< "|";
	}
	cout << endl;
	return 0;
}
int quit(fstream& file, const char file_name[], int k, Grid* cells) //command for quitting
{
	file.close();
	cout << "Exiting the program ..." << endl;
	return 1;
}
void print_menu() // start menu
{
	cout << "MENU" << endl;
	for (int i = 0; commands[i].name; ++i)
	{
		cout << commands[i].key << " : "
			<< commands[i].name << "\n";
	}
}
int read_command()
{
	char key = -1;
	cin >> key;
	if (!cin) return 1;
	for (int i = 0; commands[i].name; ++i) //loop which compares the possible
	{                                      // commands and the command entered by the user
		if (commands[i].key == key)         //and returns the index of the command
			return i;
	}
	cout << "Command cannot be executed" << endl;
	return -1;
}
int execute_command(int command, fstream& file,  const char file_name[], int k, Grid* cells) 
{
	if (command >= 0 && commands[command].function)  
	{
		return commands[command].function(file, file_name, k, cells); // brings the execution of a command
	}
	return 0;
}
void start(fstream& file, char fileName[MAX]) //initicialization of the file
{
	cout << "File name: ";
	cin.getline(fileName, MAX);
	file.open(fileName);
	if (!file)
	{
		cout << "The file does not exist..." << endl << "...creating a new file" << endl;
		file.open(fileName, ios::in | ios::out | ios::binary | ios::trunc);
	}
}

int main()
{

	char fileName[MAX];
	cout << "Opening a file" << endl;
	fstream file;
	start(file, fileName);
	file.seekg(0, file.end);
	int length = file.tellg();
	file.seekg(0, file.beg);
	bool error = false;//checks for errors
	if (length == 0)
	{
		cout << "The file is empty!" << endl;
		return 0;
	}
	bool quotes = true;
	int columns = maxCol(file);
	file.clear();
	file.seekg(0, file.beg);
	Grid* cells = new Grid[length + 1];
	int count = 0;
	//counts how many times a comma is found
	int column = 1, row = 1;
	bool type = true;
	char ch;
	int j = 0, k = 0;
	char* buff = new char[length + 1]; //a temporary buffer which is used
									// to contain the elements which will
									//be extracted
	while (file) // a loop which goes over the file and extracts
	{            // the symbols and puts them in differtent cells
		ch = file.get();
		
		if (ch == '"' && j == 0)
		{
			type = false;
			ch = file.get();
		}
		else if (j == 0)
			type = true;
		if (file.peek() == ',' && ch != '"' && !type)
		{
			quotes = false;
			type = true;
		}
			 if (ch == '\\')
		{
			if (file.peek() == '"')
				ch = file.get();
		}
		if (ch != ',' && ch != ' ' && ch != '\n')// && length != file.tellg()) 
		{
			buff[j] = ch;
			j++;

		}
		if (ch == ',')
		{
			count++;
			if (j != 0)
			{
				buff[j] = { '\0' };
				cells[k].setContent(buff);
			}
			else
			{
				cells[k].setContent("");
			}
			cells[k].setRow(row);
			cells[k].setColumn(column);
			if (type == true)
				if (cells[k].checkStr(cells[k]) == false || quotes==false)
				{
					cells[k].printError();
					error = true;
					quotes = true;
				}

			++column;
			j = 0;
			k++;
		}
		if (ch == '\n' || length == file.tellg())
		{

			if (j != 0 || (j == 0 && count == columns))
			{
				buff[j] = { '\0' };
				cells[k].setContent(buff);
				cells[k].setRow(row);
				cells[k].setColumn(column);
				if (type == true)
					if (cells[k].checkStr(cells[k]) == false || quotes==false)
					{
						cells[k].printError();
						error = true;
						quotes = true;
					}
				
				column++;
				count++;
				k++;
				j = 0;

			}
			if (j == 0)
			{
				while (count <= columns)
				{
					cells[k].setContent("");
					cells[k].setRow(row);
					cells[k].setColumn(column);
					column++;
					count++;
					k++;
				}
			}
			column = 1;
			count = 0;
			row++;
		}
	}
	if (error == true)
	{
		system("pause");
	return 0;
    }
    delete[] buff;
	calculateCells(cells, k);
	int command, result = 0;
	do    // loop which reads the command from the user 
	{
		print_menu();
		command = read_command();
		if (command < 0)
			command = 0;
		result = execute_command(command, file, fileName, k, cells);
	} while (result == 0);
	
	system("pause");
	return 0;
}