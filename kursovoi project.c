#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <windows.h>
#include <stdlib.h>
#include<locale.h>

typedef struct {
	int data[3];
	int hour;
	int min;
	char name[100], reason[100];
}ITEM;

void sort(ITEM* point, int size);
int compare(const ITEM* a, const ITEM* b);
int init(ITEM* point, int, int n);
int read(ITEM* point, int size);
void write(ITEM* point, int size, int n, int q);
void put(ITEM* point, int size, int one_point);
int searchUser(ITEM* point, char* name, int);
int searchData(ITEM* point, int hour, int min, int size, int b);
ITEM* del(ITEM* point, int size, int index);

void main()
{
	setlocale(LC_ALL, "RUS");
	system("chcp 1251");
	ITEM* point = NULL; // объявление БД
	int size, v;
	FILE* f;
	int file_size, vivod;

	do {
		printf("Выберите действие:\n1)Ввод данных пользователя.\n2)Поиск по пользователю.\n3)Вывод.\n4)Поиск по времени.\n5)Удаление записи.\n6)Сортировка.\n0)Выйти из программы.\n");
		scanf("%d", &v);

		switch (v) {
		case 1: { /*Добавить запись*/
			int n;
			printf("Введите кол-во записей: ");
			scanf("%d", &n);

			f = fopen("Бд.txt", "r");
			int file_size = 0;
			while (!feof(f)) {
				if (fgetc(f) == '\n') file_size++;
			}
			size = file_size;
			printf("%d-----size\n", size);

			point = (ITEM*)malloc((file_size + 1 + n) * sizeof(ITEM));
			size = init(point, size, n);
			printf("%d-----\n", size);
			write(point, size, n, 0);
			break;
		}
		case 2: { /*Поиск по имени*/
			char name[100];
			int ind_res;
			printf("Укажите имя пользователя: ");
			scanf("%s", name);

			ind_res = searchUser(point, name, size);
			if (ind_res >= 0)put(point, ind_res + 1, ind_res);
			else printf("Пльзователь не найден.\n");
			break;
		}

		case 3: { /*Вывод данных*/
			puts("1. Вывод данных с файла");
			puts("2. Вывод после сортировки");
			scanf("%d", &vivod);
			switch (vivod) {
			case 1:
				file_size = 0;

				f = fopen("Бд.txt", "r");
				while (!feof(f)) {
					if (fgetc(f) == '\n') file_size++;
				} size = file_size;
				point = (ITEM*)malloc(size * sizeof(ITEM));

				size = read(point, size);
				put(point, size, 0);
				break;

			case 2:
				put(point, size, 0);
				break;
			}

			break;
		}

		case 4: { /*Поиск по времени*/

			int hour, min;

			printf("Укажите время(час:минуты): ");
			scanf("%d:%d", &hour, &min);
			printf("***************************************************************\n");

			int ind_poisk = -1;
			for (int u = 0; u < size - 1; u++)
			{
				ind_poisk = searchData(point, hour, min, size, ind_poisk + 1);

				if (ind_poisk >= 0)
					put(point, ind_poisk + 1, ind_poisk);

				if (ind_poisk >= size) break;

			}
			break;
		}

		case 5: { /*Удаление записи*/
			int index;
			printf("Выберите строку которую хотите удалить:");
			scanf("%d", &index);
			getchar();

			ITEM* point2 = (ITEM*)malloc(size * sizeof(ITEM));
			point2 = point;

			point = (ITEM*)malloc((size - 1) * sizeof(ITEM));
			point = del(point2, size, index);

			write(point, size, size - 1, 1);
			break;
		}

		case 6: { /* Сортировка*/
			sort(point, size);
			put(point, size, 0);
			break;
		}
		}
	} while (v != 0);
}

int init(ITEM* point, int size, int n) {
	for (int i = size; i < size + n; i++) {
		printf("Введите дату: ");
		scanf("%d.%d.%d", &point[i].data[0], &point[i].data[1], &point[i].data[2]);
		printf("Введите время: ");
		scanf("%d:%d", &point[i].hour, &point[i].min);
		printf("Введите причину перезагрузки: ");
		scanf("%s", &point[i].reason);
		printf("Введите имя пользователя(Фамилия_Имя): ");
		scanf("%s", &point[i].name);
	}
	size++;
	return size;
}

int read(ITEM* point, int size)//чтение
{
	FILE* f; // объявляем переменную

	char b[200];

	f = fopen("Бд.txt", "r");
	int ind = 0;
	if (f != NULL)
	{
		while (!feof(f)) {
			fgets(b, 200, f);
			if (b[0] == '\n') continue;

			sscanf(b, "%d.%d.%d %d:%d %s %s",
				&point[ind].data[0],
				&point[ind].data[1],
				&point[ind].data[2],
				&point[ind].hour,
				&point[ind].min,
				point[ind].reason,
				point[ind].name);
			ind++;
		}
		return size;
	}
	else printf("Файл не удалось открыть");
}

void write(ITEM* point, int
	size, int n, int q) //функция записи в файл
{
	FILE* f;
	if (size == 0 || q == 1)f = fopen("Бд.txt", "w");
	else f = fopen("Бд.txt", "a");
	printf("size---%d\n", size);
	for (int i = size - n; i < size; i++) {
		fprintf(f, "%d.%d.%d ", point[i].data[0], point[i].data[1], point[i].data[2]); //дата
		fprintf(f, "%d:%d ", point[i].hour, point[i].min); //время
		fprintf(f, "%s ", point[i].reason); //причина
		fprintf(f, "%s \n", point[i].name); //пользователь
	}
	fclose(f);
}

void put(ITEM* point, int size, int one_point) {

	for (int i = 0 + one_point; i < size; i++) {
		printf("Номер строки %d\n", i + 1);
		printf("Дата %d.%d.%d\n", point[i].data[0], point[i].data[1], point[i].data[2]);
		printf("Время %2d:%02d\n", point[i].hour, point[i].min);
		printf("Причина %s\n", point[i].reason);
		printf("Пользователь %s\n", point[i].name);
		printf("\n");
	}
}

int searchUser(ITEM* point, char* name, int size) {
	for (int i = 0; i < size; i++) {
		if (strcmp(name, point[i].name) == 0) { // проверяем строки на соответствие введенным данным
			return i; //возвращаем номер
		}
	}
	return -1;
}

int searchData(ITEM* point, int hour, int min, int size, int b) {

	for (int i = b; i < size; i++) {
		if (hour == point[i].hour && min == point[i].min)
			return i;
	}
	return -2;
}

ITEM* del(ITEM* point, int size, int index)
{
	size = read(point, size);

	ITEM* list = (ITEM*)malloc(size - 1 * (sizeof(ITEM)));

	if (index < 0 || index > size)
	{
		printf("строка с номером %d не существует\n", index);
		return 0;
	}
	else {
		for (int i = 0; i < size; i++)
		{
			if (i != index || i < index) {
				continue;
			}
			else if (i == index || i >= index) {
				*(point + i) = *(point + i + 1);
			}
		}
		return point;
	}
}

int compare(const ITEM* a, const ITEM* b) {
	if (strcmp(a->reason, b->reason) > 0) return 1;
	if (strcmp(a->reason, b->reason) < 0) return -1;

	if (a->data[0] > b->data[0]) return 1;
	if (a->data[0] < b->data[0]) return -1;

	if (a->data[1] > b->data[1]) return 1;
	if (a->data[1] < b->data[1]) return -1;
	return 0;
}

void sort(ITEM* point, int size) {
	qsort(point, size, sizeof(ITEM), compare);
}
