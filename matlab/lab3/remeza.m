function P = remeza(X, epsilon)
% нахождение наилучшего приближения к функции методом альтернанса 
% @params
% X - первоначально выбранный квазиальтернанс
% epsilon - переменная для отслеживания приблизительного равенства сигмы и
% дельты
% @return
% P - полином альтернанса
% 
deltaStep = 0.01;
i = 1;
while i < 3 || abs(sigma - delta) >= epsilon
    if i > 1
        sigma = delta;
        sigma
    end
    [P, delta] = quasialternance(X);
    P
    delta
    x_max = maxdelta(0, 7, P, deltaStep);
    X = replacement(X, P, x_max);
    X
    i = i + 1;
end
end