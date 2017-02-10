#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int conversion[4];
int block[16] = {[15] = 0};

void initialize(void);
void move(char);
void new(void);
void show(void);

void main(void) {
    initialize();
    show();
    char ch[1];
    printf("please enter a key(h/j/k/l) to move: ");
    scanf("%s", ch);
    int i, j;
    int tmp[16];
    while (ch[0] != 'q') {
        // only when the block is changed after the block moves, 
        // we can add a new '2' in the block.
        for (i = 0; i < 16; i++)
            tmp[i] = block[i];
        move(ch[0]);
        for (i = 0, j = 0; i < 16; i++) {
            if (tmp[i] == block[i])
                j++;
            else
                break;
        }
        if (j != 16)
            new();
        show();

        // test whether the game is over.
        // if end equals to 1, the game is over.
        for (i = 0, j = 0; i < 16; i++) {
            if (block[i] != 0)
                j++;
            else
                break;
        }
        int end = 1;
        if (j == 16) {
            for (i = 0; i < 16; i += 4) {
                for (j = 0; j < 3; j++) {
                    if (block[i+j] == block[i+j+1])
                        end = 0;
                }
            }
            for ( i = 0; i < 4; i++) {
                for (j = 0; j < 12; j += 4) {
                    if (block[i+j] == block[i+j+4])
                        end = 0;
                }
            }
        } else {
            end = 0;
        }
        if (end == 1)
            break;
        
        printf("please enter a key(h/j/k/l) to move: ");
        scanf("%s", ch);
    }
    printf("\nTHE END\n\n");
}

void initialize(void) {
    // initialize a block with two numbers.
    // the two numbers can be 2 and 2 or 2 and 4 at a random place.
    srand((unsigned)time(NULL));
    int init_range[2] = {2, 4};
    int init_a, init_b;
    do {
        int init_index_a = rand() % 2;
        init_a = init_range[init_index_a];
        int init_index_b = rand() % 2;
        init_b = init_range[init_index_b];
    } while (init_a == 4 && init_b == 4);

    int index_a, index_b;
    do {
        index_a = rand() % 16;
        index_b = rand() % 16;
    } while (index_a == index_b);
    block[index_a] = init_a;
    block[index_b] = init_b;
}

int sort(int * row) {
    // extract a row without the number 0 called extraction
    // and fill with the number 0 in the end of row  called conversion.
    // this function return an int variable called size.
    int i, j;
    int size = 0;
    for (i = 0; i < 4; i++) {
        if (row[i] != 0) 
            size++;
    }
    int extraction[size];
    for (i = 0, j = 0; i < 4; i++) {
        if (row[i] != 0)
            extraction[j++] = row[i];
    }
    for (i = 0, j = 0; i < 4; i++) {
        if (j < size)
            conversion[i] = extraction[j++];
        else
            conversion[i] = 0;
    }
    return size;
}

void combine(int index) {
    // combine one number with the next same number.
    conversion[index] *= 2;
    conversion[index+1] = 0;
    int size;
    size = sort(conversion);
}

void magic(int * row) {
    // the basic logic in 2048.
    int i, size;
    size = sort(row);
    if (size == 4 && conversion[0] == conversion[1] && conversion[2] == conversion[3]) {
        conversion[0] = conversion[0] * 2;
        conversion[1] = conversion[2] * 2;
        conversion[2] = 0;
        conversion[3] = 0;
    } else {
        for (i = 0; i < size - 1; i++) {
            if (conversion[i] == conversion[i+1]) {
                combine(i);
                break;
            }
        }
    }
}

void reverse(int * row) {
    // reverse a row
    int i;
    int tmp[4];
    for (i = 0; i < 4; i++) {
        tmp[i] = row[3-i];
    }
    for (i = 0; i < 4; i++) {
        row[i] = tmp[i];
    }
}

void move(char direction) {
    // move the block
    int i, j;
    if (direction == 'h') {
        for (i = 0; i < 16; i += 4) {
            magic(block + i);
            for (j = 0; j < 4; j++)
                block[i+j] = conversion[j];
        }
    } else if (direction == 'l') {
        for (i = 0; i < 16; i += 4) {
            reverse(block + i);
            magic(block + i);
            for (j = 0; j < 4; j++)
                block[i+j] = conversion[j];
            reverse(block + i);
        }
    } else if (direction == 'k') {
        int transposition[4];
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++)
                transposition[j] = block[i+4*j];
            magic(transposition);
            for (j = 0; j < 4; j++) 
                block[i+4*j] = conversion[j];
        }
    } else if (direction == 'j') {
        int transposition[4];
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++)
                transposition[j] = block[i+4*j];
            reverse(transposition);
            magic(transposition);
            reverse(conversion);
            for (j = 0; j < 4; j++)
                block[i+4*j] = conversion[j];
        }
    } else {
        printf("please enter a correct direction key!\n");
    }
}

void new(void) {
    // add a new '2' in the block
    int i, j;
    int size = 0;
    for (i = 0; i < 16; i++) {
        if (block[i] == 0)
            size++;
    }
    int index[size];
    for (i = 0, j = 0; i < 16; i++) {
        if (block[i] == 0)
            index[j++] = i;
    }
    srand((unsigned)time(NULL));
    int rand_index = index[rand() % size];
    block[rand_index] = 2;
}

void show(void) {
    // show the block
    printf(" ——————————————————————————————  \n");
    int i;
    for (i = 0; i < 16; i += 4) {
        printf("|%6d |%6d |%6d |%6d |\n", \
               block[i], block[i+1], block[i+2], block[i+3]);
        printf(" ——————————————————————————————  \n");
    }
}
