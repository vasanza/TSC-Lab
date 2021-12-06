%   ****************************** TSC-Lab *******************************
%   ***************************** PRACTICE 15 *****************************
%   This practice is about DC motor system identification
%   By: Ulbio Alejandro
%   Reviewed: Víctor Asanza
%   More information: https://tsc-lab.blogspot.com/
%   More examples: https://github.com/vasanza/TSC-Lab
%   Dataset: http://ieee-dataport.org/4138

%% Práctica Identificación de Sistemas
clc
clear all
close all
Datos=load('OpenLoopMotor.csv'); %Cargar los datos del excel y guardarlos en la variable
OUT=Datos(:,1); %Salida del Sistema
IN=Datos(:,2); %Entrada del Sistema (Verificar que este valor este entre 0 y 255, caso contrario *255)
Time=(0:1:length(Datos)-1)'; %Vector de tiempo
%% Gráfica de los datos
figure(1)
plot(Time,IN); %Se grafica la entrada del sistema
title('Entrada: Altos y Bajos del Microcontrolador')
ylabel('Señal del microcontrolador')
xlabel('Tiempo [s]')
grid on

figure(2)
plot(Time,OUT); %Se grafica la salida del sistema
title('Salida: Velocidad del motor DC')
ylabel('Velocidad [RPM]')
xlabel('Tiempo [s]')
grid on

%% Aplicación del Filtro en la señal
Yop=0; %Punto de operación según la gráfica
%Tomando los puntos de operación, se realiza un promedio, donde se obtiene
Yss=(5820+5820+5760+5760+5820+5760+5760+5700+5700+5700+5760+5700+5700+5700+5760+5700+5700)/17
Ts=1 %Tiempo de muestreo que es el paso de todo el vector de tiempo
%Tarr=119.9 %Tiempo de arranque del sistema (produce la variación)
%Cálculo de la banda del 2%
banda=0.02*(Yss-Yop); %Se obtiene  el 2% de la variación de Yss-Yop
band=banda*ones(length(Time),1); %Un vector continuo con dimensión t de banda
figure(3)
plot(Time,OUT); %Se grafica la salida del sistema
title('Salida: Velocidad del motor DC')
ylabel('Velocidad [RPM]')
xlabel('Tiempo [s]')
grid on
hold on
plot(Time,[band+Yss Yss-band],'--') %Grafica la banda del 2%
legend('Gráfica de la salida','Gráfica de la Banda del 2%') %Identificación de las gráficas
Tss=4; %Parámetro obtenido de la gráfica

%% Estimación de polo o polos dominantes del sistema
tau=Tss/4 %La constante de tiempo es estimada a partir del Tiempo de estabilización
p=1/tau   %polo dominante, Se lo estima a través de los índices de desempeño.

%% Creación y prueba de filtros
%Filtro 1
pf1=5*p; %Condición de 5 veces el polo dominante, valor mínimo para 
%considerar que existe dominancia

F1=tf(pf1,[1 pf1])       %Función de transferencia de filtro de primer orden;
                       %debe variar el valor de pf de acuerdo al criterio 
                       %de dominancia y al valor del polo dominante del 
                       %sistema. Simule el filtro y repita este
                       %procedimiento al menos 3 veces verificando que se
                       %mantenga la dinámica de la señal filtrada.
%Filtro 2
pf2=25*p; %Condición de 25 veeces el polo dominante                       
F2=tf(pf2,[1 pf2]) %Función de transferencia del filtro 2

%Filtro 3
pf3=35*p; %Condición de 35 veces el polo dominante
F3=tf(pf3,[1 pf3]) %Función de transferencia del filtro 3
                                            
%Comprobación de Filtro 1
outf1=lsim(F1,OUT,Time); %El comando lsim() permite simular el sistema 
%del filtro 1, el OUT corresponde a la entrada de ese filtro que es la
%salida de la planta en estudio y por supuesto el tiempo que vamos a simular
figure(4)
plot(Time,[OUT outf1]) %Gráfica de la señal original de salida de la planta con el filtro
grid on %Enciende la cuadrícula
legend('Original','Filtrada') %Identifica las señales
title('Comprobación del Filtro 1') 
xlabel('Tiempo [s]') 
ylabel('Voltaje del sensor caudal')
%% Identificación de sistemas
systemIdentification %Se llama un comando de matlab importante para la identificación

%% Función de Transferencia obtenida (Metodo 1 y 2)
FT=zpk(-0.06005,[-0.7829 -0.0417],3746.6) %Función de transferencia obtenida con el 74.87% ajuste

%% Función de Transferencia obtenida (Metodo 3)
G=tf(d2c(bj20120));%<---Modelo con mejor FIT
num=cell2mat(G.numerator);
den=cell2mat(G.denominator);
FTmotor=tf(num,den)

Time=(0:1:length(IN)-1)'; %Vector de tiempo
y = lsim(G,IN,Time); % system response
figure(1)
plot(y);
hold on;
plot(IN);
plot(OUT);
legend('Estimate','Input','Output');

%%
pidtune(G,'PID',1)

