#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TRUE 1
#define FALSE 0
#define COUNT_CHAR 256  
typedef unsigned char uchar;
typedef unsigned int uint;

struct Node {
    int frequency;
    int symbol;
    struct Node* right;
    struct Node* left;
};
typedef struct Node T;


void write_bit(FILE* out, uint* pos, uchar bit, uchar* buffer) {
    if ((*pos) == 8) {
        fwrite(buffer, sizeof(uchar), 1, out);
        *buffer = 0;
        *pos = 0;
    }
    uchar shift = 7 - (*pos);
    uchar buf = bit << shift;
    (*buffer) += buf;
    (*pos)++;
}

void write_byte(FILE* out, uint* pos, uchar byte, uchar* buffer) {
    int i;
    for (i = 0; i < 8; i++)
        write_bit(out, pos, (byte >> (7 - i)) & 1, buffer);
}

void save_tree(FILE* out, T* root, uint* total_bitpos, uchar* outbuf) {
    if (root) {
        if ((!root->left) && (!root->right)) {
            write_bit(out, total_bitpos, 1, outbuf);
            write_byte(out, total_bitpos, root->symbol, outbuf);
        }
        else {
            write_bit(out, total_bitpos, 0, outbuf);
            save_tree(out, root->left, total_bitpos, outbuf);
            save_tree(out, root->right, total_bitpos, outbuf);
        }
    }
}

void write_encode(FILE* in, FILE* out, uchar** code, size_t lenght, uchar* buf, uint* pos) {
    for (size_t i = 0; i < lenght; i++) {
        uchar symbol;
        symbol = fgetc(in);
        uchar* sym_code = code[symbol];
        for (int j = 0; sym_code[j] != '\0'; j++) {
            write_bit(out, pos, sym_code[j] - 48, buf);
        }
    }

    if (pos != 0) {
        fwrite(buf, sizeof(char), 1, out);
    }
}

uchar** init_table() {
    uchar** table = (uchar**)malloc(COUNT_CHAR * sizeof(uchar*));
    if (table == NULL) {
        printf("Error memory allocation");
        return NULL;
    }
    for (int i = 0; i < COUNT_CHAR; i++) table[i] = NULL;
    return table;
}

uchar** make_table(uchar* path, uchar level, uchar** code, T* root) {
    if (root->right || root->left) {
        path[level] = '0';
        if (make_table(path, level + 1, code, root->left) == NULL) return NULL;
        path[level] = '1';
        if (make_table(path, level + 1, code, root->right) == NULL) return NULL;
    }
    else {
        code[root->symbol] = (uchar*)malloc((level + 1) * sizeof(uchar));
        if (code[root->symbol] == NULL) {
            printf("Error memory allocation!");
            return NULL;
        }
        path[level] = '\0';
        memcpy(code[root->symbol], path, level + 1);
    }
    return code;
}

T* create_node(uint fr, int symbol, T* right, T* left) {
    T* node = (T*)malloc(sizeof(T));
    if (node == NULL) {
        printf("Error memory allocation!");
        return NULL;
    }
    node->left = left;
    node->right = right;
    node->frequency = fr;
    node->symbol = symbol;
    return node;
}

T* build_tree(uint* fr) {
    T* nodes[COUNT_CHAR] = { NULL };
    int count = 0;
    for (int i = 0; i < COUNT_CHAR; i++) {
        if (fr[i] != 0) {
            nodes[count] = create_node(fr[i], i, NULL, NULL);
            if (nodes[count] == NULL) return NULL;
            count++;
        }
    }
    while (count > 1) {
        for (int i = 0; i < count - 1; ++i) {
            for (int j = 0; j < count - i - 1; ++j) {
                if (nodes[j]->frequency < nodes[j + 1]->frequency) {
                    T* temp = nodes[j + 1];
                    nodes[j + 1] = nodes[j];
                    nodes[j] = temp;
                }
            }
        }
        T* new_node = create_node(nodes[count - 2]->frequency + nodes[count - 1]->frequency, 0, nodes[count - 1], nodes[count - 2]);
        if (new_node == NULL) {
            printf("Error memory allocation!");
            return NULL;
        }
        nodes[count - 2] = new_node;
        count--;
    }
    return nodes[0];
}

void free_tree(T* root) {
    if (root->left != NULL) free_tree(root->left);
    if (root->right != NULL) free_tree(root->right);
    if (root != NULL) free(root);
}

void cleaner_encode(uchar** table, uint* frequency, T* root) {
    free(table);
    free(frequency);
    free_tree(root);
}

void free_code(uchar** code) {
    for (int i = 0; i < COUNT_CHAR; i++) {
        if (code[i] != NULL) free(code[i]);
    }
}

