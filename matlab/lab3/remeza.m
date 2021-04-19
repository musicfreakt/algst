function P = remeza(X, epsilon)
% нахождение полинома наилучшего приближения 
% @params
% X - первоначально выбранный набор точек
% epsilon - погрешность
% @return
% P - полином альтернанса
% 
deltaStep = 0.01;
i = 1;
while i < 3 || abs(sigma - delta) >= epsilon
    if i > 1
        sigma = delta;
        sigma % вывод сигм на экран
    end
    [P, delta] = quasialternance(X);
    x_max = maxdelta(0, 7, P, deltaStep);
    % вот тут вывести графики
    X = replacement(X, P, x_max);
    i = i + 1;
end
end
