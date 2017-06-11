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
	unsigned long int p_seed, q_seed;
    mpz_t p, q;
	gmp_rand_state_t state;

    mpz_init(p);
	mpz_init(q);

	if (getrandom(p_seed, (size_t)sizeof(unsigned long int), GND_NONBLOCK) == -1) {
		// Error
	}
	
	gmp_randseed_ui(state, p_seed);
	gmp_randinit_default(state);
	mpz_urandomb(p, state, PRIME_SIZE);

	if (mpz_prob_prime_p(p, 31) == 0) {  // Certainly not prime
		mpz_nextprime(p, p);  // TODO -> Check if I can call mpz_nextprime this way	
		if (mpz_cmp(p, 2^PRIME_SIZE - 1) >= 0) {
			// Try again to generate the prime
		} 
	}

//	if (getrandom(q_seed, (size_t)sizeof(unsigned long int), GND_NONBLOCK) == -1) {
		// Error
//	}
	
//	gmp_randseed_ui(state, q_seed);
//	gmp_randinit_default(state);
//	mpz_urandomb(q, state, PRIME_SIZE);
	

	

	
}
