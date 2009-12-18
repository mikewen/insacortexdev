clear all ;
close all;

Te=2E-3; % temps d'échantillonnage

%% Valeurs des N modèles
N=3;
Kn=485.6529;
Taun=0.04;
Kdn=1.919;

sigma_K = Kn/10*0;
K=(randn(N,1)*sigma_K)+Kn;
sigma_Tau = 0.00;
Tau=((rand(N,1))*sigma_Tau)+Taun;
%K(3)=K(3)*1;
K(2)=K(2)*0.9;
Tau(2) = 0.1;
K(3)=K(3)*1.1;
Tau(3) = 0.04;

if (N>1) 
    M_ecart=[eye(N-1) zeros(N-1,1)]-[zeros(N-1,1) eye(N-1)];
else
    M_ecart=0;
end;

%% Calcul retour
[Kr,S,E,Kry,Sy,Ey]=calcul_retour(Kn,Taun,Kdn,N,Te,0,1,1,10,10,5,2000);
dim=3;
Ki=Kr(:,1:N)*(dim-2)
Ka=Kr(:,(N+1):2*N)
Kv=Kr(:,(2*N+1):end)

%% Calcul trajectoire
dpmax=800
Tm = 0.2;
Td = 1;
Vmax = dpmax/Kdn;
acc_max=dpmax/Tm;
Pfin = 2000;
% (D+Tm/2+Td/2)*dpmax=Pfin
D=Pfin/dpmax-Tm/2-Td/2;



L=D+Tm+Td+2;
Tstart=0.5;
Tstop=Tstart+D+Tm+Td;
Delta=10;
sim n_metro;
Puissance_LQ = sum(commandes_courant.*commandes_courant)/length(commandes_courant)
Puissance_Tot_LQ = sum(Puissance_LQ)/N
visu_crash;


%plot(temps,ecarts);
hold on;
pause;
close all;
hold off;
Ki=Kry(:,1:N)*(dim-2)
Ka=Kry(:,(N+1):2*N)
Kv=Kry(:,(2*N+1):end)
sim n_metro;
Puissance_LQY = sum(commandes_courant.*commandes_courant)/length(commandes_courant)
Puissance_Tot_LQY = sum(Puissance_LQY)/N

%plot(temps,ecarts,'--')
visu_crash