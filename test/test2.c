#include <stdio.h>
#include <stdlib.h>
#include "ft_printf.h"

int main()
{
	ft_printf("%p\n", malloc(-11));
}
