#include <stdio.h>
#include <stdlib.h>
 
int * p10(int key[10]) {
    int permutation[] = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6}; //p10 permutation table
    int *permuted_key = malloc(sizeof(int) * 10);

    for (int i = 0; i < 10; i++) {
        permuted_key[i]=key[permutation[i] - 1];
    }
    return permuted_key;
}


int * p8(int key[10]) {
    int permutation[] = {6, 3, 7, 4, 8, 5, 10, 9}; //p8 permutation table
    int *permuted_key = malloc(sizeof(int) * 8);

    for (int i = 0; i < 8; i++) {
      permuted_key[i] = key[permutation[i] - 1];
    }
    return permuted_key;
}


int * rotate_left(int key_section[10], int shift_amount) {

  shift_amount = shift_amount % 5;

  int *rotated_key = malloc(sizeof(int) * 10);

  for (int i = 0; i < 5; i++) {
    rotated_key[i] = key_section[(i + shift_amount) % 5];
  }
  for (int i = 5; i < 10; i++) {
    rotated_key[i] = key_section[5+((i + shift_amount) % 5)];
  }
  return rotated_key;
}

int main() {
  int key[10] = {1, 0, 1, 0, 0, 0, 0, 0, 1, 0};

  int * permutated10 = p10(key);
  int * r_left1= rotate_left(permutated10,1);
  int * r_left2= rotate_left(r_left1, 2);
  

  int * K1 = p8(r_left1);
  int * K2 = p8(r_left2); 
  
  

  for (int i = 0; i < 8; i++) {
    printf("%d ", K1[i]);
    
  }
  printf("\n");
  for (int i = 0; i < 8; i++) {
    printf("%d ", K2[i]);
    
  }

  return 0;
}