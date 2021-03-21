function f = chebyshev(x, n)
    f(n) = 2x*f(n-2) - f(n-1);
end