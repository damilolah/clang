#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <functional>
#include <algorithm>  
#include <list>       
 

#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/CommandLine.h"
#include "clang/AST/Expr.h"

using namespace std;
using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::driver;
using namespace clang::tooling;
using namespace llvm;

// Subscript Business
    static int subscriptIncr = 0;
    static string subscript = "";
    static bool subscriptCheck = false;
    static string allSubscript[10] = {"s", "t", "a","b", "c", "d",  "v"};
    static bool multipleWhile = false;
    static int sumOfWhile = 0;



// Apply a custom category to all command-line options so that they are the
// only ones displayed.
static llvm::cl::OptionCategory MyToolCategory("my-tool options");

// CommonOptionsParser declares HelpMessage with a description of the common
// command-line options related to the compilation database and input files.
// It's nice to have this help message in all tools.
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

// A help message for this specific tool can be added afterwards.
static cl::extrahelp MoreHelp("\nMore help text...");




class FormatExpression
{
  public:
    //Method Split String
    void split(const string& s, char c,
           vector<string>& v) {
   string::size_type i = 0;
   string::size_type j = s.find(c);

   while (j != string::npos) {
      v.push_back(s.substr(i, j-i));
      i = ++j;
      j = s.find(c, j);

      if (j == string::npos)
         v.push_back(s.substr(i, s.length()));
   }
}

   //Method Operand Folding 
   void operandFolding(string& str,  string& answer){
    // string str = "x = sin(a[i]+b[j]) + b";  //sythen_ab(i,j);
    int numPar = 0;
    int numKuo = 0;
    size_t loc = 0;
    if(str.find("sin") != std::string::npos){
         loc = str.find("sin");
    }else if(str.find("cos") != std::string::npos){
        loc = str.find("cos");
    }else if(str.find("tan") != std::string::npos){
         loc = str.find("tan");
    }else{
     // answer = str;
    }
    string result = "synthetic_";
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
   
    answer = str.substr(0,loc) + result +'['+ subscript + ']' + str.substr(loca+1);
    
   }


};




    
class ForLoopPrinter : public MatchFinder::MatchCallback {
public :
  virtual void run(const MatchFinder::MatchResult &Result) {
    printf("------- A For Loop ----------\n");
    if (const ForStmt *FS = Result.Nodes.getNodeAs<clang::ForStmt>("forLoop"))
      FS->dump();
  }
};

class ForLoopSIPrinter : public MatchFinder::MatchCallback {
public :
  virtual void run(const MatchFinder::MatchResult &Result) {
    printf("------- A For Loop w/ Single Init----------\n");
    if (const ForStmt *FS = Result.Nodes.getNodeAs<clang::ForStmt>("forLoopSI"))
      FS->dump();
  }
};

class ForLoopCallPrinter : public MatchFinder::MatchCallback {
public :
  virtual void run(const MatchFinder::MatchResult &Result) {
    printf("------- A For Loop w/ Calls----------\n");
    if (const ForStmt *FS = Result.Nodes.getNodeAs<clang::ForStmt>("callInLoop"))
      FS->dump();
  }
};


static bool areSameVariable(const ValueDecl *First, const ValueDecl *Second) {
  return First && Second && First->getCanonicalDecl() == Second->getCanonicalDecl();

}


