/* 
*  Hardware specific settings: initializing pins.
*  Helper functions for float->string conversions
*/

//// Function declarations /////////////////////////////////////////////////////////
void initialize_pins();
void reverse(char* str, int len);                   // Reverses a string 'str' of length 'len'
int intToStr(int x, char str[], int d);             // Converts a given integer x to string str[]
void ftoa(float n, char* res, int afterpoint);      // Converts a floating-point/double number to a string


//// Variables /////////////////////////////////////////////////////////////////////
#ifdef ENABLE_PIR
const int pirSensorPin = 18;
#endif
#ifdef ENABLE_LIGHT
const int lightSensorPin = 34; // 35 is also safe
#endif

/*const int analogTempPin1 = 25;
const int analogTempPin = 5;*/


//// Functions /////////////////////////////////////////////////////////////////////
void initialize_pins(){
    #ifdef ENABLE_PIR
    pinMode(pirSensorPin, INPUT);
    #endif
    #ifdef ENABLE_LIGHT
    pinMode(lightSensorPin, INPUT);
    #endif
}


// Reverses a string 'str' of length 'len'
void reverse(char* str, int len)
{
    int i = 0, j = len - 1, temp;
    while (i < j) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}

  
// Converts a given integer x to string str[]. 
// d is the number of digits required in the output. 
// If d is more than the number of digits in x, 
// then 0s are added at the beginning.
int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x) {
        str[i++] = (x % 10) + '0';
        x = x / 10;
    }
  
    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';
  
    reverse(str, i);
    str[i] = '\0';
    return i;
}


// Converts a floating-point/double number to a string.
void ftoa(float n, char* res, int afterpoint)
{
    // Extract integer part
    int ipart = (int)n;
  
    // Extract floating part
    float fpart = n - (float)ipart;
  
    // convert integer part to string
    int i = intToStr(ipart, res, 0);
  
    // check for display option after point
    if (afterpoint != 0) {
        res[i] = '.'; // add dot
  
        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter 
        // is needed to handle cases like 233.007
        fpart = fpart * pow(10, afterpoint);
  
        intToStr((int)fpart, res + i + 1, afterpoint);
    }
}
