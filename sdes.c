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


/*__ENC/DEC MODELS__*/
int* ip(int plain_text[8]) {

  int permutation[] = {2, 6, 3, 1, 4, 8, 5, 7};
  int *permuted_text = malloc(sizeof(int) * 8);

  for (int i = 0; i < 8; i++) {
    permuted_text[i] = plain_text[permutation[i] - 1];
  }

  return permuted_text;
}

int* inverse_ip(int permuted_text[8]) {

  int* output = malloc(sizeof(int) * 8);

  int inverse_permutation[] = {4, 1, 3, 5, 7, 2, 8, 6};

  for (int i = 0; i < 8; i++) {
    output[i] = permuted_text[inverse_permutation[i] - 1];
  }

  return output;
}
//Expand and permutate
int *ep(int block[4]) {
  int ep_table[] = {4, 1, 2, 3, 2, 3, 4, 1};
  int *expanded_permuted_block = malloc(sizeof(int) * 8);

  for (int i = 0; i < 8; i++) {
    expanded_permuted_block[i] = block[ep_table[i] - 1];
  }
  
  return expanded_permuted_block;
}

int *xor(int K[], int S[], int size) {
  int *xor_results = malloc(sizeof(int) * size);

  for (int i = 0; i < size; i++) {
    xor_results[i] = K[i] ^ S[i];
  }

  return xor_results;
}

//SUBSTITUTION BOXES
static const int S0[4][4] = {
  {1, 0, 3, 2},
  {3, 2, 1, 0},
  {0, 2, 1, 3},
  {3, 1, 3, 2},
};

static const int S1[4][4] = {
  {0, 1, 2, 3},
  {2, 0, 1, 3},
  {3, 0, 1, 0},
  {2, 1, 0, 3},
};

int get_matrix_index(int block[4]) {
  int outer_ints[2] = {block[0], block[3]};
  int inner_ints[2] = {block[1], block[2]};

  int outer_decimal = (outer_ints[0] << 1) | outer_ints[1];
  int inner_decimal = (inner_ints[0] << 1) | inner_ints[1];

  return (outer_decimal << 2) | inner_decimal;
}

int* get_matrix_value(int xored_block[4], int const S[4][4]) {
  int matrix_index = get_matrix_index(xored_block);

  int value =S[matrix_index >> 2][matrix_index & 0x03];
  int* binary_representation= malloc(sizeof(int) * 2);
  // Convert the matrix value to binary representation.
  for (int i = 1; i >= 0; i--) {
    binary_representation[i] = value & 1;
    value >>= 1;
  }
  
  return binary_representation;
}

int* sub_boxes( int xored[8]){
  int* binS0 = get_matrix_value(xored, S0);
  int* binS1 = get_matrix_value(xored+4,S1);

  int* output = malloc(4 * sizeof(int));

  for (int i = 0; i < 2; i++) {
    output[i] = binS0[i];
  }

  for (int i = 0; i < 2; i++) {
    output[i + 2] = binS1[i];
  }

  return output;
}

int * p4(int key[4]) {
    int permutation[] = {2, 4, 3, 1}; //p8 permutation table
    int *permuted_key = malloc(sizeof(int) * 4);

    for (int i = 0; i < 4; i++) {
      permuted_key[i] = key[permutation[i] - 1];
    }
    return permuted_key;
}

int * encrypt( int plaintext[8], int K1[8], int K2[8] ){
  //IP
  int* i_perm =ip(plaintext);

  //Fk1
  int* ext_perm = ep(i_perm+4);
  int* xored_k1 = xor(K1,ext_perm, 8);
  int* s_boxed = sub_boxes(xored_k1);
  int* permutated_4 = p4(s_boxed);

  //XOR
  int* xored = xor(permutated_4,i_perm, 4);

  //free Fk1
  free(ext_perm);
  free(xored_k1);
  free(permutated_4);

  //SWAP
  int* perm_r = i_perm+4;
  int* swap = malloc(8 * sizeof(int));
  for (int i = 0; i < 4; i++) {
    swap[i] = perm_r[i];
  }
  for (int i = 0; i < 4; i++) {
    swap[i + 4] = xored[i];
  }

  free(xored);

  //Fk2
  ext_perm = ep(swap+4);
  int* xored_k2 = xor(K2,ext_perm, 8);
  s_boxed = sub_boxes(xored_k2);
  permutated_4 = p4(s_boxed);
  xored = xor(permutated_4,swap, 4);
  int* swap_r = swap+4;
  int *output = malloc(8 * sizeof(int));

  free(ext_perm);
  free(xored_k2);
  free(s_boxed);
  free(permutated_4);

  //unite
  for (int i = 0; i < 4; i++) {
    output[i] = xored[i];
  }
  for (int i = 0; i < 4; i++) {
    output[i + 4] = swap_r[i];
  }

  free(swap);
  free(i_perm);

  //IP^-1
  output = inverse_ip(output);
  return output;

}




int main() {
  /*start*/
  int L;
  scanf("%d\n", &L);

  for (int i = 0; i < L; i++) {
    char operation;
    scanf(" %c", &operation); // Read the operation character

    // Lê a chave
    int key[10];

    // Read the key from the pipe
    for (int j = 0; j < 10; j++) {
        scanf("%1d", &key[j]); // Read one digit at a time for the key
    }

    // Generate the subkeys.
    int **sub_keys = keygen(key);

    int block[10];
    for (int j = 0; j < 8; j++) {
        scanf("%1d", &block[j]); // Read one digit at a time for the block
    }

    // Encrypt or decrypt the block.
    if (operation == 'C') {
      int *encrypted = encrypt(block, sub_keys[0], sub_keys[1]);
      for (int i = 0; i < 8; i++) {
        printf("%d", encrypted[i]);
      }
      free(encrypted);
    } else if (operation == 'D') {
      int *decrypted = encrypt(block, sub_keys[1], sub_keys[0]);
      for (int i = 0; i < 8; i++) {
        printf("%d", decrypted[i]);
      }
      free(decrypted);
    } else {
      printf("Invalid operation: %c\n", operation);
    }

    printf("\n");
    
    free(sub_keys);

  }

  return 0;
}