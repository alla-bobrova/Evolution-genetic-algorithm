// эволюционно гететический алгоритм.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
// Л.р. №10

#include <iostream>
#include <string>
#include <time.h>
#include <locale.h>
#include <fstream>
#include <vector>

using namespace std;

vector<string> generation (int* w, int* c, int maxW, int N, int choise, int choise2)   //генерация популяции или кодирование
{
	vector<string> gener;
	int rnd;
	if (choise == 1) // Жадный алгоритм Данцига
	{
		for (int i = 0; i < choise2; i++)
		{
			int size = 0;                   //текущий вес ранца
			int elem;                       //предмет
			string str = "";
			for (int j = 0; j < N; j++) str = str + '0';      //формируем нулевую строку
			rnd = rand() % N;                                //берем какой-то предмет рандомный
			str[rnd] = '1';                                  //взяли, значит к строке-решению добавляем 1
			size = w[rnd];                                  //+вес этого предмета к текущему

			for (int j = 0; j < N - 1; j++)                      //n-1, т.к один предмет уже взяли
			{
				int max = 0;
				elem = -1;                                     //когда уже взяли предмет
				for (int k = 0; k < N; k++)                   //проходим по всем предметам
					if (c[k] / w[k] > max && str[k] == '0')  //удельная стоимость больше макисмума и предмет еще не взят
					{
						max = c[k] / w[k];                
						elem = k;                   // запомнили номер предмета
					}
				if (size + w[elem] < maxW)         // если добавить элемент, будет ли вес ранца меньше или равен его вместимости
				{
					size = size + w[elem];       // +вес элемента
					str[elem] = '1';            // в строку 1
				}
				else break;
			}
			gener.push_back(str);               //записали решение в вектор
		}
	}

	else if (choise == 2)    // случайная генерация
	{
		for (int i = 0; i < choise2; i++)       //choise2-введенное количество особей в популяции
		{
			string str = "";
			for (int j = 0; j < N; j++)         //проходим по предметам
			{
				rnd = rand() % 100 + 1;
				if (rnd < 25) str = str + '1';       //
				else str = str + '0';
			}
			gener.push_back(str);
		}
	}
	return gener;
}

string сros(string par1, string par2)       // кроссовер моделирует перемешивание генов при скрещивании
{
	string child = "";
	int rnd = rand() % 2;
	if (rnd == 0)             // одноточечный кроссовер
	{
		
		int rnd2 = rand() % par1.size();                //rnd2-точка разрыва
		for (int i = 0; i < rnd2; i++)               
			child = child + par1[i];                   //записываем потомку информацию от 1го родителя 
		for (int i = rnd2; i < par2.size(); i++)      
			child = child + par2[i];                  //записываем инф-ю 2го родителя
	}

	else                   //двухточечный кроссовер
	{
		
		int rnd2 = rand() % (par1.size() / 2);              //rnd2-точка разрыва
		int rnd3 = rand() % (par1.size() / 2) + 7;         //rnd3-точка разрыва
		for (int i = 0; i < rnd2; i++)                       
			child = child + par1[i];                     //инф 1го родителя
		for (int i = rnd2; i < rnd3; i++)                  
			child = child + par2[i];                   //инф 2го
		for (int i = rnd3; i < par2.size(); i++)
			child = child + par1[i];                 //инф 1го
	}
	return child;
}

string mutation(string str)         //операторы мутации. Мутация моделирует изменение генов при скрещивании
{
	int rnd = rand() % 2;
	if (rnd == 0)            // генная мутация (точечная)
	{
		int rnd2 = rand() % str.size();                   // т.к бинарное кодирование, значение алелли меняется на противоположное
		if (str[rnd2] == '1') str[rnd2] = '0';           // меняем на противоположное в точке разрыва
		else str[rnd2] = '1';
	}
	else                    // макромутация (сальтация)
	{
		int rnd2 = rand() % (str.size() / 2) + 1;
		char tmp;
		tmp = str[rnd2];                             //запомнили значение в точке разрыва
		str[rnd2] = str[str.size() - rnd2];         //точке разрыва присваиваем значение с позиции размер строки- точка разрыва
		str[str.size() - rnd2] = tmp;              //а на нее соотв значение точки разрыва
	}
	return str;
}