class AllForLoopCallPrinter : public MatchFinder::MatchCallback {
public :
//WhileLoopCallPrinter whileprinter;
  virtual void run(const MatchFinder::MatchResult &Result) {
  //  ASTContext *context = Result.Context;

   
    string rValue;
    string body;
    string interim;
    string finalExpression;
    string beforeValue;
    string expressionOnly;
    string checkForStmt;
    string temp1;
    string temp2;
    string temp3;
    string mySubscript = "";
    cout<<"\n\n"<<endl;
    

      const VarDecl *IncVar = Result.Nodes.getNodeAs<VarDecl>("incVarName");
      const VarDecl *InitVar = Result.Nodes.getNodeAs<VarDecl>("initVarName");
    
      string uBound;
      raw_string_ostream stream2(uBound);
      auto* upBound = Result.Nodes.getNodeAs<Expr>("upperBound");
      upBound->printPretty(stream2, NULL, PrintingPolicy(LangOptions()));
      stream2.flush();
   

      string lBound;
      raw_string_ostream stream3(lBound);
      auto* loBound = Result.Nodes.getNodeAs<IntegerLiteral>("lowerBound");
      loBound->printPretty(stream3, NULL, PrintingPolicy(LangOptions()));
      stream3.flush();

      string expression;
      raw_string_ostream stream4(expression);
      auto* expr = Result.Nodes.getNodeAs<Stmt>("forLoopBody");
      expr->printPretty(stream4, NULL, PrintingPolicy(LangOptions()));
      stream4.flush();

      //Format Expression
       vector<string> newExpression;
       vector<string> newExpression2; //remove semicolon
       vector<string> getFirstChar;
       
      FormatExpression formatExpression;
      formatExpression.split(expression, '{', newExpression);
      interim = newExpression[1];
      formatExpression.split(interim, ';', newExpression2);
      bool check1 = false;
      bool check2 = false;
      string checkIVal;
     

      for(int e = 0; e < (newExpression2.size() - 1); e++){ //the last item in newExpression2.size() will always be }
      vector<string> newExpression3; //remove white space
      vector<string> newExpression4; // remove =
      string finalExpression;
      body = newExpression2[e];
    

      formatExpression.split(body, ' ', newExpression3);
     

      for (int i = 0; i < newExpression3.size(); i++) { 
       if(newExpression3[i] == " "){    
           
           }else{finalExpression.append(newExpression3[i]) ;}
       
       }



       formatExpression.split(finalExpression, '=', newExpression4);


        rValue = newExpression4[0];
        rValue.erase(std::remove(rValue.begin(), rValue.end(), '\n'),
              rValue.end());

      expressionOnly = newExpression4[1];
      beforeValue = expressionOnly.at(0);

      //Check for sine and cosine
                     if((expressionOnly.find('sin') != std::string::npos)|| (expressionOnly.find('cos') != std::string::npos)){
                        string result;
                        formatExpression.operandFolding(expressionOnly, result);
                        expressionOnly = result;
                      }
                      else{} 

      //search the  newExpression2[0] and newExpression[1]
      if ((e==0)&&(rValue.find('for') != std::string::npos)){ //no rvalue or body to print out
          //for statement found, break the int e for loop
        check1 = true;
        checkIVal = rValue.back();
        cout<<"Σ"+InitVar->getNameAsString()+"∫"+lBound+","+uBound+"∫"<<endl;
       /* 
        cout<<InitVar->getNameAsString()<<endl;
        cout<<lBound<<endl;
        cout<<IncVar->getNameAsString()<<endl;
        cout<<uBound<<endl;
        */
        
        break;
       }
        else if(rValue.find('for')!= std::string::npos){

        check2 = true;
        checkIVal = rValue.back();
      
        break;
          }else if((e==0)&&(rValue.find('for') == std::string::npos)){ //first statement is expression with no for loop
            //not find for loop - must be an expression - print out the expression and rVal  
            string checkForOp;
            bool checkRes = false;
            bool checkProd = false;
            bool checkForReduction = true;
            checkForOp = rValue.back();
          
            if ((checkForOp.find('+') != std::string::npos) || (checkForOp.find('-') != std::string::npos)||(checkForOp.find('/') != std::string::npos)){
              //Check if LHS contains an operator; if true this is a reduction loop
                 checkRes = true;
                 rValue.erase(rValue.end() - 1);
               }
               //Check for product - Extra feature for Bonus points
               else if(checkForOp.find('*') != std::string::npos) {
                     checkProd = true;
                    } 
                    //Split expressionOnly to check for x[i] = x[i] + ...

               else{
               
                
                    //compare size and value of rValue and beforeValue 
                  if(expressionOnly.size() >= rValue.size()){
                     for(int y = 0; y<rValue.size(); y++){  
                           temp1 = expressionOnly.at(y);
                           temp2 = rValue.at(y);
                        if(temp1.compare(temp2) != 0 ){
                            //They are Not the same, then regular

                            checkForReduction = false;
                        }else{   checkForReduction = true;}
                    }
                  }else{
                   checkForReduction = false;
                  }

                }
                // Check what operator is after x[i] in x[i] = x[i]
                if((checkForReduction==true)&&(expressionOnly.size() >= rValue.size())){
                 //  beforeValue = expressionOnly.at(0);
                  //string temp3 = expressionOnly.at(rValue.size());

                   temp3 = expressionOnly.at(rValue.size());

                  if(temp3.compare("*")==0 ){
                    checkProd = true;
                  }else if(temp3.compare("+")==0 ){
                    checkRes = true;
                  }else{//check for x[i]=x[i]j + situation
                    checkForReduction = false;
                  }
                }
                      //cout<<checkRes<<endl;

                    if(checkProd==1){
                       printf("-------------------product-------------------\n");
                        cout<<"\u03A0" + InitVar->getNameAsString() +"∫"+lBound+","+uBound+"∫" <<endl;
                        /*cout<<InitVar->getNameAsString()<<endl;
                        cout<<lBound<<endl;
                        cout<<IncVar->getNameAsString()<<endl;
                        cout<<uBound<<endl;
                  */

                        //Subscript Business
                   if(subscriptCheck==true){
                    if(multipleWhile==true){
                      for(int m = 0; m<sumOfWhile; m++){
                        mySubscript.append("$");
                        mySubscript.append(allSubscript[m]);
                      }
                      cout<<"("+expressionOnly+mySubscript+")" + "="+ rValue+mySubscript<<endl;
                     // cout<<rValue+mySubscript<<endl;
                    }else{
                         cout<<"("+expressionOnly+"$"+subscript+")" + "="+ rValue+"$"+subscript<<endl;
                         // cout<<rValue+"$"+subscript<<endl;
                      }
                      subscriptCheck = false;
                    }

                      else{cout<<"("+expressionOnly+")" + "="+ rValue<<endl;
                      //cout<<rValue<<endl;
                    }
                    
                    } 
                    // if((beforeValue.compare(rValue) == 0 )||(checkRes==1)){
                    else if((checkForReduction==1)||(checkRes==1)){
                    //They are the same
                     //printf("-------------------reduction-------------------\n");
                      cout<<"\u03A3"+InitVar->getNameAsString()+"∫"+lBound+","+uBound+"∫"<<endl;
                       /* 
                        cout<<InitVar->getNameAsString()<<endl;
                        cout<<lBound<<endl;
                        cout<<IncVar->getNameAsString()<<endl;
                        cout<<uBound<<endl;
                        */
                   
                    //Subscript Business
                  if(subscriptCheck==true){
                    if(multipleWhile==true){
                      for(int m = 0; m<sumOfWhile; m++){
                        mySubscript.append("$");
                        mySubscript.append(allSubscript[m]);
                      }
                      cout<<"("+expressionOnly+mySubscript+")" + "="+ rValue+mySubscript <<endl;
                      //cout<<rValue+mySubscript<<endl;
                    }else{
                      cout<<"("+expressionOnly+"$"+subscript+")" + "="+ rValue+"$"+subscript<<endl;
                      //cout<<rValue+"$"+subscript<<endl;
                    }
                      subscriptCheck = false;
                    }

                      else{cout<<"("+expressionOnly+")"  + "="+ rValue <<endl;
                      //cout<<rValue<<endl;
                    }
                    
                  }

                  else {

                      //printf("-------------------regular------------------\n");
                    //they are NOT the same
                      cout<<"\u0393"+InitVar->getNameAsString()+"∫"+lBound+","+uBound+"∫"<<endl;
                       /* 
                        cout<<InitVar->getNameAsString()<<endl;
                        cout<<lBound<<endl;
                        cout<<IncVar->getNameAsString()<<endl;
                        cout<<uBound<<endl;
                        */
                  

                     //Subscript Business
                      if(subscriptCheck==true){
                    if(multipleWhile==true){
                      for(int m = 0; m<sumOfWhile; m++){
                        mySubscript.append("$");
                        mySubscript.append(allSubscript[m]);
                      }
                      cout<<"("+expressionOnly+mySubscript+")" + "="+ rValue+mySubscript<<endl;
                     // cout<<rValue+mySubscript<<endl;
                    }else{
                      cout<<"("+expressionOnly+"$"+subscript+")" + "="+ rValue+"$"+subscript<<endl;
                      //cout<<rValue+"$"+subscript<<endl;
                    }
                      subscriptCheck = false;
                    }

                      else{cout<<"("+expressionOnly+")" + "="+ rValue<<endl;
                      //cout<<rValue<<endl;
                    }
                     

                  }



            }else if(rValue.find('for') == std::string::npos){
             // cout<<"More expressions E and result R in the same loop"<<endl;
              cout<<"("+expressionOnly+")"+ "="+ rValue<<endl;
              //cout<<rValue<<endl;
            }

    } //end of e loop

    //Determine what type of loop and number of descendant for loop in this for loop
    if (check1 == true){

       // printf("-------Above is a Nested For Loop With Condition Whose immediate descendant For Loop has incVariable is\n");
        printf("--Parent of for loop with increment variable--");
        cout<<checkIVal<<endl;
  
        // printf("------------Nested For Loop With Condition end------------");

    }else if((check1 == false) &&(check2 == true)){
         //printf("-------Above is a Nested For Loop With Condition and Expression and forStmt Whose immediate descendant For Loop has incVariable is\n");
        printf("--Parent of for loop with increment variable--");
               cout<<checkIVal<<endl;

           // printf("------------Above is a Nested For Loop With Condition and Expression and forStmt end------------");

    } else if((check1 == false)&&(check2==false)){

          //  printf("-------Above is a Normal For Loop With Condition and Expression-------\n");
  
      //printf("------------Normal For Loop end------------");
    }


    multipleWhile=false;
     
  }
};




