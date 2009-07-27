clear all ;
close all;

Te=1E-3; % temps d'échantillonnage

%% Valeurs des N modèles
N=3;
Kn=485.6529;
Taun=0.04;
Kdn=1.919;

sigma_K = Kn/10;
K=(randn(N,1)*sigma_K)+Kn;
sigma_Tau = 0.060;
K=((randn(N,1)+1)*sigma_Tau)+Kn;
%K(3)=K(3)*1;

if (N>1) 
    M_ecart=[eye(N-1) zeros(N-1,1)]-[zeros(N-1,1) eye(N-1)];
else
    M_ecart=0;
end;


[Kr,S,E,Kry,Sy,Ey]=calcul_retour_etat(Kn,Taun,Kd,N);
dim=3;
Ki=Kr(:,1:dim:dim*N)*(dim-2)
Ka=Kr(:,dim-1:dim:dim*N)
Kv=Kr(:,dim:dim:dim*N)


L=20;
Delta=0.1;
sim n_metro;
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
sim n_trotinettes_crash_discret;
Puissance_LQY = sum(commandes_courant.*commandes_courant)/length(commandes_courant)
Puissance_Tot_LQY = sum(Puissance_LQY)/N

%plot(temps,ecarts,'--')
visu_crash