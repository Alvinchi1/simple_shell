#include <stdio.h>
/**
 * main - Entry point
 * It calculates the sum of the first 10 natural numbers
 * and prints the result to the console.
 * Return: 0 (Success)
 */

int main(void)
{
int i;
int sum = 0;
for (i = 1; i <= 10; i++)
{
sum += i;
}
printf("The sum of the first 10 natural numbers is: %d\n", sum);
return (0);
}