class WhileLoopCallPrinter : public MatchFinder::MatchCallback {
public :
//AllForLoopCallPrinter forPrinter;
  virtual void run(const MatchFinder::MatchResult &Result) {
   // ASTContext *context = Result.Context;

    string rValue;
    string body;
    string interim;
    string finalExpression;
    string beforeValue;
    string expressionOnly;
    string checkForStmt;
    cout<<"\n\n"<<endl;
 
    const VarDecl *CondVar = Result.Nodes.getNodeAs<VarDecl>("condVarName");

       string CondVal;
      raw_string_ostream stream(CondVal);

      auto* condVal = Result.Nodes.getNodeAs<Expr>("condValue");
      condVal->printPretty(stream, NULL, PrintingPolicy(LangOptions()));
      stream.flush();


    
      string expression;
      raw_string_ostream stream2(expression);
      auto* expr = Result.Nodes.getNodeAs<Stmt>("whileLoopBody");
      expr->printPretty(stream2, NULL, PrintingPolicy(LangOptions()));
      stream2.flush();

        //Format Expression
       vector<string> newExpression;
       vector<string> newExpression2; //remove semicolon
       vector<string> getFirstChar;
       
      FormatExpression formatExpression;
      formatExpression.split(expression, '{', newExpression);
      interim = newExpression[1];
      formatExpression.split(interim, ';', newExpression2);
      bool check1 = false;
      bool check2 = false;
      string checkIVal;
      
      for(int e = 0; e < (newExpression2.size() - 1); e++){ //the last item in newExpression2.size() will always be }
      vector<string> newExpression3; //remove white space
      vector<string> newExpression4; // remove =
      string finalExpression;
      body = newExpression2[e];
      
      //subscribt business
       if(body.find('while') != std::string::npos){

           printf("\n\n-------This while loop contains while statement-------\n\n");
           if(multipleWhile==false){//means we've not counted multiple while before
            
            sumOfWhile++;
          }
        }else{}

      formatExpression.split(body, ' ', newExpression3);
     

      for (int i = 0; i < newExpression3.size(); i++) { 
       if(newExpression3[i] == " "){    
           
           }else{finalExpression.append(newExpression3[i]) ;}
       
       }



       formatExpression.split(finalExpression, '=', newExpression4);


        rValue = newExpression4[0];
        rValue.erase(std::remove(rValue.begin(), rValue.end(), '\n'),
              rValue.end());

      expressionOnly = newExpression4[1];

    //Check for sine and cosine
                     if((expressionOnly.find('sin') != std::string::npos)|| (expressionOnly.find('cos') != std::string::npos)){
                        string result;
                        formatExpression.operandFolding(expressionOnly, result);
                        expressionOnly = result;
                      }
                      else{} 


      const IfStmt *_stmt = Result.Nodes.getNodeAs<IfStmt>("IfStatmnt");
      const Expr *_expr = _stmt->getCond();


      string _expression;
      raw_string_ostream stream5(_expression);
      _expr->printPretty(stream5, NULL, PrintingPolicy(LangOptions()));
      stream5.flush();


      //Subscript Business
      //Choose from a number of subscripts
      subscript = allSubscript[subscriptIncr];
      

      //search the  newExpression2[0] and newExpression[1]
      if ((e==0)&&(rValue.find('for') != std::string::npos)){ //no rvalue or body to print out i.e. expression is a for statement
          //for statement found, break the int e for loop
        check1 = true;
        checkIVal = rValue.back();
         
       //  printf("-------------------While-------------------\n");

         
         if(CondVar == NULL){

                 if(_expression.compare("") !=0 ){
                  cout<<"\u03C8$"+subscript + _expression<<endl; //I just removed condVal
                   //  cout << _expression<<endl;
                    // cout<<CondVal<<endl;
                       } 
                       else{
                        cout<<"\u03C8$"+subscript+ "NULL" + CondVal<<endl;
                           // cout<<"NULL"<<endl;
                            //cout<<CondVal<<endl;
                             }

              } else{
                cout<<"\u03C8$"+subscript+ CondVar->getNameAsString()+CondVal<<endl;
              //  cout<<CondVar->getNameAsString()<<endl;
              //cout<<CondVal<<endl;
              }
         
         //Subscript Business
         subscriptCheck = true;

        //cout<<"breaking e for loop at 1"<<endl;
        break;
       }
        else if(rValue.find('for')!= std::string::npos){
        //found for
        check2 = true;
        checkIVal = rValue.back();
        //Subscript Business
        subscriptCheck = true;
        //cout<<"breaking e for loop at 2"<<endl;
        break;
          }else if((e==0)&&(rValue.find('for') == std::string::npos)){ //first statement is expression not for loop
            //not find for loop - must be an expression - print out the expression and rVal  
                    
                   // printf("-------------------While-------------------\n");
                    //cout<<"W"<<endl;
              if(CondVar == NULL){

                 if(_expression.compare("") !=0 ){
                  //Ψ == \u03C8
                  cout<<"\u03C8$"+subscript + _expression <<endl; //I just removed conVal
                   //  cout << _expression<<endl;
                    // cout<<CondVal<<endl;
                       } 
                       else{
                        cout<<"\u03C8$"+subscript+ "NULL" + CondVal<<endl;
                           // cout<<"NULL"<<endl;
                            //cout<<CondVal<<endl;
                             }

              } else{
                cout<<"\u03C8$"+subscript+ CondVar->getNameAsString()+CondVal<<endl;
              //  cout<<CondVar->getNameAsString()<<endl;
              //cout<<CondVal<<endl;
              }
                    //subscript business
                    cout<<"∫"+expressionOnly+ "="+ rValue+"$"+subscript <<endl;
                    //cout<<rValue+"$"+subscript<<endl;


                    }else if(rValue.find('for') == std::string::npos){
                      //cout<<"More expressions E and result R in the same loop"<<endl;
                      cout<<"∫"+expressionOnly + "="+  rValue+"$"+subscript<<endl;
                      //cout<<rValue+"$"+subscript<<endl;
                    }

      
          } //end of e loop

            //Determine what type of loop and number of descendant for loop in this for loop
            if (check1 == true){

               // printf("-------Above is a While Loop With Condition And descendant For Loop which has incVariable is\n");
                printf("--Parent of for loop with increment variable--");
                cout<<checkIVal<<endl;
                
                 //printf("------------While Loop With Condition end------------");

            }else if((check1 == false) &&(check2 == true)){
                 //printf("-------Above is a While Loop With Condition and Expression and forStmt Whose immediate descendant For Loop has incVariable is\n");
                printf("--Parent of for loop with increment variable--");
               cout<<checkIVal<<endl;

                    //printf("------------While Loop With Condition and Expression and forStmt end------------");

             } else if((check1 == false)&&(check2==false)){

                    //printf("-------Above is a While Loop With Condition and Expression-------\n");
          
              //printf("------------While Loop end------------");
               }


        if(expression.find('if') != std::string::npos){

           //printf("\n\n-------This while loop contains if statement-------\n\n");
        }

       subscriptIncr++; 
       if(interim.find('while') != std::string::npos){
       multipleWhile = true;
     }
    
  }
};



