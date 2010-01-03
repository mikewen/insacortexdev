function [K,S,E,Ky,Sy,Ey,ntrot,Q,R,Qy,Ry]=calcul_retour_etat(NKpwm,NKphiSJv,NKtach,NDelta,N,dim)

if dim==2
%% Modèle d'état pour un moteur attaqué en courant
%      _
%  X = | Angle
%      | Vitesse Angulaire
%      |_
%
%dim=2;
    Ai = [0 1/NDelta ; 0 0];
    Bi = [0; NKpwm*NKphiSJv] ;
else
%% Pour ajouter un intégrateur, le vecteur d'état est donc
%      _
%      | Intégrale de l'angle
%  X = | Angle
%      | Vitesse Angulaire
%      |_
%
%  Y = | Vitesse
%      | Ecart devant
%dim=3;
    Ai = [0 1 0 ; 0 0 1/NDelta; 0 0 0];
    Bi = [0; 0;  NKpwm*NKphiSJv];
end

%% Matrices A et B
A = zeros(dim*N);
B = zeros(dim,1);
for d=1:N
    for i=1:dim
        for j=1:dim
            A((i-1)*N+d,d+(j-1)*N)=Ai(i,j);
        end
        B(d+(i-1)*N,d)=Bi(i);
    end
end


%% Matrice de sortie C et pondération Q
S=1;
if dim==3
    C = zeros(2*N,dim*N);
    D=zeros(2*N,N);
    Q=zeros(2*N,2*N);
    
    % 1 à N-1 => Sortie en écarts d'intégrales
    T=1;
    C(S:(S+N-2),T:(T+N-2))=C(S:(S+N-2),T:(T+N-2))+eye(N-1);
    C(S:(S+N-2),(T+1):(T+N-1)) = C(S:(S+N-2),(T+1):(T+N-1)) - eye(N-1);
    Q(S:(S+N-2),S:(S+N-2))=eye(N-1)/(N-1)*1;
    S=S+N-1;

    % N à 2*N-2 => Sortie des écarts
    T=N+1;
    C(S:(S+N-2),T:(T+N-2))=C(S:(S+N-2),T:(T+N-2))+eye(N-1);
    C(S:(S+N-2),(T+1):(T+N-1)) = C(S:(S+N-2),(T+1):(T+N-1)) - eye(N-1);
    Q(S:(S+N-2),S:(S+N-2))=eye(N-1)/(N-1)*12;
    S=S+N-1;
    % 2*N-1 => Position de la première trottinette
    C(S,T:T+N-1)=ones(1,N)/N;
    Q(S,S)=2    ;
    S=S+1;

    % 2*N => Intégrale de la position
    T+1;
    C(S,T+N:T+2*N-1)=ones(1,N)/N;
    Q(S,S)=2;
    S=S+1;
else
    C = zeros(N,dim*N);
    D=zeros(N,N);
    Q=zeros(N,N);
    % 1 à N-1 => Sortie des écarts
    T=1;
    C(S:(S+N-2),T:(T+N-2))=C(S:(S+N-2),T:(T+N-2))+eye(N-1);
    C(S:(S+N-2),(T+1):(T+N-1)) = C(S:(S+N-2),(T+1):(T+N-1)) - eye(N-1);
    Q(S:(S+N-2),S:(S+N-2))=eye(N-1)/(N-1)*15;
    S=S+N-1;
    % N => Position de la première trottinette
    C(S,T:T+N-1)=ones(1,N)/N;
    Q(S,S)=1;
    S=S+1;
end


%% 
R=eye(N)*1000;
ntrot=ss(A,B,C,D);
Qy=Q;
Ry=R;
[Ky,Sy,Ey]=lqry(ntrot,Qy,Ry);

Q=eye(dim*N)*1;
Q((2*N+1):(3*N))=Q((2*N+1):(3*N))*1;
R=eye(N)*1000;
[K,S,E]=lqr(A,B,Q,R);

return;