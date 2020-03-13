int factorial(int a0)
{
    if(a0 == 0)
        return 1;
    else
        return a0 * factorial(a0 - 1);
}