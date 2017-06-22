#include <sys/random.h>
#include <gmp.h>

#define PRIME_SIZE 1536  // bits


int main(int argc, char *argv[]) {
	// TODO -> Tratar entrada
	
	// if option == generate keys
	generate_keys();	
}


// Generate a public/private key-pair
// TODO -> p and q generation can be made in parallel
void generate_keys() {
    unsigned long int seed;
    mpz_t n, p, q, g, lambda, mi;
    gmp_rand_state_t state;

    mpz_init2(p, PRIME_SIZE+1);
    mpz_init2(q, PRIME_SIZE+1);

    // Linux syscall to get random bytes
    if (getrandom(seed, sizeof(unsigned long int), GND_NONBLOCK) == -1) {
	    // Error
    }
	
    gmp_randseed_ui(state, seed);
    gmp_randinit_default(state);

    // Generating "p"
    do {
        mpz_urandomb(p, state, PRIME_SIZE);
        mpz_setbit(p, PRIME_SIZE - 2);
        // TODO -> Test if it is faster to check first if p is even and increment it by 1, to then call "mpz_mod_prime_p"
        if (mpz_probab_prime_p(p, 31) == 0) {  // Certainly not prime
            mpz_nextprime(p, p);  // TODO -> Check if I can call mpz_nextprime this way	
            // mpz_nextprime could generate a prime greather than 2^PRIME_SIZE - 1, that's why we don't "break"
        } else {  
            break; 
        }
    } while (mpz_cmp(p, 2^PRIME_SIZE - 1) > 0);

    // Generating "q"
    do {
        mpz_urandomb(q, state, PRIME_SIZE);
        mpz_setbit(q, PRIME_SIZE - 2);
        // TODO -> Test if it is faster to check first if p is even and increment it by 1, to then call "mpz_mod_prime_p"
        if (mpz_probab_prime_p(q, 31) == 0) {  // Certainly not prime
            mpz_nextprime(q, q);  // TODO -> Check if I can call mpz_nextprime this way	
            // mpz_nextprime could generate a prime greather than 2^PRIME_SIZE - 1, that's why we don't "break"
        } else {  
            break;
        }
    } while (mpz_cmp(q, 2^PRIME_SIZE - 1) > 0);

	mpz_mul(n, p, q);
	mpz_add_ui(g, n, 1);
	mpz_mul(lambda, p-1, q-1);
    mpz_mul(mi, lambda^-1); // TODO -> Find GMP function to do this

}

