#include <stdio.h>
#include <stdarg.h>
#include "gmp.h"
#include "maplec.h"
#define TRUE 1
#define FALSE 0
typedef unsigned long int uli_t;
typedef signed long int sli_t;
/* Global variables */
char *MAPLE_rslt;
/* Function prototypes */
static void M_DECL textCallBack(void *, int, char *);
void mpz_logbase2cl(sli_t *, mpz_t);
void mpz_logbase2fl(sli_t *, mpz_t);
void find_smallest_r(mpz_t, mpz_t);
int r_good(mpz_t, mpz_t, mpz_t);
int poly_eq_holds(MKernelVector, mpz_t, mpz_t, mpz_t);
int aks(mpz_t);
/* Function implementations */
/* MAPLE Callback used for directing result output */
static void M_DECL textCallBack( void *data, int tag, char *output )
{
  MAPLE_rslt = output;
}
/* rslt is always integral due to ceiling operation */
void mpz_logbase2cl(sli_t *rslt, mpz_t n)
{
  if (mpz_get_d_2exp(rslt, n) == 0.5) {
    --(*rslt);
  }
}
/* rslt is always integral due to floor operation */
void mpz_logbase2fl(sli_t *rslt, mpz_t n)
{
  if (mpz_get_d_2exp(rslt, n) == 0.5) {
    --(*rslt);
  }
  else {
    ++(*rslt);
  }
}
void find_smallest_r(mpz_t r, mpz_t n)
{
  /* Try out successive values of r and test if n^k != 1 (mod r) for every
   * k <= 4(log n)^2.
   */
  mpz_t max_k;
  mpz_t logn;
  mpz_t logn_sqrd;
  sli_t exp_cl;
  sli_t exp_fl;
  mpz_init(max_k);
  mpz_init(logn);
  mpz_init(logn_sqrd);
  /* Compute log n
   * Ceiling has been applied!
   */
  mpz_logbase2cl(&exp_cl, n);
  mpz_set_si(logn, exp_cl);
  /* log n is computed, compute 4 * (log n)^2
   * Compute (log n)^2
   */
  mpz_mul(logn_sqrd, logn, logn);
  /* Compute max_k */
  mpz_mul_si(max_k, logn_sqrd, 4);
  gmp_printf("max_k = %Zd\n", max_k);
  /* Now find the appropriate r:
   * r = 2;
   * while (1) {
   * if (r_good(r, max_k, n)) {
   * break;
   * }
   * r++;
   *}
   */
  mpz_set_ui(r, 2);
  while (1) {
    if (r_good(r, max_k, n)) {
      break;
    }
    mpz_add_ui(r, r, 1);
  }
}
int r_good(mpz_t r, mpz_t max_k, mpz_t n)
{
  mpz_t k;
  mpz_t mod_rslt;
  mpz_init(k);
  mpz_init(mod_rslt);
  mpz_set_ui(k, 1);
  /* while (k <= max_k) {
   * mod_rslt = n^k mod r;
   * if (mod_rslt == 1) {
   * return false;
   * }
   * k++;
   *}
   */
  while (mpz_cmp(k, max_k) < 0 || mpz_cmp(k, max_k) == 0) {
    mpz_powm(mod_rslt, n, k, r);
    if (mpz_cmp_ui(mod_rslt, 1) == 0) {
      return FALSE;
    }
    mpz_add_ui(k, k, 1);
  }
  return TRUE;
}
int poly_eq_holds(MKernelVector kv, mpz_t a, mpz_t n, mpz_t r)
{
  char *a_str;
  char *n_str;
  char *r_str;
  char *a_buf;
  char *n_buf;
  char *r_buf;
  size_t a_size;
  size_t n_size;
  size_t r_size;
  char maple_stmt[4092];
  int maple_sez;
  /* La..La..La..La.. */
  a_size = mpz_sizeinbase(a, 10)+2;
  r_size = mpz_sizeinbase(r, 10)+2;
  n_size = mpz_sizeinbase(n, 10)+2;
  a_buf = (char *) malloc (a_size*sizeof(char));
  r_buf = (char *) malloc (r_size*sizeof(char));
  n_buf = (char *) malloc (n_size*sizeof(char));
  a_str = mpz_get_str(a_buf, 10, a);
  n_str = mpz_get_str(r_buf, 10, n);
  r_str = mpz_get_str(n_buf, 10, r);
  /* Build up MAPLE statement */
  sprintf(maple_stmt,
    "if(powmod(x+%s,%s,x^%s-1,x) mod %s = "
    "powmod(x,%s,x^%s-1,x)+%s mod %s) then 1 "
    "else 0 end if;",
    a_str,n_str,r_str,n_str,
    n_str,r_str,a_str,n_str);
  printf("maple_stmt = %s\n", maple_stmt);
  /* Feed the statement to MAPLE kernel */
 EvalMapleStatement(kv, maple_stmt);
  /* To avoid memory leakage */
  free(a_buf);
  free(r_buf);
  free(n_buf);
  if (atoi(MAPLE_rslt) == 1) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}
