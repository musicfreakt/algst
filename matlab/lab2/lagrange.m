function polynom = lagrange(x, y)
% вычисление интерполяционного полинома в форме Лагранжа
% @params
% x - массив узлов
% y - массив значений интерполируемой функции
% @return
% polynom - массив коэффициентов полинома

n = length(x);
polynom = zeros(size(x));

for i=1:n
    phi = 1;
    for j = [1:i-1, i+1:n]
        c = poly(x(j))/(x(i)-x(j));
        phi = conv(phi,c); % умножение многочленов
    end
    polynom = polynom + phi*y(i);
end

end