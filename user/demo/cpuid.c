// check cpuid output

#include <sgx-lib.h>
#include <stdio.h>

// http://stackoverflow.com/questions/6491566/getting-the-machine-serial-number-and-cpu-id-using-c-c-in-linux
static inline void native_cpuid(unsigned int *eax, unsigned int *ebx,
                                unsigned int *ecx, unsigned int *edx)
{
    /* ecx is often an input as well as an output. */
    asm volatile("cpuid"
        : "=a" (*eax),
          "=b" (*ebx),
          "=c" (*ecx),
          "=d" (*edx)
        : "0" (*eax), "2" (*ecx));
}

void enclave_main()
{
    // https://gist.github.com/ayeks/5ebefd33321cfd245a17
    unsigned eax, ebx, ecx, edx;
    eax = 1; /* processor info and feature bits */
    native_cpuid(&eax, &ebx, &ecx, &edx);
    printf("eax: %x ebx: %x ecx: %x edx: %x\n", eax, ebx, ecx, edx);

    printf("stepping %d\n", eax & 0xF); // Bit 3-0
    printf("model %d\n", (eax >> 4) & 0xF); // Bit 7-4
    printf("family %d\n", (eax >> 8) & 0xF); // Bit 11-8
    printf("processor type %d\n", (eax >> 12) & 0x3); // Bit 13-12
    printf("extended model %d\n", (eax >> 16) & 0xF); // Bit 19-16
    printf("extended family %d\n", (eax >> 20) & 0xFF); // Bit 27-20
  
    // if smx set - SGX global enable is supported
    printf("smx: %d\n", (ecx >> 6) & 1); // CPUID.1:ECX.[bit6]

    /* Extended feature bits (EAX=07H, ECX=0H)*/
    printf("\nExtended feature bits (EAX=07H, ECX=0H)\n");
    eax = 7;
    ecx = 0;
    native_cpuid(&eax, &ebx, &ecx, &edx);
    printf("eax: %x ebx: %x ecx: %x edx: %x\n", eax, ebx, ecx, edx);

    //CPUID.(EAX=07H, ECX=0H):EBX.SGX = 1,
    printf("sgx available: %d\n", (ebx >> 2) & 0x1);


    sgx_exit(NULL);
}

