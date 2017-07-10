clear all; close all; clc;
data = csvread('temp_curve.csv', 1, 0);
time = data(:, 1);
temp = data(:, 2);

s = tf('s');
Ks = 570;
Tu = 19;
Tg = 268.8;
alpha = 0.2;  % From table in pt2_parameter.pdf page 9
Tg_T1 = 1.48; % From table in pt2_parameter.pdf page 9
T1 = Tg / Tg_T1;
T2 = T1 * alpha;

Ks = 340;
T1 = 120;
T2 = 30;

% Reverse calculations
alpha = 0.25;
Tg_T1 = 1.55;
Tg = Tg_T1 * T1;
Tu_Tg = 0.077;
Tu = Tu_Tg * Tg;

w0 = 1 / sqrt(T1 * T2);
D = (T1 + T2) / (2 * sqrt(T1 * T2));
Gs = Ks / (T1*T2*s^2 + (T1+T2)*s + 1);
step(Gs);

hold on;
plot(time/1000-2, temp-40, 'r');
axis([0 1200 0 600]);
grid on;
xlabel('{\itt} / ms');
ylabel('{\it\theta} / °C');
title('Oven Step Response');

% Simulink
a2 = T1*T2;
a1 = T1+T2;
a0 = 1;
tstart = 0;
tstop = 1200;
tstep = 0;
reflow_times = [0 150 300 360 400 460 1200];
reflow_temps = [0 150 200 260 260 0 0];

Tn = Tg;
Tv = 0.5 * Tu;

Kp = 0.65 * (Tg / (Tu*Ks));
Ki = Kp / Tn;
Kd = 0;%Kp * Tv;

simOut = sim('oven', 'StartTime', num2str(tstart), 'StopTime', num2str(tstop));
t = simOut.get('tout');
Ue = simOut.get('Ue');
temp = simOut.get('temp');
figure;
plot(reflow_times, reflow_temps);
hold on;
plot(t, temp, 'g');
figure;
plot(t, Ue, 'b');