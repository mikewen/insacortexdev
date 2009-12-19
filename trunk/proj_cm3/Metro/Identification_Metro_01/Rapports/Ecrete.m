function Ecrete()
% Ecrete les valeur en vitesse

display ('Ecrete les valeurs des vitesses ...');

% traite les données du train 1

V = evalin('base','Vitesse_AR_A_01');

for i = 1:length(V)
    if (V(i) > 2500)
        V(i)=2500;
    end
end

assignin ('base', 'Vitesse_AR_A_01', V); 

V = evalin('base','Vitesse_AV_A_01');

for i = 1:length(V)
    if (V(i) > 2500)
        V(i)=2500;
    end
end

assignin ('base', 'Vitesse_AV_A_01', V); 

% traite les données du train 2

V = evalin('base','Vitesse_AR_A_02');

for i = 1:length(V)
    if (V(i) > 2500)
        V(i)=2500;
    end
end

assignin ('base', 'Vitesse_AR_A_02', V); 

V = evalin('base','Vitesse_AV_A_02');

for i = 1:length(V)
    if (V(i) > 2500)
        V(i)=2500;
    end
end

assignin ('base', 'Vitesse_AV_A_02', V);

% traite les données du train 3

V = evalin('base','Vitesse_AR_A_03');

for i = 1:length(V)
    if (V(i) > 2500)
        V(i)=2500;
    end
end

assignin ('base', 'Vitesse_AR_A_03', V); 

V = evalin('base','Vitesse_AV_A_03');

for i = 1:length(V)
    if (V(i) > 2500)
        V(i)=2500;
    end
end

assignin ('base', 'Vitesse_AV_A_03', V);