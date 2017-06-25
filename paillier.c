#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <gmp.h>

#define PRIME_SIZE 1536  // bits

void generate_keys();

int main(int argc, char *argv[]) {
    // TODO -> Tratar entrada
	
    // if option == generate keys
    generate_keys();
    return 0;
}


// Generate a public/private key-pair
// TODO -> p and q generation can be made in parallel
void generate_keys() {
    int file;
    unsigned long int seed;
    mpz_t n, p, q, g, lambda, mi, max_num;
    gmp_randstate_t state;
    
    mpz_init(n);
    mpz_init(g);
    mpz_init(lambda);
    mpz_init(mi);
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

    mpz_mul(n, p, q);
    mpz_add_ui(g, n, 1);
    mpz_sub_ui(p, p, 1);
    mpz_sub_ui(q, q, 1);
    mpz_mul(lambda, p, q);
    mpz_invert(mi, lambda, n);
    
}

