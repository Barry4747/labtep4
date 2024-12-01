#include <iostream>
#include "CResult.h"

CResult<double, CError> eDivide(double dDividend, double dDivisor)
{
    if (dDivisor == 0)
    {
        return CResult<double, CError>::cFail(new CError("cannot divide by zero"));
    }
    return CResult<double, CError>::cOk(dDividend / dDivisor);
}


int main()
{
    CResult<double, CError> result1(eDivide(4, 0));

    CResult<double, CError> result2(result1);

    result2 = eDivide(3, 1);

    cout << result1.cGetValue()<<endl;

    result1 = result2;

    result2 = eDivide(4, 0);

    cout << result2.vGetErrors()[0]->sGetError();
}
