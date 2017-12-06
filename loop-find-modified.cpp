#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <functional>
#include <algorithm>

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
    
/* Subscript Business
     static int subscriptIncr = 0;
    string subscript = WhileLoopCallPrinter::subscript;
    bool subscriptCheck = WhileLoopCallPrinter::subscriptCheck;
*/

      const VarDecl *IncVar = Result.Nodes.getNodeAs<VarDecl>("incVarName");
      const VarDecl *InitVar = Result.Nodes.getNodeAs<VarDecl>("initVarName");
    
      string uBound;
      raw_string_ostream stream2(uBound);
      auto* upBound = Result.Nodes.getNodeAs<Expr>("upperBound");
      upBound->printPretty(stream2, NULL, PrintingPolicy(LangOptions()));
      stream2.flush();
     // cout<<uBound<<endl;

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
      cout<<"printing expression and interim"<<endl;
      cout<<expression<<endl;
      cout<<interim<<endl;

      for(int e = 0; e < (newExpression2.size() - 1); e++){ //the last item in newExpression2.size() will always be }
      vector<string> newExpression3; //remove white space
      vector<string> newExpression4; // remove =
      string finalExpression;
      body = newExpression2[e];
      // cout<<"printing e and size"<<endl;
      //cout<<e<<endl;
      //cout<<newExpression2.size()<<endl;
     // checkForStmt = newExpression2[1];


      formatExpression.split(body, ' ', newExpression3);
     

      for (int i = 0; i < newExpression3.size(); i++) { 
       if(newExpression3[i] == " "){    
           
           }else{finalExpression.append(newExpression3[i]) ;}
       
       }



       formatExpression.split(finalExpression, '=', newExpression4);


        rValue = newExpression4[0];
        rValue.erase(std::remove(rValue.begin(), rValue.end(), '\n'),
              rValue.end());
printf("-------------------rValue------------------\n");
        cout<<rValue<<endl;
       // cout<<beforeValue<<endl;

      expressionOnly = newExpression4[1];
      beforeValue = expressionOnly.at(0);



      //search the  newExpression2[0] and newExpression[1]
      if ((e==0)&&(rValue.find('for') != std::string::npos)){ //no rvalue or body to print out
          //for statement found, break the int e for loop
        check1 = true;
        checkIVal = rValue.back();
        cout<<"sum or L depends on descendant forLoop"<<endl;
        cout<<InitVar->getNameAsString()<<endl;
        cout<<lBound<<endl;
        cout<<IncVar->getNameAsString()<<endl;
        cout<<uBound<<endl;
        cout<<"breaking e for loop at 1"<<endl;
        break;
       }
        else if(rValue.find('for')!= std::string::npos){

        check2 = true;
        checkIVal = rValue.back();
        cout<<"breaking e for loop at 2"<<endl;
        break;
          }else if((e==0)&&(rValue.find('for') == std::string::npos)){ //first statement is expression with no for loop
            //not find for loop - must be an expression - print out the expression and rVal  
            string checkForOp;
            bool checkRes = false;
            bool checkProd = false;
            bool checkForReduction = true;
            checkForOp = rValue.back();
          //  cout<<checkForOp<<endl;
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
                        cout<<"II"<<endl;
                        cout<<InitVar->getNameAsString()<<endl;
                        cout<<lBound<<endl;
                        cout<<IncVar->getNameAsString()<<endl;
                        cout<<uBound<<endl;
                        cout<<expressionOnly<<endl;
                        //Subscript Business
                   /* if(subscriptCheck==true){cout<<rValue+"$"+subscript<<endl; subscriptCheck = false;}
                        else{cout<<rValue<<endl;}*/
                    cout<<rValue<<endl;
                    } 
                    // if((beforeValue.compare(rValue) == 0 )||(checkRes==1)){
                    else if((checkForReduction==1)||(checkRes==1)){
                    //They are the same
                     printf("-------------------reduction-------------------\n");
                    cout<<"sum"<<endl;
                    cout<<InitVar->getNameAsString()<<endl;
                    cout<<lBound<<endl;
                    cout<<IncVar->getNameAsString()<<endl;
                    cout<<uBound<<endl;
                    cout<<expressionOnly<<endl; 
                    //Subscript Business
                   /* if(subscriptCheck==true){cout<<rValue+"$"+subscript<<endl; subscriptCheck = false;}
                        else{cout<<rValue<<endl;}*/
                    cout<<rValue<<endl;
                  }

                  else {

                      printf("-------------------regular------------------\n");
                    //they are NOT the same
                    cout<<"L"<<endl;
                    cout<<InitVar->getNameAsString()<<endl;
                    cout<<lBound<<endl;
                     cout<<IncVar->getNameAsString()<<endl;
                    cout<<uBound<<endl;
                    cout<<expressionOnly<<endl;
                     //Subscript Business
                   /* if(subscriptCheck==true){cout<<rValue+"$"+subscript<<endl; subscriptCheck = false;}
                        else{cout<<rValue<<endl;}*/
                    cout<<rValue<<endl;
                  }



            }else if(rValue.find('for') == std::string::npos){
              cout<<"More expressions E and result R in the same loop"<<endl;
              cout<<expressionOnly<<endl;
              cout<<rValue<<endl;
            }

       /* for (int i = 3; i < newExpression3.size(); ++i) {
       
          finalExpression.append(newExpression3[i]) ;
        }

*/
    } //end of e loop

    //Determine what type of loop and number of descendant for loop in this for loop
    if (check1 == true){

        printf("-------Above is a Nested For Loop With Condition Whose immediate descendant For Loop has incVariable is\n");
        cout<<checkIVal<<endl;
  
         printf("------------Nested For Loop With Condition end------------");

    }else if((check1 == false) &&(check2 == true)){
         printf("-------Above is a Nested For Loop With Condition and Expression and forStmt Whose immediate descendant For Loop has incVariable is\n");

       cout<<checkIVal<<endl;

            printf("------------Above is a Nested For Loop With Condition and Expression and forStmt end------------");

    } else if((check1 == false)&&(check2==false)){

            printf("-------Above is a Normal For Loop With Condition and Expression-------\n");
  
      printf("------------Normal For Loop end------------");
    }



     



    
    


 //  }
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

      /*Subscript Business
      Choose from a number of subscripts
    
    string subscript;
    
    bool subscriptCheck = false;
    int subscriptIncr = AllForLoopCallPrinte::subscriptIncr;

  
    string allSubscript[10] = {"a","b", "c", "d", "s", "t", "v"};
    subscript = allSubscript[subscriptIncr];
*/


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
      //cout<<"printing interim"<<endl;
      //cout<<interim<<endl;

      for(int e = 0; e < (newExpression2.size() - 1); e++){ //the last item in newExpression2.size() will always be }
      vector<string> newExpression3; //remove white space
      vector<string> newExpression4; // remove =
      string finalExpression;
      body = newExpression2[e];
      // cout<<"printing e and size"<<endl;
      //cout<<e<<endl;
      //cout<<newExpression2.size()<<endl;
     // checkForStmt = newExpression2[1];


      formatExpression.split(body, ' ', newExpression3);
     

      for (int i = 0; i < newExpression3.size(); i++) { 
       if(newExpression3[i] == " "){    
           
           }else{finalExpression.append(newExpression3[i]) ;}
       
       }



       formatExpression.split(finalExpression, '=', newExpression4);


        rValue = newExpression4[0];
        rValue.erase(std::remove(rValue.begin(), rValue.end(), '\n'),
              rValue.end());
     printf("-------------------rValue------------------\n");
        cout<<rValue<<endl;
       // cout<<beforeValue<<endl;

      expressionOnly = newExpression4[1];
     // beforeValue = expressionOnly.at(0);




      const IfStmt *_stmt = Result.Nodes.getNodeAs<IfStmt>("IfStatmnt");
      const Expr *_expr = _stmt->getCond();


      string _expression;
      raw_string_ostream stream5(_expression);
      _expr->printPretty(stream5, NULL, PrintingPolicy(LangOptions()));
      stream5.flush();



      //search the  newExpression2[0] and newExpression[1]
      if ((e==0)&&(rValue.find('for') != std::string::npos)){ //no rvalue or body to print out i.e. expression is a for statement
          //for statement found, break the int e for loop
        check1 = true;
        checkIVal = rValue.back();
         
         printf("-------------------While-------------------\n");

         //cout<<"W$"+subscript<<endl;
         cout<<"W"<<endl;
         if(CondVar == NULL){

           if(_expression.compare("") !=0 ){
               cout << _expression<<endl;
                 } 
                 else{
                      cout<<"NULL"<<endl;
                       }


        } else{cout<<CondVar->getNameAsString()<<endl;}
         cout<<CondVal<<endl;
         //Subscript Business
        // subscriptCheck = true;

        cout<<"breaking e for loop at 1"<<endl;
        break;
       }
        else if(rValue.find('for')!= std::string::npos){
        //found for
        check2 = true;
        checkIVal = rValue.back();
        //Subscript Business
       // subscriptCheck = true;
        cout<<"breaking e for loop at 2"<<endl;
        break;
          }else if((e==0)&&(rValue.find('for') == std::string::npos)){ //first statement is expression not for loop
            //not find for loop - must be an expression - print out the expression and rVal  
                    
                    printf("-------------------While-------------------\n");
                    cout<<"W"<<endl;
                    if(CondVar == NULL){


                    if(_expression.compare("")!=0 ){
                        cout << _expression<<endl;
                    } else{
                      cout<<"NULL"<<endl;
                    }


                  } else{cout<<CondVar->getNameAsString()<<endl;}
                    cout<<CondVal<<endl;
                    cout<<expressionOnly<<endl;
                    cout<<rValue<<endl;


                    }else if(rValue.find('for') == std::string::npos){
                      cout<<"More expressions E and result R in the same loop"<<endl;
                      cout<<expressionOnly<<endl;
                      cout<<rValue<<endl;
                    }

               /* for (int i = 3; i < newExpression3.size(); ++i) {
               
                  finalExpression.append(newExpression3[i]) ;
                }

        */
          } //end of e loop

            //Determine what type of loop and number of descendant for loop in this for loop
            if (check1 == true){

                printf("-------Above is a While Loop With Condition And descendant For Loop which has incVariable is\n");
                cout<<checkIVal<<endl;
                
                 printf("------------While Loop With Condition end------------");

            }else if((check1 == false) &&(check2 == true)){
                 printf("-------Above is a While Loop With Condition and Expression and forStmt Whose immediate descendant For Loop has incVariable is\n");

               cout<<checkIVal<<endl;

                    printf("------------While Loop With Condition and Expression and forStmt end------------");

             } else if((check1 == false)&&(check2==false)){

                    printf("-------Above is a While Loop With Condition and Expression-------\n");
          
              printf("------------While Loop end------------");
               }


        if(expression.find('if') != std::string::npos){

           printf("\n\n-------This while loop contains if statement-------\n\n");
        }

       // subscriptIncr++;
    
  }
};

