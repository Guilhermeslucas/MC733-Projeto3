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
    mpz_t n, p, q, g, lambda, mi, max_num;
    gmp_randstate_t state;
    
    mpz_init(kp->n);
    mpz_init(kp->g);
    mpz_init(kp->lambda);
    mpz_init(kp->mi);

    mpz_init2(p, PRIME_SIZE+1);
    mpz_init2(q, PRIME_SIZE+1);
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
        mpz_setbit(p, PRIME_SIZE - 1); // TODO -> Checar significancia dos bits
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
    mpz_invert(kp->mi, lambda, n);
    
}

void encrypt(char *buf, int buf_size, struct key_pair *kp) {
    int file, i;
    unsigned long int seed,  n_size, blocks;
    mpz_t m, r, n_square, e1, e2, e3, c;
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

    mpz_init(r);
    mpz_init(m);
    mpz_init(e1);
    mpz_init(e2);
    mpz_init(e3);

    gmp_randinit_default(state);
    gmp_randseed_ui(state, seed);
    
    mpz_init(n_square);
    mpz_pow_ui(n_square, kp->n, 2);

    if (buf_size < blocks) {
        // encrypt and return
    } else {
        i = 0;
        do {
            // TODO -> Set 'm'
            // IMPORTANT!!! -> mpz_import discards leading zeroes! In decryption, must check for exact blocks. If a partial block is present, add leading zeroes to it! 
            mpz_import(m, blocks, 1, sizeof(buf[0]), 0, 0, &buf[i]);
            mpz_urandomm(r, state, kp->n);
            mpz_powm_sec(e1, kp->g, m, n_square);
            mpz_powm_sec(e2, r, kp->n, n_square);
            mpz_mul(e3, e1, e2);
            mpz_mod(c, e3, n_square);
           
            // TODO -> Break this in two functions

            i += blocks;
            buf_size -= blocks;
        } while(buf_size >= blocks);

        if (buf_size > 0) {
            // encrypt remaining blocks
        }
    }
    
    mpz_init(m);
    mpz_import(m, buf_size, 1, sizeof(buf[0]), 0, 0, buf);
}
