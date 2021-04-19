 % тестовый набор точек для аппроксимации todo: ВЫБРАТЬ ДРУГИЕ ТОЧКИ И ПОБОЛЬШЕ
x = [10.1; 10.2; 10.3; 10.8; 10.9; 11; 11.1; 11.4; 12.2; 13.3; 13.8; 14; 14.4; 14.5; 15; 15.6; 15.8; 17; 18.2; 19];
y = [24; 36; 26; 45; 34; 37; 55; 51; 75; 84; 74; 91; 85; 87; 94; 92; 96; 97; 98; 99];

X = x(1):0.1:x(length(x));
Y1 = polyval(polynom(x, y, 1), X);
Y2 = polyval(polynom(x, y, 2), X);
Y3 = polyval(polynom(x, y, 3), X);
f = sincos(x, y);
Y4 = f(1)*cos(X) + f(2)*sin(X) + f(3);

plot(x,y,'*');
hold on;
grid on;
plot(X,Y1,'Color','r');
plot(X,Y2,'Color','g');
plot(X,Y3,'Color','b');
plot(X,Y4,'Color','y');