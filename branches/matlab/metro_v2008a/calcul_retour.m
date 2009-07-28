function [K,S,E,Ky,Sy,Ey]=calcul_retour(K,Tau,Kd,N,Te,I1,P1,V1,IE,PE,VE,RR)


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

A = zeros(dim*N,dim*N);
A(1:N,N+1:2*N)=eye(N);
A((N+1):2*N,(2*N+1):end)=eye(N)*Kd;
A((2*N+1):end,(2*N+1):end)=-eye(N)*Tau;


 B=[ zeros(2*N,N) ; 
     K/Tau  *eye(N)
   ];
      
i=1;
C=zeros(N*dim,N*dim);
Q=eye(dim*N);

if (N>1)

Ecs= [eye(N-1) zeros(N-1,1)];
Ecs(:,2:end) = Ecs(:,2:end) - eye(N-1);
if (IE>0)
% N-1 => Sortie intégrale des écarts
j=1;
C(i:(i+N-2),j:(j+N-1))=Ecs;
Q(i:(i+N-2),:)=Q(i:(i+N-2),:)*IE;
i=i+N-1;
end

if (PE>0)
% N-1 => Sortie en écarts
j=N+1;
C(i:(i+N-2),j:(j+N-1))=Ecs;
Q(i:(i+N-2),:)=Q(i:(i+N-2),:)*PE;
i=i+N-1;
end

if (VE>0)
% N-1 => Sortie dérivée des écarts
j=2*N+1;
C(i:(i+N-2),j:(j+N-1))=Ecs;
Q(i:(i+N-2),:)=Q(i:(i+N-2),:)*VE;
i=i+N-1;
end

end

if (I1>0)
% 1 => Integrale de Position de la première trottinette
%C(N,:)=[ 0 1 0 zeros(1,dim*(N-1))];
C(i,:)=[ 1 zeros(1,N-1) zeros(1,N) zeros(1,N)];
Q(i,:)=Q(i,:)*I1;
i=i+1;
end

if (P1>0)
% 1 => Position de la première trottinette
%C(N,:)=[ 0 1 0 zeros(1,dim*(N-1))];
C(i,:)=[zeros(1,N) 1 zeros(1,N-1) zeros(1,N)];
Q(i,:)=Q(i,:)*P1;
i=i+1;
end

if (V1>0)
% 1 => Dérivée de la première trottinette
%C(N,:)=[ 0 1 0 zeros(1,dim*(N-1))];
C(i,:)=[zeros(1,N) zeros(1,N) 1 zeros(1,N-1) ];
Q(i,:)=Q(i,:)*V1;
i=i+1;
end

R=eye(N)*RR;
ntrot=ss(A,B,C(1:i-1,:),zeros(i-1,N));
[Ky,Sy,Ey]=lqrd(ntrot.a,ntrot.b,C'*Q*C,R,Te);

Q=eye(dim*N)*1;
Q((2*N+1):(3*N))=Q((2*N+1):(3*N))*1;
R=eye(N)*RR;
[K,S,E]=lqr(A,B,Q,R);

return;