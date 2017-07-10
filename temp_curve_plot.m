clear all; close all; clc;
data = csvread('temp_curve.csv', 1, 0);
time = data(:, 1);
set_temp = data(:, 2);
cur_temp = data(:, 3);
on_time = data(:, 4) / 200;

figure;
subplot(2, 1, 1);
plot(time, set_temp, 'b');
hold on;
plot(time, cur_temp, 'r');
hold off;
grid on;
xlabel('{\itt} / ms');
ylabel('{\it\theta} / °C');
title('Oven Temperature');

subplot(2, 1, 2);
plot(time, on_time, 'g');
grid on;
xlabel('{\itt} / ms');
ylabel('{\itd}');
title('Heater Duty Cycle');