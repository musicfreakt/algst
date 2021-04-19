function p = polynom(x, y, n)
% Аппроксимация набора точек полиномами
% @param x - абсциссы точек
% @param y - ординаты точек
% @param n - степень полинома аппроксимации
% @return p - полином, аппроксимирующий заданный набор точек

for i = 1:length(x)
    for j = n:-1:0
        A(i, n-j+1) = x(i)^j;
    end
end

A_T = transpose(A);

p = inv(A_T*A)*A_T*y;
end