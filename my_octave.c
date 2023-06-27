//	315CAb_StanMihai-Catalin
#include <stdio.h>
#include <stdlib.h>
#define NUM 10007
typedef struct {
	int **mat;
	int n, m;
} mat;
typedef struct {
	mat *arr;
	unsigned long long used, size;
} arr;

//	Definim doua structuri de date
//	Prima structura ce va memora o matrice alocata dinamic si dimensiunile ei
//	A doua structura memoreaza un vector de dimensiune variabila (*arr) si:
//	used - numarul curent de matrici
//	size - numarul maxim de matrici care pot fi alocate momentan
//	In *arr vor fi memorate valorile primului struct

void quit(arr *v)
{
	for (unsigned long long k = 0 ; k < (*v).used ; k++) {
		for (int i = 0 ; i < (*v).arr[k].n ; i++)
			free((*v).arr[k].mat[i]);
		free((*v).arr[k].mat);
	}
	free((*v).arr);
}

void free_but_better(arr *v)
{
	unsigned long long k;
	scanf("%lld", &k);
	if ((*v).used == 0 || k >= (*v).used) {
		printf("No matrix with the given index\n");
	} else {
		for (int i = 0 ; i < (*v).arr[k].n ; i++)
			free((*v).arr[k].mat[i]);
		free((*v).arr[k].mat);
		for (unsigned long long i = k ; i < ((*v).used - 1) ; i++)
			(*v).arr[i] = (*v).arr[i + 1];
		(*v).used--;
		if (2 * (*v).used == (*v).size && ((*v).size - 1)) {
			(*v).size /= 2;
			(*v).arr = (mat *)realloc((*v).arr, sizeof(mat) * (*v).size);
		}
	}
}

//	Dupa cum implica si numele, o functie de free, dar mai buna
//	Subprogramul da free la o matrice din vectorul de matrice
//	Iar apoimuta pe restul pe pozitia anterioara

void multiply_power(int **matrix1, int **matrix2, int n)
{
	int i;
	int **tmp;
	tmp = (int **)calloc(sizeof(int *), n);
	if (!tmp) {
		fprintf(stderr, "Error! Allocation failed!");
		exit(0);
	}
	for (i = 0 ; i < n ; i++) {
		tmp[i] = (int *)calloc(sizeof(int), n);
		if (!tmp) {
			fprintf(stderr, "Error! Allocation failed!");
			exit(0);
		}
	}
	for (int i = 0 ; i < n ; i++)
		for (int j = 0 ; j < n ; j++)
			for (int k = 0 ; k < n ; k++)
				tmp[i][j] = tmp[i][j] + (matrix1[i][k] * matrix2[k][j] % NUM);
	for (int i = 0 ; i < n ; i++)
		for (int j = 0 ; j < n ; j++) {
			matrix1[i][j] = tmp[i][j] % NUM;
			if (matrix1[i][j] < 0)
				matrix1[i][j] += NUM;
		}
	for (int i = 0; i < n; i++)
		free(tmp[i]);
	free(tmp);
}

//	O varianta modificata a subprogramului de inmultire a matricilor
//	Folosit pentru raise_to_power, nu stocheaza matricea cu rezultatul cerut

void raise_to_power(arr *v)
{
	unsigned long long k;
	int power;
	scanf("%lld %d", &k, &power);
	if (k >= (*v).used) {
		printf("No matrix with the given index\n");
	} else if (power < 0) {
		printf("Power should be positive\n");
	} else if ((*v).arr[k].n != (*v).arr[k].m) {
		printf("Cannot perform matrix multiplication\n");
	} else {
		int **in;
		int n = (*v).arr[k].n;
		in = (int **)calloc(sizeof(int *), n);
		if (!in) {
			fprintf(stderr, "Error! Allocation failed!");
			exit(0);
		}
		for (int i = 0 ; i < n ; i++) {
			in[i] = (int *)calloc(sizeof(int), n);
			if (!in[i]) {
				fprintf(stderr, "Error! Allocation failed!");
				for (int j = 0 ; j < i ; j++)
					free(in[j]);
				free(in);
				exit(0);
			}
		}
		for (int i = 0 ; i < n ; i++)
			for (int j = 0 ; j < n ; j++)
				if (i == j)
					in[i][j] = 1;
		while (power) {
			if (power % 2 == 1) {
				multiply_power(in, (*v).arr[k].mat, n);
				power--;
			} else {
				multiply_power((*v).arr[k].mat, (*v).arr[k].mat, n);
				power /= 2;
			}
		}
		for (int i = 0 ; i < n ; i++)
			for (int j = 0 ; j < n ; j++)
				(*v).arr[k].mat[i][j] = in[i][j];
		for (int i = 0 ; i < n ; i++)
			free(in[i]);
		free(in);
	}
}

