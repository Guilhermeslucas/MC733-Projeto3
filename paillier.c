#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <gmp.h>

#define PRIME_SIZE 1536  // bits

struct key_pair {
    // Public key
    mpz_t g;
    mpz_t n;

    // Private key
    mpz_t lambda;
    mpz_t mi;    
};

void generate_keys(struct key_pair *kp);
void encrypt(char *buf, int buf_size, struct key_pair *kp);
void encrypt_block(mpz_t m, struct key_pair *kp, gmp_randstate_t state, mpz_t *c);

int main(int argc, char *argv[]) {
    // TODO -> Tratar entrada
    char *a = "Vitor";
    struct key_pair kp;
	
    // if option == generate keys
    generate_keys(&kp);
    encrypt(a, 5, &kp);
    return 0;
}


// Generate a public/private key-pair
// TODO -> p and q generation can be made in parallel
void generate_keys(struct key_pair *kp) {
    int file;
    unsigned long int seed;
    mpz_t n, p, q, max_num;
    gmp_randstate_t state;
    
    mpz_init(kp->n);
    mpz_init(kp->g);
    mpz_init(kp->lambda);
    mpz_init(kp->mi);

    mpz_init2(p, PRIME_SIZE);
    mpz_init2(q, PRIME_SIZE);
    mpz_init(max_num);
    mpz_ui_pow_ui(max_num, 2, 1536);
    mpz_sub_ui(max_num, max_num, 1);

    file = open("/dev/urandom", O_RDONLY);
    if (file == -1) {
        perror("Failed to open \"/dev/urandom\"");
        exit(1);
    }
    
    if (read(file, &seed, sizeof(unsigned long int)) == -1) {
        perror("Failed to read random bytes");
        exit(1);
    }

    close(file);
    
    gmp_randinit_default(state);
    gmp_randseed_ui(state, seed);
    
    // Generating "p"
    do {
        mpz_urandomb(p, state, PRIME_SIZE);
        mpz_setbit(p, PRIME_SIZE - 1);
        mpz_setbit(p, 0);
        if (mpz_probab_prime_p(p, 31) == 0) {  // Certainly not prime
            mpz_nextprime(p, p);  // TODO -> Check if I can call mpz_nextprime this way	
            // mpz_nextprime could generate a prime greather than 2^PRIME_SIZE - 1, that's why we don't "break"
        } else {  
            break; 
        }
    } while (mpz_cmp(p, max_num) > 0);
    
    // Generating "q"
    do {
        mpz_urandomb(q, state, PRIME_SIZE);
        mpz_setbit(q, PRIME_SIZE - 1);
        mpz_setbit(q, 0);
        if (mpz_probab_prime_p(q, 31) == 0) {  // Certainly not prime
            mpz_nextprime(q, q);  // TODO -> Check if I can call mpz_nextprime this way	
            // mpz_nextprime could generate a prime greather than 2^PRIME_SIZE - 1, that's why we don't "break"
        } else {  
            break;
        }
    } while (mpz_cmp(q, max_num) > 0);

    mpz_mul(kp->n, p, q);
    mpz_add_ui(kp->g, n, 1);
    mpz_sub_ui(p, p, 1);
    mpz_sub_ui(q, q, 1);
    mpz_mul(kp->lambda, p, q);
    mpz_invert(kp->mi, kp->lambda, n);
    
}

void encrypt(char *buf, int buf_size, struct key_pair *kp) {
    int file, i;
    unsigned long int seed,  n_size, blocks;
    mpz_t m, c;
    gmp_randstate_t state;

    n_size = mpz_sizeinbase(kp->n, 2);
    blocks = n_size/8;
    
    file = open("/dev/urandom", O_RDONLY);
    if (file == -1) {
        perror("Failed to open \"/dev/urandom\"");
        exit(1);
    }
    
    if (read(file, &seed, sizeof(unsigned long int)) == -1) {
        perror("Failed to read random bytes");
        exit(1);
    }

    close(file);
    
    gmp_randinit_default(state);
    gmp_randseed_ui(state, seed);

    i = 0;
    while(buf_size >= blocks) {
        // IMPORTANT!!! -> mpz_import discards leading zeroes! In decryption, must check for exact blocks. If a partial block is present, add leading zeroes to it! 
        mpz_import(m, blocks, 1, sizeof(buf[0]), 0, 0, &buf[i]);
        encrypt_block(m, kp, state, &c);
        i += blocks;
        buf_size -= blocks;
    }

    if (buf_size > 0) {
        mpz_import(m, buf_size, 1, sizeof(buf[0]), 0, 0, &buf[i]);
        encrypt_block(m, kp, state, &c);
    }
    
    mpz_out_str(stdout, 2, m);
    printf("\n\n");

    mpz_out_str(stdout, 2, c);
    printf("\n\n");
}

void encrypt_block(mpz_t m, struct key_pair *kp, gmp_randstate_t state, mpz_t *c) {
    mpz_t r, n_square, e1, e2, e3;
    
    mpz_init(r);
    mpz_init(e1);
    mpz_init(e2);
    mpz_init(e3);

    mpz_init(n_square);
    mpz_pow_ui(n_square, kp->n, 2);

    mpz_urandomm(r, state, kp->n);
    mpz_powm_sec(e1, kp->g, m, n_square);
    mpz_powm_sec(e2, r, kp->n, n_square);
    mpz_mul(e3, e1, e2);
    mpz_mod(*c, e3, n_square);
}
/*
void decrypt(mpz_t c, struct key_pair *kp) {
    int i;
    unsigned long int n_size, blocks;
    mpz_t m, n_square, d1, d2;
    
    n_size = mpz_sizeinbase(kp->n, 2);
    blocks = n_size/8;
    
    /*i = 0;
    while(buf_size >= blocks) {
        // IMPORTANT!!! -> mpz_import discards leading zeroes! In decryption, must check for exact blocks. If a partial block is present, add leading zeroes to it! 
        mpz_import(m, blocks, 1, sizeof(buf[0]), 0, 0, &buf[i]);
        i += blocks;
        buf_size -= blocks;
    }

    if (buf_size > 0) {
        mpz_import(m, buf_size, 1, sizeof(buf[0]), 0, 0, &buf[i]);
    }
    mpz_init(n_square);
    mpz_pow_ui(n_square, kp->n, 2);

    mpz_powm_sec(d1, c, kp->lambda, n_square);
    mpz_sub_ui(d2, d1, 1);
    mpz_div

    //L(c^lambda mod n_square)*mi mod n
    L = (x - 1)/n
    
    mpz_out_str(stdout, 2, m);
    printf("\n\n");

    mpz_out_str(stdout, 2, c);
    printf("\n\n");
}*/
