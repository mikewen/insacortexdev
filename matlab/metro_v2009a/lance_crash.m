clear all ;
close all;

Te=1E-3; % temps d'échantillonnage

%% Valeurs des N modèles
N=1;
Knom = 0.5; %% Gain trottinette
sigma_K = Knom/10;
K=(randn(N,1)*sigma_K)+Knom;
K(end)=K(end)*0.4;


Vitmax = 1200 %% pas/s
Taunom=3E-2; %% 
%% Vit(p) = K /(1+Tm p )

sigma_Tau=Taunom/10;
Tau=(randn(N,1)*sigma_Tau)+Taunom;
Tau(end) = 5*Tau(end);

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


L=20;
Delta=10;
sim n_metro_discret;
Puissance_LQ = sum(commandes_courant.*commandes_courant)/length(commandes_courant)
Puissance_Tot_LQ = sum(Puissance_LQ)/N
visu_crash;


%plot(temps,ecarts);
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