//	Ridicarea la puterea in timp logaritmic

void load(arr *v)
{
	int n, m;
	scanf("%d%d", &n, &m);
	if ((*v).used == (*v).size) {
		(*v).size *= 2;
		(*v).arr = (mat *)realloc((*v).arr, sizeof(mat) * (*v).size);
		if (!(*v).arr) {
			fprintf(stderr, "Error! Reallocation failed!");
			quit(v);
			exit(0);
		}
	}
	(*v).arr[(*v).used].n = n;
	(*v).arr[(*v).used].m = m;
	(*v).arr[(*v).used].mat = (int **)malloc(sizeof(int *) * n);
	if (!(*v).arr[(*v).used].mat) {
		fprintf(stderr, "Error! Reallocation failed!");
		quit(v);
		exit(0);
	}
	for (int i = 0 ; i < n ; i++) {
		(*v).arr[(*v).used].mat[i] = (int *)malloc(sizeof(int) * m);
		if (!(*v).arr[(*v).used].mat[i]) {
			fprintf(stderr, "Error! Reallocation failed!");
			quit(v);
			exit(0);
		}
	}
	for (int i = 0 ; i < n ; i++)
		for (int j = 0 ; j < m ; j++)
			scanf("%d", &(*v).arr[(*v).used].mat[i][j]);
	(*v).used++;
}

//	Incarcam o matrice in vectorul nostru de matrici
//	Atunci cand acesta este plin ((*v).used == (*v).size)
//	Dublam spatiul alocat pentru matrici si realocam vectorul

void determine(arr v)
{
	unsigned long long k;
	scanf("%llu", &k);
	if (k >= v.used)
		printf("No matrix with the given index\n");
	else
		printf("%d %d\n", v.arr[k].n, v.arr[k].m);
}

//	Determina dimensiunile unei matrici cerute

void print(arr v)
{
	unsigned long long k;
	scanf("%llu", &k);
	if (k >= v.used)
		printf("No matrix with the given index\n");
	else
		for (int i = 0 ; i < v.arr[k].n ; i++) {
			for (int j = 0 ; j < v.arr[k].m ; j++)
				printf("%d ", v.arr[k].mat[i][j]);
			printf("\n");
		}
}

//	Afiseaza o matrice ceruta

void free_crop(int *la, int *ca, int **tmp, int k)
{
	if (la)
		free(la);
	if (ca)
		free(ca);
	if (tmp) {
		for (int i = 0 ; i < k ; i++)
			free(tmp[i]);
		free(tmp);
	}
}

//	Un free dedicat variabilelor alocate dinamic in subprogramul 'crop'

void crop(arr *v)
{
	unsigned long long k;
	int l, c, *la, *ca, **tmp;
	scanf("%lld%d", &k, &l);
	la = (int *)malloc(sizeof(int) * l);
	if (!la) {
		fprintf(stderr, "Error! Allocation failed!");
		quit(v);
		exit(0);
	}
	for (int i = 0 ; i < l ; i++)
		scanf("%d", &la[i]);
	scanf("%d", &c);
	ca = (int *)malloc(sizeof(int) * c);
	if (!ca) {
		fprintf(stderr, "Error! Allocation failed!");
		free_crop(la, ca, NULL, 0);
		quit(v);
		exit(0);
	}
	for (int i = 0 ; i < c ; i++)
		scanf("%d", &ca[i]);
	tmp = (int **)malloc(sizeof(int *) * l);
		if (!tmp) {
			fprintf(stderr, "Error! Allocation failed!");
			free_crop(la, ca, tmp, 0);
			quit(v);
			exit(0);
		}
	if (k >= (*v).used) {
		printf("No matrix with the given index\n");
	} else {
		for (int i = 0 ; i < l ; i++) {
			tmp[i] = (int *)malloc(sizeof(int) * c);
			if (!tmp[i]) {
				fprintf(stderr, "Error! Allocation failed!");
				free_crop(la, ca, tmp, i + 1);
				quit(v);
				exit(0);
			}
		}
		for (int i = 0 ; i < l ; i++)
			for (int j = 0 ; j < c ; j++)
				tmp[i][j] = (*v).arr[k].mat[la[i]][ca[j]];
		for (int i = 0 ; i < (*v).arr[k].n ; i++)
			free((*v).arr[k].mat[i]);
		free((*v).arr[k].mat);
		(*v).arr[k].n = l;	(*v).arr[k].m = c;
		(*v).arr[k].mat = (int **)malloc(sizeof(int *) * l);
		if (!(*v).arr[k].mat) {
			fprintf(stderr, "Error! Allocation failed!");
			free_crop(la, ca, tmp, l);
			quit(v);
			exit(0);
		}
		for (int i = 0 ; i < l ; i++) {
			(*v).arr[k].mat[i] = (int *)malloc(sizeof(int) * c);
			if (!(*v).arr[k].mat[i]) {
				fprintf(stderr, "Error! Allocation failed!");
				free_crop(la, ca, tmp, l);
				quit(v);
				exit(0);
			}
		}
		for (int i = 0 ; i < l ; i++)
			for (int j = 0 ; j < c ; j++)
				(*v).arr[k].mat[i][j] = tmp[i][j];
		for (int i = 0 ; i < l ; i++)
			free(tmp[i]);
	}
	free(tmp);
	free(ca);
	free(la);
}

