clear;
clc;

N = 10; % степень многочлена наилучшего приближения
EPS = 0.00000001; % точность приближения
A = 1; % левая граница отрезка
B = 6; % правая граница отрезка

% выбор начального альтернанса
% т.е на отрезке [A, B] выбираем N+2 точки
%x = [A:(B-A)/(N+1):B]
x = zeros(1, N+2);
for i = 1 : N+2
    x(i) = (A+B)/2 + ((B-A)*cos(((2*(i-1)+1)*pi)/(2*(N+2)))/2);
end

x = sort(x)
y = func(x);
h = 0.001;
X = A:h:B;

alternance_polynom = remeza(x, EPS);

% Построение графиков
f = figure('Color','w');
set(f, 'Position', [200, 100, 1000, 495]) 
% вывод графика
subplot(1,2,1)
fplot(@func,[A-1 B+1])
hold on
% вывод графика полинома наилучшего приближения
title('Alternance');
plot(X,polyval(alternance_polynom, X),'r')
% вывод узлов интерполяции
plot(x,y,'bo')

% разметка
grid on
xlabel('x');
ylabel('y');
