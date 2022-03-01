float Tan(float xRadians)
{
    //if oTanX is undefined theoritically we just have to debug I guess :)
    float oTanX = xRadians + ( xRadians * xRadians * xRadians / 3 ) + ( 2 * xRadians * xRadians * xRadians * xRadians * xRadians / 15);
    return oTanX;
}

float Sin(float xRadians)
{
    float oSinX = xRadians - ((xRadians * xRadians * xRadians) / 6) + ((xRadians * xRadians * xRadians * xRadians * xRadians) / 120);
    return oSinX;
}

float Cos(float xRadians)
{
    float oCosX = 1 - ((xRadians * xRadians) / 2) + ((xRadians * xRadians * xRadians * xRadians) / 24); 
    return oCosX;
}