/*
class IfStmtCallPrinter : public MatchFinder::MatchCallback {
public :
  virtual void run(const MatchFinder::MatchResult &Result) {
   // ASTContext *context = Result.Context;


      string ifCond;
      raw_string_ostream stream1(ifCond);
      auto* expr = Result.Nodes.getNodeAs<Expr>("ifStmtCond");
      expr->printPretty(stream1, NULL, PrintingPolicy(LangOptions()));
      stream1.flush();

      string ifBody;
      raw_string_ostream stream2(ifBody);
      auto* expr2 = Result.Nodes.getNodeAs<Stmt>("ifStmtBody");
      expr2->printPretty(stream2, NULL, PrintingPolicy(LangOptions()));
      stream2.flush();

        printf("-------------------If Statement-------------------\n");
       // cout<<"L"<<endl;
        cout<<"if"<<endl;
        cout<<ifCond<<endl;
        cout<<ifBody<<endl;

      printf("-------A nested For Loop With N for loops (N = 1 to n)-------\n");
      printf("------------nested end------------");
  
 //  }
  }
};

*/

// string ifCondInside = "";

// class IfStmtCallPrinter : public MatchFinder::MatchCallback {
// public :
//   virtual void run(const MatchFinder::MatchResult &Result) {
//    // ASTContext *context = Result.Context;


