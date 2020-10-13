# Laboratory 3: Use a library

| **Data type** | **Number of bits** | **Range** | **Description** |
| :-: | :-: | :-: | :-- | 
| `uint8_t`  | 8 | 0, 1, ..., 255 | Unsigned 8-bit integer |
| `int8_t`   | 8 | -128,-127, ..., 127 | 8-bit integer |
| `uint16_t` | 16 | 0, 1, ..., 65535 | Unsigned 16-bit integer |
| `int16_t`  | 16 | -32768, -32767, ..., 32767 | 16-bit integer |
| `float`    | 32 | -3.4e+38, ..., 3.4e+38 | Single-precision floating-point |
| `void`    | Nemá velikost | Nemá hodnoty | Prázdný návratový typ |

### Preparation to Lab

'''C
#include <avr/io.h>

// Function declaration (prototype)
uint16_t calculate(uint8_t x, uint8_t y);

int main(void)
{
    uint8_t a = 156;
    uint8_t b = 14;
    uint16_t c;

    // Function call
    c = calculate(a, b);

    while (1)
    {
    }
    return 0;
}

// Function definition (body)
uint16_t calculate(uint8_t x, uint8_t y)
{
    uint16_t result;    // result = x^2 + 2xy + y^2

    result = x*x+2*(x*y)+y*y; //result = x*x;
			      //result = result + 2*(x*y);
			      //result = result + (y*y);
    return result;
}
'''

## Laboratory

Documentation of your files [Goxygen commands](http://www.doxygen.nl/manual/docblocks.html#specialblock)