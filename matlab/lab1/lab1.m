clear;
clc;

DOTS = 5; % количество узлов интерполяции
A = 1; % левая граница 
B = 6; % правая граница

x = A:(B-A)/(DOTS-1):B; % равностоящие узлы интерполяции
y = interpfunc(x); % значения интерполируемой функции
h = 0.001;
X = A:h:B;

% Выбор случайной точки, не совпадающей ни с одним узлом
random_x = A + ((B-A)*rand) % todo: ПРОВЕРКА НА НЕСОВПАДЕНИЕ C УЗЛАМИ

% Вычисление теоретической погрешности интерполяции в выбранной точке

omega = 1;
for i = 1 : DOTS
    omega = omega * (random_x - x(i)); 
end

d = 1;
if DOTS == 5
    d = 1; % ВЫЧИСЛИТЬ МАКСИМУМ ПРОИЗВОДНОЙ
elseif DOTS == 12
    d = 1;  % ВЫЧИСЛИТЬ МАКСИМУМ ПРОИЗВОДНОЙ
end

interpolation_error = abs((d*omega)/factorial(DOTS+1));

% нахождение интерполяционного полинома Лагранжа
lagrange_polynom = lagrange(x,y);
newton_polynom = newton(x,y);

% Вычисление реальной погрешности интерполяции в выбранной точке
real_error = abs(interpfunc(random_x) - polyval(lagrange_polynom, random_x));

% Сравнение теоретической погрешности и реальной
interpolation_error
real_error

% Построение графиков
f = figure('Color','w');
set(f, 'Position', [200, 100, 1000, 495]) 
% вывод графика sin(x)
subplot(1,2,1)
fplot(@interpfunc,[A-1 B+1])
hold on
% вывод графиков полиномов
title('Lagrange');
plot(X,polyval(lagrange_polynom, X),'r')
% вывод узлов интерполяции
plot(x,y,'bo')

% разметка
grid on
xlabel('x');
ylabel('y');

subplot(1,2,2)
fplot(@interpfunc,[A-1 B+1])
hold on
% вывод графиков полиномов
title('Newton');
plot(X, polyval(newton_polynom, X), 'g')
% вывод узлов интерполяции
plot(x,y,'bo')

% разметка
grid on
xlabel('x');
ylabel('y');
