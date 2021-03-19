clear;
clc;

DOTS = 12; % количество узлов интерполяции
A = 1; % левая граница 
B = 6; % правая граница

x = A:(B-A)/(DOTS-1):B; % равностоящие узлы интерполяции
y = interpfunc(x); % значения интерполируемой функции
h = 0.001;
X = A:h:B;

% Выбор случайной точки, не совпадающей ни с одним узлом
random_x = A + ((B-A)*rand); % todo: ПРОВЕРКА НА НЕСОВПАДЕНИЕ C УЗЛАМИ

bool = 1;
while bool == 1
    bool = 0;
    for i = 1:length(x)
        if(random_x == x(i))
            random_x = A + ((B-A)*rand);
            bool = 1;
            break
        end
    end
end

% Вычисление теоретической погрешности интерполяции в выбранной точке

omega = 1;
for i = 1 : DOTS
    omega = omega * (random_x - x(i)); 
end

% Вычисление производной
%interpfunc(sym("z"))
diff(interpfunc(sym("z")), DOTS)

if DOTS == 5
    [maxx,maxy]=fminbnd(@derivative5, A, B);
    maxy = abs(maxy);
    if maxy < derivative5(A)
        maxy = derivative5(A);
    elseif maxy < derivative5(B)
        maxy = derivative5(B);
    end
elseif DOTS == 12
    [maxx,maxy]=fminbnd(@derivative12, A, B);
    maxy = abs(maxy);
    if maxy < derivative12(A)
        maxy = derivative12(A);
    elseif maxy < derivative12(B)
        maxy = derivative12(B);
    end
end

interpolation_error = abs((maxy*omega)/factorial(DOTS));

% нахождение интерполяционного полинома Лагранжа
lagrange_polynom = lagrange(x,y);
newton_polynom = newton(x,y);

% Вычисление реальной погрешности интерполяции в выбранной точке
real_error = abs(interpfunc(random_x) - polyval(lagrange_polynom, random_x));

% Сравнение теоретической погрешности и реальной
random_x
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
