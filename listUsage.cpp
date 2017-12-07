/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>
#include <iostream>   
#include <list>   
#include <numeric>   
#include <algorithm> 

using namespace std;

//vector<string> s;
 
typedef list<string> LISTINT;  
LISTINT listOne; 

int main()
{
    //s.push_back("hello world");
    listOne.push_back ("hello");   
    listOne.push_back ("world");
    LISTINT::iterator i;
    //cout<<test;
    for (i = listOne.begin(); i != listOne.end(); ++i)   
        cout << *i << " ";   
    cout << endl;
    return 0;
}
