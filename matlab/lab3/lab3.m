clear;
clc;

N = 3; % степень многочлена наилучшего приближения
EPS = 0.000001; % точность приближения
A = 1; % левая граница отрезка
B = 6; % правая граница отрезка

% выбор начального альтернанса
% т.е на отрезке [A, B] выбираем N+2 точки
x = [A:(B-A)/(N+1):B];
%x = zeros(1, N+2);
%for i = 1 : N+2
%    x(i) = (A+B)/2 + ((B-A)*cos(((2*(i-1)+1)*pi)/(2*(N+2)))/2);
%end

x = sort(x)

alternance_polynom = remeza(x, EPS);
