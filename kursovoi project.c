#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <stdlib.h>

struct Data {
	int hour;
	int min;
	char name[100];
	char reason[100];
	struct tm t;
};

typedef struct Data ITEM_t;
char* file(char* filename); /*Выбор файла*/
void init(ITEM_t* point, int i); /*Заполнение одной записи*/
int size_out_file(char* filename); /*Подсчёт количества записей в файле*/
ITEM_t* read(ITEM_t* point, char* filename); /* Считывание всех записей из файла*/
void put(ITEM_t point); /*Вывод одной записи*/
void write(ITEM_t point, int i, char* filename); /*Запись данных об одной записи в файл*/
int* searchUser(ITEM_t* point, char* name, int size); /*Поиск по имени, возвращает несколько значений*/
int searchTime(ITEM_t* point, struct tm time_search, int size); /* Поиск по времени, возвращает одно значение*/
ITEM_t* del(ITEM_t* point, int size, int index); /*Удаление записи*/
ITEM_t* sort_insert(ITEM_t* point, int size); /*Сортировка*/
void tabl(); /*Таблица*/

void main()
{
	setlocale(LC_ALL, "RUS");
	system("chcp 1251");

	ITEM_t* point = malloc(sizeof(ITEM_t));
	int size = 0, vibor;
	char* filename = malloc(30 * sizeof(char));

	filename = file(filename); /*Выбор файла*/

	do {
		puts("Выберите действие:");
		puts("1) Ввод данных пользователя");
		puts("2) Вывод");
		puts("3) Поиск по пользователю");
		puts("4) Поиск по времени");
		puts("5) Удаление записи");
		puts("6) Сортировка");
		puts("7) Сменить файл");
		puts("0) Выход");
		scanf("%d", &vibor);

		switch (vibor)
		{
		case 1:
		{
			int n;
			printf("Введите количество новых записей\n");
			scanf("%d", &n);

			size = size_out_file(filename);
			point = realloc(point, (size + n + 1) * sizeof(ITEM_t));
			point = read(point, filename);

			for (int i = size; i < size + n; i++)
			{
				init(point, i);
				write(point[i], i, filename);
			}
			break;
		}
		case 2:
		{
			system("cls");
			size = size_out_file(filename); /*Подсчёт записей в файле*/
			point = realloc(point, (size + 1) * sizeof(ITEM_t)); //Выделение памяти под записи в файле
			point = read(point, filename); //Заполнение данными из файла

			for (int i = 0; i < size; i++)
			{
				if (i == 0) tabl();
				printf("|%3d|", i + 1);
				put(point[i]);
			}
			break;
		}
		case 3:
		{
			char name[100];
			int i = 0;
			int* arr = malloc(size * sizeof(int));

			puts("Введите имя пользователя(Фамилия_имя)");
			scanf("%s", name);

			arr = searchUser(point, name, size);

			while (arr[i] >= 0)
			{
				if (i == 0)tabl();
				printf("|%3d|", i + 1);
				put(point[arr[i]]);
				i++;
			}
			break;
		}
		case 4:
		{
			struct tm time_search;
			int ind;

			puts("Введите время для поиска(__:__)");
			scanf("%d:%d", &time_search.tm_hour, &time_search.tm_min);

			ind = searchTime(point, time_search, size);
			if (ind >= 0) {
				tabl();
				printf("| 1|");
				put(point[ind]);
			}
			break;
		}
		case 5:
		{
			int index;
			printf("Выберите строку которую хотите удалить:");
			scanf("%d", &index);
			if (index > size || index < 0) {
				puts("Такой строки не существует");
				break;
			}
			point = del(point, size, index);
			size--;
			for (int i = 0; i < size; i++)
				write(point[i], i, filename);
			break;
		}
		case 6:
		{
			sort_insert(point, size);
			for (int i = 0; i < size; i++)
			{
				if (i == 0) tabl();
				printf("|%3d|", i + 1);
				put(point[i]);
			}
			break;
		}
		case 7:
		{
			system("cls");
			filename = file(filename);
			break;
		}
		}
	} while (vibor != 0);
}

char* file(char* filename)
{
	int vibor = -2;
	FILE* f;
	while (vibor != -1)
	{
		puts("Выберите действие:");
		puts("1) Создать новый файл");
		puts("2) Открыть существующий файл");
		scanf("%d", &vibor);
		if (vibor == 1) {
			puts("Введите название файла");
			scanf("%s", filename);

			if (!(f = fopen(filename, "w"))) {
				puts("Файл уже существует");
				continue;
			}

			else {
				f = fopen(filename, "w");
				fprintf(f, "");
				fclose(f);
				vibor = -1;
				continue;
			}
		}
		else if (vibor == 2)
		{
			puts("Введите название файла");
			scanf("%s", filename);
			if (!(f = fopen(filename, "r")))
			{
				puts("Файл не найден");
				continue;
			}
			else vibor = -1;
		}
		else continue;
	}
	return filename;
}