//       const IfStmt *stmt = Result.Nodes.getNodeAs<IfStmt>("IfStatmnt");
//       const Expr *expr = stmt->getCond();


//       string expression;
//       raw_string_ostream stream5(expression);
//       expr->printPretty(stream5, NULL, PrintingPolicy(LangOptions()));
//       stream5.flush();

//       cout<<"If stmt found!------------------------------------"<<endl;
//       cout<<"body:" + expression<<endl; 
//       ifCondInside = expression;
//  //  }
//   }
// };



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


      //TO DO 
      //For loop with other operators
      //While Loop
      //Nested For loop
      //Multiple For loop



private:
  ForLoopPrinter ForPrinter;
  ForLoopSIPrinter ForSIPrinter;
  ForLoopCallPrinter ForCallPrinter;
  AllForLoopCallPrinter AllForPrinter;
  WhileLoopCallPrinter WhileLoopPrinter;
  //IfStmtCallPrinter IfStmtPrinter;
  
  

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




//    If stmt Matcher
  
    /*  StatementMatcher IfStmtMatcher = 
      ifStmt(

        hasCondition().bind("ifStmtCond"),

       hasBody(stmt().bind("ifStmtBody")
          ) 
       ).bind("IfStatmnt");

*/

      // StatementMatcher IfStmtMatcher = 
      // ifStmt(
      //  ).bind("IfStatmnt");


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