int getWQ(string str, int* arg)     // возвращает вес и цену
{
	int WQ = 0;
	for (int i = 0; i < str.size(); i++)
		if (str[i] == '1') WQ = WQ + arg[i];
	return WQ;
}

int main()
{
	setlocale(0, "");
	srand(time(0));
	int N = 15, maxW = 88, choise, choise2, choise3;
	vector<int> Q;
	vector<int> W;
	vector<string> population;        // вектор популяции
	int q_max = 0, weight_max = 0;
	string str_max;
	int* S, * w, * c;
	S = new int[N];
	w = new int[N];
	c = new int[N];

	ifstream in("file.txt");
	if (in.is_open())                   // считали из файла, занесли в соответствующие массивы
	{
		for (int i = 0; i < N; i++)
		{
			in >> c[i];
			in >> w[i];
			S[i] = 0;
		}
	}
	else exit(1);
	in.close();

	cout << "Исходные данные:" << endl;
	for (int i = 0; i < N; i++)
	cout << "Предмет: " << i + 1 << "\tВес: " << w[i]  <<'\t' << "\tСтоимость: " << c[i] << endl;
	cout << "Вместимость ранца: " << maxW << endl;

	cout << endl << "Выберите способ генерации начальной популяции" << endl;
	cout << "1) Создание начальной популяции жадным алгоритмом Данцига" <<'\t'<< "2) Создание начальной популяции случайным образом"<< endl;
	cin >> choise;

	cout << "Введите кол-во особей в популяции: ";
	cin >> choise2;

	population = generation (w, c, maxW, N, choise, choise2);          // заносим в вектор сгенерированную популяцию
	cout << endl<< "Сгенерированная начальная популяция:  "<< endl;
	
	for (int i = 0; i < population.size(); i++)          //проходим по популяции
	{ 
		Q.push_back(0);                                 //вектор значений критерия
		cout << population[i];                         //вывели популяцию (сами решения-кодировки)
		int weight = getWQ(population[i], w);         //вернули вес
		if (weight > maxW) Q[i] = 0;                 //!!!!!!!!!проверили, является ли кандидат решением!!!!!!!!!!!
		else Q[i] = getWQ(population[i], c);        //если является, критерий=цене
		W.push_back(weight);                       //вес ранца при каком-то решении
		
		if (Q[i] > q_max)                        //ищем лучшее решение (по критерию), то есть в популяцию мы выбрали лучшие решения
		{
			q_max = Q[i];                         
			weight_max = W[i];
			str_max = population[i];
		}
		cout << " Критерий: " << Q[i] << " Вес: " << W[i] << endl;
	}
	cout << endl << "Введите кол-во поколений: ";
	cin >> choise3;
	int stop = 0;
	
	for (int i = 0; i < choise3; i++)
	{
		stop++;  //число поколений без изменения решения (по приспособленности проверяем)
		cout << endl << "Номер поколения: " << i + 1 << endl<< "Текущее оптимальное решение: " << str_max << " Значение критерия: " << q_max << " Вес: " << weight_max << endl;
		
		vector<string> child;
		vector<int> childQ;
		vector<int> childW;
		vector<int> mutants;

		cout << endl;

		cout << "Текущая популяция: " << endl;
		for (int j = 0; j < population.size(); j++)
			cout << population[j] << " критерий: " << Q[j] << " вес: " << W[j] << endl;

		cout << endl;

		cout << "Родительские пары: " << endl;
		for (int j = 0; j < rand() % 3 + population.size(); j++)
		{
			string par1 = "", par2 = "";
			int rnd1 = rand() % 2;          // выбор способа формирования родительской пары задан случайно
			
			if (rnd1 == 0)                 // панмиксия (равновероятный выбор родителей из популяции)
			{
				int rnd2 = rand() % population.size();
				int rnd3 = rand() % population.size();
				if (rnd2 == rnd3)
					if (rnd2 + 1 < population.size()) rnd2++;   //если совпали берем следующего в популяции (если сущ), иначе предыдущего
					else rnd2--;
				par1 = population[rnd2];
				par2 = population[rnd3];
			}
			else     // отрицательный ассортативный принцип (выбираются особи, сильно отличающиеся по степени приспособленности)
			{
				int min = 99999; int max = -1; int kmax = -1; int kmin = -1;
				for (int i = 0; i < population.size(); i++)
				{
					if (Q[i] > max)           //ищем родителя с максимальной приспособленностью, с минимальной, из них составляем пару
					{
						max = Q[i];
						kmax = i;
					}
					if (Q[i] < min)
					{
						max = Q[i];
						kmin = i;
					}
				}
				par1 = population[kmax]; par2 = population[kmin];
			}
			for (int k = 0; k < rand() % 3; k++)
			{
				string tmp = сros(par1, par2);       //вызываем ф-ю кроссовера
				child.push_back(tmp);               //получили потомка
			}
			cout << par1 << " + " << par2 << endl;
		}

		cout << endl;

		cout << "Потомки: " << endl;
		for (int j = 0; j < child.size(); j++)
		{
			childQ.push_back(0);
			cout << child[j];
			int weight = getWQ(child[j], w);
			if (weight > maxW) childQ[j] = 0;
			else childQ[j] = getWQ(child[j], c);
			childW.push_back(weight);
			cout << " Критерий: " << childQ[j] << " Вес: " << childW[j] << endl;
		}

		cout << endl;

		cout << "Мутанты: " << endl;

		for (int j = 0; j < child.size(); j++)
		{
			int rnd = rand() % 100;
			if (rnd < 10)                               //
			{
				child[j] = mutation(child[j]);
				int weight = getWQ(child[j], w);
				if (weight > maxW) childQ[j] = 0;
				else childQ[j] = getWQ(child[j], c);
				childW[j] = weight;
				mutants.push_back(j);
			}
		}

		for (int j = 0; j < mutants.size(); j++)          
			cout << child[mutants[j]] << " Критерий: " << childQ[mutants[j]] << " Вес: " << childW[mutants[j]] << endl;
		for (int j = 0; j < child.size(); j++)
			if (childQ[j] > q_max) 
			{
				stop = 0;
				q_max = childQ[j];
				str_max = child[j];
				weight_max = childW[j];
			}

		population[0] = str_max;
		Q[0] = q_max;
		W[0] = weight_max;
		vector<int> rang;

		for (int j = 0; j < child.size(); j++)
			rang.push_back(0);
		int sum = 0;

		for (int j = 0; j < child.size(); j++)      // сортируем потомков по рангам (в порядке убывания приспособленности)
		{
			int max = 0, number = 0;
			for (int k = 0; k < child.size(); k++)
				if (childQ[k] >= max && rang[k] == 0)
				{
					max = childQ[k];
					number = k;
				}
			rang[number] = j + 1;
			sum = sum + max;
		}

		vector<float> proc;
		for (int j = 0; j < child.size(); j++)
			proc.push_back((float)childQ[j] / sum);       //вектор вероятностей

		for (int j = 1; j < population.size(); j++)      //по вектору популяции
		{
			int rnd = rand() % 2;                      

			if (rnd == 0)               //селекция, пропорциональная схема
			{
				for (int k = 0; k < child.size(); k++)
				{
					float rnd2 = (float)(rand() % 100) / 100;
					if (proc[k] > rnd2)
					{
						if (childQ[k] == 0) continue;
						population[j] = child[k];
						Q[j] = childQ[k];
						W[j] = childW[k];
						proc[k] = -1;
						break;
					}
				}
			}
			else                       //селекция, ранговая схема
			{
				for (int k = 0; k < child.size(); k++)       //по вектору потомков
				{
					int n1 = 0, n2 = 1;
					float rnd2 = (float)(rand() % 100) / 100;
					if (rang[k] == 0) continue;
					if ((float)(n1 + (n2 - n1) * (rang[k] - 1) / ((population.size() - 1)) / population.size()) > rnd2)    //ожидаемое число копий
					{
						if (childQ[k] == 0) continue;
						population[j] = child[k];
						Q[j] = childQ[k];
						W[j] = childW[k];
						proc[k] = -1;
						break;
					}
				}
			}
		}
		if (stop > 7) break;
	}

	cout << endl<< "-----------------------------------------------------------------" << endl;
	cout << "Ответ: " << endl;
	cout << "Итоговое решение: " << str_max << " Значение критерия: " << q_max << " Вес: " << weight_max << endl;
	cout << endl << "-----------------------------------------------------------------" << endl;

}