//	Subprogram pentru redimensionare unei matrici
//	Salveaza liniile, coloanele si matricea redimensionata
//	Sterge matricea curenta de pe pozitia ceruta
//	Creeaza o matrice de dimensiunile cerute si i se atribuie valorile cerute

void multiply(arr *v)
{
	unsigned long long k1, k2;
	scanf("%llu%llu", &k1, &k2);
	if (k1 >= (*v).used || k2 >= (*v).used) {
		printf("No matrix with the given index\n");
	} else if ((*v).arr[k1].m != (*v).arr[k2].n) {
		printf("Cannot perform matrix multiplication\n");
	} else {
		int **tmp, product;
		int tmp_n = (*v).arr[k1].n, tmp_m = (*v).arr[k2].m;
		tmp = (int **)calloc(sizeof(int *), tmp_n);
		if (!tmp) {
			fprintf(stderr, "Error! Allocation failed!");
			quit(v);
			exit(0);
		}
		for (int i = 0 ; i < tmp_n ; i++) {
			tmp[i] = (int *)calloc(sizeof(int), tmp_m);
			if (!tmp[i]) {
				fprintf(stderr, "Error! Allocation failed!");
				for (int j = 0 ; j < i ; j++)
					free(tmp[j]);
				free(tmp);
				quit(v);
				exit(0);
			}
		}
		for (int i = 0 ; i < tmp_n ; i++)
			for (int j = 0 ; j < tmp_m ; j++) {
				for (int k = 0 ; k < (*v).arr[k1].m ; k++) {
					product = (*v).arr[k1].mat[i][k] * (*v).arr[k2].mat[k][j];
					tmp[i][j] += product % NUM;
					if (tmp[i][j] < 0)
						tmp[i][j] += NUM;
				}
				tmp[i][j] %= NUM;
				if (tmp[i][j] < 0)
					tmp[i][j] += NUM;
			}
		if ((*v).used == (*v).size) {
			(*v).size *= 2;
			(*v).arr = (mat *)realloc((*v).arr, sizeof(mat) * (*v).size);
			if (!(*v).arr) {
				fprintf(stderr, "Error! Reallocation failed!");
				quit(v);
				exit(0);
			}
		}
		(*v).arr[(*v).used].n = tmp_n;
		(*v).arr[(*v).used].m = tmp_m;
		(*v).arr[(*v).used].mat = (int **)malloc(sizeof(int *) * tmp_n);
		if (!(*v).arr[(*v).used].mat) {
			fprintf(stderr, "Error! Reallocation failed!");
			quit(v);
			exit(0);
		}
		for (int i = 0 ; i < tmp_n ; i++) {
			(*v).arr[(*v).used].mat[i] = (int *)malloc(sizeof(int) * tmp_m);
			if (!(*v).arr[(*v).used].mat[i]) {
				fprintf(stderr, "Error! Reallocation failed!");
				for (int j = 0 ; j < i ; j++)
					free((*v).arr[(*v).used].mat[j]);
				free((*v).arr[(*v).used].mat);
				quit(v);
				exit(0);
			}
		}
		for (int i = 0 ; i < tmp_n ; i++)
			for (int j = 0 ; j < tmp_m ; j++)
				(*v).arr[(*v).used].mat[i][j] = tmp[i][j];
		(*v).used++;
		for (int i = 0 ; i < tmp_n ; i++)
			free(tmp[i]);
		free(tmp);
	}
}

//	Inmultirea a doua matrici date
//	Se memoreaza valoarea noii matrici intr-o nou matrice initializata cu 0
//	Matricea cu rezultatul inmultirii este dupa adaugata la vectorul de matrici

