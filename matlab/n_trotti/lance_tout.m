clear all ;
close all;

Te=2E-3; % temps d'échantillonnage
%% Parametres trajectoire
L=20;
D=0.01;
start=1;
dt=2;
vmax=3;
gama = vmax/dt;
stop=start+10;

%% Valeurs des N modèles
N=3;
Imax=9;
% valeur normales Nx et Ecarts type Sx (en %Nominal) et les N valeurs x
NKpwm = 5;
SKpwm = 0.1;
Kpwm=(randn(N,1)*SKpwm/100*NKpwm)+NKpwm;

NSi = 0.55;
SSi = 1;
Si=(randn(N,1)*SSi/100*NSi)+NSi;

NKphiSJv=22;
SKphiSJv=10;
KphiSJv=(randn(N,1)*SKphiSJv/100*NKphiSJv)+NKphiSJv;
KphiSJv(2)=KphiSJv(2)/500;

NKtach=2.5/3.33;
SKtach= 1;
Ktach=(randn(N,1)*SKtach/100*NKtach)+NKtach;

NDelta = 4.254e-3;
SDelta = 0.01;
Delta=(randn(N,1)*SDelta/100*NDelta)+NDelta;

if (N>1) 
    M_ecart=[eye(N-1) zeros(N-1,1)]-[zeros(N-1,1) eye(N-1)];
else
    M_ecart=0;
end;

%% calcul des retours d'état

[Kr,S,E,Kry,Sy,Ey,ntrot,Q,R,Qy,Ry]=calcul_retour_etat(NKpwm/NSi,NKphiSJv/500,NKtach,NDelta,N);
dim=3;
Ki=Kr(:,1:N)*(dim-2)
Ka=Kr(:,N+1:N*2)
Kv=Kr(:,N*2+1:3*N)


sim n_trotinettes_tout_discret;
Puissance_LQ = sum(commandes_courant.*commandes_courant)/length(commandes_courant)
Puissance_Tot_LQ = sum(Puissance_LQ)/N
visu_crash;


%plot(temps,ecarts);
hold on;
pause;
close all;
hold off;
Ki=Kry(:,1:N)*(dim-2)
Ka=Kry(:,N+1:N*2)
Kv=Kry(:,N*2+1:3*N)
sim n_trotinettes_tout_discret;
Puissance_LQY = sum(commandes_courant.*commandes_courant)/length(commandes_courant)
Puissance_Tot_LQY = sum(Puissance_LQY)/N

%plot(temps,ecarts,'--')
visu_crash