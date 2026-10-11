/* Check POPCNT results for both x86 operand widths. */

#include <assert.h>
#include <stdint.h>
#include <stddef.h>

int main(void)
{
    static const struct {
        uint64_t input;
        uint32_t count32;
        uint64_t count64;
    } cases[] = {
        { 0,                  0,  0 }, /* No bits. */
        { 0x0000000080000000,  1,  1 }, /* One low-half bit. */
        { 0x8000000000000000,  0,  1 }, /* One high-half bit. */
        { 0x0000000055555555, 16, 16 }, /* Multiple low-half bits only. */
        { 0xaaaaaaaa00000000,  0, 16 }, /* Multiple high-half bits only. */
        { 0xaaaaaaaa55555555, 16, 32 }, /* Both halves, not all bits. */
        { UINT64_MAX,        32, 64 }, /* All bits. */
    };
    size_t i;

    for (i = 0; i < sizeof(cases) / sizeof(cases[0]); i++) {
        uint32_t result32;
        uint32_t input32 = cases[i].input;
#ifdef __x86_64__
        uint64_t result64;
#endif

        asm volatile("popcntl %1, %0"
                     : "=r"(result32)
                     : "r"(input32)
                     : "cc");
        assert(result32 == cases[i].count32);
#ifdef __x86_64__
        asm volatile("popcntq %1, %0"
                     : "=r"(result64)
                     : "r"(cases[i].input)
                     : "cc");
        assert(result64 == cases[i].count64);
#endif
    }
    return 0;
}