int aks(mpz_t n)
{
  mpz_t r;
  mpz_t a;
  mpz_t max_a;
  mpz_t gcd_rslt;
  mpz_t totient_r;
  mpf_t ftotient_r;
  mpf_t sqrt_rslt;
  mpf_t sqrt_rslt2;
  mpf_t temp;
  mpf_t temp2;
  sli_t logn;
   /* For the sake of maple kernel */
  int argc = 0;
  char **argv;
  char err[2048];
  mpz_init(r);
  mpz_init(a);
  mpz_init(max_a);
  mpz_init(gcd_rslt);
  mpz_init(totient_r);
  mpf_init(ftotient_r);
  mpf_init(sqrt_rslt);
  mpf_init(sqrt_rslt2);
  mpf_init(temp);
  mpf_init(temp2);
  /* 1. If (n = a^k for a in N and b > 1) output COMPOSITE */
  if (mpz_perfect_power_p(n) != 0) {
    printf("Step 1 detected composite\n");
    return FALSE;
  }
  /* 2. Find the smallest r such that or(n) > 4(log n)^2 */
  find_smallest_r(r, n);
  gmp_printf("good r seems to be %Zd\n", r);
/* 3. If 1 < gcd(a, n) < n for some a <= r, output COMPOSITE */
/* for (a = 1; a <= r; a++) {
 * gcd_rslt = gcd(a, n);
 * if (gcd_rslt > 1 && gcd_rslt < n) {
 * return FALSE;
 * }
 *}
 */
 for (mpz_set_ui(a, 1);
      mpz_cmp(a, r) < 0 || mpz_cmp(a, r) == 0;
      mpz_add_ui(a, a, 1)) {
    mpz_gcd(gcd_rslt, a, n);
    if (mpz_cmp_ui(gcd_rslt, 1) > 0 && mpz_cmp(gcd_rslt, n) < 0) {
      printf("Step 3 detected composite\n");
      return FALSE;
    }
 }
/* 4. If n <= r, output PRIME */
if (mpz_cmp(n, r) < 0 || mpz_cmp(n, r) == 0) {
  printf("Step 4 detected prime\n");
  return TRUE;
}
/* 5. For a = 1 to floor(2*sqrt(totient(r))*(log n)
 *      if ( (X+a)^n != X^n + a (mod X^r-1, n) ), output COMPOSITE
 *
 * Choices of implementation to evaluate the polynomial equality:
 * (1) Implement powermodreduce on polynomial ourselves (tough manly way)
 * (2) Use MAPLE (not so manly, but less painful)
 */
/* Compute totient(r), since r is prime, this is simply r-1 */
mpz_sub_ui(totient_r, r, 1);
/* Compute log n (ceilinged) */
mpz_logbase2cl(&logn, n);
/* Compute sqrt(totient(r)) */
mpf_set_z(ftotient_r, totient_r);
mpf_sqrt(sqrt_rslt, ftotient_r);
/* Compute 2*sqrt(totient(r)) */
mpf_mul_ui(sqrt_rslt2, sqrt_rslt, 2);
/* Compute 2*sqrt(totient(r))*(log n) */
mpf_set(temp, sqrt_rslt2);
mpf_set_si(temp2, logn);
mpf_mul(temp, temp, temp2);
  /* Finally, compute max_a, after lots of singing and dancing */
  mpf_floor(temp, temp);
  mpz_set_f(max_a, temp);
  gmp_printf("max_a = %Zd\n", max_a);
  /* Now evaluate the polynomial equality with the help of maple kernel */
  /* Set up maple kernel incantations */
  MKernelVector kv;
  MCallBackVectorDesc cb = { textCallBack,
                       0, /* errorCallBack not used */
                       0, /* statusCallBack not used */
                       0, /* readLineCallBack not used */
                       0, /* redirectCallBack not used */
                       0, /* streamCallBack not used */
                       0, /* queryInterrupt not used */
                       0 /* callBackCallBack not used */
                    };
  /* Initialize Maple */
  if( ( kv = StartMaple(argc, argv, &cb, NULL, NULL, err) ) == NULL ) {
      printf( "Could not start Maple, %s\n", err );
      exit(666);
  }
  /* Here comes the complexity and bottleneck */
  /* for (a = 1; a <= max_a; a++) {
   * if (!poly_eq_holds(kv, a, n, r)) {
   *       return FALSE;
   * }
   *}
*/
  /* Make max_a only up to 5 */
  mpz_set_ui(max_a, 5);
  for (mpz_set_ui(a, 1);
       mpz_cmp(a, max_a) < 0 || mpz_cmp(a, max_a) == 0;
       mpz_add_ui(a, a, 1)) {
       if (!poly_eq_holds(kv, a, n, r)) {
         printf("Step 5 detected composite\n");
         return FALSE;
       }
  }
  /* 6. Output PRIME */
  printf("Step 6 detected prime\n");
  return TRUE;
}
int main()
{
  mpz_t n;
  mpz_init(n);
  printf("Enter a number: ");
  gmp_scanf("%Zd", &n);
  printf("AKS says: %s\n", aks(n)?"prime":"composite");
  return 0;
}
C       Source code of Fermat and Miller-Rabin primality tests in
       verifier.c.
     #include <stdio.h>
     #include <stdarg.h>
     #include <gmp.h>
     #define TRUE 1
     #define FALSE 0
     typedef struct mk {
       mpz_t m;
       unsigned long int k;
     } mk_t;
     /* Function prototypes (Miller-Rabin) */
     mk_t get_mk(mpz_t);
     int millerrabin (mpz_t);
     int mrcore(mpz_t, mpz_t, unsigned long int, unsigned long int, mpz_t, mpz_t,
             mpz_t, mpz_t);
     int in_array(mpz_t, unsigned long int*, int);
     /* Function prototype (Fermat) */
     int fermat(mpz_t);
     /* Function implementations */
     /* n must be an odd integer */
     mk_t get_mk (mpz_t n)
     {
       unsigned long int k = 1;
       unsigned long int pow2 = 2;
  mpz_t quot;
  mpz_t n_cpy;
  mk_t mk;
  mpz_init(quot);
  mpz_init(n_cpy);
  mpz_init(mk.m);
  /* Do not clobber the value of n */
  mpz_set(n_cpy, n);
  /* n_cpy-- */
  mpz_sub_ui(n_cpy, n_cpy, 1);
  /* n_cpy div pow2 and get the quotient */
  mpz_cdiv_q_ui(quot, n_cpy, pow2);
  while (mpz_even_p(quot)) {
    pow2 *= 2;
    k++;
    mpz_cdiv_q_ui(quot, n_cpy, pow2);
  }
  mpz_set(mk.m, quot);
  mk.k = k;
   /* Free up no longer used stuffs */
  mpz_clear(n_cpy);
  return mk;
}
int mrcore(mpz_t min_one,
         mpz_t a,
         unsigned long int a_given,
         unsigned long int k,
         mpz_t b0,
         mpz_t b,
         mpz_t m,
         mpz_t n)
{
  unsigned long int i;
  /* Choose a random integer a with 1 < a < n-1 */
  mpz_set_ui(a, a_given);
  /* Compute b0 = a^m mod n */
  mpz_powm(b0, a, m, n);
  /* If b0 = +/- 1 mod n, then n is probably prime */
  if (mpz_cmp_ui(b0, 1) == 0 ||
      mpz_cmp(b0, min_one) == 0) {
    return TRUE;
  }
  /* b = b0 */
  mpz_set(b, b0);
  for (i = 1; i <= k-1; i++) {
    mpz_powm_ui(b, b, 2, n);
    if (mpz_cmp_ui(b, 1) == 0) {
      return FALSE;
    }
    else if (mpz_cmp(b, min_one) == 0) {
      return TRUE;
    }
  }
  return FALSE;
}
int millerrabin (mpz_t n)
{
  const int SIZE = 30;
  mk_t mk;
  mpz_t a, b0, b, m, min_one;
  unsigned long int k;
  unsigned long int as[] =
    { 2, 3, 5, 7, 11,
       13, 17, 19, 23, 29,
       31, 37, 41, 43, 47,
       53, 59, 61, 67, 71,
       73, 79, 83, 89, 97,
      101, 103, 107, 109, 113
    };
  int mr_rslt = TRUE;
  int i;
  mpz_init(a);
  mpz_init(b0);
  mpz_init(b);
  mpz_init(m);
  mpz_init(min_one);
  /* Check for all the a's, if it is one of the a's, it's automatically a
   * prime.
   */
  if (in_array(n, as, SIZE)) {
    return TRUE;
  }
  /* Let n > 1 be an odd integer */
  if (mpz_cmp_ui(n, 1) <= 0) {
    return FALSE;
  }
  if (mpz_even_p(n)) {
    return FALSE;
  }
  /* Write n - 1 = 2^k * m where m is odd */
  mk = get_mk(n);
  mpz_set(m, mk.m);
  k = mk.k;
  /* Compute n-1 which is -1 */
  mpz_sub_ui(min_one, n, 1);
  /* Call mrcore for a = first 30 primes */
  for (i = 0; i < SIZE; i++) {
    mr_rslt = mr_rslt && mrcore(min_one, a, as[i], k, b0, b, m, n);
    if (!mr_rslt) {
      return FALSE;
    }
  }
  return mr_rslt;
}
int in_array(mpz_t needle, unsigned long int* arr, int arr_sz)
{
int i;
  for (i = 0; i < arr_sz; i++) {
    if (mpz_cmp_ui(needle, arr[i]) == 0) {
      return TRUE;
    }
  }
  return FALSE;
}
int fermat(mpz_t n)
{
  unsigned long int i, a;
  mpz_t g, fer, base, exp1;
  i = 0;
  a = 2;
  mpz_init(g);
  mpz_init(fer);
  mpz_init(base);
  mpz_init(exp1);
  while (i < 10) {
    mpz_gcd_ui(g, n, a);
    if (mpz_cmp_ui (g, 1) == 0) {
      i++;
      mpz_set_ui(base, a);
      mpz_sub_ui(exp1, n, 1);
      mpz_powm(fer,base,exp1, n);
      if (mpz_cmp_ui(fer, 1) != 0) {
         return FALSE;
      }
    }
    a++;
  }
  if (mpz_cmp_ui(fer, 1) == 0) {
    return TRUE;
  }
}
int main()
{
  mpz_t n;
  mpz_init(n);
  printf("Miller-Rabin/Fermat Primality Testing\n");
  printf("Enter a number: ");
  gmp_scanf("%Zd", n);
  printf("Miller-Rabin says: %s\n",
         millerrabin(n)?"probably a prime...":"not a prime.");
  printf("Fermat says: %s\n",
         fermat(n)?"probably a prime...":"not a prime.");
  return 0;
}
