function X_new = replacement(X, P, x_max)
% замена одной из точек квазиальтернанса на точку с максимальным
% отклонением
% @params
% X - текущий квазиальтернанс
% P - коэффициенты полинома текущего квазиальтернанса
% x_max - максимум модуля отклонения
% @return
% X_new - новый квазиальтернанс

new_index = 1;
y_max = func(x_max) - polyval(P, x_max);

while new_index < length(X) + 1 && X(new_index) < x_max
    new_index = new_index + 1;
end

if new_index == 1
    if sign(func(X(1))-polyval(P, X(1))) == sign(y_max)
        X(1) = x_max;
    else
        X = [x_max X(1:end-1)];
    end
else
    if new_index > length(X)
        if sign(func(X(end))) == sign(y_max)
            X(end) = x_max;
        else
            X = [X(2:end) x_max];
        end
    else
        if sign(func(X(new_index))-polyval(P, X(new_index))) == sign(y_max)
            X(new_index) = x_max;
        else
            X(new_index - 1) = x_max;
        end
    end
end

X_new = X;

end
