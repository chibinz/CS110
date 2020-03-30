int first1posshift(unsigned _, unsigned a1)
{
    int n = -1;
    for (int i = 0; i < 32; i++, a1 >>= 1)
        if (a1 & 1)
            n = i;

    return n;
}

int first1posmask(unsigned _, unsigned a1)
{
    int n = -1;
    unsigned mask = 0x80000000;
    for (int i = 31; i >= 0; i--, mask >>= 1)
        if (a1 & mask)
        {
            n = i;
            break;
        }

    return n;
}