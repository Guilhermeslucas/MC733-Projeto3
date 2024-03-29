//////////////////////////////////////////////////////////////////////////////
// Standard includes
// SystemC includes
// ArchC includes

#include "peripheral.h"

//////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "gmpxx.h"
#include "gmp.h"

#include <string.h>

#define PRIME_SIZE 1536  // bits

/// Value
int value = 0;

struct key_pair {
    // Public key
    mpz_t g;
    mpz_t n;

    // Private key
    mpz_t lambda;
    mpz_t mi;    
};

void generate_keys(struct key_pair *kp);
void encrypt(char *buf, int buf_size, struct key_pair *kp, mpz_t *c);
void encrypt_block(mpz_t m, struct key_pair *kp, gmp_randstate_t state, mpz_t *c);
void decrypt(mpz_t *c, struct key_pair *kp);

/// Constructor
ac_tlm_peripheral::ac_tlm_peripheral( sc_module_name module_name , int k ) :
  sc_module( module_name ),
  target_export("iport")
{
    /// Binds target_export to the memory
    target_export( *this );

}

/// Destructor
ac_tlm_peripheral::~ac_tlm_peripheral() {

}

/** Internal Write
  * Note: Always write 32 bits
  * @param a is the address to write
  * @param d id the data being write
  * @returns A TLM response packet with SUCCESS
*/
ac_tlm_rsp_status ac_tlm_peripheral::writem( const uint32_t &a , const uint32_t &d )
{
    if (a == 536870912U) {
        value = d;
    } else if (a == 536870912U + 4) {
        struct key_pair kp;
        mpz_t c;

        value = 1;

        generate_keys(&kp);
        encrypt("MC733\0", strlen("MC733\0"), &kp, &c);
        decrypt(&c, &kp);
        value = 0;
    }
    
    return SUCCESS;
}

/** Internal Read
  * Note: Always read 32 bits
  * @param a is the address to read
  * @param d id the data that will be read
  * @returns A TLM response packet with SUCCESS and a modified d
*/
ac_tlm_rsp_status ac_tlm_peripheral::readm( const uint32_t &a , uint32_t &d )
{
    if (a == 67108872U) {
        d = value;
        value = 1;
    }

    return SUCCESS;
}


// Generate a public/private key-pair
// TODO -> p and q generation can be made in parallel
void generate_keys(struct key_pair *kp) {
    int file;
    unsigned long int seed;
    mpz_t p, q, max_num;
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
    mpz_add_ui(kp->g, kp->n, 1);

    mpz_sub_ui(p, p, 1);
    mpz_sub_ui(q, q, 1);
    mpz_mul(kp->lambda, p, q);

    mpz_invert(kp->mi, kp->lambda, kp->n);
}

void encrypt(char *buf, int buf_size, struct key_pair *kp, mpz_t *c) {
    int file;
    unsigned long int seed;
    mpz_t m;
    gmp_randstate_t state;
    
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
    
    mpz_init(m);
    mpz_import(m, buf_size, 1, sizeof(buf[0]), 0, 0, buf);
   
    if (mpz_cmp(m, kp->n) > 0) { // TODO -> Check if it must be m < n or m <= n
        perror("m must be smaller than n");
        exit(1);
    }
    printf("Message: ");
    mpz_out_str(stdout, 16, m);
    printf("\n\n");

    encrypt_block(m, kp, state, c);

    printf("Encrypted messaged: ");
    mpz_out_str(stdout, 16, *c);
    printf("\n\n");
}

void encrypt_block(mpz_t m, struct key_pair *kp, gmp_randstate_t state, mpz_t *c) {
    mpz_t r, n_square, e1, e2, e3;
   
    mpz_init(*c);
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

void decrypt(mpz_t *c, struct key_pair *kp) {
    mpz_t n_square, d1;
 
    /*
    xp = x mod p
    xq = x mod q

    yp = xp^(dp) mod p
    yq = xq^(dq) mod q

    dp = d mod (p-1)
    dq = d mod (q-1)

    y = q*cp*yp + p*cq*yq mod n

    cp = q^(-1) mod p
    cq = p^(-1) mod q

    */
    mpz_init(n_square);
    mpz_pow_ui(n_square, kp->n, 2);
    
    mpz_init(d1);
    mpz_powm_sec(d1, *c, kp->lambda, n_square);
    mpz_sub_ui(d1, d1, 1);
    mpz_divexact(d1, d1, kp->n);

    mpz_mul(d1, d1, kp->mi);
    mpz_mod(d1, d1, kp->n);
    
    printf("Recovered messaged: ");
    mpz_out_str(stdout, 16, d1);
    printf("\n\n");
}
