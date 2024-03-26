//Kullman Alexandru, 313CA

#include <stdio.h>
#include <stdlib.h>

void free_matrix(int n, int **a)
{
	for (int i = 0; i < n; i++)
		free(a[i]);
	free(a);
}

void swap(char *a, char *b)
{
	char aux;
	aux = *a;
	*a = *b;
	*b = aux;
}

long sum(int **a, int *m, int n)
{
	char *p;
	long s = 0;
	// For the first and the last row, sum all bytes
	for (int j = 0; j < m[0]; j++) {
		p = (char *)&a[0][j];
		s += p[0] + p[1] + p[2] + p[3];
	}

	for (int j = 0; j < m[n - 1]; j++) {
		p = (char *)&a[n - 1][j];
		s += p[0] + p[1] + p[2] + p[3];
	}

	// For the other row, sum the first and the last byte
	for (int i = 1; i < n - 1; i++) {
		p = (char *)&a[i][0];
		s += p[0];
		p = (char *)&a[i][m[i] - 1];
		s += p[3];
	}

	return s;
}

int black_hole(int **a, int *m, int n, int i, int j, int **visited)
{
	char *p = (char *)&a[i][0];
	p += j;

	// Check conditions
	if (i < 0 || i >= n || j < 0 || j >= (int)(m[i] * sizeof(int)) ||
		visited[i][j] == 1 || *p != 0)
		return 0;
	int count = 1;
	visited[i][j] = 1;

	// Call black_hole for all neighbours, if it is possible

	if (i + 1 < n)
		count += black_hole(a, m, n, i + 1, j, visited);

	if (i - 1 >= 0)
		count += black_hole(a, m, n, i - 1, j, visited);

	if (j + 1 < (int)(m[i] * sizeof(int)))
		count += black_hole(a, m, n, i, j + 1, visited);

	if (j - 1 >= 0)
		count += black_hole(a, m, n, i, j - 1, visited);

	return count;
}

int **read_alloc_matrix(int *m, int n)
{
	int **a = (int **)malloc(n * sizeof(int *));

	if (!a)
		return NULL;

	for (int i = 0; i < n; i++) {
		scanf("%d", &m[i]);
		a[i] = (int *)malloc(m[i] * sizeof(int));
		for (int j = 0; j < m[i]; j++)
			scanf("%x", &a[i][j]);
	}
	return a;
}

void modify(int **a, int *m, char dim_char, int data_index, int row)
{
	int new_value, dim, step;
	char *p;
	scanf("%x", &new_value);
		if (dim_char == 'C') {
			dim = 1;
			// If the new value will be outside of the current map
			if (data_index * dim >= (int)(m[row] * sizeof(int))) {
				m[row] = (data_index * dim + sizeof(int) - dim) /
				sizeof(int);
				int *tmp = (int *)realloc(a[row], m[row] * sizeof(int));
				if (tmp)
					a[row] = tmp;
				else
					printf("Eroare memorie");
				for (int j = m[row]; j < data_index * dim; j++) {
					// bytes = 0, if they are from the outside of the map
					p = (char *)&a[row][j];
					*p = 0;
					p++;
				}
			}
			p = (char *)&a[row][0];
			step = (data_index - 1) * dim;
			p += step;
			*p = new_value;
		} else if (dim_char == 'S') {
			dim = 2;
			// If the new value will be outside of the current map
			if (data_index * dim > (int)(m[row] * sizeof(int))) {
				int *tmp = (int *)realloc(a[row], m[row] * sizeof(int));
				if (tmp)
					a[row] = tmp;
				else
					printf("Eroare memorie");
				for (int j = m[row]; j < data_index * dim; j++) {
					// bytes = 0, if they are from the outside of the map
					p = (char *)&a[row][j];
					*p = 0;
					p++;
				}
				m[row] = (data_index * dim + sizeof(int) - dim) /
				sizeof(int);
			}
			p = (char *)&a[row][0];
			step = (data_index - 1) * dim;
			p += step;
			short *new = NULL;
			new = (short *)p;
			*new = new_value;
		} else if (dim_char == 'I') {
			dim = 4;
			// If the new value will be outside of the current map
			if (data_index * dim > (int)(m[row] * sizeof(int))) {
				int *tmp = realloc(a[row], (data_index * dim));
				if (tmp)
					a[row] = tmp;
				else
					printf("Eroare memorie	");
				for (int j = m[row]; j < data_index; j++)
				// bytes = 0, if they are from the outside of the map
					a[row][j] = 0;

				m[row] = data_index;
			}
			a[row][data_index - 1] = new_value;
		}
}

