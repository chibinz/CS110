## Exercise 1
- Four floating point divisions in single precision (i.e. float)
`__m128 _mm_div_ps (__m128 a, __m128 b)`
- Sixteen max operations over signed 8-bit integers (i.e. char)
`__m128i _mm_max_epi8 (__m128i a, __m128i b)`
- Arithmetic shift right of eight signed 16-bit integers (i.e. short)
`__m128i _mm_sra_epi16 (__m128i a, __m128i count); shift specified for each operand`
`__m128i _mm_srai_epi16 (__m128i a, int imm8); shift specified by single immmediate`