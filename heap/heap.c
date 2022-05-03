#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>

#define ARR_LEN 20U
#define MAX 100

void print_array(int32_t *arr, int32_t len)
{
	int32_t i;
	for(i = 0; i < len; i++)
	{
		printf("%d ", arr[i]);
	}
	printf("\n");
}

void swap(int32_t *a, int32_t *b)
{
	static int32_t tmp;
	if((a == NULL) || (b == NULL))
		return;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void heap(int32_t *arr, int32_t i, int32_t len)
{
	int32_t left;
	int32_t right;
	int32_t largest;

	left = 2 * i + 1;
	right = 2 * i + 2;
	largest = i;

	print_array(arr, len);

	if(left < len && arr[left] > arr[largest])
		largest = left;

	if(right < len && arr[right] > arr[largest])
		largest = right;

	if(largest != i)
	{
		swap(&arr[i], &arr[largest]);
		heap(arr, largest, len);
	}
}

void make_heap(int32_t *arr, int32_t len)
{
	int32_t i;
	for(i = len /2; i >= 0; i--)
		heap(arr, i, len);
}

int main(void)
{
	int32_t arr[ARR_LEN];
	int32_t tmp;
	int32_t i;

	srand(time(NULL));

	for(i = 0; i < ARR_LEN; i++)
	{
		arr[i] = rand() % MAX;
	}
	make_heap(arr, ARR_LEN);
	printf("Done\n");
	return 0;
}
