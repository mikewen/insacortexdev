clear all ;
close all;

%% Contraintes
% Pour une rampe classique
% avec une côte de +- 4A situé en dehors des accel
% Defaut d'inertie sur nimporte quelle trot allan jusqu'à /80
%Erreur max position en fin de rampe 5 pas
%Erreur max de vitesse en fin de rampe 0.1m/s
%Espace nominal entre deux voitures 10 pas ! => ecarts entre deux voitures max = 10 pas



Te=2E-3; % temps d'échantillonnage
%% Parametres trajectoire
NDelta = 4.254e-3;
vmax=3.3;
D=NDelta*10;
L=3000*NDelta;
Ltrot = 1;
start=1;
dt=2;
gama = vmax/dt;
stop=start+L/vmax;

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
SKphiSJv=0;
KphiSJv=(randn(N,1)*SKphiSJv/100*NKphiSJv)+NKphiSJv;
KphiSJv(1)=KphiSJv(1)/80;

NKtach=2.5/3.33;
SKtach= 1;
Ktach=(randn(N,1)*SKtach/100*NKtach)+NKtach;

SDelta = 0.01;
Delta=(randn(N,1)*SDelta/100*NDelta)+NDelta;

if (N>1) 
    M_ecart=[eye(N-1) zeros(N-1,1)]-[zeros(N-1,1) eye(N-1)];
else
    M_ecart=0;
end;

%% Filtres à Fc=300 Hz
Tau = 1/2/pi/300;
Af=-eye(3)/Tau;
Bf=eye(N)/Tau;
Cf=eye(N);
Df=zeros(N,N);
filtre_300=ss(Af,Bf,Cf,Df);

%% calcul des retours d'état
dim=3;
[Kr,S,E,Kry,Sy,Ey,ntrot,Q,R,Qy,Ry]=calcul_retour_etat(NKpwm/NSi,NKphiSJv/15,NKtach,NDelta,N,dim);

if dim==2
    Ka=Kr(:,1:N)
    Kv=Kr(:,N+1:N*2)
    Ki=Ka*0
else
    Ki=Kr(:,1:N)
    Ka=Kr(:,N+1:N*2)
    Kv=Kr(:,N*2+1:3*N)
end
sim n_trotinettes_tout_discret;
Puissance_LQ = sum(commandes_courant.*commandes_courant)/length(commandes_courant)
Puissance_Tot_LQ = sum(Puissance_LQ)/N
visu_crash;


%plot(temps,ecarts);
hold on;
pause;
close all;
hold off;

if dim==2
    Ka=Kry(:,1:N)
    Kv=Kry(:,N+1:N*2)
    Ki=Ka*0
else
    Ki=Kry(:,1:N)
    Ka=Kry(:,N+1:N*2)
    Kv=Kry(:,N*2+1:3*N)
end
sim n_trotinettes_tout_discret;
Puissance_LQY = sum(commandes_courant.*commandes_courant)/length(commandes_courant)
Puissance_Tot_LQY = sum(Puissance_LQY)/N

%plot(temps,ecarts,'--')
visu_crash