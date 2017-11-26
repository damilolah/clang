#include <string>
#include <iostream>
#include <sstream>

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


class CommonPlusLoopCallPrinter : public MatchFinder::MatchCallback {
public :
  virtual void run(const MatchFinder::MatchResult &Result) {

    const VarDecl *IncVar = Result.Nodes.getNodeAs<VarDecl>("incVarName");
  //  const VarDecl *CondVar = Result.Nodes.getNodeAs<VarDecl>("condVarName");
  //  const VarDecl *InitVar = Result.Nodes.getNodeAs<VarDecl>("initVarName");
    // if (!areSameVariable(IncVar, CondVar) || !areSameVariable(IncVar, InitVar))
    //   return;

    const VarDecl *rValue = Result.Nodes.getNodeAs<VarDecl>("resultValue");
    const VarDecl *beforeValue = Result.Nodes.getNodeAs<VarDecl>("valuebeforePlus"); 
    const BinaryOperator *op = Result.Nodes.getNodeAs<BinaryOperator>("CommonPlus");
  //  const Expr * vInformula = Result.Nodes.getNodeAs<Expr>("valueInFormula");
   // const FloatingLiteral *IL = dyn_cast<FloatingLiteral>("upperBound");
   //  double val = IL->getValue().convertToDouble();
   // cout<<val<<endl; 
   // int ival = (int) val;
//    }

    string vIn = "";
    raw_string_ostream stream(vIn);

    auto* statement = Result.Nodes.getNodeAs<Expr>("valueInFormula");
    statement->printPretty(stream, NULL, PrintingPolicy(LangOptions()));

    stream.flush();

    //cout<<vIn<<endl;

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

    if(!areSameVariable(rValue, beforeValue))
      printf("-------------------different------------------\n");

    else {
      printf("-------------------reduction-------------------\n");
      cout<<"L"<<endl;
      cout<<"sum"<<endl;
      cout<<vIn<<endl;
      cout<<lBound<<endl;
      cout<<uBound<<endl;
      cout<<"="<<endl;
      cout<<rValue->getNameAsString()<<endl;
      //cout<<IncVar->getNameAsString()<<endl;
   //   count<<valueInformula<<endl;
    }


    printf("-------A common plus For Loop-------\n");
    // const BinaryOperator *op = Result.Nodes.getNodeAs<BinaryOperator>("CommonPlus");
    // int c_value = 0;
    // FloatingLiteral *IFE = dyn_cast<FloatingLiteral>(op->getRHS());
    // c_value = (int) IFE->getValue().convertToDouble();
    // printf("%d\n",c_value);
    printf("------------Common Plus end------------");
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
    Finder.addMatcher(ForLoopMatcher, &ForPrinter);
    Finder.addMatcher(ForLoopMatcher_withSI, &ForSIPrinter);
    Finder.addMatcher(ForLoopMatcher_withCall, &ForCallPrinter);
    Finder.addMatcher(CommonPlusMatcher, &CommonPlusPrinter);
  }
  void HandleTranslationUnit(ASTContext &Context) override {
    // Run the matchers when we have the whole TU parsed.
    Finder.matchAST(Context);
  }


private:
  ForLoopPrinter ForPrinter;
  ForLoopSIPrinter ForSIPrinter;
  ForLoopCallPrinter ForCallPrinter;
  CommonPlusLoopCallPrinter CommonPlusPrinter;
  StatementMatcher ForLoopMatcher= forStmt(hasLoopInit(declStmt(hasSingleDecl(varDecl(hasName("i")))))).bind("forLoop");
  StatementMatcher ForLoopMatcher_withSI=
      forStmt(hasLoopInit(declStmt(hasSingleDecl(varDecl(hasInitializer(integerLiteral(equals(0)))))))).bind("forLoopSI");
  StatementMatcher ForLoopMatcher_withCall=
      forStmt(hasBody(compoundStmt(has(callExpr())))).bind("callInLoop");
  StatementMatcher CommonPlusMatcher = 
      forStmt(
        hasLoopInit(declStmt(
          hasSingleDecl(varDecl(hasInitializer(integerLiteral().bind("lowerBound")))
            .bind("initVarName")))),
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
        hasBody(
          compoundStmt(
      has(
            binaryOperator(
              hasOperatorName("="),
              hasLHS(ignoringParenImpCasts(declRefExpr(
                to(varDecl().bind("resultValue"))))) ,
              hasRHS(
                binaryOperator(
                  hasOperatorName("+"),
                  hasLHS(ignoringParenImpCasts(declRefExpr(
                    to(varDecl().bind("valuebeforePlus")))))
     ,
                  hasRHS(expr(hasType(isInteger())).bind("valueInFormula"))).bind("CommonPlus")
                  )
                  ).bind("CommonEqual")
                  )
            )
          )
    ).bind("CommonLoop");

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
