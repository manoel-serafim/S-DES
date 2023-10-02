#include <stdio.h>
#include <stdlib.h>


/*KEY GENERATION MODULES*/
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

/*__KEY GENERATOR FUNCTION__*/
int** keygen(int key[10]) {

  int * permutated10 = p10(key);
  int * r_left1= rotate_left(permutated10,1);
  int * r_left2= rotate_left(r_left1, 2);

  int ** keys = malloc(sizeof(int*) * 2);;
  keys[0] = p8(r_left1);
  keys[1] = p8(r_left2); 

  free(permutated10);
  free(r_left1);
  free(r_left2);
  return keys;
}

void main() {


  int ** keys = keygen()
}