int sum(arr *v, unsigned long long k)
{
	int s = 0;
	for (int i = 0 ; i < (*v).arr[k].n ; i++)
		for (int j = 0 ; j < (*v).arr[k].m ; j++) {
			s += (*v).arr[k].mat[i][j] % NUM;
			if (s < 0)
				s += NUM;
		}
	s %= NUM;
	if (s < 0)
		s += NUM;
	return s;
}

//	Subprogram folosit pentru functia de sortare
//	Calculeaza suma elementelor unei matrici %10007

void order(arr *v)
{
	if ((*v).used > 0) {
		unsigned long long min_k, sum_min, sum_j;
		mat tmp;
		for (unsigned long long i = 0 ; i < (*v).used - 1 ; i++) {
			min_k = i;
			sum_min = sum(v, i);
			for (unsigned long long j = i + 1 ; j < (*v).used ; j++) {
				sum_j = sum(v, j);
				if (sum_j < sum_min) {
					sum_min = sum_j;
					min_k = j;
				}
			}
			if (min_k != i) {
				tmp = (*v).arr[i];
				(*v).arr[i] = (*v).arr[min_k];
				(*v).arr[min_k] = tmp;
			}
		}
	}
}

//	O varianta adaptata conform cerinte a metodei de sortare select_sort
//	Operatia de swap este realizata intre matrici in loc de valori numerice
//	Matricea cautata de select_sort este cea cu suma minima

void transpose(arr *v) //!!! PROGRAMARE DEFENSIVA
{
	unsigned long long k;
	scanf("%lld", &k);
	if (k >= (*v).used) {
		printf("No matrix with the given index\n");
	} else {
		mat tmp;
		tmp.n = (*v).arr[k].m;
		tmp.m = (*v).arr[k].n;
		tmp.mat = (int **)malloc(sizeof(int *) * tmp.n);
		for (int i = 0 ; i < tmp.n ; i++)
			tmp.mat[i] = (int *)malloc(sizeof(int) * tmp.m);
		for (int i = 0 ; i < tmp.m ; i++)
			for (int j = 0 ; j < tmp.n ; j++)
				tmp.mat[j][i] = (*v).arr[k].mat[i][j];
		for (int i = 0 ; i < (*v).arr[k].n ; i++)
			free((*v).arr[k].mat[i]);
		free((*v).arr[k].mat);
		(*v).arr[k].mat = (int **)malloc(sizeof(int *) * tmp.n);
		if (!(*v).arr[k].mat) {
			printf("Error! Allocation failed!");
			quit(v);
			exit(0);
		}
		for (int i = 0 ; i < tmp.n ; i++) {
			(*v).arr[k].mat[i] = (int *)malloc(sizeof(int) * tmp.m);
			if (!(*v).arr[k].mat[i]) {
				printf("Error! Allocation failed!");
				quit(v);
				exit(0);
			}
		}
		(*v).arr[k].n = tmp.n;
		(*v).arr[k].m = tmp.m;
		for (int i = 0 ; i < tmp.n ; i++)
			for (int j = 0 ; j < tmp.m ; j++)
				(*v).arr[k].mat[i][j] = tmp.mat[i][j];
		for (int i = 0 ; i < tmp.n ; i++)
			free(tmp.mat[i]);
		free(tmp.mat);
	}
}

//	Subprogram pentru transpunerea unei matrici
//	Transpusa este calculata intr-o matrice temporara
//	Ulterior matricea temporara v-a inlocui matricea acutala din vector
//	Dimensiunile matricii initiale vor fi si ele inlocuite

int main(void)
{
	char c;
	int check = 1;
	arr v;
	v.used = 0;
	v.size = 1;
	v.arr = (mat *)malloc(sizeof(mat) * v.size);
	scanf("%c", &c);
	do {
		switch (c) {
			case 'L':{
				load(&v);
				break;
			}
			case 'D':{
				determine(v);
				break;
			}
			case 'P':{
				print(v);
				break;
			}
			case 'C':{
				crop(&v);
				break;
			}
			case 'F':{
				free_but_better(&v);
				break;
			}
			case 'M':{
				multiply(&v);
				break;
			}
			case 'O':{
				order(&v);
				break;
			}
			case 'T':{
				transpose(&v);
				break;
			}
			case 'R':{
				raise_to_power(&v);
				break;
			}
			case 'Q':{
				quit(&v);
				exit(0);
				break;
			}
			default:{
				printf("Unrecognized command\n");
				break;
			}
		}
	getchar();
	scanf("%c", &c);
	} while (check);
	return 0;
}
