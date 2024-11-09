#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define max_data_bits 25
#define max_divisor_length 10

void bin_div();

int n, m;
char data[max_data_bits + 1];
char divisor[max_divisor_length + 1];
char *CRC = NULL;

void XOR() {
    for (int i = 0; i < m; i++)
        CRC[i] = ((CRC[i] == divisor[i]) ? '0' : '1');
    return;
}

void receiver() {
    int i;

    printf("Enter the received data: ");
    scanf("%s", data);

    printf("\n-----------------------------\n");
    printf("Data received: %s\n", data);

    bin_div();

    // Check if any bit in the check_value is 1, indicating an error
    for (i = 0; (i < m - 1) && (CRC[i] != '1'); i++);

    if (i < m - 1)
        printf("Error detected\n\n");
    else
        printf("No error detected\n\n");
}

int main() {
    int i, z, y, k;

    printf("Enter the data message: ");
    scanf(" %s", data);

    // Initialize the divisor array to zeros
    memset(divisor, '0', max_divisor_length);
    divisor[max_divisor_length] = '\0';

    printf("Enter the number of terms in the divisor: ");
    scanf(" %d", &z);

    printf("Enter the powers of the secret divisor polynomial: ");
    for (int w = 0; w < z; w++) {
        scanf(" %d", &y);
        divisor[y] = '1';
    }

    printf("Divisor after initialization: %s\n", divisor);

    n = strlen(data);
    m = strlen(divisor);

    CRC = (char *)malloc(m * sizeof(char));

    // Step 1: Append (m-1) 0s to the data
    for (i = n; i < n + m - 1; i++) {
        data[i] = '0';
    }
    data[i] = '\0';

    printf("Dividend = %s\n", data);

    // Step 2: Perform binary division(with XOR)
    bin_div();

    // Step 3: Append CRC to the original message
    for (i = n; i < n + m - 1; i++) {
        data[i] = CRC[i - n];
    }
    data[i] = '\0';

    printf("Data with CRC appended = %s\n", data);

    // Step 4: Perform binary division for the received data and divisor
    bin_div();

    // Step 5: If R == 0, then no error else there is an error
    receiver();

    free(CRC);
    return 0;
}

void bin_div() {
    int i, j;

    // Initialize CRC with the first m bits of data
    for (i = 0; i < m; i++)
        CRC[i] = data[i];

    // Perform division
    for (i = m; i <= n + m - 1; i++) {
        // If the leftmost bit is 1, perform XOR
        if (CRC[0] == '1')
            XOR();

        // Shift bits left by 1
        for (j = 0; j < m - 1; j++)
            CRC[j] = CRC[j + 1];

        // Append the next bit from data, if available
        CRC[j] = data[i];
    }
}
