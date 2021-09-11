#include <iostream>
#include <stdlib.h>
using namespace std;
int main()
{
    int N;
    int count = 1;
    // bool isPrime = false;

    cin >> N;
    if (N > 1)
    {
        for (int i = 2; i < N; i++)
        {
            for (int j = 2; j < i; j++)
            {
                if (i % j == 0)
                {
                    count++;
                }
            }

            // if(count == 1){
            //     isPrime= true;
            // }

            if (count == 1)
            {
                printf("%d ", i);
            }

            count =1;
            // isPrime = false;
        }
    }
    return 0;
}
