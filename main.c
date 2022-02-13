#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define H_PARENT(x) (x - 1) / 2
#define MONSTER 9223372036854775807

typedef enum {
    false, true
} bool;

typedef struct maxHeapNode {
    unsigned int index;
    unsigned long int value;
} maxHeapNode;

typedef struct maxHeap {
    unsigned int size;
    unsigned int maxSize;
    struct maxHeapNode **node;
} maxHeap;

struct ListNode {
    unsigned int value;
    struct ListNode *next;
};

//Misc
unsigned long int getMinPathSum(struct ListNode **matrixHead, unsigned int d, unsigned long *distance, bool *picked);

//Max Heap Function
void maxH_restorePropriety(maxHeap *heap, unsigned int index);

void maxH_insertNode(maxHeap *heap, unsigned long value, unsigned int index);

//Main Block
int main() {
    char *buffer;
    char *err;
    int bufferSize = 4400;
    unsigned int k, d, i;
    bool d1 = false;
    bool firstTopK = false;
    bool isNode0Isolated;
    buffer = malloc(bufferSize);
    struct maxHeap *sumHeap;
    unsigned int graphIndex = 0;
    unsigned long int sumResult;
    struct ListNode *rowHead;
    struct ListNode *row;

    err = fgets(buffer, bufferSize, stdin);
    if (err == NULL)
        return -1;
    sscanf(buffer, "%d %d", &d, &k);
    if (1 == d) d1 = true;
    if (0 == d) return 0;

    sumHeap = malloc(sizeof(struct maxHeap)); //Alloc MaxHeap For Store TopK
    sumHeap->size = 0;
    sumHeap->maxSize = k;
    sumHeap->node = malloc(sizeof(struct maxHeapNode) * k);

    struct ListNode *matrix[d];
    unsigned long int distance[d];
    bool picked[d];

    for ( i = 0; i < d; ++i) {
        row = malloc(sizeof(struct ListNode));
        rowHead = row;
        row->value = 0;
        for (unsigned int j = d-1; j; --j) {
            struct ListNode *new_node = malloc(sizeof(struct ListNode));
            row->next = new_node;
            row = row->next;
        }
        row->next = NULL;
        matrix[i] = rowHead;

        distance[i] = MONSTER;
        picked[i] = false;
    }

    while (fgets(buffer, bufferSize, stdin)) {
        if (buffer == NULL) {
            return 0;
        }
        if (strcmp(buffer, "AggiungiGrafo\n") == 0) {
            char *ptr;
            char *end;
            isNode0Isolated = true;
            err = fgets(buffer, bufferSize, stdin);
            if (err == NULL)
                break;
            row = matrix[0];
            ptr = buffer;
            strtol(ptr, &end, 0);
            row = row->next;
            for (ptr = end + 1; *end == ','; ptr = end + 1) {
                row->value = strtol(ptr, &end, 0);
                if (isNode0Isolated && row->value != 0)
                    isNode0Isolated = false;
                row = row->next;
            }
            if (!isNode0Isolated) {
                for (i = 1; i < d; ++i) {//Read Matrix
                    err = fgets(buffer, bufferSize, stdin);
                    if (err == NULL)
                        break;
                    row = matrix[i];
                    ptr = buffer;
                    strtol(ptr, &end, 0);
                    row = row->next;
                    for (ptr = end + 1; *end == ','; ptr = end + 1) {
                        row->value = strtol(ptr, &end, 0);
                        row = row->next;
                    }
                }
            }

            if (d1 || isNode0Isolated) {
                sumResult = 0;
            } else {
                distance[0] = 0;
                sumResult = getMinPathSum(matrix, d, distance, picked);
            }
            maxH_insertNode(sumHeap, sumResult, graphIndex);
            graphIndex++;
        } else if (strcmp(buffer, "TopK\n") == 0) {

            if (sumHeap->size != 0) {
                if (firstTopK) {
                    printf("\n%u", sumHeap->node[0]->index);
                    for (i = 1; i < sumHeap->size; ++i)
                        printf(" %u", sumHeap->node[i]->index);
                } else {
                    firstTopK = true;
                    printf("%u", sumHeap->node[0]->index);
                    for (i = 1; i < sumHeap->size; ++i)
                        printf(" %u", sumHeap->node[i]->index);
                }
            } else { // TopK Before Graph
                printf("\n");
            }
        }
    }
    return 0;
}

//Misc Functions
unsigned long int getMinPathSum(struct ListNode **matrixHead, unsigned int d, unsigned long *distance, bool *picked) {
    unsigned int targetIndex = 0;
    unsigned long int targetDistance;
    unsigned int edgeFromIndexToI;
    struct ListNode *targetRow;
    unsigned int i,j,k;

    for (k = 0; k < d - 1; ++k) {
        targetDistance = MONSTER;
        for (j = 0; j < d; j++)// Get Min Value
            if (picked[j] == false && distance[j] <= targetDistance) {
                targetDistance = distance[j];
                targetIndex = j;
                if (targetDistance == 0)
                    break;
            }
        targetRow = matrixHead[targetIndex];
        picked[targetIndex] = true;

        for (i = 1; i < d; ++i) {
            targetRow = targetRow->next;
            edgeFromIndexToI = targetRow->value;
            if (!picked[i] &&
                edgeFromIndexToI != 0 &&
                targetDistance != MONSTER &&
                targetDistance + edgeFromIndexToI < distance[i])
                distance[i] = targetDistance + edgeFromIndexToI;
        }
    }
    unsigned long int sum = 0;
    for (i = 0; i < d; ++i) {
        if (distance[i] != MONSTER)
            sum += distance[i];
        distance[i] = MONSTER;
        picked[i] = false;
    }
    return sum;
}

//Max Heap Functions
void maxH_restorePropriety(maxHeap *heap, unsigned int index) {
    unsigned int maxIndex = index;
    unsigned int left = 2 * index + 1;
    unsigned int right = 2 * index + 2;
    if (left < heap->size && heap->node[left]->value > heap->node[index]->value)
        maxIndex = left;
    if (right < heap->size && heap->node[right]->value > heap->node[maxIndex]->value)
        maxIndex = right;
    if (maxIndex != index) {
        struct maxHeapNode *tmp = heap->node[index];
        heap->node[index] = heap->node[maxIndex];
        heap->node[maxIndex] = tmp;
        maxH_restorePropriety(heap, maxIndex);
    }
}

void maxH_insertNode(maxHeap *heap, unsigned long value, unsigned int index) {
    if (heap->size < heap->maxSize || value < heap->node[0]->value) {
        struct maxHeapNode *node = malloc(sizeof(struct maxHeapNode));
        node->index = index;
        node->value = value;
        unsigned int i = heap->size++;
        while (i && node->value > heap->node[H_PARENT(i)]->value) {
            heap->node[i] = heap->node[H_PARENT(i)];
            i = H_PARENT(i);
        }
        heap->node[i] = node;
    }
    if (heap->size > heap->maxSize) {
        heap->node[0]->value = heap->node[heap->size - 1]->value;
        heap->node[0]->index = heap->node[heap->size - 1]->index;
        heap->size--;
        maxH_restorePropriety(heap, 0);
    }
}
