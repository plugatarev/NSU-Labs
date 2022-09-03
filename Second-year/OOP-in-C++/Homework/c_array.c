typedef struct c_array {
    int * data;
    int size;
    int capacity;
} array_t;

array_t * create(int capacity);
int push_back(array_t * arr, int i);
int at(array_t * arr, int i);
int pop_back(array_t * arr);
void clear(array_t * arr);

int main(int argc, char const *argv[]) {
    array_t * arr = create(42);
    
    push_back(arr, 1);
    printf("element at 0 : %d", at(arr, 0));
    clear(arr);

    return 0;
}