class MyASTConsumer: public ASTConsumer {
public:
  MyASTConsumer () {
    /*    ForLoopMatcher = forStmt().bind("forLoop");
    ForLoopMatcher_withSI =
      forStmt(hasLoopInit(declStmt(hasSingleDecl(varDecl(hasInitializer(integerLiteral(equals(0)))))))).bind("forLoopSI");
    ForLoopMatcher_withCall =
      forStmt(hasBody(compoundStmt(has(callExpr())))).bind("callInLoop");
    */
    //Finder.addMatcher(ForLoopMatcher, &ForPrinter);
    //Finder.addMatcher(ForLoopMatcher_withSI, &ForSIPrinter);
    //Finder.addMatcher(ForLoopMatcher_withCall, &ForCallPrinter);
    Finder.addMatcher(AllForLoopMatcher, &AllForPrinter);
    Finder.addMatcher(WhileLoopMatcher, &WhileLoopPrinter);
    //Finder.addMatcher(IfStmtMatcher, &IfStmtPrinter);

    
  }
  void HandleTranslationUnit(ASTContext &Context) override {
    // Run the matchers when we have the whole TU parsed.
    Finder.matchAST(Context);
  }



private:
  ForLoopPrinter ForPrinter;
  ForLoopSIPrinter ForSIPrinter;
  ForLoopCallPrinter ForCallPrinter;
  AllForLoopCallPrinter AllForPrinter;
  WhileLoopCallPrinter WhileLoopPrinter;
  
  

