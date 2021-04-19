function [P, sigma] = quasialternance(X)
% Нахождение полинома и максимального отклонения для заданного
% квазиальтернанса
% @params
% X - массив точек квазиальтернанса
% @return
% Полином квазиальтернанса
% P - коэффициенты полинома квазиальтернанса
% sigma - значение максимального отклонения полинома от функции по модулю

for i = 1:length(X)
    valueMatrix(i) = func(X(i));

    for j = 1:length(X)
        koefsMatrix(i, j) = X(i)^(j-1);
    end

    koefsMatrix(i, length(X)) = (-1)^i;
end

% ' - в нашем случае перевести столбец в строку
rslt = inv(koefsMatrix)*valueMatrix';
% Дело в том, что без flip и ' мы получим строку коэффициентов в порядке
% возрастания степени, а нужно в порядке убывания
P = flip(rslt(1:length(rslt)-1))';
sigma = rslt(length(rslt));

end