char encode(FILE* in, FILE* out) { //кодируем
    uint* frequency = (uint*)calloc(COUNT_CHAR, sizeof(uint));
    if (frequency == NULL) {
        printf("Error memory allocation!");
        return FALSE;
    }
    fseek(in, 3, SEEK_SET);
    uint message_length = 0;
    while (1) {
        uchar symbol = fgetc(in);
        if (feof(in)) break;
        message_length++;
        frequency[symbol]++;
    }
    uchar sym = 0;
    for (int i = 0; i < 256; i++) {
        if (frequency[i] != 0) sym++;
    }
    T* root = build_tree(frequency);
    if (root == NULL) {
        free(frequency);
        return FALSE;
    }
    uchar** table = init_table();
    if (table == NULL) {
        printf("Error memory allocation!");
        free(frequency);
        return FALSE;
    }
    uchar* path = (uchar*)calloc(COUNT_CHAR, sizeof(uchar));
    if (path == NULL) {
        printf("Error memory allocation!");
        cleaner_encode(table, frequency, root);
        return FALSE;
    }
    if (make_table(path, 0, table, root) == NULL) {
        free_code(table);
        cleaner_encode(table, frequency, root);
        free(path);
        return FALSE;
    }
    uchar outbuf = 0;
    uint bitpos = 0;
    fwrite(&message_length, sizeof(uint), 1, out);
    save_tree(out, root, &bitpos, &outbuf);
    fseek(in, 3, SEEK_SET);
    write_encode(in, out, table, message_length, &outbuf, &bitpos);

    free_code(table);
    cleaner_encode(table, frequency, root);
    free(path);
    return TRUE;
}

void read_byte(FILE* in, uchar* sym, uint* pos) {
    if (fread(sym, sizeof(uchar), 1, in) != 1)
        return;
    *pos = 0;
}

uchar read_bit(FILE* in, uchar* sym, uint* pos) {
    if (*pos == 8)
        read_byte(in, sym, pos);
    (*pos)++;
    char c = (8 - *pos);
    return (*sym >> c) & 1;
}

uchar read_sym(FILE* in, uchar* sym, uint* pos) {
    int i;
    uchar c = 0;
    for (i = 0; i < 8; i++)
        c |= read_bit(in, sym, pos) << (7 - i);
    return c;
}


T* read_tree(FILE* in, uchar* s, uint* pos) {
    T* root = (T*)malloc(sizeof(T));
    if (root == NULL) {
        printf("Error memory allocation!");
        return NULL;
    }
    root->left = NULL; root->right = NULL;
    uchar c = read_bit(in, s, pos);
    if (c == 0) {
        root->left = read_tree(in, s, pos);
        root->right = read_tree(in, s, pos);
    }
    else {
        root->left = NULL;
        root->right = NULL;
        root->symbol = read_sym(in, s, pos);
    }
    return root;
}

void write_decode(FILE* in, FILE* out, T* root, uint* pos, uchar* sym) {
    if ((!root->right) && (!root->left))
    {
        fwrite(&(root->symbol), sizeof(uchar), 1, out);
        return;
    }
    uchar c = read_bit(in, sym, pos);
    if (c == 1)
        write_decode(in, out, root->right, pos, sym);
    else
        write_decode(in, out, root->left, pos, sym);
}

char decode(FILE* in, FILE* out) { //декодируем
    uchar s = 0; uint pos = 0;
    fseek(in, 3, SEEK_SET);
    uint mess_len = 0;
    if (!fread(&mess_len, 4, 1, in)) return FALSE;

    s = fgetc(in);
    T* root = read_tree(in, &s, &pos);
    if (root == NULL) return FALSE;

    while (mess_len != 0) {
        write_decode(in, out, root, &pos, &s);
        mess_len--;
    }
    free_tree(root);
    return TRUE;
}

int main(void) {
    FILE* input = fopen("in.txt", "rb");
    if (input == NULL) {
        printf("Error opening file!");
        return 0;
    }
    FILE* output = fopen("out.txt", "wb");
    if (output == NULL) {
        printf("Error opening file!");
        fclose(input);
        return 0;
    }
    char flag;
    if (!fscanf(input, "%c", &flag)) {
        printf("Error reading file!");
        fclose(input);
        fclose(output);
        return 0;
    }
    if (flag == 'c') {
        if (!encode(input, output)) {
            fclose(input);
            fclose(output);
            return 0;
        }
    }
    else if (flag == 'd') {
        if (!decode(input, output)) {
            fclose(input);
            fclose(output);
            return 0;
        }
    }
    fclose(input);
    fclose(output);
    return 0;
}
