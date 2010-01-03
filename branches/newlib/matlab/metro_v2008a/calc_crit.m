function [f] = calc_crit(P)

P=abs(P)
Te=2E-3; % temps d'échantillonnage

%% Valeurs des N modèles
N=3;
Kn=485.6529;
Taun=0.04;
Kdn=1.919;

sigma_K = Kn/10*0;
K=(randn(N,1)*sigma_K)+Kn;
sigma_Tau = 0.;
Tau=((rand(N,1))*sigma_Tau)+Taun;
%K(3)=K(3)*1;
Tau(2)=0.1;

if (N>1) 
    M_ecart=[eye(N-1) zeros(N-1,1)]-[zeros(N-1,1) eye(N-1)];
else
    M_ecart=0;
end;

%% Calcul retour
[Kr,S,E,Kry,Sy,Ey]=calcul_retour(Kn,Taun,Kdn,N,Te,P(1),P(2),P(3),P(4),P(5),P(6),P(7));
dim=3;
Ki=Kry(:,1:N)*(dim-2);
Ka=Kry(:,(N+1):2*N);
Kv=Kry(:,(2*N+1):end);

%% Calcul trajectoire
dpmax=800;
Tm = 0.2;
Td = 2;
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
Puissance_LQ = sum(commandes_courant.*commandes_courant)/length(commandes_courant);
Puissance_Tot_LQ = sum(Puissance_LQ)/N;

Istop = ceil(interp1(temps,1:length(temps),Tstop));

vit_max=max(abs(vitesses(Istop,:)));

dist_max=max(max(abs(ecarts)));

%if dist_max>10
%    crit=crit+100;
%end

%if vit_max> 20
%    crit=crit+20;
%end

eca_max=max(angles(Istop,:)-(Pfin+(0:N-1)*Delta));
%if eca_max>100
%    crit=crit+20;
%end

f = vit_max
