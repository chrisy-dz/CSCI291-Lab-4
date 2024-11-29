// Including library headers that are needed for the working of the code
#include <iostream>
#include <iomanip>
#include <cstdlib>

// Telling the compiler to consider all the names in the std namespace as if they are in the global namespace
using namespace std;

// Function used to print the value of number in a fixed point-notation, in an output field of fieldspace where the number of decimal digits is precision
void print_real(float number, int fieldspace, int precision) 
{
    // Using printf to display the output with specified fieldspace, precision and number
    cout << fixed << setw(fieldspace) << setprecision(precision) << number << endl;
}

// Using function template for creating functions that can be used with different data types
template <typename T>
// Using a void function to update parameters
void update_scale(T& m1, T& m2, T m3 = 10) 
{
    T original_m1 = m1;
    T original_m2 = m2;
    
    // Summing m1 and m2, then multiplying by m3
    m1 = (original_m1 + original_m2) * m3;
    // Subtracting m1 and m2, then multiplying by m3
    m2 = (original_m1 - original_m2) * m3;
}

int main() 
{
     float a, b;
    cout << "Please input two real numbers: ";
    cin >> a >> b;
    cout << "Inputted values:\n"; 
    cout << "a: " << a << ", b: " << b << endl;	

    print_real(a, 7, 3);
    print_real(b, 7, 3);
    // Call the function update_scale with only the actual parameters a and b
     update_scale(a, b);

     cout << "Updated values:\n";
     cout << "a: " << a << ", b: " << b << endl;

    print_real(a, 7, 3);
    print_real(b, 7, 3);

    return 0;
}