  StatementMatcher ForLoopMatcher= forStmt(hasLoopInit(declStmt(hasSingleDecl(varDecl(hasName("i")))))).bind("forLoop");
  StatementMatcher ForLoopMatcher_withSI=
      forStmt(hasLoopInit(declStmt(hasSingleDecl(varDecl(hasInitializer(integerLiteral(equals(0)))))))).bind("forLoopSI");
  StatementMatcher ForLoopMatcher_withCall=
      forStmt(hasBody(compoundStmt(has(callExpr())))).bind("callInLoop");


  StatementMatcher AllForLoopMatcher = 
      forStmt(

        

        hasLoopInit(anyOf(declStmt(
          hasSingleDecl(varDecl(hasInitializer(integerLiteral().bind("lowerBound")))
            .bind("initVarName")))   ,  binaryOperator(
          hasOperatorName("="),
          hasLHS(ignoringParenImpCasts(declRefExpr(
            to(varDecl(hasType(isInteger())).bind("initVarName"))))),
          hasRHS(expr(hasType(isInteger())).bind("lowerBound"))) )    ),

        hasIncrement(unaryOperator(
          hasOperatorName("++"),
          hasUnaryOperand(declRefExpr(
            to(varDecl(hasType(isInteger())).bind("incVarName")))))),
        
        hasCondition(binaryOperator(
          hasOperatorName("<"),
          hasLHS(ignoringParenImpCasts(declRefExpr(
            to(varDecl(hasType(isInteger())).bind("condVarName"))))),
          hasRHS(expr(hasType(isInteger())).bind("upperBound"))))
  ,
       hasBody(stmt().bind("forLoopBody")
          )
    ).bind("CommonLoop");




//    While Loop Matcher
      StatementMatcher WhileLoopMatcher = 
      whileStmt(

        anyOf( hasCondition(binaryOperator(
          hasOperatorName("<"),
          hasLHS(ignoringParenImpCasts(declRefExpr(
            to(varDecl(hasType(isInteger())).bind("condVarName"))))),
          hasRHS(expr(hasType(isInteger())).bind("condValue")))), hasCondition(stmt().bind("condValue"))),



       hasBody(
        anyOf(

        stmt(has(
      ifStmt(
       ).bind("IfStatmnt")
      )).bind("whileLoopBody")
          ,
          stmt().bind("whileLoopBody")
          )
          )
       

    ).bind("WhileLoop");



  MatchFinder Finder;
};



// For each source file provided to the tool, a new FrontendAction is created.
class MyFrontendAction : public ASTFrontendAction {
public:
  MyFrontendAction() {}
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
             StringRef file) override {
    return llvm::make_unique<MyASTConsumer>();
  }
};


int main(int argc, const char **argv) {
  CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
  ClangTool Tool(OptionsParser.getCompilations(),
     OptionsParser.getSourcePathList());

  return Tool.run(newFrontendActionFactory<MyFrontendAction>().get());
}