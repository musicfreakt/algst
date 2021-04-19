function f = sincos(x, y)
% Аппроксимация набора точек функцией вида acosx + bsinx + c 
% @param x - абсциссы точек
% @param y - ординаты точек
% @param n - степень полинома аппроксимации
% @return f - коэффициенты функции, которая аппроксимирует заданный набор точек

for i = 1:length(x)
    A(i, 1) = cos(x(i));
    A(i, 2) = sin(x(i));
    A(i, 3) = 1;
end

A_T = transpose(A);

f = inv(A_T*A)*A_T*y;
end