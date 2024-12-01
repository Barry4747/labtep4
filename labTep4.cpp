#include <iostream>
#include "CTree.h"
#include "Serializer.h"

CResult<double, CError> eDivide(double dDividend, double dDivisor)
{
    if (dDivisor == 0)
    {
        return CResult<double, CError>::cFail(new CError("cannot divide by zero"));
    }
    return CResult<double, CError>::cOk(dDividend / dDivisor);
}

CResult<CTree*, CError> createTree(string str) {
    CTree *tree = new CTree();
    CResult<void, CError> result = tree->crEnter(str);
    if (result.bIsSuccess())
    {
        return CResult<CTree*, CError>::cOk(tree);
    }
    else {
        return CResult<CTree*, CError>::cFail(result.vGetErrors());
    }
}


int main()
{
    CResult<double, CError> result1(eDivide(4, 0));

    CResult<double, CError> result2(result1);

    result2 = eDivide(3, 1);

    result1 = result2;

    result2 = eDivide(4, 0);


    string str = "+ a a+ a 1";
    CResult<CTree*, CError> cr_tree(createTree(str));

   
    Serializer<CTree*>::saveResult(cr_tree);
    Serializer<double>::saveResult(result1);
    Serializer<double>::saveResult(result2);
    
}
