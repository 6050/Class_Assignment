// 2018007974_김서연_12838

#include <stdio.h> 
#include <stdlib.h> 

int huffmanBit = 0;

typedef struct node { 

	char data; 
	int freq; 
	struct node *left;
	struct node *right; 

}node; 

typedef struct minHeap { 

	int size; 
	int capacity; 
	struct node** arr; 

}minHeap; 

node* makeNode(char data, int freq){
 
	node* tmp = (node*)malloc(sizeof(node)); 

	tmp->left = NULL;
	tmp->right = NULL; 
	tmp->data = data; 
	tmp->freq = freq; 

	return tmp;
 
} 

minHeap* createMinHeap(int capacity){ 

	minHeap* heap = (minHeap*)malloc(sizeof(minHeap)); 

	heap->size = 0; 
	heap->capacity = capacity; 
	heap->arr = (node**)malloc(sizeof(node*)*heap->capacity); 
	
	return heap;
 
} 

void swapNode(node** a,	node** b){ 

	node* tmp = *a; 
	*a = *b; 
	*b = tmp;
 
} 

void minHeapify(minHeap* heap, int idx){ 

	int min = idx; 
	int left = idx * 2 + 1; 
	int right = idx * 2 + 2; 

	if (left < heap->size && heap->arr[left]->freq < heap->arr[min]->freq) 
		min = left; 

	if (right < heap->size && heap->arr[right]->freq < heap->arr[min]->freq) 
		min = right; 

	if (min != idx) { 

		swapNode(&heap->arr[min], &heap->arr[idx]); 
		minHeapify(heap, min); 
	
	} 

} 

node* extractMin(minHeap* heap){ 

	node* tmp = heap->arr[0]; 

	heap->arr[0] = heap->arr[heap->size - 1]; 
	heap->size--; 

	minHeapify(heap, 0); 

	return tmp; 

} 

void insertMinHeap(minHeap* heap, node* node){ 

	int i = heap->size++;

	while (i && node->freq < heap->arr[(i-1)/2]->freq) { 

		heap->arr[i] = heap->arr[(i-1)/2]; 
		i = (i-1)/2; 
	
	} 

	heap->arr[i] = node; 

} 

void buildMinHeap(minHeap* heap){ 

	int n = heap->size - 1; 

	for (int i=(n-1)/2; i>=0; i--) 
		minHeapify(heap, i); 

} 

// create and build
minHeap* makeMinHeap(char data[], int freq[], int size){ 

	minHeap* heap = createMinHeap(size); 

	for (int i=0; i<size; i++) 
		heap->arr[i] = makeNode(data[i], freq[i]); 

	heap->size = size; 
	buildMinHeap(heap); 

	return heap;
 
} 

node* buildHuffmanTree(char data[], int freq[], int size){
 
	node *left, *right, *top; 

	minHeap* heap = makeMinHeap(data, freq, size); 

	while (heap->size != 1) { 

		left = extractMin(heap); 
		right = extractMin(heap); 

		top = makeNode('$', left->freq + right->freq); 

		top->left = left; 
		top->right = right; 

		insertMinHeap(heap, top); 

	} 

	return extractMin(heap); 

} 

void printCodes(node* root, int arr[], int top){ 

	if (root->left) { 

		arr[top] = 0; 
		printCodes(root->left, arr, top+1); 
	
	} 

	if (root->right) { 

		arr[top] = 1; 
		printCodes(root->right, arr, top+1); 
	
	} 

	if (!(root->left) && !(root->right)){ 

		huffmanBit += top * root->freq;

	} 

} 

void HuffmanCodes(char data[], int freq[], int size){
 
	node* root = buildHuffmanTree(data, freq, size); 

	int arr[10000], top = 0; 

	printCodes(root, arr, top); 

} 

int main() 
{ 

	int num;
	scanf("%d", &num);

	int bit = 1 , first = 1, second = 2;
	for (int i=0; i<20; i++){

                if (first <= num && num <= second)
                        break;

                bit++;
                first *= 2;
                second *= 2;

        }
	
	char** ch = (char**)malloc(sizeof(char*)*num);
        for (int i=0; i<num; i++)
                ch[i] = (char*)malloc(sizeof(char)*4);
	
	int* freq = (int*)malloc(sizeof(int)*num);

	for (int i=0; i<num; i++)
		scanf("%s %d", ch[i], &freq[i]);

	char* arr = (char*)malloc(sizeof(char)*num);
	for (int i=0; i<num; i++) arr[i] = ch[i][0];


	int total;
	scanf("%d", &total);

	HuffmanCodes(arr, freq, num); 

	printf("%d\n", bit*total);
	printf("%d\n", huffmanBit);
	
	free(ch);
	free(freq);
	free(arr);

	return 0;
 
} 

