clear all ;
close all;

load identif_values;



Tc=4e-3;
Te=1E-3; % temps d'�chantillonnage

%% Valeurs des N mod�les
Tm=33.5E-3; %% Chez thierry Kphi*Ktach/Inertie/Si = 1/Tm
Si=0.55;
Kpwm=5; %+/- 5V 
Vmax = 12 *1000 /60/60; %% 3.33 m/s
Ktach = 2.5/Vmax;
N=1;
Kmli = 1.8*2; %% Gain trottinette
sigma_K = Kmli/10;
K=(randn(N,1)*sigma_K)+Kmli;
%K(3)=K(3)*1;
Kmes = 512/3.3;
%% Vit(p) = Kmli /(1+Tm p )

Inertie=Kmli*Ktach*Tm;
sigma_Ig=Inertie/10;
Ig=(randn(N,1)*sigma_Ig)+Inertie;
%Ig(3) = 5*Ig(3);

%% Valeur boite noire
K = premier_up.Kp.value;
Tau = premier_up.Tp1.value;
Kd = pos_integ.Kp.value;

est=tf([K],[Tau 1]);
crit= @(P) sum((lsim(ss(tf([P(1)],[P(2) 1])),comm,t,P(3))-Vit).^2) 
simuler = @(P) lsim(tf([P(1)],[P(2) 1]),comm,t,P(3))
Popt=fminsearch(crit,[K Tau Vit(1)])



if (N>1) 
    M_ecart=[eye(N-1) zeros(N-1,1)]-[zeros(N-1,1) eye(N-1)];
else
    M_ecart=0;
end;


L=4;
Delta=0.1;
sim test_identification;
vitest=lsim(ss(tf([Popt(1)],[Popt(2) 1])),comm,t,Popt(3))
plot(t,Vit,'k');
hold on;
plot(temps,vitesses,'r');
plot(t,vitest,'g');


figure
plot(t,Pos,'k');
hold on;
plot(temps,angles,'r');

