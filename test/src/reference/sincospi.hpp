#pragma once

#include <cmath>

namespace sincospi
{
/* Writes result sine result sin(πa) to the location pointed to by sp
   Writes result cosine result cos(πa) to the location pointed to by cp

   In exhaustive testing, the maximum error in sine results was 0.96677 ulp,
   the maximum error in cosine results was 0.96563 ulp, meaning results are
   faithfully rounded.

   Copied from: https://stackoverflow.com/questions/42792939/implementation-of-sinpi-and-cospi-using-standard-c-math-library
*/
void sincospif (float a, float *sp, float *cp)
{
    float az, t, c, r, s;
    int32_t i;

    az = a * 0.0f; // must be evaluated with IEEE-754 semantics
    /* for |a| > 2**24, cospi(a) = 1.0f, but cospi(Inf) = NaN */
    a = (fabsf (a) < 0x1.0p24f) ? a : az;
    r = nearbyintf (a + a); // must use IEEE-754 "to nearest" rounding
    i = (int32_t)r;
    t = fmaf (-0.5f, r, a);
    /* compute core approximations */
    s = t * t;
    /* Approximate cos(pi*x) for x in [-0.25,0.25] */
    r =              0x1.d9e000p-3f;
    r = fmaf (r, s, -0x1.55c400p+0f);
    r = fmaf (r, s,  0x1.03c1cep+2f);
    r = fmaf (r, s, -0x1.3bd3ccp+2f);
    c = fmaf (r, s,  0x1.000000p+0f);
    /* Approximate sin(pi*x) for x in [-0.25,0.25] */
    r =             -0x1.310000p-1f;
    r = fmaf (r, s,  0x1.46737ep+1f);
    r = fmaf (r, s, -0x1.4abbfep+2f);
    r = (t * s) * r;
    s = fmaf (t, 0x1.921fb6p+1f, r);
    if (i & 2) {
        s = 0.0f - s; // must be evaluated with IEEE-754 semantics
        c = 0.0f - c; // must be evaluated with IEEE-754 semantics
    }
    if (i & 1) {
        t = 0.0f - s; // must be evaluated with IEEE-754 semantics
        s = c;
        c = t;
    }
    /* IEEE-754: sinPi(+n) is +0 and sinPi(-n) is -0 for positive integers n */
    if (a == floorf (a)) s = az;
    *sp = s;
    *cp = c;
}

float sin2pi (float x)
{
    float s, c;
    sincospif (2.0f * x, &s, &c);
    return s;
}

float cos2pi (float x)
{
    float s, c;
    sincospif (2.0f * x, &s, &c);
    return c;
}
}
