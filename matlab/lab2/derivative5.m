function f = derivative5(z)
    f = -((40*cos(z)^2)/(z + 3)^3 - (40*cos(z)^2)/(z + 3) + (40*sin(z)^2)/(z + 3) - (40*sin(z)^2)/(z + 3)^3 + (24*sin(z)^2)/(z + 3)^5 + (80*cos(z)*sin(z))/(z + 3)^2 - (60*cos(z)*sin(z))/(z + 3)^4 + 32*log(z + 3)*cos(z)*sin(z));
end