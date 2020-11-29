#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <omp.h>

using namespace std;

vector<int> GenerateArray(int size)
{
	srand(time(0));
	vector<int> array;
	for (size_t i = 0; i < size; i++)
	{
		array.push_back(1 + rand() % 100);
	}
	return array;
}

int getIndex(vector<int> v, int K)
{
	auto it = find(v.begin(), v.end(), K);

	if (it != v.end())
	{
		int index = it - v.begin();
		return index;
	}
	else {
		return -1;
	}
}

void FightMode(int guanin, int guanyan, vector<int>& array1, vector<int>& array2)
{
	srand(time(0));
#pragma omp critical
	{
		if (guanin > guanyan)
		{
			array2[getIndex(array2, guanyan)] = 0;
		}
		else if (guanin < guanyan)
		{
			array1[getIndex(array1, guanin)] = 0;
		}
		else if (guanin == guanyan)
		{
			if ((rand() % 2) == 0)
			{
				array2[getIndex(array2, guanyan)] = 0;
			}
			else
			{
				array1[getIndex(array1, guanin)] = 0;
			}
		}
	}
}

int main()
{
	setlocale(LC_ALL, "ru");
	cout << "Введите пожалуйста количество участников в турнире!(положительное больше нуля!)" << endl;
	vector<int> guanin;
	vector<int> guanyan;
	vector<int> energy;
	string s;
	cin >> s;

	char* end;
	long i = strtol(s.c_str(), &end, 10);
	if (*end == '\0')
	{
		if (i > 0)
		{
			energy = GenerateArray(i);
			for (size_t i = 0; i < energy.size(); i++)
			{
				if ((i % 2) == 0)
				{
					guanin.push_back(energy[i]);
				}
				else
				{
					guanyan.push_back(energy[i]);
				}
			}

			while (guanin.size() != 0 && guanyan.size() != 0)
			{
				for (size_t i = 0; i < guanin.size(); i++)
				{
					if (guanin[i] == 0)
					{
						guanin.erase(guanin.begin() + i);
						--i;
					}
				}
				for (size_t i = 0; i < guanyan.size(); i++)
				{
					if (guanyan[i] == 0)
					{
						guanyan.erase(guanyan.begin() + i);
						--i;
					}
				}
				int size = guanyan.size() < guanin.size() ? guanyan.size() : guanin.size();

				omp_set_num_threads(size > 100 ? 100 : (size + 8) / 8);
#pragma omp parallel
				{
#pragma omp for
					for (int i = 0; i < size; i++)
					{
						FightMode(guanin[i], guanyan[i], guanin, guanyan);
					}
				}
			}

			if (guanin.size() != 0)
			{
				cout << "Победил монастырь Гуань-Инь. Их Путь Кулака более совершенен. Они забирают себе на хранение статую боддисатвы Араватти." << endl;
			}
			else
			{
				cout << "Победил монастырь Гуань-Янь. Их Путь Кулака более совершенен. Они забирают себе на хранение статую боддисатвы Араватти." << endl;
			}
		}
		else
		{
			cout << "К сожалению вы вввели некорректные данные!" << endl;
		}
	}
	else
	{
		cout << "К сожалению вы вввели некорректные данные!" << endl;
	}
}