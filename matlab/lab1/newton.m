function polynom = newton(x, y)
% вычисление интерполяционного полинома в форме Ньютона
% @params
% x - массив узлов
% y - массив значений интерполируемой функции
% d(k, j) - равносильно разделенной разности f(x(k),x(k+1),...,x(k+j))
% @return
% polynom - массив коэффициентов полинома

n = length(x);
d = zeros(n-1, n-1);
a = zeros(n-1);
polynom = zeros(size(x));

% разделенная разность для двух переменных
for k = 1 : n - 1
   d(k, 1) = (y(k+1) - y(k))/(x(k+1) - x(k));
end

% разделенная разность для оставшихся
for j = 2 : n - 1
   for k = 1 : n - j % d(k, j) - состоит из j+1 переменных
      d(k, j) = (d(k+1, j - 1) - d(k, j - 1))/(x(k+j) - x(k));
   end
end

for j = 2 : n
   a(j) = d(1, j-1);
end

polynom(n) = y(1);
for j = 2 : n
    r = x(1:j-1);
    % Так как мы складываем коэффициенты полиномов - дополняем вектор
    % слагаемого нулями
    len = length(a(j) * poly(r));
    polynom = polynom + [zeros(1, length(polynom) - len), a(j) * poly(r)];
end
