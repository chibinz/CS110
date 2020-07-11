int source[] = {3, 1, 4, 1, 5, 9, 0};
int dest[] = {1, 1, 1, 1, 1, 1, 0, 0, 0, 0};

int main()
{
    int k;
    for (k = 0; source[k] != 0; k++)
    {
        dest[k] = source[k];
    }
    return 0;
}