void init(ITEM_t* point, int i)
{
	printf("Введите дату(__:__:____): ");
	scanf("%d.%d.%d", &point[i].t.tm_mday, &point[i].t.tm_mon, &point[i].t.tm_year);
	printf("Введите время(__:__): ");
	scanf("%d:%d", &point[i].hour, &point[i].min);
	printf("Введите причину перезагрузки: ");
	scanf("%s", &point[i].reason);
	printf("Введите имя пользователя(Фамилия_Имя): ");
	scanf("%s", &point[i].name);
}

ITEM_t* read(ITEM_t* point, char* filename)
{
	FILE* f = fopen(filename, "r");
	int ind = 0;
	char b[200];
	while (!feof(f)) {
		fgets(b, 200, f);
		if (b[0] == '\n') continue;
		else {
			sscanf(b, "%d.%d.%d %d:%d %s %s",
				&point[ind].t.tm_mday,
				&point[ind].t.tm_mon,
				&point[ind].t.tm_year,
				&point[ind].hour,
				&point[ind].min,
				point[ind].reason,
				point[ind].name);

			ind++;
		}
	}
	return point;
}

int size_out_file(char* filename)
{
	int s = 0;
	FILE* f = fopen(filename, "r");
	while (!feof(f))
		if (fgetc(f) == '\n' && fgetc(f) != ' ') s++;

	return s;
}

void put(ITEM_t point)
{
	printf(" %2d.%2d.%2d |", point.t.tm_mday, point.t.tm_mon, point.t.tm_year);
	printf(" %2d:%2d |", point.hour, point.min);
	printf("%29s|", point.reason);
	printf("%41s|", point.name);
	printf("\n");
	puts("----------------------------------------------------------------------------------------------------");
}

void write(ITEM_t point, int i, char* filename)
{
	FILE* f;
	if (i == 0) f = fopen(filename, "w");
	else f = fopen(filename, "a");

	fprintf(f, "%2d.%2d.%2d ", point.t.tm_mday, point.t.tm_mon, point.t.tm_year); //дата
	fprintf(f, "%d:%d ", point.hour, point.min); //время
	fprintf(f, "%s ", point.reason); //причина
	fprintf(f, "%s \n", point.name); //пользователь

	fclose(f);
}
int* searchUser(ITEM_t* point, char* name, int size) {
	int count = 0;
	int* arr = (int*)malloc(size * sizeof(int));

	for (int i = 0; i < size; i++) {
		if (strcmp(name, point[i].name) == 0) {
			arr[count] = i;
			count++;
		}
	}
	if (count > 0) return arr;
	else {
		free(arr);
		return arr;
	}
}
int searchTime(ITEM_t* point, struct tm time_search, int size) {
	for (int i = 0; i < size; i++) {
		if (time_search.tm_hour == point[i].hour && time_search.tm_min == point[i].min)
			return i;
	}
	return -1;
}

ITEM_t* del(ITEM_t* point, int size, int index)
{
	index--;

	for (int i = 0; i < size; i++)
	{
		if (i < index) continue;
		else {
			*(point + i) = *(point + i + 1);
		}
	}
	return point;
}

ITEM_t* sort_insert(ITEM_t* point, int size) {
	char temp[120];
	char r[120];
	int a, b, c, h, m;
	int j;
	for (int i = 1; i < size - 1; i++)
	{
		a = point[i].t.tm_mday;
		b = point[i].t.tm_mon;
		c = point[i].t.tm_year;
		h = point[i].hour;
		m = point[i].min;
		strcpy(temp, point[i].reason);
		strcpy(r, point[i].name);
		j = i;
		if (strcmp(point[j - 1].reason, temp) > 0 && j > 0) {
			strcpy(point[j].reason, point[j - 1].reason);
			strcpy(point[j].name, point[j - 1].name);
			point[j].t.tm_mday = point[j - 1].t.tm_mday;
			point[j].t.tm_mon = point[j - 1].t.tm_mon;
			point[j].t.tm_year = point[j - 1].t.tm_year;
			point[j].hour = point[j - 1].hour;
			point[j].min = point[j - 1].min;
			strcpy(point[j - 1].reason, temp);
			strcpy(point[j - 1].name, r);
			point[j - 1].t.tm_mday = a;
			point[j - 1].t.tm_mon = b;
			point[j - 1].t.tm_year = c;
			point[j - 1].hour = h;
			point[j - 1].min = m;
			j--;
		}
	}
	return point;
}
void tabl()
{
	puts("----------------------------------------------------------------------------------------------------");
	puts("| # |     Дата   | Время |           Причина           |               Фамилия_Имя               |");
	puts("----------------------------------------------------------------------------------------------------");
}
