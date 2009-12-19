function packTrain()
% Pack les données en courant, position et vitesse des 3 trains

display ('Pack les données des 3 trains ...');
% pack les données en courant

C_01 = evalin('base','Courant_AV_A_01');
C_02 = evalin('base','Courant_AV_A_02');
C_03 = evalin('base','Courant_AV_A_03');

C(:,1) = C_01;
C(:,2) = C_02;
C(:,3) = C_03;

assignin ('base', 'Courant_AV_A', C); 

C_01 = evalin('base','Courant_AR_A_01');
C_02 = evalin('base','Courant_AR_A_02');
C_03 = evalin('base','Courant_AR_A_03');

C(:,1) = C_01;
C(:,2) = C_02;
C(:,3) = C_03;

assignin ('base', 'Courant_AR_A', C);

% pack les données en position

P_01 = evalin('base','Position_AV_A_01');
P_02 = evalin('base','Position_AV_A_02');
P_03 = evalin('base','Position_AV_A_03');

P(:,1) = P_01;
P(:,2) = P_02;
P(:,3) = P_03;

assignin ('base', 'Position_AV_A', P); 

P_01 = evalin('base','Position_AR_A_01');
P_02 = evalin('base','Position_AR_A_02');
P_03 = evalin('base','Position_AR_A_03');

P(:,1) = P_01;
P(:,2) = P_02;
P(:,3) = P_03;

assignin ('base', 'Position_AR_A', P);

P_01 = evalin('base','Position_AV_F_01');
P_02 = evalin('base','Position_AV_F_02');
P_03 = evalin('base','Position_AV_F_03');

P(:,1) = P_01;
P(:,2) = P_02;
P(:,3) = P_03;

assignin ('base', 'Position_AV_F', P); 

P_01 = evalin('base','Position_AR_F_01');
P_02 = evalin('base','Position_AR_F_02');
P_03 = evalin('base','Position_AR_F_03');

P(:,1) = P_01;
P(:,2) = P_02;
P(:,3) = P_03;

assignin ('base', 'Position_AR_F', P);

% pack les données en vitesse

V_01 = evalin('base','Vitesse_AV_A_01');
V_02 = evalin('base','Vitesse_AV_A_02');
V_03 = evalin('base','Vitesse_AV_A_03');

V(:,1) = V_01;
V(:,2) = V_02;
V(:,3) = V_03;

assignin ('base', 'Vitesse_AV_A', V); 

V_01 = evalin('base','Vitesse_AR_A_01');
V_02 = evalin('base','Vitesse_AR_A_02');
V_03 = evalin('base','Vitesse_AR_A_03');

V(:,1) = V_01;
V(:,2) = V_02;
V(:,3) = V_03;

assignin ('base', 'Vitesse_AR_A', V);

V_01 = evalin('base','Vitesse_AV_F_01');
V_02 = evalin('base','Vitesse_AV_F_02');
V_03 = evalin('base','Vitesse_AV_F_03');

V(:,1) = V_01;
V(:,2) = V_02;
V(:,3) = V_03;

assignin ('base', 'Vitesse_AV_F', V); 

V_01 = evalin('base','Vitesse_AR_F_01');
V_02 = evalin('base','Vitesse_AR_F_02');
V_03 = evalin('base','Vitesse_AR_F_03');

V(:,1) = V_01;
V(:,2) = V_02;
V(:,3) = V_03;

assignin ('base', 'Vitesse_AR_F', V);