function [K,S,E,Ky,Sy,Ey]=calcul_retour_etat(K,Tau,N)


%% Pour ajouter un intégrateur, le vecteur d'état est donc
%      _
%      | Intégrale de l'angle
%  X = | Angle
%      | Vitesse Angulaire
%      |_
%
%  Y = | Vitesse
%      | Ecart devant
dim=3;
Ai = [0 1 0 ; 0 0 1; 0 0 -1/Tau];
Bi = [0; 0;  K/Tau] ;
Ci = [0 0 0 0  0 1;
      0 1 0 0 -1 0]

A = zeros(dim*N);
B = zeros(dim,1);
C = zeros(2*N+1,dim*N);
for i=1:N 
    A(dim*i-dim+1:dim*i,dim*i-dim+1:dim*i)=Ai;
    B(dim*i-dim+1:dim*i,i)=Bi;
end
%C(1:N,dim:dim:dim*N)=eye(N) ;%%vitesse de la trotti 1

% 1 à N-1 => Sortie en écarts
C(1:(N-1),(dim-1):dim:dim*(N-1))=eye(N-1);
C(1:(N-1),(2*dim-1):dim:dim*(N)) = C(1:(N-1),(2*dim-1):dim:dim*(N)) - eye(N-1) ;
Q=eye(2*N+1)*10;

% N+1 à 2*N-1 => Sortie intégrale des écarts
C(N+1:(2*N-1),1:N*dim-1)=C(1:(N-1),2:N*dim);
Q(N+1:(2*N-1),N+1:(2*N-1))=Q(N+1:(2*N-1),N+1:(2*N-1))*100;

% N => Position de la première trottinette
C(N,:)=[ 0 1 0 0 1 0 0 1 0];
Q(N,N)=10;

% 2*N => Intégrale de la position
C(2*N,:)=[1 0 0  1 0 0 1 0 0];
Q(2*N,2*N)=Q(2*N,2*N)*100;

% 2*N +1 => vitesse
C(2*N+1,:)=[0 0 1  0 0 1 0 0 1];
Q(2*N+1,2*N+1)=Q(2*N+1,2*N+1)*10;



R=eye(N)*1;
ntrot=ss(A,B,C,zeros(2*N+1,N));
[Ky,Sy,Ey]=lqry(ntrot,Q,R);

Q=eye(dim*N)*100;
Q(1:N)=Q(1:N)*100;
Q((N+1):(2*N))=Q((N+1):(2*N))*10;
Q((2*N+1):(3*N))=Q((2*N+1):(3*N))*10;
R=eye(N)*1;
[K,S,E]=lqr(A,B,Q,R);

return;