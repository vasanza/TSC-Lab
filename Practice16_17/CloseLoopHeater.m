%% Función de Transferencia obtenida (Metodo 3)
clear;clc;
IN=load('IN.mat');IN=IN.IN;
OUT=load('OUT.mat');OUT=OUT.OUT;
load('bj22221.mat');
G=tf(d2c(bj22221));%<---Modelo con mejor FIT
num=cell2mat(G.numerator);
den=cell2mat(G.denominator);
FTtemperature=tf(num,den);

Time=(0:1:length(IN)-1)'; %Vector de tiempo
y = lsim(G,IN-mean(IN),Time)+mean(OUT); % system response
figure(1)
plot(y);
hold on;
plot(IN);
plot(OUT);
legend('Estimate','Input','Output');
%% Close Loop
%pidTuner
[C_pi,info] = pidtune(FTtemperature,'PID');
kp=C_pi.kp;
ki=C_pi.ki;
kd=C_pi.kd;
T_pi = feedback(C_pi*FTtemperature, 1);
step(T_pi)