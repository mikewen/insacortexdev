clear all ;
close all;

Te=2E-3; % temps d'échantillonnage


Delta=10;
Dist=32000;
tda=0.5;
tpsm=2;
Vmax=1000;
tdd=Dist/Vmax;
Amax = Vmax/tpsm;
L=tdd+tpsm+2;

Vitmax = 1200 %% pas/s


%% Valeurs des N modèles
N=3;
Knom = 0.4979; %% Gain trottinette
sigma_K = Knom/100;
K=(randn(N,1)*sigma_K)+Knom;
K(end)=K(end)*1;


Taunom=3E-2; %% 
%% Vit(p) = K /(1+Tm p )

sigma_Tau=Taunom/100;
Tau=(randn(N,1)*sigma_Tau)+Taunom;
Tau(end) = 1*Tau(end);

if (N>1) 
    M_ecart=[eye(N-1) zeros(N-1,1)]-[zeros(N-1,1) eye(N-1)];
else
    M_ecart=0;
end;


[Kr,S,E,Kry,Sy,Ey]=calcul_retour_etat(Knom,Taunom,N);
dim=3;
Ki=Kr(:,1:dim:dim*N)*(dim-2)
Ka=Kr(:,dim-1:dim:dim*N)
Kv=Kr(:,dim:dim:dim*N)



sim n_metro_discret;
Puissance_LQ = sum(commandes_courant.*commandes_courant)/length(commandes_courant)
Puissance_Tot_LQ = sum(Puissance_LQ)/N
visu_crash;


plot(temps,ecarts);
hold on;
pause;
close all;
hold off;
Ki=Kry(:,1:dim:dim*N)*(dim-2)
Ka=Kry(:,dim-1:dim:dim*N)
Kv=Kry(:,dim:dim:dim*N)
sim n_metro_discret;
Puissance_LQY = sum(commandes_courant.*commandes_courant)/length(commandes_courant)
Puissance_Tot_LQY = sum(Puissance_LQY)/N

%plot(temps,ecarts,'--')
 visu_crash