int input_size = 7;                 
int input_inorder[] = {4, 2, 5, 1, 6, 3, 7};   
int input_postorder[] = {4, 5, 2, 6, 7, 3, 1}; 

void __attribute__ ((noinline)) print(int a)
{
    asm("mv a1, %0"::"r"(a):"a1");
    asm("li a0, 1":::"a0");
    asm("ecall");
}

void __attribute__ ((noinline)) my_exit()
{
    asm("mv a1, zero");
    asm("li a0, 17");
    asm("ecall");
}

int search(int data) 
{ 
    for (int i = 0; i < input_size; i++) 
        if (input_inorder[i] == data) 
            return i;
    
    return -1;
} 
  
void traverse(int start, int end, int right) 
{ 
    if (start > end) 
        return; 
  
    int value = input_postorder[end]; 
    print(value);

    int in = search(value) - right; 
    traverse(start, in - 1, right); 
    traverse(in, end - 1, right + 1);
} 


int main()
{
    traverse(0, input_size - 1, 0);

    my_exit();
}