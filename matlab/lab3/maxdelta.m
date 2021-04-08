function xMax = maxdelta(a, b, poly, step)
% нахождение максимального отклонения
% @params
% a,b - интервал поиска
% poly - текущий полином квазиальтернанса
% step - шаг поиска
% @return
% xMax, yMax - координаты точки максимального отклонения
yMax = -1;
xMax  = a;

for x=a:step:b
    delta = abs(func(x)-polyval(poly, x));
    if delta > yMax
        xMax = x;
        yMax = delta;
    end
end
end