void delete(int **a, char dim_char, int data_index, int row)
{
	int dim, step;
	char *p = (char *)&a[row][0];

	if (dim_char == 'C') {
		dim = 1;
		step = (data_index - 1) * dim;
		p += step;
		*p = 0;
	} else if (dim_char == 'S') {
		dim = 2;
		int step = (data_index - 1) * dim;
		p += step;
		short *new = NULL;
		new = (short *)p; // cast to the corresponding type
		*new = 0;
	} else if (dim_char == 'I') {
		dim = 4;
		step = (data_index - 1) * dim;
		p += step;
		int *new = NULL;
		new = (int *)p; // cast to the corresponding type
		*new = 0;
	}
}

void swap_bytes(int **a, char dim_char, int data_index, int row)
{
	char *p = (char *)&a[row][0];
	int dim, step;
	// data_index starting from 0
	// no swap for 'char' case
	if (dim_char == 'S') {
		dim = 2;
		step = (data_index) * dim;
		p += step;
		swap(p, p + 1);
	} else if (dim_char == 'I') {
		dim = 4;
		step = (data_index) * dim;
		p += step;
		swap(p, p + 3);
		swap(p + 1, p + 2);
	}
}

int main(void)
{
	int n;
	scanf("%d", &n);

	int *m = (int *)malloc(n * sizeof(int));
	if (!m)
		return 1;

	int **a = read_alloc_matrix(m, n);

	long s;
	s = sum(a, m, n);
	// Calculate sum based on the edge of the map
	long countcopy = 4 * (m[0] + m[n - 1]) + 2 * (n - 2);

	int k;
	scanf("%d", &k);
	while (k > 0) {
		char action, dim_char;
		int row, data_index;
		scanf(" %c %c %d %d", &action, &dim_char, &row, &data_index);
		if (action == 'M')
			modify(a, m, dim_char, data_index, row);
		else if (action == 'D')
			delete(a, dim_char, data_index, row);
		else if (action == 'S')
			swap_bytes(a, dim_char, data_index, row);
		k--;
	}

	printf("task 1\n");
	printf("%.8lf", (double)s / countcopy);
	printf("\ntask 2\n");

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m[i]; j++)
			printf("%08X ", a[i][j]);
		printf("\n");
	}
	printf("task 3\n");

	// Calculate row's size in bytes
	int bytes_per_line;
	for (int i = 0; i < n; i++)
		bytes_per_line = m[i] * sizeof(int);

	int **visited = (int **)malloc(n * sizeof(int *));

	// Initialise and allocate memory for visited
	for (int i = 0; i < n; i++) {
		bytes_per_line = m[i] * sizeof(int);
		visited[i] = (int *)calloc(bytes_per_line, sizeof(int));
	}

	int rezultat, max = -1, mini, minj;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < (int)((m[i]) * sizeof(int)); j++) {
			char *p = (char *)&a[i][0];
			p += j;
			if (*p == 0) {
				rezultat = black_hole(a, m, n, i, j, visited);
				if (rezultat > max) {
					max = rezultat;
					mini = i;
					minj = j;
				}
			}
		}
	}

	printf("%d %d %d", mini, minj, max);

	free_matrix(n, a);
	free_matrix(n, visited);
	free(m);

	return 0;
}
