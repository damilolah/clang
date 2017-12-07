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


#include <iostream>
#include <iostream>   
#include <list>       // std::cout
#include <string>   

using namespace std;


int main()
{
    string str = "x = sin(a[i]+b[j]) + b";  //sythen_ab(i,j);
    int numPar = 0;
    int numKuo = 0;
    size_t loc = str.find("sin");
    string result = "syntheic_";
  //size_t loc = str.find("cos");
  //string result = "cosblalbal_";
    string subscript = "";
    bool which = true;
    int loca = 0;
    for(int i = loc + 3; i < str.length(); i++) {
        if(str[i] == ' '||str[i] == '+'){
            continue;
        }else if(str[i] == '(') {
            numPar++;
        }else if (str[i] == ')'){
            numPar--;
            if(numPar == 0)
            loca = i; 
            break;
        }else if(str[i] == '[') {
            numKuo++;
            which = false;
        }else if(str[i] == ']') {
            numKuo--;
            if(numKuo == 0)
            which = true;
        }else if (which) {
            result += str[i];
        }else {
            subscript += str[i];
        }
            
    }
    cout<< "result is "<<endl;
    string answer = str.substr(0,loc) + result +'['+ subscript + ']' + str.substr(loca+1);
    cout<<answer<<endl;
}

