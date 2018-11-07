#include <pbc.h>
#include <pbc_test.h>

int testbls();

int main()
{
    int result = testbls();
    return result;
}

int testbls()
{
    pbc_param_s params;
    pbc_param_init_a_gen(&params, 160, 512);
    pairing_s pairing;
    pairing_init_pbc_param(&pairing, &params);

    element_s g;
    element_init_G2(&g, &pairing);
    element_set0(&g);
    element_random(&g);

    element_s privKey;
    element_init_Zr(&privKey, &pairing);
    element_set0(&privKey);
    element_random(&privKey);

    element_s pubKey;
    element_init_G2(&pubKey, &pairing);
    element_set0(&pubKey);
    element_pow_zn(&pubKey, &g, &privKey);

    element_s h;
    element_init_G1(&h, &pairing);
    element_set0(&h);
    const char* hash = "some text to sign";
    //todo hash
    element_from_hash(&h, (void*)hash, strlen(hash));

    element_s signature;
    element_init_G2(&signature, &pairing);
    element_set0(&signature);
    element_pow_zn(&signature, &h, &privKey);

    element_s temp1;
    element_init_GT(&temp1, &pairing);
    element_set0(&temp1);
    pairing_apply(&temp1, &h, &pubKey, &pairing);

    element_s temp2;
    element_init_GT(&temp2, &pairing);
    element_set0(&temp2);
    pairing_apply(&temp2, &signature, &g, &pairing);

    int result = element_cmp(&temp1, &temp2);

    element_clear(&temp2);
    element_clear(&temp1);
    element_clear(&signature);
    element_clear(&h);
    element_clear(&pubKey);
    element_clear(&privKey);
    element_clear(&g);
    pairing_clear(&pairing);
    pbc_param_clear(&params);
    return result;
}

