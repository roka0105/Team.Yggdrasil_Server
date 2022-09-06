#include <stdio.h>

//int size = 10;
//int heap[10] = { 5,1,7,4,2,9,8,6,3,10 };

int main()
{
	//for (int i = 1; i < size; i++)
	//{
	//	int child = i;
	//	do
	//	{
	//		int root = (child - 1) / 2;
	//		
	//		if (heap[root] < heap[child])
	//		{
	//			int temp = heap[root];
	//			heap[root] = heap[child];
	//			heap[child] = temp;
	//		}
	//		child = root;
	//	} while (child != 0);
	//}

	//for (int i = size - 1; i >= 0; i--) {
	//	int temp = heap[0];
	//	heap[0] = heap[i];
	//	heap[i] = temp;
	//	int root = 0;
	//	int child = 1;
	//	do {
	//		child = 2 * root + 1;
	//		// 자식 중 더 큰 값을 찾기
	//		if (heap[child] < heap[child + 1] && child < i - 1) {
	//			child++;
	//		}
	//		// root보다 자식이 더 크면 교환
	//		if (heap[root] < heap[child] && child < i) {
	//			int temp = heap[root];
	//			heap[root] = heap[child];
	//			heap[child] = temp;
	//		}
	//		root = child;
	//	} while (child < i);
	//}
	//for (int i = 0; i < size; i++)
	//	printf("%d ", heap[i]);



	return 0;
}
	