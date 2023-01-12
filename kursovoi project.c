#define _CRT_SECURE_NO_DEPRECATE 
#include <stdio.h> 
#include <string.h> 
#include <malloc.h> 
#include <windows.h> 
#include <stdlib.h> 

typedef struct {
	int data[3];
	int hour;
	int min;
	char name[100], reason[100];
}ITEM;

int init(ITEM* point, int, int n);
int read(ITEM* point, int);
void write(ITEM* point, int size, int n, int q);
void put(ITEM* point, int size, int one_point);
int searchUser(ITEM* point, char* name, int);
int searchData(ITEM* point, int hour, int min, int size, int b);
ITEM* del(ITEM* point, int, int n);

void main()
{
	system("chcp 1251");
	ITEM* point = NULL; // ���������� �� 
	int size, v;
	FILE* f;

	do {
		printf("�������� ��������:\n1)���� ������ ������������.\n2)����� �� ������������.\n3)�����.\n4)����� �� �������.\n5)�������� ������.\n0)����� �� ���������.\n");
		scanf("%d", &v);

		switch (v) {
		case 1: {  /*�������� ������*/
			int n;
			printf("������� ���-�� �������: ");
			scanf("%d", &n);

			f = fopen("KP.txt", "r");
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
		case 2: {  /*����� �� �����*/
			char name[100];
			int ind_res;
			printf("������� ��� ������������: ");
			scanf("%s", name);

			ind_res = searchUser(point, name, size);
			if (ind_res >= 0)put(point, ind_res + 1, ind_res);
			else printf("����������� �� ������.\n");
			break;
		}

		case 3: {  /*����� ������*/
			int file_size = 0;

			f = fopen("KP.txt", "r");
			while (!feof(f)) {
				if (fgetc(f) == '\n') file_size++;
			} size = file_size;
			point = (ITEM*)malloc(size * sizeof(ITEM));

			size = read(point, size);
			put(point, size, 0);
			break;
		}

		case 4: {  /*����� �� �������*/

			int hour, min;

			printf("������� �����(���:������): ");
			scanf("%d:%d", &hour, &min);
			printf("***************************************************************\n");

			int ind_poisk = 0;
			for (int u = 0; u < size - 1; u++)
			{
				ind_poisk = searchData(point, hour, min, size, ind_poisk + 1);

				if (ind_poisk >= 0)
					put(point, ind_poisk + 1, ind_poisk);

				if (ind_poisk >= size) break;

			}
			break;
		}

		case 5: {   /*�������� ������*/
			int index;
			printf("�������� ������ ������� ������ �������:");
			scanf("%d", &index);
			getchar();

			ITEM* point2 = (ITEM*)malloc(size * sizeof(ITEM));
			point2 = point;

			point = (ITEM*)malloc((size - 1) * sizeof(ITEM));
			point = del(point2, size, index);

			write(point, size, size - 1, 1);
			break;
		}

		case 6: { /* ����������*/

			break;
		}
		}
	} while (v != 0);
}

int init(ITEM* point, int size, int n) {
	for (int i = size; i < size + n; i++) {
		printf("������� ����: ");
		scanf("%d.%d.%d", &point[i].data[0], &point[i].data[1], &point[i].data[2]);
		printf("������� �����: ");
		scanf("%d:%d", &point[i].hour, &point[i].min);
		printf("������� ������� ������������: ");
		scanf("%s", &point[i].reason);
		printf("������� ��� ������������(�������_���): ");
		scanf("%s", &point[i].name);
	}
	size++;
	return size;
}

int read(ITEM* point, int size)//������ 
{
	FILE* f; // ��������� ���������� 

	char b[200];

	f = fopen("KP.txt", "r");
	int ind = 0;
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

void write(ITEM* point, int size, int n, int q) //������� ������ � ���� 
{
	FILE* f;

	if (size == 0 || q == 1)f = fopen("KP.txt", "w");
	else f = fopen("KP.txt", "a");
	printf("size---%d\n", size);
	for (int i = size - n; i < size; i++) {
		fprintf(f, "%d.%d.%d ", point[i].data[0], point[i].data[1], point[i].data[2]); //���� 
		fprintf(f, "%d:%d ", point[i].hour, point[i].min); //����� 
		fprintf(f, "%s ", point[i].reason); //������� 
		fprintf(f, "%s \n", point[i].name); //������������ 
	}
	fclose(f);
}

void put(ITEM* point, int size, int one_point) {

	for (int i = 0 + one_point; i < size; i++) {
		printf("����� ������  %d\n", i + 1);
		printf("����          %d.%d.%d\n", point[i].data[0], point[i].data[1], point[i].data[2]);
		printf("�����         %d:%d\n", point[i].hour, point[i].min);
		printf("�������       %s\n", point[i].reason);
		printf("������������  %s\n", point[i].name);
		printf("\n");
	}
}

int searchUser(ITEM* point, char* name, int size) {
	for (int i = 0; i < size; i++) {
		if (strcmp(name, point[i].name) == 0) { // ��������� ������ �� ������������ ��������� ������ 
			return i; //���������� ����� 
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
		printf("������ � ������� %d �� ����������\n", index);
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